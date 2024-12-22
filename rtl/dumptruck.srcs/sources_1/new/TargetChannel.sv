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

/**
	@brief Controller for a single flash socket channel
 */
module TargetChannel#(
	parameter DEBUG = 0
)(

	//APB to root bridge
	APB.completer 		apb,

	//IOs for the flash chip
	inout wire[49:0]	bank_io
);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pipeline register heading up to root bridge

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(16), .USER_WIDTH(0)) apb_root();
	APBRegisterSlice #(.DOWN_REG(0), .UP_REG(0)) regslice_apb_root(
		.upstream(apb),
		.downstream(apb_root));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// We only support 32-bit APB, throw synthesis error for anything else

	if(apb.DATA_WIDTH != 32)
		apb_bus_width_is_invalid();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APB bridge for peripherals on this bus

	localparam NUM_PERIPHERALS	= 6;
	localparam BLOCK_SIZE		= 32'h400;
	localparam ADDR_WIDTH		= $clog2(BLOCK_SIZE);
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_int[NUM_PERIPHERALS-1:0]();
	APBBridge #(
		.BASE_ADDR(32'h0000_0000),
		.BLOCK_SIZE(BLOCK_SIZE),
		.NUM_PORTS(NUM_PERIPHERALS)
	) bridge (
		.upstream(apb_root),
		.downstream(apb_int)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Bidirectional buffer for I/O ports

	wire[49:0]	io_in;
	logic[49:0]	io_out;
	logic[49:0]	io_tris;

	for(genvar i=0; i<50; i = i+1) begin: buffers
		IOBUF iobuf(
			.I(io_out[i]),
			.IO(bank_io[i]),
			.O(io_in[i]),
			.T(io_tris[i])
		);
	end

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I/O muxing selector (0xc00x_0000)

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_muxsel();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(0)) regslice_apb_muxsel(
		.upstream(apb_int[0]),
		.downstream(apb_muxsel));

	wire[3:0]	muxsel;
	IOMuxConfig muxcfg(
		.apb(apb_muxsel),
		.muxsel(muxsel)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO for chip selects and such (0xc00x_0400)

	wire[31:0]	gpio_out;
	wire[31:0]	gpio_in;
	wire[31:0]	gpio_tris;

	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_gpio();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_gpio(
		.upstream(apb_int[1]),
		.downstream(apb_gpio));

	APB_GPIO gpio(
		.apb(apb_gpio),

		.gpio_out(gpio_out),
		.gpio_in(gpio_in),
		.gpio_tris(gpio_tris)
	);

	//for now, tie off inputs
	assign gpio_in = 32'h0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Quad SPI controller (0xc00x_0800)

	//SPI bus controller
	APB #(.DATA_WIDTH(32), .ADDR_WIDTH(ADDR_WIDTH), .USER_WIDTH(0)) apb_spi();
	APBRegisterSlice #(.DOWN_REG(1), .UP_REG(1)) regslice_apb_spi(
		.upstream(apb_int[2]),
		.downstream(apb_spi));

	wire		qspi_sck;
	wire[3:0]	qspi_dq_out;
	wire[3:0]	qspi_dq_in;
	wire[3:0]	qspi_dq_tris;
	wire		qspi_cs_n;

	APB_QSPIHostInterface spi(
		.apb(apb_spi),

		.qspi_sck(qspi_sck),
		.qspi_dq_out(qspi_dq_out),
		.qspi_dq_in(qspi_dq_in),
		.qspi_dq_tris(qspi_dq_tris),
		.qspi_cs_n(qspi_cs_n)
	);

	//Inputs
	assign qspi_dq_in[0] = io_in[2];
	assign qspi_dq_in[1] = io_in[3];
	assign qspi_dq_in[2] = io_in[1];
	assign qspi_dq_in[3] = io_in[4];

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I/O muxing

	typedef enum logic[3:0]
	{
		//Not using this port, all IOs tristated
		MUX_SEL_INACTIVE	= 0,

		//x4 SPI (peripheral may be used in x1 or x4 mode)
		MUX_SEL_X4_SPI		= 1
	} muxsel_t;

	always_comb begin

		//Default to floating all IOs
		io_out	= 50'h0;
		io_tris	= {50{1'b1}};

		case(muxsel)

			//Nothing active
			MUX_SEL_INACTIVE: begin
			end

			//x4 SPI
			MUX_SEL_X4_SPI: begin

				//IO0 is clock
				io_out[0]	= qspi_sck;
				io_tris[0]	= 1'h0;

				//IO1 is WP#/IO2
				io_out[1]	= qspi_dq_out[2];
				io_tris[1]	= qspi_dq_tris[2];

				//IO2 is DI/IO0
				io_out[2]	= qspi_dq_out[0];
				io_tris[2]	= qspi_dq_tris[0];

				//IO3 is DO/IO1
				io_out[3]	= qspi_dq_out[1];
				io_tris[3]	= qspi_dq_tris[1];

				//IO4 is HOLD# / IO3, tie high
				io_out[4]	= qspi_dq_out[3];
				io_tris[4]	= qspi_dq_tris[3];

				//IO5 is CS#
				io_out[5]	= qspi_cs_n;
				io_tris[5]	= 1'h0;

			end

			//Tristate in case of invalid mux selector
			default: begin
			end
		endcase

	end

endmodule
