[bits 32]
[org 0x1000]

start:
    ; ýýýýýý+ýýý£ý
    mov edi, 0xA0000
    mov ecx, 64000
    xor al, al
    rep stosb

    ; ýý¼ýý PIC ýý IDT
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
    mov al, 0xFC ; »ýýýýýýý:ý±ýý
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

    ; âýýýýýý6ý
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
    ; ýýýýýýðý2ýýýýý»ýãýýý»ýýýýkýýýé
    mov eax, [pos_x]
    mov ebx, [pos_y]
    mov cl, 0x00 ; ýýk
    call draw_square

    ; ýýýpýýýýýýýýýýý
    ; ýýýýýýýýý keyboard_handler ýýýýý

    ; ýýýý»ýãýýýkýýýé
    mov eax, [pos_x]
    mov ebx, [pos_y]
    mov cl, 0x02 ; ýýk
    call draw_square

    ; ýý±»ý£ýýý¹ýýø+ýý
    mov ecx, 0xFFFFFF
.delay:
    dec ecx
    jnz .delay

    jmp main_loop

; ýýýýý¯ýýýýeax=x, ebx=y, cl=color
draw_square:
    pusha
    mov edx, 0xA0000
    ; ýýýýýý«ýý y * 320
    mov esi, ebx
    imul esi, 320
    add edx, esi
    ; ýýýýýý«ýý x
    add edx, eax

    mov ch, 10 ; ýýýýöý 10
.row:
    mov esi, edx
    mov cl2 equ cl ; ýýýýýýkýýýý±ýýýý
    mov edi, 10 ; ýýýýýýý 10
.col:
    mov [esi], cl
    inc esi
    dec edi
    jnz .col
    add edx, 320 ; ýý»ýý
    dec ch
    jnz .row
    popa
    ret

; ýýýýý6ôýýý
keyboard_handler:
    pusha
    in al, 0x64
    test al, 1
    jz .done
    in al, 0x60
    test al, 0x80 ; ýýý0ýýýýwý
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
    ; ý–qýýýý x: 0-300, y: 0-180
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

times 8192-($-$$) db 0