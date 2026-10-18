[bits 32]
[org 0x1000]
start:
  mov ebx, 0xB8000
  mov ecx, 80*25
  clr:
  mov word [ebx], 0x0F20
  add ebx, 2
  loop clr
  mov esi, msg_boot
  call print_str
  ; Í  PIC vãd.ØO=
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
  mov al, 0xFD
  out 0x21, al
  mov al, 0xFF
  out 0xA1, al
  ; Ë IDT
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
  mov eax, keyboard_handler
  mov word [0x200000 + 0x21*8], ax
  mov word [0x200000 + 0x21*8 + 2], 0x08
  mov byte [0x200000 + 0x21*8 + 4], 0
  mov byte [0x200000 + 0x21*8 + 5], 0x8E
  shr eax, 16
  mov word [0x200000 + 0x21*8 + 6], ax
  lidt [idt_descriptor]
  sti
  hang:
  hlt
  jmp hang

keyboard_handler:
  pusha
  in al, 0x60
  test al, 0x80
  jnz .done
  movzx ebx, al
  mov al, [keymap + ebx]
  test al, al
  jz .done
  call print_char
  .done:
  mov al, 0x20
  out 0x20, al
  popa
  iret

print_str:
  push ebx
  mov ebx, [cursor]
  .loop:
  lodsb
  test al, al
  jz .end
  mov [ebx], al
  mov byte [ebx+1], 0x0F
  add ebx, 2
  jmp .loop
  .end:
  mov [cursor], ebx
  pop ebx
  ret

print_char:
  pusha
  mov ebx, [cursor]
  cmp al, 10
  je .newline
  mov [ebx], al
  mov byte [ebx+1], 0x0F
  add ebx, 2
  jmp .check_scroll
.newline:
  mov eax, ebx
  sub eax, 0xB8000
  mov ecx, 160
  xor edx, edx
  div ecx
  inc eax
  mul ecx
  mov ebx, eax
  add ebx, 0xB8000
  mov byte [ebx], 62
  mov byte [ebx+1], 0x0A
  mov byte [ebx+2], 32
  mov byte [ebx+3], 0x0A
  add ebx, 4
.check_scroll:
  mov eax, ebx
  sub eax, 0xB8000
  cmp eax, 160*25
  jl .done
  call scroll
.done:
  mov [cursor], ebx
  popa
  ret

scroll:
  mov esi, 0xB8000 + 160
  mov edi, 0xB8000
  mov ecx, 160*24 / 4
  rep movsd
  mov edi, 0xB8000 + 160*24
  mov ecx, 80
  mov ax, 0x0F20
.clr_last:
  mov [edi], ax
  add edi, 2
  loop .clr_last
  sub dword [cursor], 160
  ret

isr_default:
  pusha
  mov al, 0x20
  out 0x20, al
  popa
  iret

cursor: dd 0xB8000 + 160*1
msg_boot db "My Super OS v0.2 - Keyboard Shell Active!", 10, 62, 32, 0
idt_descriptor: dw 256*8-1
dd 0x200000
keymap db 0,27,49,50,51,52,53,54,55,56,57,48,45,61,8,9,113,119,101,114,116,121,117,105,111,112,91,93,10,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118,98,110,109,44,46,47,0,0,0,32,0
times 4096-($-$$) db 0