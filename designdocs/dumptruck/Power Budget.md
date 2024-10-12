## Breakdown

Target power budget: 900 mA @ 5V using USB3 host on type-C connector 4.5W

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
* XC7K160T considered since i had some gathering dust, but would be over power budget
* XC7S100-1FGGA484I: 1.78W plus whatever design uses beyond boot surge
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
* Maybe use a bit more for a HyperRAM on the STM32 if we wanted to play with that for lulz
* Add the usual small STM32L431 for rail sequencing and reset, this doesn't need much juice
* FPGA boot flash is negligible
## Rail Targets
* 1V0: 1A (~500 mA FPGA)
* 1V2: 1A (221 mA PHY + one FPGA VCCO bank)
* 1V8: 1A (180 mA FPGA + PHY IO + one FPGA VCCO bank)
* 2V5: 1A (60 mA PHY + one FPGA VCCO bank)
* 3V3: 1A (130 mA MCU + one FPGA VCCO bank)

Plus whatever the DUT actually needs on Vcore but we assume that's small
* Use MUN12AD03-SE for each rail? Easy and tiny, but really wants 12V in and has minimum Vin of 4.5V. Won't handle Vbus droop well.
* Use LTC3374A? Might be a bit noisier but this is a digital board without tight ripple requirements. It can do every rail comfortably, we can gang several phases to improve efficiency. This is probably best bet.