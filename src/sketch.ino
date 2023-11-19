#include "Arduino.h"
#include "wifi_utils.hpp"
#include "http_utils.hpp"
#include "weight_sensor.hpp"
#include "lcd_display.hpp"
#define DELAY 1500
#define BTN_PIN 33
#define DEVICE_ID 1

String authToken;

void sendWeightData(String requestBody)
{
  Serial.println("Button pressed");
  showInDisplay("Waiting server...", 2, 3, false);
  int httpResponseCode = sendPUTRequest(requestBody.c_str(), authToken.c_str(), DEVICE_ID);
  checkResponseCode(httpResponseCode);
  if (httpResponseCode == 200)
    showInDisplay("Data sent", 3, 11, false);
  else
    showInDisplay("Error sending data", 3, 2, false);
  delay(DELAY);
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  setupWiFi();
  authToken = authAndGetToken();
  setupWeightSensor();
  setupDisplay();
  delay(DELAY);
}

void loop() {
  float weight = sense();

  if (!isnan(weight)) {
    String requestBody = String("{\"weight\":") + String(weight) + String("}");
    Serial.println(requestBody);

    showInDisplay((String(weight) + String(" kg")).c_str());
    if (digitalRead(BTN_PIN) == LOW)
    {
      sendWeightData(requestBody);
    }
    delay(DELAY);
  }
}