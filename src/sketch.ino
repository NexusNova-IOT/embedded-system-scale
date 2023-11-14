#include "Arduino.h"
#include "wifi_utils.h"
#include "http_utils.h"
#include "weight_sensor.h"
#include "lcd_display.h"
#define DELAY 1000

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupWeightSensor();
  setupDisplay();
}

void loop() {
  float weight = sense();
  String requestBody = String("{\"weight\":") + String(weight) + String("}");
  Serial.println(requestBody);
  showInDisplay((String(weight) + String(" kg")).c_str());
  delay(DELAY);
}