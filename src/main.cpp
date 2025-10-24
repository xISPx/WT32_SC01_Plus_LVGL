#include <Arduino.h>
#include <lvgl.h>
#include "core/SystemManager.h"
#include "touch/TouchManager.h"
#include "display/DisplayManager.h"
#include "utils/Logger.h"

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_display_t* disp = nullptr;
static lv_indev_t* indev = nullptr;
static lv_obj_t* touch_label = nullptr;
static lv_color_t* lvgl_buf1 = nullptr;
static lv_color_t* lvgl_buf2 = nullptr;
static const uint32_t buf_size = screenWidth * 40;

void my_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
  auto& tft = DisplayManager::getInstance().getTft();
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixels((uint16_t*)px_map, w * h, true);
  tft.endWrite();
  lv_display_flush_ready(display);
}

void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data) {
  uint16_t x, y;
  if (DisplayManager::getInstance().getTouch(&x, &y)) {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = x;
    data->point.y = y;
    char buf[64];
    snprintf(buf, sizeof(buf), "Touch: %d, %d", x, y);
    lv_label_set_text(touch_label, buf);
  } else {
    data->state = LV_INDEV_STATE_REL;
    lv_label_set_text(touch_label, "Touch: none");
  }
}

static lv_color_t bg_colors[] = {
    lv_color_hex(0x0000FF),
    lv_color_hex(0x00FF00),
    lv_color_hex(0xFF0000),
    lv_color_hex(0x000000)
};
static uint8_t color_index = 0;

void btn_color_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED) {
    color_index = (color_index + 1) % (sizeof(bg_colors) / sizeof(bg_colors[0]));
    lv_obj_set_style_bg_color(lv_screen_active(), bg_colors[color_index], 0);
    LOG_INFO("Background color changed to index %d", color_index);
  }
}

void create_demo_ui() {
  lv_obj_t* screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x003a57), 0);
  
  lv_obj_t *title = lv_label_create(screen);
  lv_label_set_text(title, "LVGL + WT32-SC01 Plus");
  lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
  lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

  lv_obj_t* status_label = lv_label_create(screen);
  lv_label_set_text(status_label, "System: READY");
  lv_obj_set_style_text_color(status_label, lv_color_hex(0x00FF00), 0);
  lv_obj_align(status_label, LV_ALIGN_TOP_MID, 0, 50);

  touch_label = lv_label_create(screen);
  lv_label_set_text(touch_label, "Touch: waiting...");
  lv_obj_set_style_text_color(touch_label, lv_color_hex(0xFFFF00), 0);
  lv_obj_align(touch_label, LV_ALIGN_TOP_MID, 0, 80);

  lv_obj_t *btn = lv_btn_create(screen);
  lv_obj_set_size(btn, 200, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btn, btn_color_event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Change Color");
  lv_obj_center(btn_label);

  lv_obj_t *footer = lv_label_create(screen);
  lv_label_set_text(footer, "LVGL v9.4.0 | ESP32-S3");
  lv_obj_set_style_text_color(footer, lv_color_hex(0x888888), 0);
  lv_obj_set_style_text_opa(footer, LV_OPA_70, 0);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, -10);
}

bool init_lvgl() {
  LOG_INFO("Initializing LVGL...");
  lv_init();
  lv_tick_set_cb((lv_tick_get_cb_t)millis);
  
  LOG_INFO("Allocating LVGL buffers...");
  lvgl_buf1 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  lvgl_buf2 = (lv_color_t*)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  
  if (!lvgl_buf1 || !lvgl_buf2) {
    LOG_ERROR("Failed to allocate LVGL buffers!");
    if (lvgl_buf1) heap_caps_free(lvgl_buf1);
    if (lvgl_buf2) heap_caps_free(lvgl_buf2);
    return false;
  }
  
  LOG_INFO("LVGL buffers allocated: %d bytes each", buf_size * sizeof(lv_color_t));
  LOG_INFO("Creating LVGL display...");
  
  disp = lv_display_create(screenWidth, screenHeight);
  if (!disp) {
    LOG_ERROR("Failed to create LVGL display!");
    heap_caps_free(lvgl_buf1);
    heap_caps_free(lvgl_buf2);
    return false;
  }

  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, lvgl_buf1, lvgl_buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
  
  LOG_INFO("Creating input device...");
  indev = lv_indev_create();
  if (!indev) {
    LOG_ERROR("Failed to create input device!");
    return false;
  }
  
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touch_read);
  create_demo_ui();
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  LOG_INFO("=== WT32-SC01 Plus LVGL Demo ===");
  SystemManager::initialize();

  if (!init_lvgl()) {
    LOG_ERROR("LVGL initialization failed!");
    auto& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 100);
    tft.println("LVGL INIT FAILED!");
    tft.setTextSize(1);
    tft.setCursor(30, 140);
    tft.println("Check serial monitor for details");
    
    while(1) {
      delay(1000);
      LOG_ERROR("System halted due to LVGL init failure");
    }
  }
}

void loop() {
  lv_timer_handler();
  TouchManager::loop();
  delay(5);
}