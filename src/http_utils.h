#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

int sendPUTRequest(const char* requestBody);
void checkResponseCode(int httpResponseCode);
#endif
