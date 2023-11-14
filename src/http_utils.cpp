#include "http_utils.h"
#include <HTTPClient.h>

const char* serverAddress= "https://lifetravel-iot-api.azurewebsites.net/api/v1/weight-balances/update-weight/1";
const char* authToken = "eyJhbGciOiJSUzI1NiIsImtpZCI6IjQ0ZmJkZTdhZGY0ZTU3YWYxZWE4MzAzNmJmZjdkMzUxNTk3ZTMzNWEiLCJ0eXAiOiJKV1QifQ.eyJpc3MiOiJodHRwczovL3NlY3VyZXRva2VuLmdvb2dsZS5jb20vbGlmZXRyYXZlbC1hcHAiLCJhdWQiOiJsaWZldHJhdmVsLWFwcCIsImF1dGhfdGltZSI6MTY5OTkzMjEzOSwidXNlcl9pZCI6IjJpRmNqeTJVaHhOdkJET1pNclFkMlZEb0lzeTIiLCJzdWIiOiIyaUZjankyVWh4TnZCRE9aTXJRZDJWRG9Jc3kyIiwiaWF0IjoxNjk5OTMyMTM5LCJleHAiOjE2OTk5MzU3MzksImVtYWlsIjoiaW90QGdtYWlsLmNvbSIsImVtYWlsX3ZlcmlmaWVkIjpmYWxzZSwiZmlyZWJhc2UiOnsiaWRlbnRpdGllcyI6eyJlbWFpbCI6WyJpb3RAZ21haWwuY29tIl19LCJzaWduX2luX3Byb3ZpZGVyIjoicGFzc3dvcmQifX0.C8TghFS_uyz4a8liqC8pHDUvwvjzAs153dOdMiPc7zcht-U9S7cjFFlr5FCkcfIMlr0th6TPyiBmFn_sGY8bpQUvLw93QJSGAs33kOxo9xmb2UyvDDe-5N-orJDoxxfbKlDvjBRBa5tbx9tcxn3tusxmpJI_jZ3sbkb0t_eHqAw1WYw-ZjPCO6c8Az63_xKaYts6OHDhZSzDBj-1Pv5MfungwjWbZ-iM3N36Ww5NGbHhGoAf4-RYW_20mlDxZi3w0TMX6cqdDclG9e3EBITvJlKt7AfMIj9gwOW5Qa_EqRdJ0QbypIF_sgCjjcx_S-2YWnXRoGa8UfkNV1Jc9fimyQ";
// please update the token here
// https://iot-lifetravel-firebase-token-gen.netlify.app/login
// email: iot@gmail.com
// password: 12345678
int sendPUTRequest(const char* requestBody) {
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", authToken);
  
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
