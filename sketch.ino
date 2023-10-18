#include "HX711.h"
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define DELAY 1000
HX711 scale;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  Serial.println("Initializing the scale");
  scale.begin(18, 4);
  scale.set_scale(420.0983);
}

void loop() {
  float weight = scale.get_units();
  Serial.print(weight, 1);
  Serial.println(" kg");
  delay(DELAY);
}