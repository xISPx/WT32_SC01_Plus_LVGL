#pragma once

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "../../config/display_config.h"

class DisplayManager {
public:
  static DisplayManager& getInstance();
  bool begin();
  void showStartupScreen();
  LGFX_Sprite& getSprite();
  bool getTouch(uint16_t* x, uint16_t* y);
  WT32SC01PlusDisplay& getTft();

private:
  DisplayManager() = default;
  WT32SC01PlusDisplay _tft;
  LGFX_Sprite _sprite{&_tft};
  bool _initialized = false;
};