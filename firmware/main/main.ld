MEMORY
{
	FMC_APB1(RW):	ORIGIN = 0xc0000000, LENGTH = 64K
	FMC_APB2(RW):	ORIGIN = 0xc0010000, LENGTH = 64K
}

SECTIONS
{
	.sfr_fpga_apb1  (NOLOAD):
	{
		. = ALIGN(1024);
		KEEP (*(.fgpioa))
		. = ALIGN(1024);
		KEEP (*(.fdevinfo))
		. = ALIGN(1024);
		KEEP (*(.fmdio))
		. = ALIGN(1024);
		KEEP (*(.fspi1))
		. = ALIGN(1024);
		KEEP (*(.fxadc))

		/*TODO after here*/
	} > FMC_APB1

	.sfr_fpga_apb2  (NOLOAD):
	{
		. = ALIGN(4096);
		KEEP (*(.fethrx))
		. = ALIGN(4096);
		KEEP (*(.fethtx))

		/*TODO after here*/
	} > FMC_APB2
}
