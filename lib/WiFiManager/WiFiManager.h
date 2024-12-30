#pragma once

#include <ESP8266WiFi.h>
#include <Arduino.h>

#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

class WiFiManager {
public:
    WiFiManager(char *ssid, char *password);

    void setOnAttempCallback(void (*callback)());

    wl_status_t tryAutoConnectAsStation(unsigned long timeout);

    boolean isConnected();

    static char *convertWifiStatus(wl_status_t status);

    void checkRealWifiStatus();

private:
    char *_ssid;
    char *_password;
    boolean _connected = false;
    void (*_onAttempCallback)() = nullptr;
};

#endif //WIFIMANAGER_H
