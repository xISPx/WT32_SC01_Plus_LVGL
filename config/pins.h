#pragma once

// LCD Parallel Interface
#define PIN_LCD_D0   GPIO_NUM_9
#define PIN_LCD_D1   GPIO_NUM_46
#define PIN_LCD_D2   GPIO_NUM_3
#define PIN_LCD_D3   GPIO_NUM_8
#define PIN_LCD_D4   GPIO_NUM_18
#define PIN_LCD_D5   GPIO_NUM_17
#define PIN_LCD_D6   GPIO_NUM_16
#define PIN_LCD_D7   GPIO_NUM_15
#define PIN_LCD_RS   GPIO_NUM_0
#define PIN_LCD_WR   GPIO_NUM_47
#define PIN_LCD_RD   -1
#define PIN_LCD_RST  GPIO_NUM_4

// Backlight
#define PIN_BL       GPIO_NUM_45

// Touch
#define PIN_TP_INT   GPIO_NUM_7
#define PIN_TP_SDA   GPIO_NUM_6
#define PIN_TP_SCL   GPIO_NUM_5
#define TP_I2C_ADDR  0x38