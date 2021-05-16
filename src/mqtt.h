#ifndef MQTT_H
#define MQTT_H
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include "string.h"

extern int speedx, speedy;
void mqtt_init();
#endif