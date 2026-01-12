#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "State.h"

struct AppState {
  WeatherState weather;
  RoomState office;
  RoomState bedroom;
  RoomState garage;
};

uint8_t screensVisibleCount(const AppState& state);
uint8_t screensResolveIndex(const AppState& state, uint8_t visibleIndex);
void screensDraw(U8G2& u8g2, const AppState& state, uint8_t screenIndex);
