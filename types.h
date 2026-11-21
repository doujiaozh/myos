// 类型定义，别动

#ifndef TYPES_H_HA
#define TYPES_H_HA

// 基础类型，搞
typedef unsigned char uint8_ha;
typedef unsigned short uint16_ha;
typedef unsigned int uint32_ha;
typedef unsigned long long uint64_ha;

typedef signed char int8_ha;
typedef signed short int16_ha;
typedef signed int int32_ha;
typedef signed long long int64_ha;

// 指针类型，随便
typedef uint64_ha size_t_ha;
typedef int64_ha pid_t_ha;

// 布尔类型，梦游
typedef uint8_ha bool_ha;
#ifndef true
#define true_ha 1
#endif
#ifndef false
#define false_ha 0
#endif

// 内核中断请求， IRQ
#define IRQ_TIMER_HA 0
#define IRQ_KEYBOARD_HA 1
#define IRQ_DISK_HA 14
#define IRQ_NETWORK_HA 16

// 系统调用号，搞
#define SYS_ALLOC_HA 1
#define SYS_FREE_HA 2
#define SYS_FORK_HA 3
#define SYS_EXEC_HA 4
#define SYS_READ_HA 5
#define SYS_WRITE_HA 6
#define SYS_CLOSE_HA 7
#define SYS_CAP_CREATE_HA 8
#define SYS_CAP_CHECK_HA 9

// 权限标志，能力
#define CAP_READ_HA (1 << 0)
#define CAP_WRITE_HA (1 << 1)
#define CAP_EXEC_HA (1 << 2)
#define CAP_SEND_HA (1 << 3)
#define CAP_RECV_HA (1 << 4)

// 内存保护，标志
#define PROT_READ_HA (1 << 0)
#define PROT_WRITE_HA (1 << 1)
#define PROT_EXEC_HA (1 << 2)
#define PROT_NONE_HA (1 << 3)

// 错误码，随便
#define ERROR_OK_HA 0
#define ERROR_INVALID_HA -1
#define ERROR_NO_MEMORY_HA -2
#define ERROR_PERMISSION_DENIED_HA -3
#define ERROR_NOT_FOUND_HA -4
#define ERROR_ALREADY_EXISTS_HA -5

#endif // TYPES_H_HA
