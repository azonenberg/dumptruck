* Target power budget: 900 mA @ 5V using USB3 host on type-C connector 4.5W
	* KSZ9031 assuming 2.5V AVDDH: 452 mW
		* 1.2V @ 221 mA = 262 mW
		* 1.8V @ 23.6 mA = 43 mW
		* ONE OF
			* 2.5V @ 58.8 mA = 147 mW
			* 3.3V @ 67.4 mA = 222 mW
	* STM32H735:
		* Worst case if using SMPS: 3.3V @ 130 mA = 429 mW
	* Whatever the DUT uses
		* Random modern NAND (per die) total 546 mW
			* 2.5 or 3.3V Vcc @ 100 mA = 330 mW
			* 1.2V Vccq @  180 mA = 216 mW
	* So far we're at total of 1.42W plus conversion losses
	* XC7K160T minimum power: 2.81W:
		* 474 mA @ 1.0V = 474 mW VCCINT (plus another 550 mA max during boot = 1.02W)
		* 40 mA @ 1.8V = 72 mW VCCAUX (+50 mA max during boot = 162 mW)
		* 2 mA @ 1.8V = 3.6 mW VCCAUX_IO (+40 mA *per bank* during boot = 579 mW)
		* VCCIO mostly negligible (but + 40 mA per bank during boot)
			* If we assume worst case 3.3V this is another 1.05W
		* Plus whatever design itself uses
		* EDIT: this also didn't account for VCCBRAM
		* Per UG476: if all GTX quads are not used, can float or ground supply pins
			* MGTAVCC
			* MGTAVTT
			* MGTAVCCAUX
		* So all told 4.23W power budget plus conversion losses and VCCBRAM. Seems awfully tight for non-PD USB
	* Alternate FPGA option: XC7S100-1FGGA484I
		* Has 7 IO banks vs 8
		* 16K slices vs 25K
		* 120 BRAMs vs 325 (still has 480 kB SRAM)
		* Both have 8 CMTs, we dont care about DSPs
		* No GTX but we weren't gonna use those anyway
		* Should be more than big enough
		* Minimum power: 1.78W 
			* 148 + 9mA @ 1V0 + 300 + 60 during boot = 517 mW
			* 43 mA @ 1V8 + 140 during boot = 329 mW
			* No GTX supplies
			* No VCXAUX_IO
			* 4 mA VCCO + 40 mA per bank = 284 mA
				* Assume worst case 3.3V this is 937 mW
		* So assuming design doesn't use much beyond boot surge, total power budget is 3.2W plus conversion losses. Comfortably fits in 4.5W target
* Data link to host PC: 1000baseT via KSZ9031
* Xilinx 7 series FPGA