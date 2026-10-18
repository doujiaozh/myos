[bits 32]
[org 0x1000]

start:
    ; ==========================================
    ; 任务 1：配置分页 (Paging)
    ; ==========================================
    ; 我们将在 0x100000 (1MB) 处建立页目录，在 0x101000 处建立页表
    ; 用于实现前 4MB 内存的恒等映射（虚拟地址 = 物理地址）
    mov edi, 0x100000
    mov cr3, edi
    xor eax, eax
    mov ecx, 1024 * 2 ; 页目录 + 页表 = 2048 个条目
    rep stosd         ; 将内存清零

    ; 设置页目录 (Page Directory)
    mov edi, 0x100000
    lea eax, [0x101000 | 0x3] ; 0x3 = 存在位 + 读写位
    mov [edi], eax

    ; 设置页表 (Page Table)
    mov edi, 0x101000
    mov eax, 0x3
    mov ecx, 1024
.set_pt:
    mov [edi], eax
    add eax, 0x1000
    add edi, 4
    loop .set_pt

    ; 开启分页（修改 CR0 的 PG 位）
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; ==========================================
    ; 任务 2：清屏并打印成功提示
    ; ==========================================
    mov ebx, 0xB8000
    mov ecx, 80*25
    mov al, ' '
    mov ah, 0x0A
clr:
    mov [ebx], ax
    add ebx, 2
    loop clr

    mov ebx, 0xB8000 + 160*10
    mov esi, msg_paging
    mov ah, 0x0F ; 白色
print:
    lodsb
    test al, al
    jz .done_print
    mov [ebx], al
    mov [ebx+1], ah
    add ebx, 2
    jmp print
.done_print:

    ; ==========================================
    ; 任务 3：配置 IDT (中断描述符表)
    ; ==========================================
    ; 初始化 256 个中断门，默认指向一个空的处理器
    mov edi, 0x200000 ; 将 IDT 放在 2MB 处
    mov ecx, 256
.init_idt:
    mov eax, isr_default
    mov word [edi], ax
    mov word [edi+2], 0x08 ; 代码段选择子
    mov byte [edi+4], 0
    mov byte [edi+5], 0x8E ; 存在位 + 32位中断门
    shr eax, 16
    mov word [edi+6], ax
    add edi, 8
    loop .init_idt

    ; 加载 IDT
    lidt [idt_descriptor]

    ; 开启硬件中断
    sti

hang:
    hlt
    jmp hang

; 默认中断处理程序（防止系统崩溃）
isr_default:
    pusha
    ; 在屏幕左上角打印 'I' 表示触发了中断
    mov byte [0xB8000], 'I'
    mov byte [0xB8001], 0x0C ; 红色
    popa
    iret

msg_paging db "Super OS: 32-bit Paging Enabled! Interrupts Wired!", 0

; IDT 描述符
idt_descriptor:
    dw 256 * 8 - 1
    dd 0x200000