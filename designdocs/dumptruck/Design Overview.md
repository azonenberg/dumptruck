* Powered by USB-C, CC pin tieoff for 900 mA power budget @ 5V, no PD, no USB data
	* One LTC3374A supplies all rails. 8 outputs among 4 phases
		* Have 3 in stock
	* Always-on 3V3_SB rail
	* Remainder (1V0, 1V2, 1V8, 2V5, 3V3) sequenced by supervisor
* STM32L431 rail sequencer / reset supervisor
	* QFN32 or QFN48 package depending on IO needs
	* GPIO for soft reset, power on/off
	* Have 7 or 8 in inventory depending on which package we go with
* Xilinx XC7S100-1FGGA484C
	* Have 3 in stock
	* Data link to host PC: 1000baseT via KSZ9031RNX
		* Have 13 in stock
	* HyperRAM footprint in case we want buffer on the FPGA
		* Infineon S27KS0642GABHI030
			* HyperBUS, 1.8V only, 200 MHz, 64 Mbit
			* Have 10 in stock
	* Boot flash: need minimum 32 Mbit
		* ISSI IS25WP256D-JLLE
			* 256 Mbit so room for multi-boot images
			* QSPI, 1.8V only, 8x6 WSON
			* Have 4 in stock
* STM32H735 as main processor
	* Have 9 in stock
	* HyperRAM in case we want buffer on the MCU?
		* Will need to account for OctoSPI errata, but this is probably OK if we're only using it for bulk data transfers.
		* Do not have any 3.3V HyperRAM in inventory. Leave DNP'd footprint
	* FMC to FPGA as primary I/O interface
	* AT24MAC402 for MAC address
		* Have a ton of these
* Four separate socket connectors (interface TBD, 0.1" header or something else?) for DUT ZIF sockets / BGA breakouts. Each one goes to a dedicated FPGA IO bank running at the target VCCO
* External 0.1" header with jumpers for muxing one of the four suitable IO rails as DUT Vcore
	* Can also feed externally generated Vcore in for unusual voltage requirements
	* No on-board variable Vcore supply
	* ADC input from Vcore to MCU so we know which is selected
* Software controlled load switch from muxed Vcore to DUT socket using NCP45560IMNTWG-H
	* Slew rate controlled, load discharge, active high enable
	* Requires external 3.3V supply
	* Can switch loads from 0.5 to 13.5V
	* Have 12 in stock
	* Max rating 17A, total overkill, but hey I've got them on the shelf