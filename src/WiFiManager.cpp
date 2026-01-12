#include "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* pass)
    : _ssid(ssid), _pass(pass) {}

void WiFiManager::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  startConnect(millis());
}

void WiFiManager::tick(unsigned long nowMs) {
  static bool connectedReported = false;
  if (WiFi.status() == WL_CONNECTED) {
    if (!connectedReported) {
      connectedReported = true;
    }
    _backoffMs = 0;
    return;
  }
  if (connectedReported) {
    connectedReported = false;
  }

  if (_backoffMs == 0 || (nowMs - _lastAttemptMs) >= _backoffMs) {
    startConnect(nowMs);
  }
}

bool WiFiManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::startConnect(unsigned long nowMs) {
  _lastAttemptMs = nowMs;
  WiFi.begin(_ssid, _pass);

  if (_backoffMs == 0) {
    _backoffMs = 2000;
  } else if (_backoffMs < 60000) {
    _backoffMs *= 2;
    if (_backoffMs > 60000) _backoffMs = 60000;
  }
}
