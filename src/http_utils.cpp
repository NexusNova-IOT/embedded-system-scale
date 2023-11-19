#include "http_utils.hpp"
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* authServerAddress = "https://lifetravel-iot-api.azurewebsites.net/iot/logger/token";
const char* serverAddress= "https://lifetravel-iot-api.azurewebsites.net/api/v1/weight-balances/update-weight/";

const char* authAndGetToken(const char* email, const char* password) {
  Serial.println("Inicio de la función authAndGetToken");
  HTTPClient http;
  http.setTimeout(10000);
  http.begin(authServerAddress);
  http.addHeader("Content-Type", "application/json");

  String requestBody = "{\"email\":\"" + String(email) + "\",\"password\":\"" + String(password) + "\"}";

  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode == 200) {
    String response = http.getString();

    // Just for debugging purposes
    //Serial.println("Server Response:");
    //Serial.println(response);

    const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);
    const char *jwtToken = doc["idToken"];
    http.end();
    Serial.println("Successful authentication");
    return jwtToken;
  } else {
    Serial.print("Authentication Error. HTTP response code:");
    Serial.println(httpResponseCode);
    http.end();
    Serial.println("An error occurred during authentication");
    return nullptr;
  }
}

int sendPUTRequest(const char* requestBody, const char* authToken, int resourceId) {
  HTTPClient http;
  http.begin(serverAddress + String(resourceId));
  http.addHeader("Content-Type", "application/json");
  String authHeader = "Bearer " + String(authToken);
  http.addHeader("Authorization", authHeader);
  
  int httpResponseCode = http.PUT(requestBody);

  http.end();
  return httpResponseCode;
}

void checkResponseCode(int httpResponseCode) {
  if (httpResponseCode == 200) {
    Serial.println("Successfully updated on the server.");
  } else {
    Serial.print("Request error. HTTP response code:");
    Serial.println(httpResponseCode);
    }
}
