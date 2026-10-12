#include "user_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd_task.h"

void init_task(void const* argument)
{
    taskENTER_CRITICAL();

    xTaskCreate(lcd_task, "lcd_sgl_task", 512, NULL, 2, NULL);

    taskEXIT_CRITICAL();

    vTaskDelete(NULL);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
    int i = 0;
    while (1) {
        i = -i;
    };
}
