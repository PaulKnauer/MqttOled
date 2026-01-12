#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

class WiFiManager {
public:
  WiFiManager(const char* ssid, const char* pass);

  void begin();
  void tick(unsigned long nowMs);
  bool isConnected() const;

private:
  void startConnect(unsigned long nowMs);

  const char* _ssid;
  const char* _pass;
  unsigned long _lastAttemptMs = 0;
  uint32_t _backoffMs = 0;
};
