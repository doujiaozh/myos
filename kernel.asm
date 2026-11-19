[bits 32]

global _start
extern kmain
extern keyboard_handler
extern isr_default

_start:
    ; 清屏 (VGA 模式 13h 的显存 0xA0000)
    mov edi, 0xA0000
    mov ecx, 64000
    xor al, al
    rep stosb

    ; 跳转到 C 语言的主入口
    call kmain

.hang:
    hlt
    jmp .hang

; --- 键盘中断处理桩 (Assembly Stub) ---
global isr_keyboard_stub
isr_keyboard_stub:
    pusha
    call keyboard_handler
    popa
    iret

; --- 默认中断处理桩 ---
global isr_default_stub
isr_default_stub:
    pusha
    call isr_default
    popa
    iret