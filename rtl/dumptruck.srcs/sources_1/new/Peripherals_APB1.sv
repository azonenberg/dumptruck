/***********************************************************************************************************************
*                                                                                                                      *
* DUMPTRUCK                                                                                                            *
*                                                                                                                      *
* Copyright (c) 2024 Andrew D. Zonenberg and contributors                                                              *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

`timescale 1ns/1ps
`default_nettype none

module Peripherals_APB1(

	//APB to root bridge
	APB.completer 		apb,

	//Low speed clock for device info
	input wire			clk_25mhz,

	//GPIO LEDs
	output wire[3:0]	led,
	output wire			led_ctrl,

	//MDIO and control lines to PHY
	inout wire			eth_mdio,
	output wire			eth_mdc,
	output wire			eth_rst_n,

	//PMOD GPIO
	inout wire[7:0]		pmod_gpio,

	//QSPI flash lines
	inout wire[3:0]		flash_dq,
	output wire			flash_cs_n,
	//flash SCK is CCLK pin from STARTUPE2

	//Status inputs from APB2 Ethernet stuff
	input wire			rgmii_link_up_core,
	input wire			rx_frame_ready
);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pipeline register heading up to root bridge

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(16), .USER_WIDTH(0)) apb1_root();
	APBRegisterSlice #(.DOWN_REG(0), .UP_REG(0)) regslice_apb1_root(
		.upstream(apb),
		.downstream(apb1_root));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We only support 32-bit APB, throw synthesis error for anything else

	if(apb.DATA_WIDTH != 32)
		apb_bus_width_is_invalid();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APB bridge for peripherals on this bus

	localparam NUM_PERIPHERALS	= 7;
	localparam BLOCK_SIZE		= 32'h400;
	localparam ADDR_WIDTH		= $clog2(BLOCK_SIZE);
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb1[NUM_PERIPHERALS-1:0]();
	APBBridge #(
		.BASE_ADDR(32'h0000_0000),
		.BLOCK_SIZE(BLOCK_SIZE),
		.NUM_PORTS(NUM_PERIPHERALS)
	) bridge (
		.upstream(apb1_root),
		.downstream(apb1)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO core (c000_0000)

	wire[31:0]	gpioa_out;
	wire[31:0]	gpioa_in;
	wire[31:0]	gpioa_tris;

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_gpioa();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_gpioa(
		.upstream(apb1[0]),
		.downstream(apb_gpioa));

	APB_GPIO gpioA(
		.apb(apb_gpioa),

		.gpio_out(gpioa_out),
		.gpio_in(gpioa_in),
		.gpio_tris(gpioa_tris)
	);

	//tie off unused bits
	assign gpioa_in[23:7] = 0;

	//LEDs
	assign led				= gpioa_out[3:0];
	assign gpioa_in[3:0]	= led;

	//Ethernet
	assign eth_rst_n		= gpioa_out[4];
	assign gpioa_in[4]		= eth_rst_n;
	assign gpioa_in[5]		= rgmii_link_up_core;
	assign gpioa_in[6]		= rx_frame_ready;

	//PMOD GPIOs
	for(genvar g=0; g<8; g++) begin : pmod
		IOBUF iobuf(
			.I(gpioa_out[24+g]),
			.O(gpioa_in[24+g]),
			.T(!gpioa_tris[24+g]),
			.IO(pmod_gpio[g]));
	end

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Device information (c000_0400)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_devinfo();
	APBRegisterSlice #(.DOWN_REG(0), .UP_REG(1)) regslice_apb_devinfo(
		.upstream(apb1[1]),
		.downstream(apb_devinfo));

	APB_DeviceInfo_7series devinfo(
		.apb(apb_devinfo),
		.clk_dna(clk_25mhz),
		.clk_icap(clk_25mhz) );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// MDIO transciever (c000_0800)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_mdio();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_mdio(
		.upstream(apb1[2]),
		.downstream(apb_mdio));

	APB_MDIO #(
		.CLK_DIV(125)
	) mdio (
		.apb(apb_mdio),

		.mdio(eth_mdio),
		.mdc(eth_mdc)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SPI flash controller for boot flash (c000_0c00). For now x1 not quad

	wire	cclk;

	//DQ2 / WP and DQ3 / HOLD aren't used for now, tie high
	IOBUF iobuf_flash_dq3(
		.I(1'b1),
		.O(),
		.T(1'b0),
		.IO(flash_dq[3]));
	IOBUF iobuf_flash_dq2(
		.I(1'b1),
		.O(),
		.T(1'b0),
		.IO(flash_dq[2]));

	//Drive DQ1 / SO to high-Z
	wire	flash_so;
	IOBUF iobuf_flash_dq1(
		.I(1'b0),
		.O(flash_so),
		.T(1'b1),
		.IO(flash_dq[1]));

	//Drive DQ0 / SI with our serial data
	wire	flash_si;
	wire	flash_si_echo;
	IOBUF iobuf_flash_dq0(
		.I(flash_si),
		.O(flash_si_echo),
		.T(1'b0),
		.IO(flash_dq[0]));

	//STARTUP block
	wire	ring_clk;
	STARTUPE2 startup(
		.CLK(ring_clk),
		.GSR(1'b0),
		.GTS(1'b0),
		.KEYCLEARB(1'b1),
		.PACK(1'b0),
		.PREQ(),
		.USRCCLKO(cclk),
		.USRCCLKTS(1'b0),
		.USRDONEO(1'b1),
		.USRDONETS(1'b0),
		.CFGCLK(),
		.CFGMCLK(ring_clk),
		.EOS()
		);

	//SPI bus controller
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_flash();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_flash(
		.upstream(apb1[3]),
		.downstream(apb_flash));

	APB_SPIHostInterface flash(
		.apb(apb_flash),

		.spi_sck(cclk),
		.spi_mosi(flash_si),
		.spi_miso(flash_so),
		.spi_cs_n(flash_cs_n)
	);

	//DEBUG
	ila_0 ila(
		.clk(apb_flash.pclk),
		.probe0(apb_flash.penable),
		.probe1(apb_flash.psel),
		.probe2(apb_flash.pready),
		.probe3(apb_flash.paddr),
		.probe4(apb_flash.pwdata),
		.probe5(apb_flash.prdata),
		.probe6(cclk),
		.probe7(flash_si),
		.probe8(flash_so),
		.probe9(flash_cs_n),
		.probe10(flash.shift_en),
		.probe11(flash.shift_data),
		.probe12(flash.shift_busy),
		.probe13(flash.burst_busy),
		.probe14(flash.shift_done),
		.probe15(flash.burst_wptr),
		.probe16(flash.burst_wr),
		.probe17(flash.rx_data),

		.probe18(flash.clkdiv),
		.probe19(apb_flash.pwrite),
		.probe20(apb1[3].penable),
		.probe21(apb1[3].psel),
		.probe22(apb.penable),
		.probe23(apb.psel),
		.probe24(flash.spi.toggle),
		.probe25(flash.spi.active),
		.probe26(flash.spi.clkcount)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// XADC for on-die sensors (c000_1000)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_xadc();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_xadc(
		.upstream(apb1[4]),
		.downstream(apb_xadc));

	APB_XADC xadc(
		.apb(apb_xadc)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Curve25519 crypto_scalarmult accelerator (c000_1400)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) cryptBus();

	APBRegisterSlice #(.UP_REG(1), .DOWN_REG(0))
		apb_regslice_crypt( .upstream(apb1[5]), .downstream(cryptBus) );

	APB_Curve25519 crypt25519(
		.apb(cryptBus)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RGB LED controller (c000_1800)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) ledBus();

	APBRegisterSlice #(.UP_REG(1), .DOWN_REG(0))
		apb_regslice_led( .upstream(apb1[6]), .downstream(ledBus) );

	APB_SerialLED #(
		.NUM_LEDS(6),
		.SHORT_TIME(30),	//Number of PCLK cycles in a "short" pulse (300 ns)
		.LONG_TIME(90),		//Number of PCLK cycles in a "long" pulse (900 ns)
		.IFG_TIME(200),		//Number of PCLK cycles between data words
		.RESET_TIME(750)	//Number of PCLK cycles in a reset pulse (>50us, do 75 to be safe)
	)  rgbled (
		.apb(ledBus),

		.led_ctrl(led_ctrl)
	);

endmodule

