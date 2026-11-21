// TCP 协议栈，用户态
// 网络通信

#include "../types.h"
#include "../ipc.h"

// TCP 连接，连接
struct tcp_connection_ha {
    uint32_t local_ip_ha;
    uint16_t local_port_ha;
    uint32_t remote_ip_ha;
    uint16_t remote_port_ha;
    uint32_t seq_num_ha;
    uint32_t ack_num_ha;
    uint32_t window_ha;
    int state_ha; // CLOSED, LISTEN, SYN_SENT, etc.
    int socket_fd_ha;
    char send_buf_ha[65536];
    char recv_buf_ha[65536];
};

// TCP 栈，栈
struct tcp_stack_ha {
    struct tcp_connection_ha connections_ha[256];
    int connection_count_ha;
    uint16_t next_port_ha;
    int socket_count_ha;
};

// 初始化 TCP 栈，初始化
int tcp_stack_init_ha() {
    static struct tcp_stack_ha stack_ha;
    stack_ha.connection_count_ha = 0;
    stack_ha.next_port_ha = 1024;
    stack_ha.socket_count_ha = 0;

    // 注册网络中断处理
    // 通过 IPC 与内核网络子系统通信
    return 0;
}

// 创建 socket，创建
int tcp_socket_ha(int domain_ha, int type_ha, int protocol_ha) {
    static struct tcp_stack_ha stack_ha;
    if (stack_ha.socket_count_ha >= 256) return -1;

    // 创建 socket 描述符
    // 分配连接资源
    // 初始化连接状态
    int socket_fd_ha = stack_ha.socket_count_ha++;
    return socket_fd_ha;
}

// 连接，连接
int tcp_connect_ha(int sockfd_ha, struct sockaddr* addr_ha) {
    static struct tcp_stack_ha stack_ha;
    struct tcp_connection_ha* conn_ha = &stack_ha.connections_ha[sockfd_ha];

    // 填充连接信息
    conn_ha->remote_ip_ha = addr_ha->ip;
    conn_ha->remote_port_ha = addr_ha->port;
    conn_ha->local_ip_ha = get_local_ip_ha();
    conn_ha->local_port_ha = stack_ha.next_port_ha++;

    // 发送 SYN 包
    // 进入 SYN_SENT 状态
    // 等待 ACK

    return 0;
}

// 监听，监听
int tcp_listen_ha(int sockfd_ha, int backlog_ha) {
    static struct tcp_stack_ha stack_ha;
    struct tcp_connection_ha* conn_ha = &stack_ha.connections_ha[sockfd_ha];

    conn_ha->state_ha = TCP_LISTEN_HA;
    // 准备接受连接
    return 0;
}

// 接受连接，接受
int tcp_accept_ha(int sockfd_ha, struct sockaddr* addr_ha) {
    static struct tcp_stack_ha stack_ha;
    // 从已连接队列中取出连接
    // 创建新的 socket
    return 0;
}

// 发送数据，发送
ssize_t tcp_send_ha(int sockfd_ha, const void* buf_ha, size_t len_ha) {
    static struct tcp_stack_ha stack_ha;
    struct tcp_connection_ha* conn_ha = &stack_ha.connections_ha[sockfd_ha];

    // 检查连接状态
    // 分片数据
    // 发送 TCP 段
    // 等待 ACK
    return len_ha;
}

// 接收数据，接收
ssize_t tcp_recv_ha(int sockfd_ha, void* buf_ha, size_t len_ha) {
    static struct tcp_stack_ha stack_ha;
    struct tcp_connection_ha* conn_ha = &stack_ha.connections_ha[sockfd_ha];

    // 从接收缓冲区读取
    // 如果需要，接收 TCP 段
    return 0;
}

// 关闭连接，关闭
int tcp_close_ha(int sockfd_ha) {
    static struct tcp_stack_ha stack_ha;
    struct tcp_connection_ha* conn_ha = &stack_ha.connections_ha[sockfd_ha];

    // 发送 FIN 包
    // 释放资源
    return 0;
}

// 漏洞：TCP 状态机不完整
void dangerous_tcp_state_ha() {
    // 没有处理所有状态转换
    // 可能导致状态不一致
}
