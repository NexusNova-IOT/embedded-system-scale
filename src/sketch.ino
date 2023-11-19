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
  showInDisplay("Long press");
  showInDisplay("RED to reset", 1, 0, false);
  showInDisplay("Press BLUE to weigh", 2, 0, false);
  showInDisplay("& send to server  ", 3, 0, false);
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_SEND_PIN, INPUT_PULLUP);
  pinMode(BTN_RESET_PIN, INPUT_PULLUP);
  setupWiFi();
  authToken = authAndGetToken();
  setupWeightSensor();
  setupDisplay();
  showBanner();
  delay(DELAY);
}

void displayWeightSummary(float actualWeight, float totalWeight, float maxWeight) {
  showInDisplay("Weight summary", 0, 0, true);
  displayWeightInfo("Actual", actualWeight, 1, 0, false);
  displayWeightInfo("Total", totalWeight, 2, 0, false);
  displayWeightInfo("Maximum", maxWeight, 3, 0, false);
}

void handleSendButton(float& actualWeight, const char* authToken, int deviceId) {
  if (digitalRead(BTN_SEND_PIN) == LOW) {
    std::pair<float, float> weightAndMax = getWeightFromServer(authToken, deviceId);
    if (weightAndMax.first == -1) {
      showInDisplay("Error getting data", 1);
      delay(DELAY);
      return;
    }
    float totalWeight = weightAndMax.first + actualWeight;
    if (totalWeight > weightAndMax.second) {
      showInDisplay("Weight limit reached");
      showInDisplay("Data not sent", 1, 0, false);
      displayWeightInfo("Actual", actualWeight, 2, 0, false);
      displayWeightInfo("Exceeded", totalWeight - weightAndMax.second, 3, 0, false);
      delay(DELAY);
      return;
    }
    displayWeightSummary(actualWeight, totalWeight, weightAndMax.second);
    updateTotalWeight(authToken, totalWeight, deviceId);
  }
}

void handleResetButton(const char* authToken, int deviceId, float& actualWeight) {
  if (digitalRead(BTN_RESET_PIN) == LOW) {
    showInDisplay("Press RED again", 1);
    showInDisplay("to reset", 2, 0, false);
    delay(2000);
    if (digitalRead(BTN_RESET_PIN) == LOW) {
      actualWeight = 0.0;
      updateTotalWeight(authToken, 0.0, deviceId);
      showInDisplay("Total weight reset", 1);
      delay(DELAY);
    } else {
      showInDisplay("Reset canceled", 1);
      delay(DELAY);
    }
  }
}

void loop() {
  float weight = sense();
  if (isnan(weight)) {
    showInDisplay("Error reading weight", 1);
    return;
  }

  handleSendButton(weight, authToken.c_str(), DEVICE_ID);
  handleResetButton(authToken.c_str(), DEVICE_ID, weight);
}
