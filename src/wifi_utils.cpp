#include "wifi_utils.h"
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setupWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}