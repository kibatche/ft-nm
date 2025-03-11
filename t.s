    .section .text
    .global symbol_data
symbol_data:
    ret

    .section .data
    .global SYmbol_data
SYMBOl_data:
    .long 42

    .section .bss
    .global Symbol_data
SYMBol_data:
    .skip 4
