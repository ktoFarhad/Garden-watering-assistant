#pragma once

#include <Arduino.h>
#include "Config.h"
#include "SettingsManager.h"
#include "SensorManager.h"
#include "ValveController.h"

class SafetyManager {
public:
  SafetyManager();
  void begin();
  void update(const SettingsManager& settings, const SensorManager& sensors, ValveController& valve);

  bool isSafetyFault() const { return _status.maxDurationReached || _status.sensorFault; }
  const SafetyStatus& getStatus() const { return _status; }
  String getLastSafetyEvent() const { return _lastSafetyEvent; }
  void clearFaults();

  String toJson() const;

private:
  SafetyStatus _status;
  String _lastSafetyEvent = "Безопасность в норме";
};
