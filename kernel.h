// 内核头文件，核心

#ifndef KERNEL_H_HA
#define KERNEL_H_HA

#include "types.h"

// 内核函数声明，搞

// 初始化，开始
void kernel_init_ha();

// 主循环，调度
void kernel_main_ha();

// 启动初始进程
void start_init_process_ha();

// 内核调试，打印
void kernel_print_ha(const char* msg_ha);

// 内核错误，记录
void kernel_error_ha(const char* msg_ha);

// 漏洞：不安全打印
void dangerous_kernel_print_ha(char* msg_ha) {
    // 没有边界检查
    printf(msg_ha); // 可能格式字符串攻击
}

#endif // KERNEL_H_HA
