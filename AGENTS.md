# AGENTS.md — ESP8266OLEDDisplay

## Project
Firmware for ESP8266 D1 Mini with 64x48 SSD1306 OLED (I2C) using PlatformIO + Arduino framework.
UI uses U8g2. Data arrives via MQTT (Mosquitto).

## Primary goals
1. Keep firmware simple and robust:
   - No direct HTTP calls to weather APIs from the ESP8266
   - Consume weather from MQTT topic `OpenWeather/Current`
   - Consume room readings from MQTT topics `Office/DHT22/Reading`, `Bedroom/DHT22/Reading`, `Garage/DHT22/Reading`
   - Consume room status from MQTT topics `Office/DHT22/Status`, `Bedroom/DHT22/Status`, `Garage/DHT22/Status`
2. Render a 4-screen rotating UI on a 64x48 OLED.
3. Provide clear configuration via `include/Config.h` (not committed) and secrets via `platformio_override.ini`.

## Tooling
- PlatformIO (VS Code)
- Build: `pio run`
- Upload: `pio run -t upload`
- Serial monitor: `pio device monitor -b 115200`

## Coding conventions
- Prefer small, testable modules:
  - `mqtt_client.*` owns MQTT connection + reconnection behavior
  - `screens.*` owns drawing and layout
  - `Main.cpp` wires components together
- Avoid dynamic allocation (new/malloc) where possible.
- Keep payload parsing lightweight.
- Use `millis()` scheduling; do not block in `loop()`.

## MQTT payload expectations
Weather (JSON):
- `temp`, `hi`, `lo`, `wind`, `humidity`, `icon`
Example:
`{"temp":26.4,"hi":29,"lo":18,"wind":4.6,"humidity":51,"icon":"03d"}`

Room climate (JSON):
- `t` (Celsius), `h` (percent)
Example:
`{"t":22.1,"h":48}`

## Display constraints
- OLED is 64x48 pixels; UI must be legible at arm’s length.
- Avoid overcrowding; do not add verbose labels.
- Prefer U8g2 fonts:
  - `u8g2_font_5x8_tf` for labels
  - `u8g2_font_9x15_tf` for primary values

## Definition of done for changes
- `pio run` succeeds
- No compiler warnings introduced (when feasible)
- UI remains within bounds (no drawing off-screen)
- MQTT reconnect behavior stable (does not reboot-loop)
