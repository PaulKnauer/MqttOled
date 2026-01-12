#pragma once

// WiFi (override via platformio_override.ini build_flags)
#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

// MQTT (override via platformio_override.ini build_flags)
#ifndef MQTT_HOST
#define MQTT_HOST "mosquitto.local"
#endif

#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

#ifndef MQTT_USER
#define MQTT_USER ""
#endif

#ifndef MQTT_PASS
#define MQTT_PASS ""
#endif

#ifndef MQTT_CLIENT_ID
#define MQTT_CLIENT_ID "esp8266-oled"
#endif

// Timing
#ifndef SCREEN_ROTATE_MS
#define SCREEN_ROTATE_MS 5000
#endif

#ifndef MQTT_RECONNECT_MS
#define MQTT_RECONNECT_MS 3000
#endif
