MEMORY
{
    code : ORIGIN = 0x80030000, LENGTH = 64K
    stack : ORIGIN = 0x80038000, LENGTH = 32K
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
