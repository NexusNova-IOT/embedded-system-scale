#include "Arduino.h"
#include "wifi_utils.hpp"
#include "http_utils.hpp"
#include "weight_sensor.hpp"
#include "lcd_display.hpp"
#define DELAY 1500
#define BTN_RESET_PIN 33
#define BTN_SEND_PIN 15
#define DEVICE_ID 3

String authToken;

void sendWeightData(String requestBody) {
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

void showBanner() {
  showInDisplay("Press RED to reset  ", 0, 0, true);
  showInDisplay("Press BLUE to weigh", 1, 0, false);
  showInDisplay("& send to server  ", 2, 0, false);
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_SEND_PIN, INPUT_PULLUP);
  pinMode(BTN_RESET_PIN, INPUT_PULLUP);
  setupWiFi();
  authToken = "uwu";//= authAndGetToken();
  setupWeightSensor();
  setupDisplay();
  showBanner();
  delay(DELAY);
}

void loop() {
  float weight = sense();
  if (isnan(weight)) {
    showInDisplay("Error reading weight", 1);
    return;
  }
  if (digitalRead(BTN_SEND_PIN) == LOW) {
    std::pair<float, float> weightAndMax = getWeightFromServer(authToken.c_str(), DEVICE_ID);
    if (weightAndMax.first == -1) {
      showInDisplay("Error getting data", 1);
      delay(DELAY);
      return;
    }
    if (weightAndMax.first + weight > weightAndMax.second) {
      showInDisplay("Weight limit reached");
      showInDisplay("Data not sent", 1, 0, false);
      displayWeightInfo("Actual", weight, 2, 0, false);
      displayWeightInfo("Exceeded", weightAndMax.first + weight - weightAndMax.second, 3, 0, false);
      delay(DELAY);
      return;
    }
    weightAndMax.first += weight;
    showInDisplay(((String("Weight sumary"))).c_str());
    displayWeightInfo("Actual", weight, 1, 0);
    displayWeightInfo("Total", weightAndMax.first, 2, 0);
    displayWeightInfo("Maximum", weightAndMax.second, 3, 0);
    updateTotalWeight(authToken.c_str(), weightAndMax.first, DEVICE_ID);
  }
  if (digitalRead(BTN_RESET_PIN) == LOW) {
    updateTotalWeight(authToken.c_str(), 0.0, DEVICE_ID);
    showInDisplay("Total weight reseted", 1);
  }
}