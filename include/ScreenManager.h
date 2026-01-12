#pragma once

#include <Arduino.h>
#include <U8g2lib.h>

#include "Screens.h"

class ScreenManager {
public:
  explicit ScreenManager(uint32_t rotateMs);

  void begin(unsigned long nowMs);
  void markDirty();
  void tick(U8G2& u8g2, const AppState& state, unsigned long nowMs);
  uint8_t currentScreen() const;

private:
  uint32_t _rotateMs;
  uint8_t _screenIndex = 0;
  unsigned long _lastSwitchMs = 0;
  bool _dirty = true;
};
