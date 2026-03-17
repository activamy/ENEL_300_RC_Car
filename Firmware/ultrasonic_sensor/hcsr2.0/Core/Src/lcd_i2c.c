#include "lcd_i2c.h"

static I2C_HandleTypeDef *_hi2c;

// PCF8574 bit mapping:
// Bit 7=D7, 6=D6, 5=D5, 4=D4, 3=BL, 2=EN, 1=RW, 0=RS

#define LCD_BL   0x08
#define LCD_EN   0x04
#define LCD_RW   0x02
#define LCD_RS   0x01

static void lcd_write_byte(uint8_t data) {
    HAL_I2C_Master_Transmit(_hi2c, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
}
/*
 * lcd write byte 0x04, delay then write ~0x04
 * */
static void lcd_pulse_enable(uint8_t data) {
    lcd_write_byte(data | LCD_EN);
    HAL_Delay(1);
    lcd_write_byte(data & ~LCD_EN);
    HAL_Delay(1);
}

/**
 * lcd write and pulse ((nibble & 0xF0)|0x08|mode)
 *
 */
static void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | LCD_BL | mode;
    lcd_write_byte(data);
    lcd_pulse_enable(data);
}

/**
 * send byte = nibble + nibble
 */
static void lcd_send_byte(uint8_t byte, uint8_t mode) {
    lcd_send_nibble(byte & 0xF0, mode);
    lcd_send_nibble((byte << 4) & 0xF0, mode);
}

void LCD_Init(I2C_HandleTypeDef *hi2c) {
    _hi2c = hi2c;
    HAL_Delay(50);


    // 4-bit init sequence
    lcd_send_nibble(0x30, 0); HAL_Delay(5);
    lcd_send_nibble(0x30, 0); HAL_Delay(5);
    lcd_send_nibble(0x30, 0); HAL_Delay(5);
    lcd_send_nibble(0x20, 0); HAL_Delay(1); // Switch to 4-bit

    lcd_send_byte(0x28, 0); // 4-bit, 2 lines, 5x8
    lcd_send_byte(0x0C, 0); // Display ON, cursor OFF
    lcd_send_byte(0x06, 0); // Entry mode: increment, no shift
    lcd_send_byte(0x01, 0); // Clear display
    HAL_Delay(2);
}

void LCD_Clear(void) {
    lcd_send_byte(0x01, 0);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_send_byte(0x80 | (col + row_offsets[row]), 0);
}

void LCD_PrintChar(char c) {
    lcd_send_byte((uint8_t)c, LCD_RS);
}

void LCD_Print(const char *str) {
    while (*str) LCD_PrintChar(*str++);
}
