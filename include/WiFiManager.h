#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "Config.h"

class WiFiManager {
public:
  WiFiManager();
  void begin(const String& ssid, const String& password);
  void update();

  bool isConnected() const;
  bool isApActive() const { return _apActive; }
  String getIpAddress() const;
  int8_t getRssi() const;
  String getSsid() const { return _currentSsid; }
  WiFiStateEnum getState() const { return _state; }

  void startAp();
  void connectSta(const String& ssid, const String& password);
  String toJson() const;

private:
  String _currentSsid;
  String _currentPass;
  bool _apActive = false;
  WiFiStateEnum _state = WIFI_STATE_DISCONNECTED;
  uint32_t _lastConnectionAttempt = 0;
  uint32_t _lastCheckTime = 0;
  uint8_t _connectAttempts = 0;
  const uint32_t STA_TIMEOUT_MS = 12000;
};
