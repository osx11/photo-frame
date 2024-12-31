#include "FusionBrainApi.h"

FusionBrainApi::FusionBrainApi(HttpClient *httpClient, String url, String apiKey, String secretKey) {
  _httpClient = httpClient;
  _url = url;
  _apiKey = apiKey;
  _secretKey = secretKey;
}

FixedSizeList<FusionBrainModel> *FusionBrainApi::getModels() {
  auto headers = FixedSizeList<HttpHeader>(2);

  headers.append({.key="X-Key", .value="Key " + String(FB_API_KEY)});
  headers.append({.key="X-Secret", .value="Secret " + String(FB_SECRET)});

  String body = _httpClient->getBody(_url, "/key/api/v1/models", &headers);
  if (!body) {
    Serial.println("Missing response body");
    return nullptr;
  }

  JsonDocument modelsDoc;
  deserializeJson(modelsDoc, body);

  const auto errObj = modelsDoc["error"];

  if (errObj.is<String>()) {
    Serial.println(errObj.as<String>());
    return nullptr;
  }

  const auto jsonArray = modelsDoc.as<JsonArray>();
  FixedSizeList<FusionBrainModel> *models = new FixedSizeList<FusionBrainModel>(jsonArray.size());

  for (const auto data : jsonArray) {
    FusionBrainModel model;
    auto obj = data.as<JsonObject>();

    model.fromJsonObject(obj);
    models->append(model);
  }

  return models;
}
