#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

#define LCD_I2C_ADDR   (0x27 << 1)  // Change to 0x3F if needed
#define LCD_COLS       16
#define LCD_ROWS       2

void LCD_Init(I2C_HandleTypeDef *hi2c);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Print(const char *str);
void LCD_PrintChar(char c);

static void lcd_write_byte(uint8_t data);
static void lcd_pulse_enable(uint8_t data);
static void lcd_send_nibble(uint8_t nibble, uint8_t mode) ;
static void lcd_send_byte(uint8_t byte, uint8_t mode);
void LCD_Init(I2C_HandleTypeDef *hi2c) ;

#endif




