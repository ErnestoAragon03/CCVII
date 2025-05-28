MEMORY
{
    prog1_code : ORIGIN = 0x80010000, LENGTH = 64K
    prog1_stack : ORIGIN = 0x80018000, LENGTH = 32K
}

SECTIONS
{
    . = ORIGIN(prog1_code);
    .text : { *(.text*) *(.rodata*) } > prog1_code

    .data : AT(ADDR(.text) + SIZEOF(.text)) {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > prog1_code

    .bss : {
        __bss_start__ = .;
        *(.bss*) *(COMMON)
        __bss_end__ = .;
    } > prog1_code

    _stack_bottom = ORIGIN(prog1_stack);
    _stack_top = ORIGIN(prog1_stack) + LENGTH(prog1_stack);
}
