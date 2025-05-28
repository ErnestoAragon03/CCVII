MEMORY
{
    prog2_code : ORIGIN = 0x80020000, LENGTH = 64K
    prog2_stack : ORIGIN = 0x80028000, LENGTH = 32K
}

SECTIONS
{
    . = ORIGIN(prog2_code);
    .text : { *(.text*) *(.rodata*) } > prog2_code

    .data : AT(ADDR(.text) + SIZEOF(.text)) {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > prog2_code

    .bss : {
        __bss_start__ = .;
        *(.bss*) *(COMMON)
        __bss_end__ = .;
    } > prog2_code

    _stack_bottom = ORIGIN(prog2_stack);
    _stack_top = ORIGIN(prog2_stack) + LENGTH(prog2_stack);
}
