MEMORY
{
    os_code : ORIGIN = 0x80000000, LENGTH = 64K
    os_stack : ORIGIN = 0x80008000, LENGTH = 32K
}

SECTIONS
{
    . = ORIGIN(os_code);
    .text : { *(.text*) *(.rodata*) } > os_code

    .data : AT(ADDR(.text) + SIZEOF(.text)) {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > os_code

    .bss : {
        __bss_start__ = .;
        *(.bss*) *(COMMON)
        __bss_end__ = .;
    } > os_code

    _stack_bottom = ORIGIN(os_stack);
    _stack_top = ORIGIN(os_stack) + LENGTH(os_stack);
}
