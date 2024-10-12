## Physical floorplan
* USB-C and Ethernet connectors along east edge
* FPGA near center with A1 in northwest corner
* MCU northeast
* Ethernet and boot flash southeast
* DUT sockets to west. May have overlapping mounting footprints as only one socket of the four will be mated at any given time
* Debug connectors along south edge
	* Bank 13 unused so can easily escape JTAG through this region
* Probably want power regulation in southeast
	* Can have a 3.3V pour snake around the edge of the board to get to the MCU area
	* Easy access to PHY and FPGA center
	* Maybe have 2.5V hook north and west to get to the bank there
## Fab / stackup
* We have a lot of power domains that need to get around
	* 3.3V only used by northeast area (MCU + DUT bank 16 IOs)
	* 2.5V used by northwest corner of FPGA plus PHY
		* Unless we run the PHY AVDDH at 3.3?
		* That might be easier since we have 3.3 in that general area from the MCU already, but will cost another ~75 mW of power
	* 1.8V used by center of FPGA, Ethernet, boot flash
	* 1.2V used by southwest corner of FPGA and Ethernet
	* 1.0V used by FPGA core only, feed in from south under bank 13 since no IOs there?
* Minimum one dedicated power layer but ideally would like some power on another layer
* Staying 6L would be nice, but tricky
	* That would allow oshpark 6L, this is slow enough fiber weave effect from the 106 PP is no factor
	* Possible stackup: SGS SGS with power pours on L3/L4 around signals?
		* Not great from an impedance matching perspective since the ground will couple and act as a CPWG but not provide a clean return path
		* Can't get large clearances from signals because of the BGA via field
* 8L probably better?
	* Possible stackup: SGS PP SGS
	* I really don't think 10L SGS GPPG SGS is necessary for this, but we do want four signal layers