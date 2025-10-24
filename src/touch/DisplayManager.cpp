#include "TouchManager.h"
#include "../display/DisplayManager.h"
#include "../utils/Logger.h"

static bool _touched = false;
static uint16_t _lastX = 0, _lastY = 0;
static uint32_t _lastTouchTime = 0;

void TouchManager::begin() {
  LOG_INFO("Touch manager initialized");
}

void TouchManager::loop() {
  uint16_t x, y;
  if (DisplayManager::getInstance().getTouch(&x, &y)) {
    if (millis() - _lastTouchTime > 50) { 
      _touched = true;
      _lastX = x;
      _lastY = y;
      _lastTouchTime = millis();
      LOG_DEBUG("Touch: %d, %d", x, y);
    }
  } else {
    _touched = false;
  }
}

bool TouchManager::isTouched() {
  return _touched;
}

void TouchManager::getCoordinates(uint16_t& x, uint16_t& y) {
  x = _lastX;
  y = _lastY;
}