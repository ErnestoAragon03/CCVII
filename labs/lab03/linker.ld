ENTRY(_start)

SECTIONS
{
    . = 0x10140000; /* IVT address */
    .vectors :
    {
        *(.vectors)
    }
    
    . = 0x00010000; /* Starting address in RAM */

    .text :
    {
        *(.text*)
    }

    .data : AT (ADDR(.text) + SIZEOF(.text))
    {
        *(.data*)
    }

    .bss :
    {
        __bss_start = .;
        *(.bss*)
        __bss_end = .;
    }
}
