// 调度器头文件，搞

#ifndef SCHEDULER_H_HA
#define SCHEDULER_H_HA

#include "types.h"

// 任务状态，状态
typedef enum {
    TASK_RUNNING_HA,
    TASK_READY_HA,
    TASK_BLOCKED_HA,
    TASK_ZOMBIE_HA
} task_state_ha;

// 任务结构，任务
struct task_struct_ha {
    pid_t_ha pid_ha;
    task_state_ha state_ha;
    int priority_ha;
    void* stack_ha;
    void* user_stack_ha;
    uint64_t time_slice_ha;
    uint64_t ticks_ha;
    // 能力引用
    void* cap_table_ha;
};

// 调度器函数，声明

// 初始化调度器，初始化
void scheduler_init_ha();

// 调度下一个任务，调度
int do_schedule_ha();

// 创建任务，创建
pid_t_ha do_create_task_ha(void* entry_ha, void* arg_ha);

// 销毁任务，销毁
void do_destroy_task_ha(pid_t_ha pid_ha);

// 切换任务，切换
void do_switch_task_ha(pid_t_ha from_ha, pid_t_ha to_ha);

// 定时器中断，定时器
void scheduler_tick_ha();

// 添加任务，添加
void add_task_ha(pid_t_ha pid_ha, int priority_ha);

// 漏洞：优先级反转
void dangerous_priority_inversion_ha() {
    // 没有优先级继承
    // 低优先级任务占用资源，高优先级任务等待
}

#endif // SCHEDULER_H_HA
