#include "WiFiManager.h"

WiFiManager::WiFiManager(char *ssid, char *password) {
  _ssid = ssid;
  _password = password;
}

void WiFiManager::setOnAttempCallback(void (*callback)()) {
  _onAttempCallback = callback;
}

wl_status_t WiFiManager::tryAutoConnectAsStation(unsigned long timeout) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);

  const auto connectingStartTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - connectingStartTime < timeout) {
    if (WiFi.status() != WL_IDLE_STATUS && WiFi.status() != WL_DISCONNECTED) break;
    if (_onAttempCallback != nullptr) _onAttempCallback();
  }

  if (WiFi.status() == WL_CONNECTED) {
    _connected = true;
  }

  return WiFi.status();
}

boolean WiFiManager::isConnected() {
  return _connected;
}

void WiFiManager::checkRealWifiStatus() {
  static auto lastCheck = millis();

  if (millis() - lastCheck > 1000) {
    lastCheck = millis();
    _connected = WiFi.status() == WL_CONNECTED;
  }
}

char *WiFiManager::convertWifiStatus(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "NO_SHIELD";
    case WL_IDLE_STATUS: return "IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
    case WL_CONNECTED: return "CONNECTED";
    case WL_CONNECT_FAILED: return "CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "CONNECTION_LOST";
    case WL_WRONG_PASSWORD: return "WRONG_PASSWORD";
    case WL_DISCONNECTED: return "DISCONNECTED";
  }

  return "UNKNOWN";
}
