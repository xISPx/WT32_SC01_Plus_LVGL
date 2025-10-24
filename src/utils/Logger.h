#pragma once

#include <Arduino.h>

#define LOG_LEVEL_VERBOSE 0
#define LOG_LEVEL_DEBUG   1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_WARN    3
#define LOG_LEVEL_ERROR   4
#define LOG_LEVEL_NONE    5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#if LOG_LEVEL <= LOG_LEVEL_VERBOSE
  #define LOG_VERBOSE(fmt, ...) Serial.printf("[V] " fmt "\n", ##__VA_ARGS__)
#else
  #define LOG_VERBOSE(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_DEBUG
  #define LOG_DEBUG(fmt, ...) Serial.printf("[D] " fmt "\n", ##__VA_ARGS__)
#else
  #define LOG_DEBUG(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_INFO
  #define LOG_INFO(fmt, ...) Serial.printf("[I] " fmt "\n", ##__VA_ARGS__)
#else
  #define LOG_INFO(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_WARN
  #define LOG_WARN(fmt, ...) Serial.printf("[W] " fmt "\n", ##__VA_ARGS__)
#else
  #define LOG_WARN(...)
#endif

#if LOG_LEVEL <= LOG_LEVEL_ERROR
  #define LOG_ERROR(fmt, ...) Serial.printf("[E] " fmt "\n", ##__VA_ARGS__)
#else
  #define LOG_ERROR(...)
#endif