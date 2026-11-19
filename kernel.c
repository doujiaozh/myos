// C: 美化输出 (VGA 文本模式驱动)
volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
int cursor_x = 0;
int cursor_y = 0;

void update_cursor() {
    unsigned short pos = cursor_y * 80 + cursor_x;
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x0F), "d"(0x3D4));
    __asm__ volatile("outb %%al, %%dx" : : "a"((unsigned char)(pos & 0xFF)), "d"(0x3D5));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x0E), "d"(0x3D4));
    __asm__ volatile("outb %%al, %%dx" : : "a"((unsigned char)((pos >> 8) & 0xFF)), "d"(0x3D5));
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (unsigned short)' ' | (0x0A << 8);
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void put_char(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) cursor_x--;
        vga_buffer[cursor_y * 80 + cursor_x] = (unsigned short)' ' | (color << 8);
    } else {
        vga_buffer[cursor_y * 80 + cursor_x] = (unsigned short)c | (color << 8);
        cursor_x++;
    }

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    // 简单滚动
    if (cursor_y >= 25) {
        for (int i = 0; i < 80 * 24; i++) vga_buffer[i] = vga_buffer[i + 80];
        for (int i = 80 * 24; i < 80 * 25; i++) vga_buffer[i] = (unsigned short)' ' | (0x0A << 8);
        cursor_y = 24;
    }
    update_cursor();
}

void print_string(const char* str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++) put_char(str[i], color);
}

// B: 内存管理 (简单的 Bump Allocator)
unsigned int free_memory_ptr = 0x200000; // 从 2MB 处开始分配

void* kmalloc(unsigned int size) {
    void* ptr = (void*)free_memory_ptr;
    free_memory_ptr += size;
    return ptr;
}

// A: 中断与键盘处理
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_gate(int num, unsigned int base) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = 0x08;
    idt[num].always0 = 0;
    idt[num].flags = 0x8E;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

extern void isr_keyboard_stub();
extern void isr_default_stub();

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    for (int i = 0; i < 256; i++) set_idt_gate(i, (unsigned int)isr_default_stub);
    set_idt_gate(0x21, (unsigned int)isr_keyboard_stub); // 注册键盘中断

    __asm__ volatile("lidt (%0)" : : "r"(&idtp));
    __asm__ volatile("sti");
}

void init_pic() {
    // 初始化 PIC
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0x20));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x11), "d"(0xA0));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x21));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x28), "d"(0xA1));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x04), "d"(0x21));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x02), "d"(0xA1));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0x21));
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x01), "d"(0xA1));
    // 允许键盘中断
    __asm__ volatile("outb %%al, %%dx" : : "a"(0xFC), "d"(0x21));
}

void keyboard_handler() {
    unsigned char scancode;
    __asm__ volatile("inb %%dx, %%al" : "=a"(scancode) : "d"(0x60));

    if (scancode < 0x80) { // 按下键
        char* scancode_ascii = "??1234567890-=??qwertyuiop[]??asdfghjkl;'`?\\zxcvbnm,./";
        char c = scancode_ascii[scancode];
        if (c != '?') {
            put_char(c, 0x0F); // 白色打印
        }
    }
    // 发送 EOI
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x20));
}

void isr_default() {
    // 处理默认中断（如未注册的硬件中断）
    __asm__ volatile("outb %%al, %%dx" : : "a"(0x20), "d"(0x20));
}

// 主函数
void kmain() {
    clear_screen();
    print_string("==========================================\n", 0x0E);
    print_string("  Welcome to myos v0.3\n", 0x0A);
    print_string("  Features: VGA Console, Memory, Keyboard\n", 0x0A);
    print_string("==========================================\n\n", 0x0E);

    init_pic();
    init_idt();

    print_string("[OK] PIC and IDT Initialized.\n", 0x0B);
    
    char* buffer = (char*)kmalloc(100);
    buffer[0] = 'M'; buffer[1] = 'e'; buffer[2] = 'm'; buffer[3] = '\0';
    print_string("[OK] Allocated memory and wrote: ", 0x0B);
    print_string(buffer, 0x0F);
    print_string("\n\n", 0x0E);

    print_string("Type something on your keyboard...\n> ", 0x0C);

    while (1) {
        __asm__ volatile("hlt");
    }
}