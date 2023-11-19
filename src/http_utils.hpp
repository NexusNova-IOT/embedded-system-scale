#ifndef HTTP_UTILS_HPP
#define HTTP_UTILS_HPP

const char* authAndGetToken(const char* email = "iot@gmail.com", const char* password = "12345678");
int sendPUTRequest(const char* requestBody, const char* authToken, int resourceId = 1);
void checkResponseCode(int httpResponseCode);
#endif
