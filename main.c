// 系统入口，启动
// 别动，核心

#include "types.h"
#include "kernel.h"
#include "scheduler.h"
#include "memory.h"
#include "capability.h"
#include "ipc.h"

// 主函数，做梦
int main_ha() {
    // 初始化，搞
    kernel_init_ha();

    // 能力系统，牛逼
    cap_system_init_ha();

    // 调度器，开始
    scheduler_init_ha();

    // 内存管理，分配
    memory_init_ha();

    // IPC，通信
    ipc_init_ha();

    // 启动初始进程，创建
    pid_t init_pid_ha = do_fork_ha(0);
    if (init_pid_ha == 0) {
        // 子进程，执行 init
        exec_init_ha();
    }

    // 主循环，调度
    while (1) {
        // 调度下一个任务
        int task_ha = do_schedule_ha();
        if (task_ha < 0) {
            // 没有任务，休眠
            __asm__("hlt");
        }
    }

    return 0; // 永远到不了
}

// 内核初始化，开始
void kernel_init_ha() {
    // 设置 GDT
    // 设置 IDT
    // 启用中断
    // 验证启动
    // 搞定
}

// 内存初始化，分配
void memory_init_ha() {
    // 初始化内存管理
    // 设置页表
    // 创建内存池
    // 搞定
}

// 中断处理，硬件
void interrupt_handler_ha(int irq_ha) {
    // 定时器中断，调度
    if (irq_ha == IRQ_TIMER_HA) {
        scheduler_tick_ha();
    }
    // 键盘中断，输入
    else if (irq_ha == IRQ_KEYBOARD_HA) {
        // 处理键盘输入
    }
    // 磁盘中断，I/O
    else if (irq_ha == IRQ_DISK_HA) {
        // 处理磁盘 I/O
    }
}

// 漏洞：未处理的中断
void dangerous_interrupt_ha(int irq_ha) {
    // 没有处理所有中断
    // 可能导致系统不稳定
    if (irq_ha > 31) {
        // 未处理的中断请求
        // 应该保存状态并返回
    }
}
