#include "Arduino.h"
#include "wifi_utils.h"
#include "http_utils.h"
#include "weight_sensor.h"
#include "lcd_display.h"
#define DELAY 1000
#define BTN_PIN 33

String authToken;

void sendWeightData(String requestBody)
{
  Serial.println("Button pressed");
  showInDisplay("Waiting server...", 3, 2, false);
  int httpResponseCode = sendPUTRequest(requestBody.c_str(), authToken.c_str());
  checkResponseCode(httpResponseCode);
  if (httpResponseCode == 200)
    showInDisplay("Data sent", 11, 3, false);
  else
    showInDisplay("Error sending data", 2, 3, false);
  delay(DELAY);
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  setupWiFi();
  authToken = "eyJhbGciOiJSUzI1NiIsImtpZCI6ImE2YzYzNTNmMmEzZWMxMjg2NTA1MzBkMTVmNmM0Y2Y0NTcxYTQ1NTciLCJ0eXAiOiJKV1QifQ.eyJpc3MiOiJodHRwczovL3NlY3VyZXRva2VuLmdvb2dsZS5jb20vbGlmZXRyYXZlbC1hcHAiLCJhdWQiOiJsaWZldHJhdmVsLWFwcCIsImF1dGhfdGltZSI6MTcwMDE3NTU1NiwidXNlcl9pZCI6IjJpRmNqeTJVaHhOdkJET1pNclFkMlZEb0lzeTIiLCJzdWIiOiIyaUZjankyVWh4TnZCRE9aTXJRZDJWRG9Jc3kyIiwiaWF0IjoxNzAwMTc1NTU2LCJleHAiOjE3MDAxNzkxNTYsImVtYWlsIjoiaW90QGdtYWlsLmNvbSIsImVtYWlsX3ZlcmlmaWVkIjpmYWxzZSwiZmlyZWJhc2UiOnsiaWRlbnRpdGllcyI6eyJlbWFpbCI6WyJpb3RAZ21haWwuY29tIl19LCJzaWduX2luX3Byb3ZpZGVyIjoicGFzc3dvcmQifX0.X4AWzFcKFRhiX0Q40w03T90TdjWqBAtgGXHglveHPnqAVcLw7DD2C4P2kWRsujSF0us0pPWtEpO9Q7HO8nCcz-rkEimvgKcZtjhTIZJ4fDbKsDVIbiVNF8yLge8O7DBEueoKIbUtoGiH0rJzjlN0C0mJ2w1HDJv3C7_6PvIvdi5gXQUJbZH05cAbMCuH4PU7pBMZDCHFh-E8wz5_m54o6s4TxNEJz-dmVJ7gi8QS-uMzlwKVDAkDFu_9VtJ8ICDFEpAbQaGjW0dBT8DkFlaF7i_fIva7Ff3ecMvQCsv0DohRp5exASf2evn4l2e3xfm_GERFuWEsM8pQmDB6M3Q89Q";
  //= authAndGetToken();
  setupWeightSensor();
  setupDisplay();
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