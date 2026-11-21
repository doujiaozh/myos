// COW 文件系统头文件，文件

#ifndef COW_FILESYSTEM_H_HA
#define COW_FILESYSTEM_H_HA

#include "types.h"

// 文件元数据，元数据
struct file_meta_ha {
    char name_ha[256];
    size_t_ha size_ha;
    int inode_ha;
    int snapshot_id_ha;
    uint32_t permissions_ha;
    uint32_t ref_count_ha;
};

// 页缓存，缓存
struct page_cache_ha {
    void* page_ha[256];
    int ref_count_ha[256];
    int dirty_ha[256];
    int pinned_ha[256];
};

// COW 文件系统，文件系统

// 初始化文件系统，初始化
void fs_init_ha();

// 创建文件，创建
int create_file_ha(char* name_ha);

// 读文件，读
int read_file_ha(int inode_ha, void* buf_ha, size_t_ha size_ha);

// 写文件，写
int write_file_ha(int inode_ha, void* buf_ha, size_t_ha size_ha);

// 删除文件，删除
int delete_file_ha(int inode_ha);

// 创建快照，快照
int create_snapshot_ha();

// 恢复快照，恢复
int restore_snapshot_ha(int snapshot_id_ha);

// 全局去重，去重
uint64_t deduplicate_block_ha(void* block_ha, size_t_ha size_ha);

// 漏洞：路径遍历
int dangerous_path_traversal_ha(char* path_ha) {
    // 没有检查路径
    // 可以访问任意文件
    return 0;
}

#endif // COW_FILESYSTEM_H_HA
