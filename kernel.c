// VGA 文本模式显存地址
volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
int cursor_pos = 0;

void print_string(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            cursor_pos += 80 - (cursor_pos % 80);
        } else {
            vga_buffer[cursor_pos++] = (unsigned short)str[i] | (0x0A << 8); // 绿字黑底
        }
    }
}

// 汇编会跳转到这里
void kmain() {
    print_string("====================================\n");
    print_string(" Welcome to myos v0.2 (C Kernel!)\n");
    print_string(" Goal: Microkernel + Capability Security\n");
    print_string("====================================\n");
    print_string("Initializing subsystems...\n");
    print_string("Done. System Halted.\n");

    while (1) {
        // 死循环，防止 CPU 跑飞
    }
}