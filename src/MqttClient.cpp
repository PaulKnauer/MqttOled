#include "MqttClient.h"

MqttClient* MqttClient::_instance = nullptr;

MqttClient::MqttClient(Client& netClient) : _mqtt(netClient) {}

void MqttClient::setServer(const char* host, uint16_t port) {
  _host = host;
  _port = port;
  _mqtt.setServer(_host, _port);
}

void MqttClient::setAuth(const char* user, const char* pass) {
  _user = user;
  _pass = pass;
}

void MqttClient::setClientId(const char* clientId) {
  _clientId = clientId;
}

void MqttClient::setSubscriptions(const char* const* topics, size_t count) {
  _topics = topics;
  _topicCount = count;
}

void MqttClient::begin(MqttMessageHandler handler) {
  _handler = handler;
  _instance = this;
  _mqtt.setCallback(MqttClient::_callbackThunk);
}

bool MqttClient::connected() {
  return _mqtt.connected();
}

bool MqttClient::ensureConnected() {
  const unsigned long now = millis();
  if (_mqtt.connected()) {
    if (!_wasConnected) {
      _wasConnected = true;
      onConnected();
    }
    return true;
  }
  _wasConnected = false;

  if (now - _lastConnectAttempt < _reconnectMs) return false;
  _lastConnectAttempt = now;

  bool ok;
  if (_user && strlen(_user) > 0) {
    ok = _mqtt.connect(_clientId, _user, _pass);
  } else {
    ok = _mqtt.connect(_clientId);
  }

  if (!ok) {
    // Back off; loop will retry
    return false;
  }

  _wasConnected = true;
  onConnected();
  return true;
}

void MqttClient::loop() {
  if (_mqtt.connected()) {
    _mqtt.loop();
  } else {
    _wasConnected = false;
  }
}

bool MqttClient::subscribe(const char* topic) {
  if (!_mqtt.connected()) return false;
  return _mqtt.subscribe(topic);
}

void MqttClient::setReconnectInterval(uint32_t ms) {
  _reconnectMs = ms;
}

void MqttClient::onConnected() {
  if (!_topics || _topicCount == 0) return;
  for (size_t i = 0; i < _topicCount; ++i) {
    _mqtt.subscribe(_topics[i]);
  }
}

void MqttClient::handleMessage(char* topic, byte* payload, unsigned int len) {
  if (_handler) _handler(topic, payload, len);
}

void MqttClient::_callbackThunk(char* topic, byte* payload, unsigned int len) {
  if (_instance) _instance->handleMessage(topic, payload, len);
}
