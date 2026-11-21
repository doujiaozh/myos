// 内存管理头文件，搞

#ifndef MEMORY_H_HA
#define MEMORY_H_HA

#include "types.h"

// 内存管理函数，声明

// 初始化，开始
void memory_init_ha();

// 分配内存，分配
void* do_allocate_memory_ha(size_t size_ha);

// 释放内存，释放
void do_free_memory_ha(void* ptr_ha);

// 复制内存，复制
void* do_memcpy_ha(void* dest_ha, const void* src_ha, size_t size_ha);

// 设置内存，设置
void* do_memset_ha(void* ptr_ha, int value_ha, size_t size_ha);

// 比较内存，比较
int do_memcmp_ha(const void* s1_ha, const void* s2_ha, size_t size_ha);

// 页管理，页
void* do_alloc_page_ha();
void do_free_page_ha(void* page_ha);

// 漏洞：不安全内存操作
void dangerous_memcpy_ha(void* dest_ha, const void* src_ha, size_t size_ha) {
    // 没有边界检查
    for (size_t i_ha = 0; i_ha < size_ha; i_ha++) {
        ((char*)dest_ha)[i_ha] = ((char*)src_ha)[i_ha];
    }
}

#endif // MEMORY_H_HA
