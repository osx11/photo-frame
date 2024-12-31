#include "FusionBrainApi.h"

FusionBrainApi::FusionBrainApi(HttpClient *httpClient, String url, String apiKey, String secretKey) {
  _httpClient = httpClient;
  _url = url;
  _apiKey = apiKey;
  _secretKey = secretKey;
}

void FusionBrainApi::getModels() {
  auto headers = FixedSizeList<HttpHeader>(2);

  headers.append({.key="X-Key", .value="Key " + String(FB_API_KEY)});
  headers.append({.key="X-Secret", .value="Secret " + String(FB_SECRET)});

  String body = _httpClient->getBody(_url, "/key/api/v1/models", &headers);
  Serial.println("BODY RECEIVED " + body);
}
