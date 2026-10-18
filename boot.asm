[bits 16]
[org 0x7c00]
    cli
    mov [BOOT_DRIVE], dl
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; 读取内核（从第 2 个扇区读 15 个扇区到 0x1000）
    mov ax, 0x0215  ; 读 15 个扇区
    mov cx, 0x0002  ; 从第 2 个扇区开始
    mov dx, 0x0000
    mov dl, [BOOT_DRIVE]
    mov bx, 0x1000  ; 加载到物理内存 0x1000
    int 0x13
    jc disk_error

    ; 开启 A20 地址线
    in al, 0x92
    or al, 2
    out 0x92, al

    ; 进入保护模式
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x90000
    call 0x1000     ; 跳转到内核入口 (kernel.asm 的 start)
    jmp $

[bits 16]
disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    jmp $

BOOT_DRIVE db 0
gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff, 0x0
    db 0x0, 0x9a, 0xcf, 0x0
gdt_data:
    dw 0xffff, 0x0
    db 0x0, 0x92, 0xcf, 0x0
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xaa55