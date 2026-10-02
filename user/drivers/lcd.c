#include "lcd.h"
#include "main.h"
#include "sgl_port.h"
#include <stdint.h>

static void lcd_bl_control(uint8_t on)
{
    HAL_GPIO_WritePin(lcd_bl_GPIO_Port, lcd_bl_Pin, on);
}

static void lcd_send_cmd(uint8_t cmd)
{
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    // HAL_SPI_Transmit_DMA(&hspi1, &cmd, 1);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

void lcd_send_data(uint8_t* data, uint16_t size)
{
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    // HAL_SPI_Transmit_DMA(&hspi1, data, size);
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_SET);
}

void lcd_send_data_with_dma(uint8_t* data, uint16_t size)
{
    extern SPI_HandleTypeDef hspi1;

    HAL_GPIO_WritePin(lcd_cs_GPIO_Port, lcd_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(lcd_nss_GPIO_Port, lcd_nss_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit_DMA(&hspi1, data, size);
    // HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
}

void lcd_update_range(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    // ST7789 Manual: RAMWR (2Ch)
    // x1, y1 upperleft; x2, y2 lowerright
    // col addr, x
    // CASET (2Ah)
    lcd_send_cmd(0x2a);
    lcd_send_data(
        (uint8_t[]){
            (uint8_t)(x1 >> 8),
            (uint8_t)(x1 & 0xFF),
            (uint8_t)(x2 >> 8),
            (uint8_t)(x2 & 0xFF),
        },
        4);

    // row addr, y
    // RASET (2Bh)
    lcd_send_cmd(0x2b);
    lcd_send_data(
        (uint8_t[]){
            (uint8_t)(y1 >> 8),
            (uint8_t)(y1 & 0xFF),
            (uint8_t)(y2 >> 8),
            (uint8_t)(y2 & 0xFF),
        },
        4);
}

void lcd_update_image(uint8_t* data, uint16_t size)
{
    // RAMWR (2Ch): Memory Write
    lcd_send_cmd(0x2c);
    lcd_send_data_with_dma(data, size);
}

static void lcd_reset(void)
{
    // 关闭背光
    lcd_bl_control(0);

    // reset
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_RESET);
    // vTaskDelay(pdMS_TO_TICKS(1));
    HAL_Delay(1);
    HAL_GPIO_WritePin(lcd_rst_GPIO_Port, lcd_rst_Pin, GPIO_PIN_SET);
    // vTaskDelay(pdMS_TO_TICKS(125));
    HAL_Delay(125);

    // 退出睡眠模式
    lcd_send_cmd(0x11);
    // vTaskDelay(pdMS_TO_TICKS(125));
    HAL_Delay(125);

    // 设置色彩格式为RGB565
    // COLMOD (3Ah)
    lcd_send_cmd(0x3a);
    lcd_send_data((uint8_t[]){0x55}, 1);

    // 不同变种型号的ST7789控制的屏幕会存在差异
    // INVOFF (20h): Display Inversion Off
    lcd_send_cmd(0x20);
    // // INVON (21h): Display Inversion On
    // lcd_send_cmd(0x21);

    // 开启屏幕显示
    lcd_send_cmd(0x29);

    // 开启背光
    lcd_bl_control(1);
}

// test
#define BLOCK_SIZE 100

void lcd_start(void)
{
    lcd_reset();

    // test
    // 1
    // col addr
    lcd_send_cmd(0x2a);
    lcd_send_data((uint8_t[]){0x00, 0x46, 0x00, 0xA9}, 4);
    // row addr
    lcd_send_cmd(0x2b);
    lcd_send_data((uint8_t[]){0x00, 0x6E, 0x00, 0xD1}, 4);
    static const uint16_t block_1[BLOCK_SIZE * BLOCK_SIZE] = {
        [0 ...(BLOCK_SIZE * BLOCK_SIZE / 2 - 1)] = 0xffff,
        [(BLOCK_SIZE * BLOCK_SIZE / 2)...(BLOCK_SIZE * BLOCK_SIZE - 1)] =
            0x0000};
    lcd_update_image((uint8_t*)block_1, sizeof(block_1));
    HAL_Delay(500);
    // 2
    lcd_update_range(0x0046, 0x006e, 0x00a9, 0x00d1);
    static const uint16_t block_2[BLOCK_SIZE * BLOCK_SIZE] = {
        [0 ...(BLOCK_SIZE * BLOCK_SIZE / 2 - 1)] = 0x00f8,
        [(BLOCK_SIZE * BLOCK_SIZE / 2)...(BLOCK_SIZE * BLOCK_SIZE - 1)] =
            0x1f00};
    lcd_update_image((uint8_t*)block_2, sizeof(block_2));
    HAL_Delay(500);
}
