#pragma once

#include <Arduino.h>

class TouchManager {
public:
  static void begin();
  static void loop();
  static bool isTouched();
  static void getCoordinates(uint16_t& x, uint16_t& y);
};