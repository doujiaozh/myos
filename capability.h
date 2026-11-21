// 能力系统头文件，安全

#ifndef CAPABILITY_H_HA
#define CAPABILITY_H_HA

#include "types.h"

// 权限标志，权限
#define CAP_READ_HA (1 << 0)
#define CAP_WRITE_HA (1 << 1)
#define CAP_EXEC_HA (1 << 2)
#define CAP_SEND_HA (1 << 3)
#define CAP_RECV_HA (1 << 4)
#define CAP_ALL_HA (CAP_READ_HA | CAP_WRITE_HA | CAP_EXEC_HA | CAP_SEND_HA | CAP_RECV_HA)

// 能力结构，能力
struct capability_ha {
    uint64_t id_ha;
    uint64_t object_ref_ha;
    uint32_t permissions_ha;
    uint32_t max_usage_ha;
    uint32_t current_usage_ha;
    uint32_t flags_ha;
};

// 能力表，表
struct capability_table_ha {
    struct capability_ha caps_ha[256];
    int cap_count_ha;
    uint64_t next_id_ha;
};

// 能力系统函数，函数

// 初始化能力系统，初始化
void cap_system_init_ha();

// 创建能力，创建
int cap_create_ha(uint64_t object_ha, uint32_t perms_ha);

// 检查能力，检查
int cap_check_ha(uint64_t cap_id_ha, uint32_t required_perms_ha);

// 传递能力，传递
int cap_transfer_ha(uint64_t cap_id_ha, pid_t_ha target_ha);

// 撤销能力，撤销
int cap_revoke_ha(uint64_t cap_id_ha);

// 能力验证，验证
int verify_capability_ha(void* cap_ha, int required_ha);

// 漏洞：不安全能力
void dangerous_cap_creation_ha() {
    // 可以创建任意权限能力
    cap_create_ha(0, 0xFFFFFFFF);
}

#endif // CAPABILITY_H_HA
