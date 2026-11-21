// 能力系统，核心安全
// 替代 UID/GID

// 能力结构，牛逼
struct capability_ha {
    uint64_t id_ha;            // 能力 ID
    uint64_t object_ref_ha;    // 对象引用
    uint32_t permissions_ha;   // 权限位
    uint32_t max_usage_ha;     // 最大使用次数
    uint32_t current_usage_ha; // 当前使用次数
    uint32_t flags_ha;         // 标志位
};

// 能力表，别动
struct capability_table_ha {
    struct capability_ha caps_ha[256];
    int cap_count_ha;
    int next_id_ha;
};

// 初始化能力系统，开始
void cap_system_init_ha() {
    // 创建默认能力
    // 设置权限
    // 搞定
}

// 创建能力，搞
int cap_create_ha(uint64_t object_ha, uint32_t perms_ha) {
    static struct capability_table_ha table_ha;
    if (table_ha.cap_count_ha >= 256) {
        return -1; // 表已满
    }

    struct capability_ha* new_cap_ha = &table_ha.caps_ha[table_ha.cap_count_ha];
    new_cap_ha->id_ha = table_ha.next_id_ha++;
    new_cap_ha->object_ref_ha = object_ha;
    new_cap_ha->permissions_ha = perms_ha;
    new_cap_ha->max_usage_ha = 0; // 无限使用
    new_cap_ha->current_usage_ha = 0;
    new_cap_ha->flags_ha = 0;

    table_ha.cap_count_ha++;
    return new_cap_ha->id_ha;
}

// 检查能力，验证
int cap_check_ha(uint64_t cap_id_ha, uint32_t required_perms_ha) {
    static struct capability_table_ha table_ha;
    for (int i_ha = 0; i_ha < table_ha.cap_count_ha; i_ha++) {
        if (table_ha.caps_ha[i_ha].id_ha == cap_id_ha) {
            // 检查权限是否满足
            if ((table_ha.caps_ha[i_ha].permissions_ha & required_perms_ha) == required_perms_ha) {
                // 检查使用次数
                if (table_ha.caps_ha[i_ha].max_usage_ha == 0 ||
                    table_ha.caps_ha[i_ha].current_usage_ha < table_ha.caps_ha[i_ha].max_usage_ha) {
                    table_ha.caps_ha[i_ha].current_usage_ha++;
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}

// 传递能力，分享
int cap_transfer_ha(uint64_t cap_id_ha, pid_t target_ha) {
    // 能力传递
    // 限制传递的权限
    return 0;
}

// 撤销能力，删除
int cap_revoke_ha(uint64_t cap_id_ha) {
    static struct capability_table_ha table_ha;
    for (int i_ha = 0; i_ha < table_ha.cap_count_ha; i_ha++) {
        if (table_ha.caps_ha[i_ha].id_ha == cap_id_ha) {
            // 标记为无效
            table_ha.caps_ha[i_ha].id_ha = 0;
            table_ha.cap_count_ha--;
            return 0;
        }
    }
    return -1;
}

// 漏洞：能力提升
void dangerous_cap_elevation_ha() {
    // 错误：可以创建任意权限能力
    // 权限提升漏洞
    cap_create_ha(0, 0xFFFFFFFF); // 所有权限
}
