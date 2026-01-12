#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>

#include "Config.h"     // create from Config.Example.h
#include "Topics.h"
#include "MqttClient.h"
#include "ScreenManager.h"
#include "Screens.h"
#include "PayloadParser.h"
#include "WiFiManager.h"

// U8g2 constructor for 64x48 SSD1306 (ER = EastRising variant, commonly used for 64x48)
// This constructor is known to be used for 64x48 SSD1306 modules. :contentReference[oaicite:2]{index=2}
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

WiFiClient wifi;
MqttClient mqtt(wifi);
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
ScreenManager screenManager(SCREEN_ROTATE_MS);

AppState app;

static const char* const kMqttTopics[] = {
  TOPIC_OFFICE_READING,
  TOPIC_BEDROOM_READING,
  TOPIC_GARAGE_READING,
  TOPIC_OFFICE_STATUS,
  TOPIC_BEDROOM_STATUS,
  TOPIC_GARAGE_STATUS,
};

static void onMqttMessage(const char* topic, const uint8_t* payload, unsigned int len) {
  // Copy payload into a null-terminated buffer
  static char buf[256];
  const unsigned int n = (len < sizeof(buf) - 1) ? len : (sizeof(buf) - 1);
  memcpy(buf, payload, n);
  buf[n] = '\0';

  const unsigned long now = millis();

  bool updated = false;

  auto setStatus = [&](RoomState& r, bool online) {
    r.online = online;
  };

  const bool statusOnline = (strcmp(buf, "online") == 0);
  const bool statusOffline = (strcmp(buf, "offline") == 0);

  if (strcmp(topic, TOPIC_OFFICE_STATUS) == 0 && (statusOnline || statusOffline)) {
    setStatus(app.office, statusOnline);
    updated = true;
  }
  if (strcmp(topic, TOPIC_BEDROOM_STATUS) == 0 && (statusOnline || statusOffline)) {
    setStatus(app.bedroom, statusOnline);
    updated = true;
  }
  if (strcmp(topic, TOPIC_GARAGE_STATUS) == 0 && (statusOnline || statusOffline)) {
    setStatus(app.garage, statusOnline);
    updated = true;
  }

  // Room climate: JSON {"t":31,"h":42.8,...}
  auto parseRoom = [&](RoomState& r) {
    parseRoomPayload(buf, r);
    r.updatedMs = now;
    r.online = true;
  };

  if (strcmp(topic, TOPIC_OFFICE_READING) == 0)  { parseRoom(app.office);  updated = true; }
  if (strcmp(topic, TOPIC_BEDROOM_READING) == 0) { parseRoom(app.bedroom); updated = true; }
  if (strcmp(topic, TOPIC_GARAGE_READING) == 0)  { parseRoom(app.garage);  updated = true; }

  if (updated) {
    screenManager.markDirty();
  }
}

void setup() {
  Serial.begin(115200);
  delay(50);

  // OLED
  u8g2.begin();
  u8g2.setContrast(255);

  // WiFi
  wifiManager.begin();

  // MQTT
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setAuth(MQTT_USER, MQTT_PASS);
  mqtt.setClientId(MQTT_CLIENT_ID);
  mqtt.setReconnectInterval(MQTT_RECONNECT_MS);
  mqtt.setSubscriptions(kMqttTopics, sizeof(kMqttTopics) / sizeof(kMqttTopics[0]));
  mqtt.begin(onMqttMessage);

  screenManager.begin(millis());
  screenManager.markDirty();
  screenManager.tick(u8g2, app, millis());
}

void loop() {
  const unsigned long now = millis();

  // Keep WiFi alive
  wifiManager.tick(now);

  // MQTT reconnect + loop
  if (wifiManager.isConnected()) {
    mqtt.ensureConnected();
    mqtt.loop();
  }

  screenManager.tick(u8g2, app, now);
}
