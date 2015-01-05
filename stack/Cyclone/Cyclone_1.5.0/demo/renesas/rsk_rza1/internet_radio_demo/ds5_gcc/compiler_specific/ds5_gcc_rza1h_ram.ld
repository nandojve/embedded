OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(start)

/* Base Address RAM Memory Table 10-Mbyte on-chip RAM */  
/* Total RAM available 10112K */
MEMORY {
	RAM0L (rwx)   : ORIGIN = 0x20020000, LENGTH = 0x000E0000  /* Page 0 Lower bank (1024KB) H'20000000 to H'200FFFFF */
	RAM1L (rwx)   : ORIGIN = 0x20100000, LENGTH = 0x00100000  /* Page 1 Upper bank (1024KB) H'20100000 to H'201FFFFF */
	RAM2L (rwx)   : ORIGIN = 0x20200000, LENGTH = 0x00100000  /* Page 2 Upper bank (1024KB) H'20200000 to H'202FFFFF */
	RAM3L (rwx)   : ORIGIN = 0x20300000, LENGTH = 0x00100000  /* Page 3 Upper bank (1024KB) H'20300000 to H'203FFFFF */
	RAM4L (rwx)   : ORIGIN = 0x20400000, LENGTH = 0x00100000  /* Page 4 Upper bank (1024KB) H'20400000 to H'204FFFFF */
	RAM0U (rwx)   : ORIGIN = 0x20500000, LENGTH = 0x00100000  /* Page 0 Upper bank (1024KB) H'20500000 to H'205FFFFF */
	RAM1U (rwx)   : ORIGIN = 0x20600000, LENGTH = 0x00100000  /* Page 1 Upper bank (1024KB) H'20600000 to H'206FFFFF */
	RAM2U (rwx)   : ORIGIN = 0x20700000, LENGTH = 0x00100000  /* Page 2 Upper bank (1024KB) H'20700000 to H'207FFFFF */
	RAM3U (rwx)   : ORIGIN = 0x20800000, LENGTH = 0x00100000  /* Page 3 Upper bank (1024KB) H'20800000 to H'208FFFFF */
	RAM4U (rwx)   : ORIGIN = 0x20900000, LENGTH = 0x00100000  /* Page 4 Upper bank (1024KB) H'20900000 to H'209FFFFF */

	RAM2U_M (rwx) : ORIGIN = 0x60500000, LENGTH = 0x00100000  /* Page 2 Upper bank  mirrored (1024KB) H'605FFFFF to H'207FFFFF */
}

PROGRAM_STACK_SIZE    = 0x8000; 	/* Application stack                  */
IRQ_STACK_SIZE	      = 0x2000;     /* IRQ mode stack                     */
FIQ_STACK_SIZE	      = 0x2000;     /* FRQ mode stack                     */
SVC_STACK_SIZE	      = 0x2000;     /* SVC mode stack                     */
ABT_STACK_SIZE	      = 0x2000;     /* ABT mode stack                     */
TTB_SIZE              = 0x8000;     /* Level-1 Translation Table for MMU  */

MIRRORED_VECTOR_TABLE = 0x20020000; /* VECTOR_MIRROR_TABLE located here */	 

SECTIONS
{
    .mvt MIRRORED_VECTOR_TABLE :
    {
         mvt_start = .;
         *mirrortable.o (.text)
         . = ALIGN(0x4);
         *(.text)
    } >RAM0L

    .CODE_RESET :
    {
        _sCODE_RESET = .;
        *(.CODE_RESET)
        _eCODE_RESET = .;
    } >RAM0L 

	.data :
	{
        _sdata = .;
	    *(.data)
		_data = .;
		_edata = .;
	} >RAM1L
 
	.DATA_RESET :
	{
		_sDATA_RESET = .;
	    *(.DATA_RESET)
		_eDATA_RESET = .;
	} >RAM0L 

	.rodata : 
	{
		_srodata = .;
		*(.rodata)
		*(.rodata.*)
		.	= ALIGN(0x4);
		_erodata = .;
	} >RAM1L

    .bss :
    {
         _bss = .;
         PROVIDE(__bss_start__ = .);         
         *(.bss)
         *(.bss.**)
         *(COMMON)
         PROVIDE(__bss_end__ = .);
         _ebss = .;
         _end = .;
         PROVIDE(end = .);
    } >RAM1L

    .BSS_RESET : 
    {
        .   = ALIGN(0x4);
        PROVIDE (__start_BSS_RESET = .);
        *(.BSS_RESET)
        PROVIDE (__stop_BSS_RESET = .);
        .   = ALIGN(0x4);
    } >RAM0L

	/* .program_stack 0x20088000 : */
    .program_stack 0x200E0000 :
    {
        program_stack_start = .;
        .  += PROGRAM_STACK_SIZE;
        .   = ALIGN(0x4);
        program_stack_end = .; 
    } >RAM0L

	.irq_stack :
	{
		irq_stack_start = .;
		.  += IRQ_STACK_SIZE;
		.	= ALIGN(0x4);
		irq_stack_end = .;
	} >RAM0L

	.fiq_stack :
	{
		fiq_stack_start = .;
		.  += FIQ_STACK_SIZE;
		.	= ALIGN(0x4);
		fiq_stack_end = .;
	} >RAM0L

	.svc_stack :
	{
		PROVIDE(svc_stack_start = .);		
		.  += SVC_STACK_SIZE;
		.	= ALIGN(0x4);
		svc_stack_end = .;
	} >RAM0L
	
	.abt_stack :
	{
		abt_stack_start = .;
		.  += ABT_STACK_SIZE;
		.	= ALIGN(0x4);
		abt_stack_end = .;
	} >RAM0L

    .ttb_mmu1 :
    {
        ttb_mmu1_base = .;
        .  += TTB_SIZE;
        .   = ALIGN(0x4);
        ttb_mmu1_end = .;
    } >RAM0L

    .BSS_DMAC_SAMPLE_INTERNAL_RAM (NOLOAD) :
    {
        *(.BSS_DMAC_SAMPLE_INTERNAL_RAM)  
    } >RAM2U_M
}
