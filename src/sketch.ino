// Main sketch for the weight measurement IoT device
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

// Function to send weight data to the server
void sendWeightData(String requestBody) {
  Serial.println("Button pressed");

  // Display waiting message on LCD
  showInDisplay("Waiting server...", 2, 3, false);

  // Send data to the server and check the response
  int httpResponseCode = sendPUTRequest(requestBody.c_str(), authToken.c_str(), DEVICE_ID);
  checkResponseCode(httpResponseCode);

  // Display appropriate message on LCD based on the server response
  if (httpResponseCode == 200)
    showInDisplay("Data sent", 3, 11, false);
  else
    showInDisplay("Error sending data", 3, 2, false);

  delay(DELAY);
}

// Function to display initial banner on LCD
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

  // Initialize WiFi connection and obtain authentication token
  setupWiFi();
  authToken = authAndGetToken();

  // Initialize weight sensor, LCD display, and show initial banner
  setupWeightSensor();
  setupDisplay();
  showBanner();

  delay(DELAY);
}

// Function to display weight summary on LCD
void displayWeightSummary(float actualWeight, float totalWeight, float maxWeight) {
  showInDisplay("Weight summary", 0, 0, true);
  displayWeightInfo("Actual", actualWeight, 1, 0, false);
  displayWeightInfo("Total", totalWeight, 2, 0, false);
  displayWeightInfo("Maximum", maxWeight, 3, 0, false);
}

// Function to handle sending button press
void handleSendButton(float& actualWeight, const char* authToken, int deviceId) {
  if (digitalRead(BTN_SEND_PIN) == LOW) {

    // Retrieve weight information from the server
    std::pair<float, float> weightAndMax = getWeightFromServer(authToken, deviceId);

    // Handle error in retrieving data
    if (weightAndMax.first == -1) {
      showInDisplay("Error getting data", 1);
      delay(DELAY);
      return;
    }

    // Calculate total weight and check for limit
    float totalWeight = weightAndMax.first + actualWeight;
    if (totalWeight > weightAndMax.second) {
      showInDisplay("Weight limit reached");
      showInDisplay("Data not sent", 1, 0, false);
      displayWeightInfo("Actual", actualWeight, 2, 0, false);
      displayWeightInfo("Exceeded", totalWeight - weightAndMax.second, 3, 0, false);
      delay(DELAY);
      return;
    }

    // Display weight summary and update total weight on the server
    displayWeightSummary(actualWeight, totalWeight, weightAndMax.second);
    updateTotalWeight(authToken, totalWeight, deviceId);
  }
}

// Function to handle reset button press
void handleResetButton(const char* authToken, int deviceId, float& actualWeight) {
  if (digitalRead(BTN_RESET_PIN) == LOW) {
    showInDisplay("Press RED again", 1);
    showInDisplay("to reset", 2, 0, false);
    delay(2000);

    // Check if the reset button is pressed again within 2 seconds
    if (digitalRead(BTN_RESET_PIN) == LOW) {
      // Reset actual weight to zero and update total weight on the server
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
  // Measure weight using the sensor
  float weight = sense();

  // Display error if reading is unsuccessful
  if (isnan(weight)) {
    showInDisplay("Error reading weight", 1);
    return;
  }

  // Handle button presses
  handleSendButton(weight, authToken.c_str(), DEVICE_ID);
  handleResetButton(authToken.c_str(), DEVICE_ID, weight);
}
