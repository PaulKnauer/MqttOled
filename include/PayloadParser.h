#pragma once

#include "State.h"

void parseWeatherPayload(const char* payload, WeatherState& out);
void parseRoomPayload(const char* payload, RoomState& out);
