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
  mov al, 10
  call print_char
  call print_prompt
  ; Í  PIC
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
  in al, 0x64
  test al, 1
  jz .done
  in al, 0x60
  test al, 0x80
  jnz .done
  movzx ebx, al
  mov al, [keymap + ebx]
  test al, al
  jz .done
  cmp al, 10
  je .enter_key
  mov ecx, [cmd_len]
  mov [cmd_buf + ecx], al
  inc dword [cmd_len]
  call print_char
  jmp .done
.enter_key:
  mov al, 10
  call print_char
  call check_command
  mov dword [cmd_len], 0
  call print_prompt
  jmp .done
.done:
  mov al, 0x20
  out 0x20, al
  popa
  iret

print_prompt:
  mov esi, prompt
  call print_str
  ret

check_command:
  mov ecx, [cmd_len]
  mov byte [cmd_buf + ecx], 0
  cmp dword [cmd_len], 0
  je .ret
  ; clear
  cmp byte [cmd_buf], 99
  jne .chk_time
  call cmd_clear
  jmp .ret
.chk_time:
  ; time
  cmp byte [cmd_buf], 116
  jne .chk_help
  cmp byte [cmd_buf+1], 105
  jne .chk_help
  call cmd_time
  jmp .ret
.chk_help:
  ; help
  cmp byte [cmd_buf], 104
  jne .chk_reboot
  call cmd_help
  jmp .ret
.chk_reboot:
  ; reboot
  cmp byte [cmd_buf], 114
  jne .chk_llm
  call cmd_reboot
  jmp .ret
.chk_llm:
  ; llm
  cmp byte [cmd_buf], 108
  jne .not_found
  cmp byte [cmd_buf+1], 108
  jne .not_found
  cmp byte [cmd_buf+2], 109
  jne .not_found
  call run_llm_inference
  jmp .ret
.not_found:
  mov esi, msg_unknown
  call print_str
.ret:
  ret

cmd_clear:
  mov ebx, 0xB8000
  mov ecx, 80*25
.clr_loop:
  mov word [ebx], 0x0F20
  add ebx, 2
  loop .clr_loop
  mov dword [cursor], 0xB8000
  ret

cmd_help:
  mov esi, msg_help
  call print_str
  ret

cmd_reboot:
  mov al, 0xFE
  out 0x64, al
  ret

cmd_time:
  mov esi, msg_time_prefix
  call print_str
  mov al, 0x00
  out 0x70, al
  in al, 0x71
  call print_bcd
  mov al, 58
  call print_char
  mov al, 0x02
  out 0x70, al
  in al, 0x71
  call print_bcd
  mov al, 58
  call print_char
  mov al, 0x04
  out 0x70, al
  in al, 0x71
  call print_bcd
  mov al, 10
  call print_char
  ret

print_bcd:
  movzx ax, al
  mov bl, al
  and al, 0x0F
  shr bl, 4
  add bl, 48
  mov cl, al
  add cl, 48
  mov al, bl
  call print_char
  mov al, cl
  call print_char
  ret

run_llm_inference:
  mov esi, msg_llm_start
  call print_str
  mov ecx, 4
  mov edi, weights
  mov esi, input_vec
  xor ebx, ebx
  xor edx, edx
calc_loop:
  mov eax, [esi]
  imul eax, [edi]
  add ebx, eax
  add esi, 4
  add edi, 4
  inc edx
  cmp edx, ecx
  jl calc_loop
  cmp ebx, 0
  jge .relu_ok
  xor ebx, ebx
.relu_ok:
  mov eax, ebx
  add eax, 48
  mov [result_char], al
  mov esi, msg_llm_res
  call print_str
  mov al, [result_char]
  call print_char
  mov al, 10
  call print_char
  ret

print_str:
  pusha
.loop_str:
  lodsb
  test al, al
  jz .end_str
  call print_char
  jmp .loop_str
.end_str:
  popa
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

cursor: dd 0xB8000
cmd_len: dd 0
cmd_buf: times 64 db 0
msg_boot db "My Super OS v0.5 - AI-Native Kernel", 0
prompt db 62, 32, 0
msg_help db "Commands: time, clear, help, reboot, llm", 10, 0
msg_time_prefix db "Current Time [HH:MM:SS]: ", 0
msg_llm_start db "[LLM] Running inference...", 10, 0
msg_llm_res db "[LLM] Predicted token: ", 0
msg_unknown db "Unknown command. Type help.", 10, 0
result_char db 0
weights dd 1, 1, 1, 2
input_vec dd 1, 1, 1, 1
idt_descriptor: dw 256*8-1
dd 0x200000
keymap db 0,27,49,50,51,52,53,54,55,56,57,48,45,61,8,9,113,119,101,114,116,121,117,105,111,112,91,93,10,0,97,115,100,102,103,104,106,107,108,59,39,96,0,92,122,120,99,118,98,110,109,44,46,47,0,0,0,32,0
times 4096-($-$$) db 0