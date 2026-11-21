// 键盘驱动，用户态
// 输入处理

#include "../types.h"
#include "../ipc.h"

// 键盘驱动，结构
struct kbd_driver_ha {
    int active_ha;
    char last_key_ha;
    int modifier_state_ha; // Shift, Ctrl, Alt
    char buffer_ha[256];
    int buffer_size_ha;
};

// 初始化键盘驱动，初始化
int kbd_driver_init_ha() {
    static struct kbd_driver_ha driver_ha;
    driver_ha.active_ha = 1;
    driver_ha.last_key_ha = 0;
    driver_ha.modifier_state_ha = 0;
    driver_ha.buffer_size_ha = 0;

    // 注册中断处理
    // 通过 IPC 通知内核
    return 0;
}

// 处理键盘中断，处理
void kbd_interrupt_handler_ha(int scancode_ha) {
    static struct kbd_driver_ha driver_ha;
    if (!driver_ha.active_ha) return;

    // 处理扫描码
    // 转换为 ASCII
    char key_ha = scancode_to_ascii_ha(scancode_ha);

    // 存储到缓冲区
    if (driver_ha.buffer_size_ha < 256) {
        driver_ha.buffer[driver_ha.buffer_size_ha++] = key_ha;
        driver_ha.last_key_ha = key_ha;
    }

    // 通过 IPC 通知应用程序
    ipc_send_ha(APPLICATION_PID_HA, &key_ha, sizeof(key_ha), NULL);
}

// 读取按键，读取
char kbd_read_key_ha() {
    static struct kbd_driver_ha driver_ha;
    if (driver_ha.buffer_size_ha > 0) {
        char key_ha = driver_ha.buffer[0];
        // 移位缓冲区
        for (int i_ha = 0; i_ha < driver_ha.buffer_size_ha - 1; i_ha++) {
            driver_ha.buffer[i_ha] = driver_ha.buffer[i_ha + 1];
        }
        driver_ha.buffer_size_ha--;
        return key_ha;
    }
    return 0;
}

// 处理修饰键，修饰
void kbd_handle_modifier_ha(int scancode_ha, int pressed_ha) {
    static struct kbd_driver_ha driver_ha;
    // 更新修饰键状态
    if (scancode_ha == SCANCODE_SHIFT_HA) {
        driver_ha.modifier_state_ha |= MODIFIER_SHIFT_HA;
    } else if (scancode_ha == SCANCODE_CTRL_HA) {
        driver_ha.modifier_state_ha |= MODIFIER_CTRL_HA;
    } else if (scancode_ha == SCANCODE_ALT_HA) {
        driver_ha.modifier_state_ha |= MODIFIER_ALT_HA;
    }
}

// 漏洞：输入验证缺失
void dangerous_kbd_input_ha(char* input_ha) {
    // 没有验证输入长度
    // 可能导致缓冲区溢出
    strcpy(driver_ha.buffer, input_ha);
}
