; boot.s
SECTION .multiboot
    align 4
    dd 0x1BADB002       ; magic
    dd 0x00010003       ; flags: align modules, memory info, video
    dd -(0x1BADB002 + 0x00010003)

SECTION .text
global start
extern kmain

start:
    ; set up stack
    cli
    xor ebp, ebp
    mov esp, stack_top

    ; call kernel entry
    call kmain
.hang:
    hlt
    jmp .hang

SECTION .bss
    align 4
stack_bottom:
    resb 16384
stack_top:
