MEMORY
{
    code : ORIGIN = 0x80020000, LENGTH = 32K
    stack : ORIGIN = 0x80028000, LENGTH = 32K
}

SECTIONS
{
    . = ORIGIN(code);
    .text : { *(.text*) *(.rodata*) } > code
    .data : AT(ADDR(.text) + SIZEOF(.text)) { *(.data*) } > code
    .bss  : { *(.bss*) *(COMMON) } > code

    _stack_bottom = ORIGIN(stack);
    _stack_top = ORIGIN(stack) + LENGTH(stack);
}
