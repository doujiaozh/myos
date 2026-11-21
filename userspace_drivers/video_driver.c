// 视频驱动，用户态
// 显示输出

#include "../types.h"
#include "../ipc.h"

// 视频驱动，结构
struct video_driver_ha {
    int width_ha;
    int height_ha;
    int bpp_ha;
    void* framebuffer_ha;
    int mode_ha;
};

// 初始化视频驱动，初始化
int video_driver_init_ha() {
    static struct video_driver_ha driver_ha;
    driver_ha.width_ha = 1920;
    driver_ha.height_ha = 1080;
    driver_ha.bpp_ha = 32;
    driver_ha.mode_ha = 0x100; // 图形模式
    driver_ha.framebuffer_ha = NULL;

    // 请求内核分配帧缓冲区
    // 通过 IPC 与内核通信
    return 0;
}

// 绘制像素，绘制
void video_draw_pixel_ha(int x_ha, int y_ha, int color_ha) {
    static struct video_driver_ha driver_ha;
    if (x_ha < 0 || x_ha >= driver_ha.width_ha ||
        y_ha < 0 || y_ha >= driver_ha.height_ha) {
        return; // 边界检查
    }

    // 绘制像素
    // 直接访问帧缓冲区
    int* fb_ha = (int*)driver_ha.framebuffer_ha;
    fb_ha[y_ha * driver_ha.width_ha + x_ha] = color_ha;
}

// 清屏，清除
void video_clear_ha(int color_ha) {
    static struct video_driver_ha driver_ha;
    int* fb_ha = (int*)driver_ha.framebuffer_ha;
    for (int i_ha = 0; i_ha < driver_ha.width_ha * driver_ha.height_ha; i_ha++) {
        fb_ha[i_ha] = color_ha;
    }
}

// 刷新屏幕，刷新
void video_flip_ha() {
    // 更新显示
    // 可能需要同步
}

// 设置渲染模式，设置
void video_set_mode_ha(int mode_ha) {
    // 切换显示模式
    // 请求内核切换视频模式
}

// 用户态驱动常见漏洞：未检查的用户输入
void dangerous_video_input_ha(int x_ha, int y_ha) {
    // 没有验证坐标
    // 可能导致越界访问
    video_draw_pixel_ha(x_ha, y_ha, 0xFF0000);
}
