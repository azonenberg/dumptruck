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

module Peripherals_APB2(

	//APB to root bridge
	APB.completer 			apb,
	APB.completer			apb64,

	//Clocks from top level PLL
	input wire				clk_125mhz,
	input wire				clk_250mhz,

	//RGMII PHY
	output wire				rgmii_tx_clk,
	output wire				rgmii_tx_en,
	output wire[3:0]		rgmii_txd,
	input wire				rgmii_rx_clk,
	input wire				rgmii_rx_dv,
	input wire[3:0]			rgmii_rxd,

	//Status outputs to GPIO core on APB1
	output wire				rgmii_link_up_core,
	output wire				rx_frame_ready
);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pipeline register heading up to root bridge

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(16), .USER_WIDTH(0)) apb2_root();
	APBRegisterSlice #(.DOWN_REG(0), .UP_REG(0)) regslice_apb2_root(
		.upstream(apb),
		.downstream(apb2_root));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We only support 32-bit APB, throw synthesis error for anything else

	if(apb.DATA_WIDTH != 32)
		apb_bus_width_is_invalid();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APB bridge for peripherals on this bus

	//APB2
	localparam NUM_PERIPHERALS	= 2;
	localparam BLOCK_SIZE		= 32'h1000;
	localparam ADDR_WIDTH		= $clog2(BLOCK_SIZE);
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb2[NUM_PERIPHERALS-1:0]();
	APBBridge #(
		.BASE_ADDR(32'h0000_0000),
		.BLOCK_SIZE(BLOCK_SIZE),
		.NUM_PORTS(NUM_PERIPHERALS)
	) bridge (
		.upstream(apb2_root),
		.downstream(apb2)
	);

	//APB64
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb2_64[NUM_PERIPHERALS-1:0]();
	APBBridge #(
		.BASE_ADDR(32'h0000_0000),
		.BLOCK_SIZE(BLOCK_SIZE),
		.NUM_PORTS(NUM_PERIPHERALS)
	) bridge64 (
		.upstream(apb64),
		.downstream(apb2_64)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ethernet MAC

	wire			mgmt0_rx_clk;
	EthernetRxBus	mgmt0_rx_bus;
	EthernetTxBus	mgmt0_tx_bus;
	wire			mgmt0_tx_ready;
	wire			mgmt0_link_up;
	lspeed_t		mgmt0_link_speed;

	RGMIIMACWrapper #(
		.CLK_BUF_TYPE("LOCAL"),
		.PHY_INTERNAL_DELAY_RX(1)
	) port_mgmt0 (
		.clk_125mhz(clk_125mhz),
		.clk_250mhz(clk_250mhz),

		.rgmii_rxc(rgmii_rx_clk),
		.rgmii_rxd(rgmii_rxd),
		.rgmii_rx_ctl(rgmii_rx_dv),

		.rgmii_txc(rgmii_tx_clk),
		.rgmii_txd(rgmii_txd),
		.rgmii_tx_ctl(rgmii_tx_en),

		.mac_rx_clk(mgmt0_rx_clk),
		.mac_rx_bus(mgmt0_rx_bus),

		.mac_tx_bus(mgmt0_tx_bus),
		.mac_tx_ready(mgmt0_tx_ready),

		.link_up(mgmt0_link_up),
		.link_speed(mgmt0_link_speed)
		);

	ThreeStageSynchronizer sync_rgmii_link_up(
		.clk_in(mgmt0_rx_clk), .din(mgmt0_link_up), .clk_out(apb.pclk), .dout(rgmii_link_up_core));

	EthernetRxBus	cdc_rx_bus;
	EthernetRxClockCrossing eth_rx_cdc(
		.gmii_rxc(mgmt0_rx_clk),
		.mac_rx_bus(mgmt0_rx_bus),

		.sys_clk(apb.pclk),
		.rst_n(apb.preset_n),
		.cdc_rx_bus(cdc_rx_bus),

		.perf_rx_cdc_frames()
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ethernet RX FIFO (c001_0000)

	APB_EthernetRxBuffer_x32 eth_rx_fifo(
		.apb(apb2[0]),
		.eth_rx_bus(cdc_rx_bus),
		.eth_link_up(rgmii_link_up_core),

		.rx_frame_ready(rx_frame_ready)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ethernet TX FIFO (c080_0000)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_tx_fifo();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(0)) regslice_apb_tx_fifo(
		.upstream(apb2_64[0]),
		.downstream(apb_tx_fifo));

	wire			fifo_tx_ready;
	EthernetTxBus	fifo_tx_bus;

	APB_EthernetTxBuffer_x32_1G eth_tx_fifo(
		.apb(apb_tx_fifo),
		.tx_clk(clk_125mhz),
		.tx_bus(fifo_tx_bus),
		.tx_ready(fifo_tx_ready),
		.link_up_pclk(rgmii_link_up_core)
	);

	EthernetChecksumOffload eth_offload(
		.clk(clk_125mhz),
		.link_up(rgmii_link_up_core),
		.buf_tx_ready(fifo_tx_ready),
		.buf_tx_bus(fifo_tx_bus),
		.mac_tx_ready(mgmt0_tx_ready),
		.mac_tx_bus(mgmt0_tx_bus)
	);

endmodule
