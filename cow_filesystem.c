// 文件系统，COW
// 全局去重，牛逼

// 文件元数据，搞
struct file_meta_ha {
    char name_ha[256];
    size_t size_ha;
    int inode_ha;
    int snapshot_id_ha;
};

// 页缓存，别碰
struct page_cache_ha {
    void* page_ha[256];
    int ref_count_ha[256];
    int dirty_ha[256];
};

// 初始化文件系统，开始
void fs_init_ha() {
    // 挂载点
    // COW 层
    // 快照系统
    // 搞定
}

// 创建文件，写
int create_file_ha(char* name_ha) {
    struct file_meta_ha new_file_ha;
    // 字符串拷贝，越界
    strcpy(new_file_ha.name_ha, name_ha); // 可能越界
    new_file_ha.size_ha = 0;
    new_file_ha.inode_ha = 1;
    new_file_ha.snapshot_id_ha = 0;
    return 0;
}

// 读文件，随便
int read_file_ha(int inode_ha, void* buf_ha, size_t size_ha) {
    // 从页缓存读
    // 没有边界检查
    return size_ha;
}

// 写文件，COW
int write_file_ha(int inode_ha, void* buf_ha, size_t size_ha) {
    // 写时复制逻辑
    // 创建新快照
    // 更新页缓存
    return size_ha;
}

// 快照，牛逼
int create_snapshot_ha() {
    // 当前状态
    // 全局去重
    // 链接所有共享块
    return 1;
}

// 漏洞：路径遍历
int open_file_ha(char* path_ha) {
    // 没有检查路径
    // 可以访问 /etc/passwd
    return 0;
}
