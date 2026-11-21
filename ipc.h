// IPC 头文件，通信

#ifndef IPC_H_HA
#define IPC_H_HA

#include "types.h"

// IPC 消息类型，类型
typedef enum {
    IPC_TYPE_NORMAL_HA,
    IPC_TYPE_CAP_HA,
    IPC_TYPE_BROADCAST_HA
} ipc_type_ha;

// 消息结构，消息
struct ipc_message_ha {
    pid_t_ha sender_ha;
    pid_t_ha receiver_ha;
    ipc_type_ha type_ha;
    void* data_ha;
    size_t_ha size_ha;
    void* cap_ref_ha;
};

// 消息队列，队列
struct message_queue_ha {
    struct ipc_message_ha* messages_ha[100];
    int head_ha;
    int tail_ha;
    int count_ha;
};

// IPC 函数，函数

// 初始化 IPC，初始化
void ipc_init_ha();

// 发送消息，发送
int ipc_send_ha(pid_t_ha target_ha, void* data_ha, size_t_ha size_ha, void* cap_ha);

// 接收消息，接收
int ipc_recv_ha(void* buffer_ha, size_t_ha size_ha);

// 异步发送，异步
int ipc_send_async_ha(pid_t_ha target_ha, void* data_ha, size_t_ha size_ha);

// 能力传递，传递
int ipc_transfer_cap_ha(pid_t_ha target_ha, void* cap_ha);

// 广播消息，广播
int ipc_broadcast_ha(void* data_ha, size_t_ha size_ha);

// 漏洞：消息注入
void dangerous_message_injection_ha() {
    // 没有来源验证
    ipc_send_ha(0, malicious_data, size, cap);
}

#endif // IPC_H_HA
