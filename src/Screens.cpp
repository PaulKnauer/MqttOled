#include "Screens.h"


static void printFloatOrDash(U8G2& u8g2, float v, const char* fmt) {
  char buf[16];
  if (isnan(v)) {
    strcpy(buf, "--");
  } else {
    snprintf(buf, sizeof(buf), fmt, v);
  }
  u8g2.print(buf);
}

static void drawRoom(U8G2& u8g2, const char* title, const RoomState& room) {
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.setCursor(0, 8);
  u8g2.print(title);

  // Big temp (room)
  u8g2.setFont(u8g2_font_9x15_tf);
  u8g2.setCursor(0, 30);
  printFloatOrDash(u8g2, room.tempC, "%.1f");
  u8g2.print("C");

  // Humidity
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.setCursor(0, 42);
  u8g2.print("H ");
  printFloatOrDash(u8g2, room.hum, "%.0f");
  u8g2.print("%");
}

uint8_t screensVisibleCount(const AppState& state) {
  uint8_t count = 1; // Weather is always visible
  if (state.office.online) count++;
  if (state.bedroom.online) count++;
  if (state.garage.online) count++;
  return count;
}

uint8_t screensResolveIndex(const AppState& state, uint8_t visibleIndex) {
  if (visibleIndex == 0) return 0; // Weather
  uint8_t idx = 1;
  if (state.office.online) {
    if (idx == visibleIndex) return 1;
    idx++;
  }
  if (state.bedroom.online) {
    if (idx == visibleIndex) return 2;
    idx++;
  }
  if (state.garage.online) {
    if (idx == visibleIndex) return 3;
  }
  return 0;
}

void screensDraw(U8G2& u8g2, const AppState& state, uint8_t screenIndex) {
  u8g2.clearBuffer();

  switch (screenIndex) {
    case 0: {
      // Weather overview
      u8g2.setFont(u8g2_font_5x8_tf);
      u8g2.setCursor(0, 8);
      u8g2.print("Weather");

      // Temp big
      u8g2.setFont(u8g2_font_9x15_tf);
      u8g2.setCursor(0, 30);
      printFloatOrDash(u8g2, state.weather.tempC, "%.1f");
      u8g2.print("C");

      // Humidity + wind small at bottom
      u8g2.setFont(u8g2_font_5x8_tf);
      u8g2.setCursor(0, 42);
      u8g2.print("H ");
      printFloatOrDash(u8g2, state.weather.hum, "%.0f");
      u8g2.print("%");

      u8g2.setCursor(36, 42);
      u8g2.print("W ");
      printFloatOrDash(u8g2, state.weather.wind, "%.1f");
      break;
    }
    case 1: drawRoom(u8g2, "Office",  state.office); break;
    case 2: drawRoom(u8g2, "Bedroom", state.bedroom); break;
    case 3: drawRoom(u8g2, "Garage",  state.garage); break;
    default: break;
  }

  u8g2.sendBuffer();
}
