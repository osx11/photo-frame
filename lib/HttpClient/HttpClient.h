#pragma once

#include <WiFiClientSecure.h>
#include "../FixedSizeList/FixedSizeList.h"

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

typedef struct {
    String key;
    String value;
} HttpHeader;

class HttpClient {
public:
    HttpClient(WiFiClientSecure *wifiClient);

    void sendRequest(String baseUrl, String endpoint, FixedSizeList<HttpHeader> *httpHeaders,
                     void (*onData)(WiFiClientSecure *wifiClient, void*), void* userData);

    String getBody(String baseUrl, String endpoint, FixedSizeList<HttpHeader> *httpHeaders);

private:
    WiFiClientSecure *_wifiClient;
};

#endif //HTTPCLIENT_H
