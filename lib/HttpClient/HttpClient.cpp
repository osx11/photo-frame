#include "HttpClient.h"


HttpClient::HttpClient(WiFiClientSecure *wifiClient) {
  _wifiClient = wifiClient;
}

void HttpClient::sendRequest(String baseUrl, String endpoint, FixedSizeList<HttpHeader> *httpHeaders, void(*onData)(WiFiClientSecure *wifiClient, void*), void* userData) {
  const auto connected = _wifiClient->connect(baseUrl, 443);

  if (!connected) {
    Serial.println("Connection failed");
    return;
  }

  String request = "GET " + endpoint + " HTTP/1.1\r\n";

  request += "Host: " + String(baseUrl) + "\r\n";
  request += "Connection: close\r\n";

  for (int i = 0; i < httpHeaders->size(); i++) {
    request += httpHeaders->get(i).key + ": " + httpHeaders->get(i).value + "\r\n";
  }

  request += "\r\n";

  _wifiClient->print(request);

  while (_wifiClient->connected() || _wifiClient->available()) {
    if (_wifiClient->available()) {
      onData(_wifiClient, userData);
    }
  }

  _wifiClient->stop();
}

String HttpClient::getBody(String baseUrl, String endpoint, FixedSizeList<HttpHeader> *httpHeaders) {
  String body = "";

  sendRequest(baseUrl, endpoint, httpHeaders, [] (WiFiClientSecure *wifiClient, void *userData){
    String line = wifiClient->readStringUntil('\n');
    if (line.startsWith("[") || line.startsWith("{"))  *(String *) userData = line;
  }, &body);

  return body;
}

const char* bodyBuff;
