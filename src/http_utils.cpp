#include "http_utils.hpp"
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* authServerAddress = "https://lifetravel-iot-api.azurewebsites.net/iot/logger/token";
const char* serverAddress= "https://lifetravel-iot-api.azurewebsites.net/api/v1/weight-balances/update-weight/";
const char* serverTest= "http://192.168.18.37:3000/weights/";

const char* authAndGetToken(const char* email, const char* password) {
  Serial.println("Waiting for the authentication response...");
  HTTPClient http;
  http.setTimeout(10000);
  http.begin(authServerAddress);
  http.addHeader("Content-Type", "application/json");

  String requestBody = "{\"email\":\"" + String(email) + "\",\"password\":\"" + String(password) + "\"}";

  int httpResponseCode = http.POST(requestBody);
  String token = "";

  if (httpResponseCode == 200) {
    String response = http.getString();
    /* just for debug
    Serial.println("Server Response:");
    Serial.println(response);
    */
    const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);

    token = doc["idToken"].as<String>();
    http.end();
    Serial.println("Successful authentication");
    return token.c_str();
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

std::pair<float, float> getWeightFromServer(const char* authToken, int resourceId) {
  HTTPClient http;
  http.begin(serverTest + String(resourceId));
  http.addHeader("Content-Type", "application/json");
  String authHeader = "Bearer " + String(authToken);
  http.addHeader("Authorization", authHeader);

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Server Response:");
    Serial.println(response);

    const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, response);

    int id = doc["id"];
    float maxWeight = doc["maxWeight"];
    float actualWeight = doc["actualWeight"];

    http.end();
    Serial.println("Successful request");
    return std::make_pair(actualWeight, maxWeight);
  } else {
    Serial.print("Error on HTTP request. Response code: ");
    Serial.println(httpResponseCode);
    http.end();
    return std::make_pair(-1, -1);
  }
}

void updateTotalWeight(const char* authToken, float totalWeight, int resourceId) {
  HTTPClient http;
  http.begin(serverTest + String(resourceId));
  http.addHeader("Content-Type", "application/json");
  String authHeader = "Bearer " + String(authToken);
  http.addHeader("Authorization", authHeader);

  String requestBody = "{\"maxWeight\": 280.0, \"actualWeight\": " + String(totalWeight) + "}";
  int httpResponseCode = http.PUT(requestBody);

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Server Response:");
    Serial.println(response);
    Serial.println("Successful request");
  } else {
    Serial.print("Error on HTTP request. Response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}