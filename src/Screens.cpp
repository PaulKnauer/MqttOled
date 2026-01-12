#include "Screens.h"

namespace {
constexpr uint8_t kScreenWidth = 64;
constexpr uint8_t kScreenHeight = 48;
}

static void printFloatOrDash(U8G2& u8g2, float v, const char* fmt) {
  char buf[16];
  if (isnan(v)) {
    strcpy(buf, "--");
  } else {
    snprintf(buf, sizeof(buf), fmt, v);
  }
  u8g2.print(buf);
}

static void drawCenteredText(U8G2& u8g2, int y, const char* text) {
  const int textWidth = u8g2.getStrWidth(text);
  const int x = (kScreenWidth - textWidth) / 2;
  u8g2.setCursor((x < 0) ? 0 : x, y);
  u8g2.print(text);
}

static void drawTempCentered(U8G2& u8g2, float tempC) {
  char buf[12];
  if (isnan(tempC)) {
    strcpy(buf, "--");
  } else {
    snprintf(buf, sizeof(buf), "%.1fC", tempC);
  }
  drawCenteredText(u8g2, 33, buf);
}

static void drawRoom(U8G2& u8g2, const char* title, const RoomState& room) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(0, 9);
  u8g2.print(title);

  // Big temp (room), centered for balance.
  u8g2.setFont(u8g2_font_10x20_tf);
  drawTempCentered(u8g2, room.tempC);

  // Humidity footer.
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.setCursor(0, kScreenHeight - 1);
  u8g2.print("H ");
  printFloatOrDash(u8g2, room.hum, "%.0f");
  u8g2.print("%");
}

uint8_t screensVisibleCount(const AppState& state) {
  uint8_t count = 0;
  if (state.office.online) count++;
  if (state.bedroom.online) count++;
  if (state.garage.online) count++;
  return count;
}

uint8_t screensResolveIndex(const AppState& state, uint8_t visibleIndex) {
  uint8_t idx = 0;
  if (state.office.online) {
    if (idx == visibleIndex) return 0;
    idx++;
  }
  if (state.bedroom.online) {
    if (idx == visibleIndex) return 1;
    idx++;
  }
  if (state.garage.online) {
    if (idx == visibleIndex) return 2;
  }
  return 0;
}

void screensDraw(U8G2& u8g2, const AppState& state, uint8_t screenIndex) {
  u8g2.clearBuffer();

  switch (screenIndex) {
    case 0: drawRoom(u8g2, "Office",  state.office); break;
    case 1: drawRoom(u8g2, "Bedroom", state.bedroom); break;
    case 2: drawRoom(u8g2, "Garage",  state.garage); break;
    default: break;
  }

  u8g2.sendBuffer();
}
