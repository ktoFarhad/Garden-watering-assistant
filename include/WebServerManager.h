#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include "Config.h"
#include "SettingsManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "ValveController.h"
#include "SafetyManager.h"
#include "IrrigationManager.h"

class WebServerManager {
public:
  WebServerManager(SettingsManager& settings,
                   WiFiManager& wifi,
                   SensorManager& sensors,
                   ValveController& valve,
                   SafetyManager& safety,
                   IrrigationManager& irrigation);

  void begin();
  void update();

private:
  WebServer _server;
  SettingsManager& _settings;
  WiFiManager& _wifi;
  SensorManager& _sensors;
  ValveController& _valve;
  SafetyManager& _safety;
  IrrigationManager& _irrigation;

  void setupRoutes();
  void sendCorsHeaders();
  void sendJsonResponse(int code, const String& body);

  // Route Handlers
  void handleRoot();
  void handleGetStatus();
  void handleGetSensors();
  void handleGetSettings();
  void handlePostSettings();
  void handleValveOpen();
  void handleValveClose();
  void handlePostMode();
  void handleGetWiFi();
  void handlePostWiFi();
  void handlePreflight();
};
