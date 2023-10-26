#include "HX711.h"
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#define DELAY 1000
#define I2C_ADDR    0x27
#define LCD_COLUMNS 20
#define LCD_LINES   4

LiquidCrystal_I2C LCD(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

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
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
}

void loop() {
  float weight = scale.get_units();
  Serial.print(weight, 1);
  Serial.println(" kg");
  LCD.clear();
  LCD.print(weight, 1);
  LCD.print(" kg");
  delay(DELAY);
}