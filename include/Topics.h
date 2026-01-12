#pragma once

// Weather published to MQTT (retain is ideal)
#define TOPIC_WEATHER "OpenWeather/Current"

// Room readings (retain is ideal)
#define TOPIC_OFFICE_READING  "Office/DHT22/Reading"
#define TOPIC_BEDROOM_READING "Bedroom/DHT22/Reading"
#define TOPIC_GARAGE_READING  "Garage/DHT22/Reading"

// Room status ("online" / "offline")
#define TOPIC_OFFICE_STATUS  "Office/DHT22/Status"
#define TOPIC_BEDROOM_STATUS "Bedroom/DHT22/Status"
#define TOPIC_GARAGE_STATUS  "Garage/DHT22/Status"
