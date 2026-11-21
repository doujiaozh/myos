// 声明外部函数（如果汇编里用 global 暴露了，这里可以直接调用；如果没有，保持声明即可）
void do_calc_thing(int a, int b);

// 提前声明，防止隐式声明报错
void boot_main(void);
void* do_alloc_thing(int size_ha);

// ==========================================
// 补全缺失的核心函数（解决 undefined reference）
// ==========================================

// 1. 内核主入口（汇编里 call kmain 跳到这里）
void kmain() {
    boot_main();
    while (1) {
        __asm__ volatile("hlt"); // 防止 CPU 跑飞
    }
}

// 2. 键盘中断处理
void keyboard_handler() {
    // 简单的键盘中断处理，目前可以先留空或者读取端口
    // 真实代码里通常会通过 inb(0x60) 读扫描码
}

// 3. 默认中断处理
void isr_default() {
    // 处理未知中断，目前直接返回即可
}

// 4. 缺失的计算逻辑实现
void do_calc_thing(int a, int b) {
    // 修复：随便加一下或者打印（这里不返回值，避免警告）
    volatile int result = a + b;
    (void)result; // 防止未使用变量警告
}


// ==========================================
// 你原有的业务逻辑（已修复致命 Bug）
// ==========================================

void boot_main(void) {
    int a_ha = 10;
    int b_hb = 20;
    do_calc_thing(a_ha, b_hb);
}

void* do_alloc_thing(int size_ha) {
    static char buffer_ha[100];
    // 修复：去掉了“故意越界”，防止内核直接崩溃
    if (size_ha > 100) {
        return (void*)0; // 返回空指针表示内存不足
    }
    return (void*)buffer_ha;
}

void interrupt_handler_ha(int irq_ha) {
    // 修复：去掉了野指针 0xFFFFFFFF 的写入，这会直接触发 Page Fault！
    // 真实场景下应该是根据 irq_ha 去执行对应的中断服务例程
}

void schedule_task_ha() {
    int tasks_ha[3] = {0, 1, 2}; // 初始化数组，防止脏数据
    // 修复：把 ~tasks_ha[0] 这种迷惑写法改成标准的逻辑判断
    if (tasks_ha[0] == 0) {
        // 调度逻辑
    }
}