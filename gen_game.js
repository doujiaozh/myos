const fs = require('fs');

// boot.asm 保持不变（负责开启 VGA 13h 并进入 32 位模式）
const boot = `[bits 16]
[org 0x7c00]
    cli
    mov [BOOT_DRIVE], dl
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov ax, 0x0013
    int 0x10
    mov ax, 0x0215
    mov cx, 0x0002
    mov dx, 0x0000
    mov dl, [BOOT_DRIVE]
    mov bx, 0x1000
    int 0x13
    jc disk_error
    in al, 0x92
    or al, 2
    out 0x92, al
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
    call 0x1000
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
dw 0xaa55`;

// kernel.asm 负责画方块和响应键盘
const kernel = `[bits 32]
[org 0x1000]

start:
    ; 清屏（全部涂黑）
    mov edi, 0xA0000
    mov ecx, 64000
    xor al, al
    rep stosb

    ; 初始化 PIC 和 IDT
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al
    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al
    mov al, 0x01
    out 0x21, al
    out 0xA1, al
    mov al, 0xFC ; 只开启键盘和时钟
    out 0x21, al
    mov al, 0xFF
    out 0xA1, al

    mov edi, 0x200000
    mov ecx, 256
init_idt:
    mov eax, isr_default
    mov word [edi], ax
    mov word [edi+2], 0x08
    mov byte [edi+4], 0
    mov byte [edi+5], 0x8E
    shr eax, 16
    mov word [edi+6], ax
    add edi, 8
    loop init_idt

    ; 注册键盘中断
    mov eax, keyboard_handler
    mov word [0x200000 + 0x21*8], ax
    mov word [0x200000 + 0x21*8 + 2], 0x08
    mov byte [0x200000 + 0x21*8 + 4], 0
    mov byte [0x200000 + 0x21*8 + 5], 0x8E
    shr eax, 16
    mov word [0x200000 + 0x21*8 + 6], ax

    lidt [idt_descriptor]
    sti

main_loop:
    ; 画方块前先擦除旧位置（画一个黑色方块）
    mov eax, [pos_x]
    mov ebx, [pos_y]
    mov cl, 0x00 ; 黑色
    call draw_square

    ; 根据按键更新坐标
    ; 坐标更新在 keyboard_handler 里完成

    ; 画新位置（绿色方块）
    mov eax, [pos_x]
    mov ebx, [pos_y]
    mov cl, 0x02 ; 绿色
    call draw_square

    ; 延时一下，防止闪烁太快
    mov ecx, 0xFFFFFF
.delay:
    dec ecx
    jnz .delay

    jmp main_loop

; 画方块函数：eax=x, ebx=y, cl=color
draw_square:
    pusha
    mov edx, 0xA0000
    ; 计算行偏移 y * 320
    mov esi, ebx
    imul esi, 320
    add edx, esi
    ; 加上列偏移 x
    add edx, eax

    mov ch, 10 ; 方块高度 10
.row:
    mov esi, edx
    mov cl2 equ cl ; 保存颜色到临时变量
    mov edi, 10 ; 方块宽度 10
.col:
    mov [esi], cl
    inc esi
    dec edi
    jnz .col
    add edx, 320 ; 下一行
    dec ch
    jnz .row
    popa
    ret

; 键盘中断处理
keyboard_handler:
    pusha
    in al, 0x64
    test al, 1
    jz .done
    in al, 0x60
    test al, 0x80 ; 忽略按键释放
    jnz .done

    ; W = 0x11, S = 0x1F, A = 0x1E, D = 0x20
    cmp al, 0x11
    je .up
    cmp al, 0x1F
    je .down
    cmp al, 0x1E
    je .left
    cmp al, 0x20
    je .right
    jmp .done

.up:
    sub dword [pos_y], 10
    jmp .check_bounds
.down:
    add dword [pos_y], 10
    jmp .check_bounds
.left:
    sub dword [pos_x], 10
    jmp .check_bounds
.right:
    add dword [pos_x], 10

.check_bounds:
    ; 简单边界检查 x: 0-300, y: 0-180
    cmp dword [pos_x], 0
    jge .chk_x_max
    mov dword [pos_x], 0
.chk_x_max:
    cmp dword [pos_x], 300
    jle .chk_y_min
    mov dword [pos_x], 300
.chk_y_min:
    cmp dword [pos_y], 0
    jge .chk_y_max
    mov dword [pos_y], 0
.chk_y_max:
    cmp dword [pos_y], 180
    jle .done
    mov dword [pos_y], 180

.done:
    mov al, 0x20
    out 0x20, al
    popa
    iret

isr_default:
    pusha
    mov al, 0x20
    out 0x20, al
    popa
    iret

pos_x: dd 160
pos_y: dd 100
idt_descriptor: dw 256*8-1
dd 0x200000

times 8192-($-$$) db 0`;

fs.writeFileSync('boot.asm', boot, 'ascii');
fs.writeFileSync('kernel.asm', kernel, 'ascii');
console.log('Game OS generated! Use W A S D to move.');