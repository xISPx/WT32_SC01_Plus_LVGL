#include "DisplayManager.h"
#include "../utils/Logger.h"

DisplayManager& DisplayManager::getInstance() {
  static DisplayManager instance;
  return instance;
}

bool DisplayManager::begin() {
  if (_initialized) return true;

  LOG_INFO("Initializing TFT...");
  

  _tft.init();
  _tft.setRotation(1); // 480x320
  _tft.fillScreen(TFT_BLACK);
  
  LOG_INFO("Display rotation set to 1 (480x320)");


  if (!psramFound()) {
    LOG_WARN("PSRAM not found - using internal RAM");
  } else {
    LOG_INFO("PSRAM found and available");
  }


  _sprite.setColorDepth(16);
  _sprite.setPsram(psramFound());
  
  if (!_sprite.createSprite(480, 320)) {
    LOG_WARN("Sprite creation failed - continuing without sprite");
  } else {
    LOG_INFO("Sprite created successfully");
  }

  _initialized = true;
  LOG_INFO("Display initialized successfully");
  return true;
}

void DisplayManager::showStartupScreen() {
  auto& tft = getTft();
  tft.fillScreen(TFT_BLACK);
  
  // Первая строка
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  String text1 = "WT32-SC01 Plus";
  int textWidth = tft.textWidth(text1);
  int xPos = (480 - textWidth) / 2;
  tft.setCursor(xPos, 140);
  tft.println(text1);
  
  // Вторая строка
  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  String text2 = "LVGL Demo Starting...";
  textWidth = tft.textWidth(text2);
  xPos = (480 - textWidth) / 2;
  tft.setCursor(xPos, 170);
  tft.println(text2);
  
  LOG_INFO("Startup screen displayed");
}

LGFX_Sprite& DisplayManager::getSprite() {
  return _sprite;
}

bool DisplayManager::getTouch(uint16_t* x, uint16_t* y) {
  return _tft.getTouch(x, y);
}

WT32SC01PlusDisplay& DisplayManager::getTft() {
  return _tft;
}