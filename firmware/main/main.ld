MEMORY
{
	FMC_APB1(RW):	ORIGIN = 0xc0000000, LENGTH = 64K
	FMC_APB2(RW):	ORIGIN = 0xc0010000, LENGTH = 64K

	FMC_APB3(RW):	ORIGIN = 0xc0020000, LENGTH = 64K
	FMC_APB4(RW):	ORIGIN = 0xc0030000, LENGTH = 64K
	FMC_APB5(RW):	ORIGIN = 0xc0040000, LENGTH = 64K
	FMC_APB6(RW):	ORIGIN = 0xc0050000, LENGTH = 64K
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

		/* Crypto accelerator */
		. = ALIGN(1024);
		KEEP (*(.fcurve25519))

		/* RGB LEDs */
		. = ALIGN(1024);
		KEEP (*(.frgbled))

	} > FMC_APB1

	.sfr_fpga_apb2  (NOLOAD):
	{
		. = ALIGN(4096);
		KEEP (*(.fethrx))
		. = ALIGN(4096);
		KEEP (*(.fethtx))

		/*TODO after here*/
	} > FMC_APB2

	/* Memory controllers on 1V2 port */
	.sfr_fpga_apb3  (NOLOAD):
	{
	} > FMC_APB3

	/* Memory controllers on 1V8 port */
	.sfr_fpga_apb4  (NOLOAD):
	{
	} > FMC_APB4

	/* Memory controllers on 2V5 port */
	.sfr_fpga_apb5  (NOLOAD):
	{
	} > FMC_APB5

	/* Memory controllers on 3V3 port */
	.sfr_fpga_apb6  (NOLOAD):
	{
	} > FMC_APB6
}
