#include "lcd_task.h"
#include "lcd.h"
#include "sgl.h"
#include "sgl_port.h"
#include "sgl_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t lcd_sem = NULL;

void lcd_task(void* pv_arg) {
    // 创建lcd二值信号量
    lcd_sem = xSemaphoreCreateBinary();

    sgl_start();

    while (1) {
        sgl_task_handler();
        vTaskDelay(pdMS_TO_TICKS(CONFIG_SGL_SYSTICK_MS/2));
    }

    vTaskDelete(NULL);
}