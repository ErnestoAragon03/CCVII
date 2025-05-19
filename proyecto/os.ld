MEMORY {
    ram : ORIGIN = 0x80000000, LENGTH = 64K
}

SECTIONS {
    . = ORIGIN(ram);
    .text : { *(.text*) *(.rodata*) } > ram
    .data : { *(.data*) } > ram
    .bss  : { *(.bss*) *(COMMON) } > ram

    _stack_top = 0x80008000;
}
