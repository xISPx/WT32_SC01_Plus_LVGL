#pragma once

#include <LovyanGFX.hpp>
#include "pins.h"

class WT32SC01PlusDisplay : public lgfx::LGFX_Device {
public:
  lgfx::Bus_Parallel8 _bus;
  lgfx::Panel_ST7796 _panel;
  lgfx::Light_PWM _light;
  lgfx::Touch_FT5x06 _touch;

  WT32SC01PlusDisplay() {
    // Bus
    auto cfg = _bus.config();
    cfg.pin_d0 = PIN_LCD_D0; cfg.pin_d1 = PIN_LCD_D1;
    cfg.pin_d2 = PIN_LCD_D2; cfg.pin_d3 = PIN_LCD_D3;
    cfg.pin_d4 = PIN_LCD_D4; cfg.pin_d5 = PIN_LCD_D5;
    cfg.pin_d6 = PIN_LCD_D6; cfg.pin_d7 = PIN_LCD_D7;
    cfg.pin_rs = PIN_LCD_RS; cfg.pin_wr = PIN_LCD_WR;
    cfg.pin_rd = PIN_LCD_RD;
    cfg.freq_write = 24000000;
    _bus.config(cfg);
    _panel.setBus(&_bus);

    // Panel
    auto p = _panel.config();
    p.pin_cs = -1;
    p.pin_rst = PIN_LCD_RST;
    p.panel_width = 320;
    p.panel_height = 480;
    p.invert = true;
    p.bus_shared = true;
    _panel.config(p);

    // Backlight
    auto l = _light.config();
    l.pin_bl = PIN_BL;
    l.freq = 44100;
    _light.config(l);
    _panel.setLight(&_light);

    // Touch
    auto t = _touch.config();
    t.pin_int = PIN_TP_INT;
    t.pin_rst = -1;
    t.pin_sda = PIN_TP_SDA;
    t.pin_scl = PIN_TP_SCL;
    t.i2c_port = 0;
    t.i2c_addr = TP_I2C_ADDR;
    t.bus_shared = false;
    t.x_min = 0; t.x_max = 320;
    t.y_min = 0; t.y_max = 480;
    _touch.config(t);
    _panel.setTouch(&_touch);

    setPanel(&_panel);
  }
};