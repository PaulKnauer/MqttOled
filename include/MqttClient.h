#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

typedef void (*MqttMessageHandler)(const char* topic, const uint8_t* payload, unsigned int len);

class MqttClient {
public:
  MqttClient(Client& netClient);

  void setServer(const char* host, uint16_t port);
  void setAuth(const char* user, const char* pass);
  void setClientId(const char* clientId);
  void setSubscriptions(const char* const* topics, size_t count);

  void begin(MqttMessageHandler handler);
  void loop();

  bool connected();
  bool ensureConnected();
  bool subscribe(const char* topic);
  void setReconnectInterval(uint32_t ms);

private:
  void onConnected();
  void handleMessage(char* topic, byte* payload, unsigned int len);
  static void _callbackThunk(char* topic, byte* payload, unsigned int len);

  PubSubClient _mqtt;
  const char* _host = nullptr;
  uint16_t _port = 1883;
  const char* _user = nullptr;
  const char* _pass = nullptr;
  const char* _clientId = "esp8266-oled";
  uint32_t _reconnectMs = 3000;

  unsigned long _lastConnectAttempt = 0;
  bool _wasConnected = false;

  MqttMessageHandler _handler = nullptr;
  const char* const* _topics = nullptr;
  size_t _topicCount = 0;
  static MqttClient* _instance;
};
