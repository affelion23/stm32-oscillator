#include "user_config.h"
#include "sgl_config.h"
#include "sgl_port.h"
#include "sgl_draw.h"
#include "sgl_core.h"
#include "lcd.h"
#include "sgl.h"
#include "main.h"
#include <string.h>

#define BUFFER_SIZE (PANEL_WIDTH * 10)

static sgl_color_t panel_buffer_1[BUFFER_SIZE];
// static sgl_color_t panel_buffer_2[BUFFER_SIZE];

static void panel_flush_area(sgl_area_t* area, sgl_color_t* src)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;
    lcd_update_range(area->x1, area->y1, area->x2, area->y2);
    // 阻塞式刷新函数, 使用DMA
    lcd_update_image((uint8_t*)src, w * h * sizeof(sgl_color_t));
    // 调用sgl_fbdev_flush_ready()函数，告诉SGL框架，刷新完成，可以继续处理下一帧处理
    sgl_fbdev_flush_ready();
}

#if CONFIG_SGL_DEBUG
static void uart_put_string(const char* str)
{
    extern UART_HandleTypeDef huart1;
    HAL_UART_Transmit(&huart1, (const uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}
#endif

uint8_t rand_data[200] = {
    0, 5, 10, 15, 20, 25, 29, 34, 39, 44, 49, 54,
    58, 63, 68, 72, 77, 81, 85, 89, 94, 98, 101, 105,
    109, 113, 116, 120, 123, 126, 129, 132, 135, 137, 140, 142,
    144, 146, 148, 150, 152, 153, 154, 156, 157, 158, 158, 159,
    159, 159, 160, 159, 159, 159, 158, 158, 157, 156, 154, 153,
    152, 150, 148, 146, 144, 142, 140, 137, 135, 132, 129, 126,
    123, 120, 116, 113, 109, 105, 101, 98, 94, 89, 85, 81,
    77, 72, 68, 63, 58, 54, 49, 44, 39, 34, 29, 25,
    20, 15, 10, 5, 0, 5, 10, 15, 20, 25, 29, 34,
    39, 44, 49, 54, 58, 63, 68, 72, 77, 81, 85, 89,
    94, 98, 101, 105, 109, 113, 116, 120, 123, 126, 129, 132,
    135, 137, 140, 142, 144, 146, 148, 150, 152, 153, 154, 156,
    157, 158, 158, 159, 159, 159, 160, 159, 159, 159, 158, 158,
    157, 156, 154, 153, 152, 150, 148, 146, 144, 142, 140, 137,
    135, 132, 129, 126, 123, 120, 116, 113, 109, 105, 101, 98,
    94, 89, 85, 81, 77, 72, 68, 63, 58, 54, 49, 44,
    39, 34, 29, 25, 20, 15, 10, 5
};

void painter_func(sgl_surf_t* surf, sgl_area_t* area, sgl_obj_t* obj)
{
    // 警惕 off-by-one
    for (int i = 0; i < obj->coords.x2 - obj->coords.x1 + 1; i++) {
        // get relative coord
        int16_t coord_x = obj->coords.x1 + i;
        int16_t coord_y =
            obj->coords.y1 + ((obj->coords.y2 - obj->coords.y1) - rand_data[i]);
        int16_t surf_x = coord_x - surf->x1;
        int16_t surf_y = coord_y - surf->y1;

        // check range, clip
        if (surf_x < 0 || surf_x > surf->x2 - surf->x1 || surf_y < 0 ||
            surf_y > surf->y2 - surf->y1) {
            continue;
        } else {
            sgl_surf_set_pixel(surf, surf_x, surf_y, SGL_COLOR_BLUE);
        }
    }
}

void sgl_start(void)
{
    sgl_fbinfo_t fbinfo = {.xres       = PANEL_WIDTH,
                           .yres       = PANEL_HEIGHT,
                           .flush_area = panel_flush_area,
                           .buffer[0]  = panel_buffer_1,
                           //    .buffer[1]   = panel_buffer_2,
                           .buffer_size = BUFFER_SIZE};

#if CONFIG_SGL_DEBUG
    // 注册日志设备，可选
    sgl_logdev_register(uart_put_string);
#endif

    // 注册Framebuffer设备
    sgl_fbdev_register(&fbinfo);

    // 必须先初始化SysTick和屏幕设备，然后再初始化SGL框架
    // 将 sgl_tick_inc(1) 添加到SysTick_Handle
    // 或者void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
    // 如果时基配置为定时器的话
    // 注意, sgl启动之后, 就不可以使用非阻塞延时了,
    // 由于sgl使用脏区域标记加上到时更新(sgl_tick_inc(1)),
    // 因此非阻塞延时会导致cpu一直被占用而sgl tick不被更新,
    // 也就无法完成刷新操作!
    lcd_init();
    sgl_init();

    sgl_obj_t* page = sgl_obj_create(NULL);
    sgl_page_set_color(page, SGL_COLOR_WHITE);
    sgl_screen_load(page);

    sgl_obj_t* label = sgl_label_create(page);
    sgl_obj_set_pos(label, 250, 200);
    sgl_obj_set_size(label, 35, 15);
    sgl_label_set_font(label, &consolas14);
    sgl_label_set_text(label, "label");
    sgl_label_set_bg_color(label, SGL_COLOR_LIGHT_SEA_GREEN);
    // sgl_label_set_radius(label, 25);

    sgl_obj_t* canvas = sgl_canvas_create(page);
    sgl_obj_set_pos(canvas, 10, 10);
    sgl_obj_set_size(canvas, 200, 160);
    sgl_canvas_set_painter_cb(canvas, painter_func);
}
