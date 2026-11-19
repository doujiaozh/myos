[bits 32]


global _start
extern kmain

_start:
    ; 清空屏幕
    mov edi, 0xA0000
    mov ecx, 64000
    xor al, al
    rep stosb

    ; 跳转到 C 语言写的 kmain 函数
    call kmain

.hang:
    hlt
    jmp .hang