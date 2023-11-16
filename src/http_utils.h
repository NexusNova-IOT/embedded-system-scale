#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

const char* authAndGetToken(const char* email = "iot@gmail.com", const char* password = "12345678");
int sendPUTRequest(const char* requestBody, const char* authToken);
void checkResponseCode(int httpResponseCode);
#endif
