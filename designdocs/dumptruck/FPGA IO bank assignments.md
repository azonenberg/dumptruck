## Available banks

We have seven banks available: 13, 14, 15, 16, 34, 35, 36 plus config bank 0.

All banks are HR and generally drop-in interchangeable, EXCEPT:
* QSPI flash pins are in bank 14
* Bank 13 has only 38 pins of 50 bonded out
## Requirements
* DUT sockets
	* Dedicate one full bank to each socket
	* Don't yet know how many pins we'll actually break out
* Boot flash
	* Bank 14, LVCMOS18, needs 4x SPI data + CS. CCLK is in bank 0
	* This means bank 0 must be LVCMOS18 as well (i.e. JTAG is 1.8V)
* HyperRAM
	* 12 pins, LVCMOS18
* Ethernet
	* 14 pins for RGMII + MDIO, assume reset driven by MCU not FPGA
	* Can do whatever voltage, but LVCMOS18 is least power
* FPGA - MCU interface
	* All LVCMOS33 to interface to MCU at full voltage
	* If we hook up optional addr/enable bits, we get 53 pins
		* If we only route one chip select (should be fine as this is what our normal FPGA IP uses) we can use 50. Fully saturate one IO bank.
	* Signals:
		* clk
		* nwait
		* noe
		* ad\[15:0\]
		* nwe
		* nbl\[1:0\]
		* nl_nadv
		* addr_hi\[25:0\]
			* Max of 26 address lines available, 15:0 are shared with AD so we can add up to another 10. Pin out as many as we can comfortably route
		* ne\[3:0\]
			* ne1 is the most important if we are using a standard config
			* Can't hurt to hook up the others if we have sufficient pins
## Tentative bank assignments
* Bank 13: 1.8V, 0 / 38 used
* Bank 14: 1.8V, 32 / 50 used
	* Boot flash (5)
	* HyperRAM (12)
	* RGMII (14 including 1 clock)
	* 25 MHz system primary clock
* Bank 15: 3.3V, 50 / 50 used
	* FMC interface
* Bank 16: 3.3V, ?? / 50 used
	* DUT socket
* Bank 36: 2.5V, ?? / 50 used
	* DUT socket
* Bank 35:  1.8V, ?? / 50 used
	* DUT socket
* Bank 34: 1.2V, ?? / 50 used
	* DUT socket

Rationale:
* Bank 14 must be 1.8V for boot flash, can shove basically all other non-MCU IO there
* Bank 13 has less pins, leave it for overflow capacity
* We want the DUT sockets in adjacent banks for routability. With 14 occupied by control stuff and 13 too small, we need to choose 4 out of 5 (34, 35, 36, 16, 15) as the DUT-facing banks. Using 15 as the other control bank seems logical so all control logic is on the same side of the FPGA.
* Bank 15 is 100% used for MCU interface and runs at 3.3V. Putting the 3.3V DUT bank in 16 simplifies power routing.
* Bank 36 is off in a corner so easier to route odd power voltage to
* Bank 35 is directly across from 14 separated by the FPGA core. Both banks, plus VCCAUX, need 1.8V so we can route a 1.8V polygon all the way across.
* Bank 34 is also in a corner so easy to route an odd voltage to