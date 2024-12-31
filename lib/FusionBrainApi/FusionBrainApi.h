#pragma once
#include "../HttpClient/HttpClient.h"

#ifndef FUSIONBRAINAPI_H
#define FUSIONBRAINAPI_H

typedef struct {
  int id;
  String name;
  String type;
} FusionBrainModel;

class FusionBrainApi {
  public:
    FusionBrainApi(HttpClient *httpClient, String url, String apiKey, String secretKey);
    void getModels();

  private:
    String _url;
    String _apiKey;
    String _secretKey;
    HttpClient *_httpClient;

    void getModelsHttpCallback();
};



#endif //FUSIONBRAINAPI_H
