MqttOled

D1 Mini (ESP8266) + 0.66" OLED Shield (64x48, SSD1306, I2C) room climate dashboard.

## Features
- 4 rotating screens:
  1) Weather overview (current, hi/lo, wind, icon)
  2) Office (temp/humidity)
  3) Bedroom (temp/humidity)
  4) Garage (temp/humidity)
- MQTT input for weather + room sensors
- U8g2 rendering
 - Room screens hidden when status is offline

## Hardware
- Wemos/Lolin D1 Mini
- OLED 0.66" Shield (64x48, SSD1306, I2C address 0x3C/0x3D)

## Build
1. Install PlatformIO in VS Code
2. Copy `platformio_override.ini.exmple` to `platformio_override.ini` and set WiFi/MQTT values (escaped quotes are ok)
3. Copy `include/Config.Example.h` to `include/Config.h` (kept as defaults)
3. Build/Upload

## MQTT Topics
- Weather: `OpenWeather/Current` (recommended retained)
- Rooms: `Office/DHT22/Reading`, `Bedroom/DHT22/Reading`, `Garage/DHT22/Reading`
- Status: `Office/DHT22/Status`, `Bedroom/DHT22/Status`, `Garage/DHT22/Status`

### Payload format (JSON)
Weather:
`{"temp":26.4,"hi":29,"lo":18,"wind":4.6,"humidity":51,"icon":"03d"}`

Room climate:
`{"t":22.1,"h":48}`
