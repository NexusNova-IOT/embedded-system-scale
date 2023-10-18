#include "HX711.h"
#define DELAY 1000
HX711 scale;

void setup() {
  Serial.begin(115200);
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