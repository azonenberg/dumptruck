* Samtec BSE-040 (80 contacts total)
* 8x support signals
	* 2x vcore
	* 2x vccio
	* Presence detect: pulled weakly low on host, tie to 3v3 on socket
	* 3v3 for ID EEPROM
	* I2C for ID EEPROM
* 50x GPIOs
* 22 grounds

## Pinout
1-14 support stuff, that leaves 66 left
50x GPIO + 16 grounds = one ground per 3.125 signals