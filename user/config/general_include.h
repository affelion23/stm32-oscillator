#ifndef _GENERAL_INCLUDE_H_
#define _GENERAL_INCLUDE_H_

#define TEST 1

#if !TEST
#include "buzzer_task.h"
#include "dmm.h"
#include "key_task.h"
#include "lcd_task.h"
#include "led_task.h"
#include "uart_task.h"
#else
#include "sgl_port.h"
#endif

void initial_task(void const *argument) {
  taskENTER_CRITICAL();

#if !TEST
  xTaskCreate(buzzer_task, "buzzer_task", 64, NULL, 1, NULL);
  xTaskCreate(led_task, "led_task", 128, NULL, 1, NULL);
  xTaskCreate(uart_task, "uart_task", 128, NULL, 1, NULL);
  xTaskCreate(lcd_task, "lcd_task", 256, NULL, 2, NULL);
  xTaskCreate(key_task, "key_task", 256, NULL, 3, NULL);
#else

#endif

  taskEXIT_CRITICAL();

  vTaskDelete(NULL);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  int i = 0;
  while (1) {
    i = -i;
  };
}

#endif // _GENERAL_INCLUDE_H_