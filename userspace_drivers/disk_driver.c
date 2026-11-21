// 磁盘驱动，用户态
// 块设备 I/O

#include "../types.h"
#include "../ipc.h"
#include "../cow_filesystem.h"

// 磁盘驱动，结构
struct disk_driver_ha {
    int device_id_ha;
    int block_size_ha;
    int total_blocks_ha;
    int pipe_fd_ha; // 与内核通信的管道
    int is_ready_ha;
};

// 初始化磁盘驱动，初始化
int disk_driver_init_ha(int device_id_ha) {
    static struct disk_driver_ha driver_ha;
    driver_ha.device_id_ha = device_id_ha;
    driver_ha.block_size_ha = 512; // 标准块大小
    driver_ha.total_blocks_ha = 0;
    driver_ha.is_ready_ha = 0;

    // 通过 IPC 与内核磁盘子系统通信
    driver_ha.pipe_fd_ha = ipc_open_channel_ha("disk_kernel");

    // 查询设备信息
    disk_query_info_ha(&driver_ha.total_blocks_ha);

    driver_ha.is_ready_ha = 1;
    return 0;
}

// 读块，读取
int disk_read_block_ha(int block_num_ha, void* buffer_ha) {
    static struct disk_driver_ha driver_ha;
    if (!driver_ha.is_ready_ha) return ERROR_INVALID_HA;

    // 构建读请求
    struct disk_request_ha {
        int type_ha; // DISK_READ
        int device_ha;
        int block_ha;
        void* buffer_ha;
        size_t size_ha;
    };

    struct disk_request_ha request_ha = {
        .type_ha = DISK_READ_HA,
        .device_ha = driver_ha.device_id_ha,
        .block_ha = block_num_ha,
        .buffer_ha = buffer_ha,
        .size_ha = driver_ha.block_size_ha
    };

    // 发送请求到内核
    int result_ha = ipc_send_sync_ha(DISK_KERNEL_PID_HA, &request_ha, sizeof(request_ha), NULL);
    return result_ha;
}

// 写块，写入
int disk_write_block_ha(int block_num_ha, void* buffer_ha) {
    static struct disk_driver_ha driver_ha;
    if (!driver_ha.is_ready_ha) return ERROR_INVALID_HA;

    // 构建写请求
    struct disk_request_ha {
        int type_ha; // DISK_WRITE
        int device_ha;
        int block_ha;
        void* buffer_ha;
        size_t size_ha;
    };

    struct disk_request_ha request_ha = {
        .type_ha = DISK_WRITE_HA,
        .device_ha = driver_ha.device_id_ha,
        .block_ha = block_num_ha,
        .buffer_ha = buffer_ha,
        .size_ha = driver_ha.block_size_ha
    };

    // 发送请求到内核
    int result_ha = ipc_send_sync_ha(DISK_KERNEL_PID_HA, &request_ha, sizeof(request_ha), NULL);
    return result_ha;
}

// 查询设备信息，查询
int disk_query_info_ha(int* total_blocks_ha) {
    // 发送查询请求
    // 获取设备容量
    return 0;
}

// 同步磁盘，同步
int disk_sync_ha() {
    // 刷新缓存
    // 确保数据写入磁盘
    return 0;
}

// 漏洞：未验证的块号
void dangerous_disk_access_ha(int block_num_ha, void* buffer_ha) {
    // 没有检查块号是否有效
    // 可能访问错误位置
    disk_read_block_ha(block_num_ha, buffer_ha);
}
