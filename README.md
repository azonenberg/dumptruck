# dumptruck
FPGA based dumper for flashes and more

## ERRATA

Bugs in the v0.1 PCB

* 1.0V shunt resistor is missing a zone fill. Will monitor droop under load and see if we need to add top layer power path.
* U33 is way too close to J10. Seems to have fit OK, not touching for now, but tough to assemble.
* INIT_B and PROG_B on FPGA are 1.8V signals but we try to drive with 3.3 from supervisor! Rework: add pull to 1.8V on board, open drain in super firmware
* JTAG connector VCC is connected to 3.3V. We need to supply 1.8 here. Temporary workaround: six pin extension cable, pass through JTAG + GND and supply 1.8V to dongle externally. Long term fix: interposer board
* DONE on FPGA is 1.8V signal driving 3.3V input on supervisor, too low for Vih. Rework: add 1.8V gate drive NMOS to pull low when done, invert logic in firmware
