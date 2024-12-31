#pragma once

#include "../HttpClient/HttpClient.h"
#include "../FixedSizeList/FixedSizeList.h"
#include <ArduinoJson.h>

#ifndef FUSIONBRAINAPI_H
#define FUSIONBRAINAPI_H

typedef struct {
  int id;
  String name;
  String type;

  void fromJsonObject(JsonObject& json) {
    id = json["id"].as<int>();
    name = json["name"].as<String>();
    type = json["type"].as<String>();
  }
} FusionBrainModel;

class FusionBrainApi {
  public:
    FusionBrainApi(HttpClient *httpClient, String url, String apiKey, String secretKey);
    FixedSizeList<FusionBrainModel> *getModels();

  private:
    String _url;
    String _apiKey;
    String _secretKey;
    HttpClient *_httpClient;

    void getModelsHttpCallback();
};



#endif //FUSIONBRAINAPI_H
