#include "ScreenManager.h"

ScreenManager::ScreenManager(uint32_t rotateMs) : _rotateMs(rotateMs) {}

void ScreenManager::begin(unsigned long nowMs) {
  _lastSwitchMs = nowMs;
  _dirty = true;
}

void ScreenManager::markDirty() {
  _dirty = true;
}

void ScreenManager::tick(U8G2& u8g2, const AppState& state, unsigned long nowMs) {
  const uint8_t visibleCount = screensVisibleCount(state);
  if (visibleCount == 0) return;

  if (_screenIndex >= visibleCount) {
    _screenIndex = 0;
    _dirty = true;
  }

  if (_rotateMs > 0 && (nowMs - _lastSwitchMs) >= _rotateMs) {
    _screenIndex = (_screenIndex + 1) % visibleCount;
    _lastSwitchMs = nowMs;
    _dirty = true;
  }

  if (_dirty) {
    const uint8_t screenIndex = screensResolveIndex(state, _screenIndex);
    screensDraw(u8g2, state, screenIndex);
    _dirty = false;
  }
}

uint8_t ScreenManager::currentScreen() const {
  return _screenIndex;
}
