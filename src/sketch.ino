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

  if (!isnan(weight)) {
    String requestBody = String("{\"weight\":") + String(weight) + String("}");
    Serial.println(requestBody);
    showInDisplay((String(weight) + String(" kg")).c_str());

    int httpResponseCode = sendPUTRequest(requestBody.c_str());
    checkResponseCode(httpResponseCode);

  } else {
    Serial.println("Error reading sensor weight");
  }

  delay(DELAY);
}