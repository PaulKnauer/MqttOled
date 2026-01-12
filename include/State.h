#pragma once

#include <stdint.h>
#include <math.h>

struct WeatherState {
  float tempC = NAN;
  float hum = NAN;
  float hiC = NAN;
  float loC = NAN;
  float wind = NAN;
  uint16_t windDirDeg = 0;
  char icon[8] = {0};     // e.g. "03d"
  char desc[32] = {0};    // short
  uint32_t updatedMs = 0;
};

struct RoomState {
  float tempC = NAN;
  float hum = NAN;
  bool online = true;
  uint32_t updatedMs = 0;
};
