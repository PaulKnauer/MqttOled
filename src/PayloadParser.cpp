#include "PayloadParser.h"

#include <stdlib.h>
#include <string.h>

static const char* findJsonValue(const char* s, const char* key) {
  const size_t klen = strlen(key);
  const char* p = s;
  while ((p = strstr(p, key)) != nullptr) {
    if (p > s && p[-1] == '"' && p[klen] == '"') {
      const char* q = p + klen + 1;
      while (*q && *q != ':') q++;
      if (*q == ':') {
        q++;
        while (*q == ' ' || *q == '\t') q++;
        return q;
      }
    }
    p += klen;
  }
  return nullptr;
}

static void parseJsonFloat(const char* s, const char* key, float& out) {
  const char* p = findJsonValue(s, key);
  if (!p) return;

  char buf[16];
  size_t i = 0;
  while (*p && i + 1 < sizeof(buf)) {
    const char c = *p;
    if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E') {
      buf[i++] = c;
      p++;
      continue;
    }
    break;
  }
  buf[i] = '\0';
  if (i == 0) return;
  out = atof(buf);
}

static void parseJsonStr(const char* s, const char* key, char* out, size_t outLen) {
  const char* p = findJsonValue(s, key);
  if (!p || *p != '"') return;
  p++;
  size_t i = 0;
  while (*p && *p != '"' && i + 1 < outLen) out[i++] = *p++;
  out[i] = '\0';
}

void parseWeatherPayload(const char* payload, WeatherState& out) {
  if (!payload) return;
  parseJsonFloat(payload, "temp", out.tempC);
  parseJsonFloat(payload, "t", out.tempC);
  parseJsonFloat(payload, "hi", out.hiC);
  parseJsonFloat(payload, "lo", out.loC);
  parseJsonFloat(payload, "wind", out.wind);
  parseJsonFloat(payload, "humidity", out.hum);
  parseJsonFloat(payload, "hum", out.hum);
  parseJsonFloat(payload, "h", out.hum);
  parseJsonStr(payload, "icon", out.icon, sizeof(out.icon));
}

void parseRoomPayload(const char* payload, RoomState& out) {
  if (!payload) return;
  parseJsonFloat(payload, "t", out.tempC);
  parseJsonFloat(payload, "h", out.hum);
}
