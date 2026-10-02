#include "sgl_port.h"
#include "lcd.h"
#include "sgl.h"
#include "sgl_config.h"
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
    // 非阻塞式刷新函数, 使用DMA
    lcd_update_image((uint8_t*)src, w * h * sizeof(sgl_color_t));
    // //
    // 调用sgl_fbdev_flush_ready()函数，告诉SGL框架，刷新完成，可以继续处理下一帧处理
    // sgl_fbdev_flush_ready();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    extern SPI_HandleTypeDef hspi1;
    if (hspi == &hspi1) {
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // /* The event has occurred, use the semaphore to unblock the task so
        // the task can process the event. */ xSemaphoreGiveFromISR(lcd_sem,
        // &xHigherPriorityTaskWoken);
        // /* Clear the interrupt here. */
        // /* Now the task has been unblocked a context switch should be
        // performed if xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The
        // syntax required to perform a context switch from an ISR varies from
        // port to port, and from compiler to compiler. Check the web
        // documentation and examples for the port being used to find the syntax
        // required for your application. */
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        // 调用sgl_fbdev_flush_ready()函数，告诉SGL框架，刷新完成，可以继续处理下一帧处理
        sgl_fbdev_flush_ready();
        // 拉高NSS, 在没有阻塞函数(FreeRTOS下就是vTaskDelay系列)的情况下,
        // 要在txcpltcb中再拉高NSS, 不能在lcd数据传输函数中拉高,
        // 因为那时传输并未完成, 会提前导致传输结束
        HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
    }
}

#if CONFIG_SGL_DEBUG
static void uart_put_string(const char* str)
{
    extern UART_HandleTypeDef huart1;
    HAL_UART_Transmit(&huart1, (const uint8_t*)str, strlen(str), HAL_MAX_DELAY);
}
#endif

void sgl_start(void)
{
    sgl_fbinfo_t fbinfo = {.xres        = PANEL_WIDTH,
                           .yres        = PANEL_HEIGHT,
                           .flush_area  = panel_flush_area,
                           .buffer[0]   = panel_buffer_1,
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
    lcd_start();

    sgl_init();

    // 添加一个测试代码
    sgl_obj_t* label = sgl_label_create(NULL);
    sgl_obj_set_size(label, PANEL_WIDTH, 30);
    sgl_obj_set_pos_align(label, SGL_ALIGN_CENTER);
    sgl_label_set_font(label, &consolas24);
    sgl_label_set_text(label, "Hello SGL!");

    sgl_obj_t *new_page = sgl_obj_create(NULL);
    sgl_page_set_color(new_page, SGL_COLOR_YELLOW);
    sgl_screen_load(new_page);
}