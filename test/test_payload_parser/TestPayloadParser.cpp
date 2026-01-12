#include <math.h>
#include <string.h>
#include <unity.h>

#include "PayloadParser.h"

static void test_weather_parse_full() {
  WeatherState w;
  parseWeatherPayload("{\"temp\":26.4,\"hi\":29,\"lo\":18,\"wind\":4.6,\"humidity\":51,\"icon\":\"03d\"}", w);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 26.4f, w.tempC);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 29.0f, w.hiC);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 18.0f, w.loC);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 4.6f, w.wind);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 51.0f, w.hum);
  TEST_ASSERT_EQUAL_STRING("03d", w.icon);
}

static void test_weather_parse_partial() {
  WeatherState w;
  parseWeatherPayload("{\"temp\":21.0,\"icon\":\"10n\"}", w);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 21.0f, w.tempC);
  TEST_ASSERT_TRUE(isnan(w.hiC));
  TEST_ASSERT_TRUE(isnan(w.loC));
  TEST_ASSERT_TRUE(isnan(w.wind));
  TEST_ASSERT_TRUE(isnan(w.hum));
  TEST_ASSERT_EQUAL_STRING("10n", w.icon);
}

static void test_room_parse_full() {
  RoomState r;
  parseRoomPayload("{\"t\":22.1,\"h\":48}", r);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 22.1f, r.tempC);
  TEST_ASSERT_FLOAT_WITHIN(0.05, 48.0f, r.hum);
}

static void test_room_parse_partial() {
  RoomState r;
  parseRoomPayload("{\"h\":55}", r);
  TEST_ASSERT_TRUE(isnan(r.tempC));
  TEST_ASSERT_FLOAT_WITHIN(0.05, 55.0f, r.hum);
}

static void test_room_parse_no_substring_match() {
  RoomState r;
  parseRoomPayload("{\"temp\":23.0}", r);
  TEST_ASSERT_TRUE(isnan(r.tempC));
  TEST_ASSERT_TRUE(isnan(r.hum));
}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_weather_parse_full);
  RUN_TEST(test_weather_parse_partial);
  RUN_TEST(test_room_parse_full);
  RUN_TEST(test_room_parse_partial);
  RUN_TEST(test_room_parse_no_substring_match);
  return UNITY_END();
}
