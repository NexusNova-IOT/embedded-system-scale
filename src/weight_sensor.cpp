#include "weight_sensor.h"
#include <HX711.h>

HX711 scale;

void setupWeightSensor() {
  Serial.println("Initializing the scale");
  scale.begin(18, 4);
  scale.set_scale(420.0983);
}

float sense() {
    return scale.get_units();
}