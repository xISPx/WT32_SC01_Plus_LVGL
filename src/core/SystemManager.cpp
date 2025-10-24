#include "SystemManager.h"
#include "../display/DisplayManager.h"
#include "../touch/TouchManager.h"
#include "../utils/Logger.h"

void SystemManager::initialize() {
  LOG_INFO("Initializing PSRAM...");
  if (!psramInit()) {
    LOG_ERROR("PSRAM init failed!");
    
    auto& tft = DisplayManager::getInstance().getTft();
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 100);
    tft.println("PSRAM FAILED!");
    
    while (1) {
      delay(1000);
      LOG_ERROR("PSRAM still not working...");
    }
  } else {
    LOG_INFO("PSRAM initialized successfully");
  }

  LOG_INFO("Initializing display...");
  if (!DisplayManager::getInstance().begin()) {
    LOG_ERROR("Display init failed!");
    while (1) {
      delay(1000);
      LOG_ERROR("Display still not working...");
    }
  }

  LOG_INFO("Initializing touch...");
  TouchManager::begin();
  
  LOG_INFO("Showing startup screen...");
  DisplayManager::getInstance().showStartupScreen();
  
  delay(2000); 
  LOG_INFO("System initialization complete");
}