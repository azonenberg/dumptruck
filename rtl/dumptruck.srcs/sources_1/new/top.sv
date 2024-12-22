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

module top(

	//System clock input
	input wire			clk_25mhz,

	//FMC pins to MCU for APB interface
	input wire			fmc_clk,
	output wire			fmc_nwait,
	input wire			fmc_noe,
	inout wire[15:0]	fmc_ad,
	input wire			fmc_nwe,
	input wire[1:0]		fmc_nbl,
	input wire			fmc_nl_nadv,
	input wire[9:0]		fmc_a_hi,
	input wire			fmc_ne3,
	input wire			fmc_ne2,
	input wire			fmc_ne1,

	//GPIO LEDs
	output wire[3:0]	led,
	output wire			led_ctrl,

	//RGMII PHY
	output wire			rgmii_tx_clk,
	output wire			rgmii_tx_en,
	output wire[3:0]	rgmii_txd,
	input wire			rgmii_rx_clk,
	input wire			rgmii_rx_dv,
	input wire[3:0]		rgmii_rxd,

	//MDIO and control lines to PHY
	inout wire			eth_mdio,
	output wire			eth_mdc,
	output wire			eth_rst_n,
	input wire[1:0]		eth_led_in,
	output wire[1:0]	eth_led_out,

	//PMOD GPIO
	inout wire[7:0]		pmod_gpio,

	//QSPI flash lines
	inout wire[3:0]		flash_dq,
	output wire			flash_cs_n,
	//flash SCK is CCLK pin from STARTUPE2

	//HyperRAM
	output wire			ram_rst_n,
	inout wire[7:0]		ram_dq,
	output wire			ram_clk_p,
	output wire			ram_clk_n,
	output wire			ram_clk_psc_p,
	output wire			ram_clk_psc_n,
	output wire			ram_cs_n,
	inout wire			ram_dqs,

	//DUT socket ports
	inout wire[49:0]	bank16_io_3v3,
	inout wire[49:0]	bank36_io_2v5,
	inout wire[49:0]	bank35_io_1v8,
	inout wire[49:0]	bank34_io_1v2
);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Buffer and level shift LEDs

	assign eth_led_out = ~eth_led_in;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Clock synthesis

	wire	clk_crypt;
	wire	clk_125mhz;
	wire	clk_250mhz;

	ClockGeneration clocks(
		.clk_25mhz(clk_25mhz),

		.clk_crypt(clk_crypt),
		.clk_125mhz(clk_125mhz),
		.clk_250mhz(clk_250mhz)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FMC to APB bridge

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(20), .USER_WIDTH(0)) fmc_apb();

	FMC_APBBridge #(
		.CLOCK_PERIOD(10),	//100 MHz
		.VCO_MULT(10),		//1 GHz VCO
		.CAPTURE_CLOCK_PHASE(-30),
		.LAUNCH_CLOCK_PHASE(-30)
	) fmcbridge(
		.apb(fmc_apb),

		.clk_mgmt(clk_125mhz),

		.fmc_clk(fmc_clk),
		.fmc_nwait(fmc_nwait),
		.fmc_noe(fmc_noe),
		.fmc_ad(fmc_ad),
		.fmc_nwe(fmc_nwe),
		.fmc_nbl(fmc_nbl),
		.fmc_nl_nadv(fmc_nl_nadv),
		.fmc_a_hi(fmc_a_hi),
		.fmc_cs_n(fmc_ne1)
	);

	//Pipeline stage for timing
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(20), .USER_WIDTH(0)) fmc_apb_pipe();
	APBRegisterSlice #(.DOWN_REG(0), .UP_REG(1)) regslice_apb_root(
		.upstream(fmc_apb),
		.downstream(fmc_apb_pipe));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APB root bridge

	//TODO: very large segment for hyperram? or do we want to add AHB or something for that?

	//Two 16-bit bus segments at 0xc000_0000 (APB1) and c001_0000 (APB2) for core peripherals
	//APB1 has 1 kB address space per peripheral and is for small stuff
	//APB2 has 4 kB address space per peripheral and is only used for Ethernet
	//APB3/4/5/6 segments are allocated to each of the I/O ports and contain all of the flash controllers etc
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(16), .USER_WIDTH(0)) rootAPB[5:0]();

	//Root bridge
	APBBridge #(
		.BASE_ADDR(32'h0000_0000),	//MSBs are not sent over FMC so we set to zero on our side
		.BLOCK_SIZE(32'h1_0000),
		.NUM_PORTS(6)
	) root_bridge (
		.upstream(fmc_apb_pipe),
		.downstream(rootAPB)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Peripherals on APB1 bus (0x0xc000_0000)

	Peripherals_APB1 apb1(
		.apb(rootAPB[0]),

		.clk_25mhz(clk_25mhz),
		.clk_crypt(clk_crypt),

		.led(led),
		.led_ctrl(led_ctrl),

		.eth_mdio(eth_mdio),
		.eth_mdc(eth_mdc),
		.eth_rst_n(eth_rst_n),

		.pmod_gpio(pmod_gpio),

		.flash_dq(flash_dq),
		.flash_cs_n(flash_cs_n),

		.rgmii_link_up_core(rgmii_link_up_core),
		.rx_frame_ready(rx_frame_ready)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Peripherals on APB2 bus (0xc001_0000)

	wire	rgmii_link_up_core;
	wire	rx_frame_ready;

	Peripherals_APB2 apb2(
		.apb(rootAPB[1]),

		.clk_125mhz(clk_125mhz),
		.clk_250mhz(clk_250mhz),

		.rgmii_tx_clk(rgmii_tx_clk),
		.rgmii_tx_en(rgmii_tx_en),
		.rgmii_txd(rgmii_txd),

		.rgmii_rx_clk(rgmii_rx_clk),
		.rgmii_rx_dv(rgmii_rx_dv),
		.rgmii_rxd(rgmii_rxd),

		.rgmii_link_up_core(rgmii_link_up_core),
		.rx_frame_ready(rx_frame_ready)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.2V flash on APB3 bus (0xc002_0000)

	TargetChannel apb3_flash_1v2(.apb(rootAPB[2]), .bank_io(bank34_io_1v2));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.8V flash on APB4 bus (0xc003_0000)

	TargetChannel apb4_flash_1v8(.apb(rootAPB[3]), .bank_io(bank35_io_1v8));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2.5V flash on APB5 bus (0xc004_0000)

	TargetChannel apb5_flash_2v5(.apb(rootAPB[4]), .bank_io(bank36_io_2v5));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3.3V flash on APB6 bus (0xc005_0000)

	TargetChannel #(.DEBUG(1)) apb6_flash_3v3(.apb(rootAPB[5]), .bank_io(bank16_io_3v3));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Placeholder RAM controller so we can constrain IOs

	HyperRAMController ram_dummy(
		.ram_clk_p(ram_clk_p),
		.ram_clk_n(ram_clk_n),
		.ram_clk_psc_p(ram_clk_psc_p),
		.ram_clk_psc_n(ram_clk_psc_n),
		.ram_dqs(ram_dqs),
		.ram_dq(ram_dq),
		.ram_cs_n(ram_cs_n),
		.ram_rst_n(ram_rst_n)
	);


endmodule
