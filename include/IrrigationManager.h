#pragma once

#include <Arduino.h>
#include "Config.h"
#include "SettingsManager.h"
#include "SensorManager.h"
#include "ValveController.h"
#include "SafetyManager.h"

class IrrigationManager {
public:
  IrrigationManager();
  void begin(SettingsManager& settings);
  void update(const SettingsManager& settings, const SensorManager& sensors, ValveController& valve, const SafetyManager& safety);

  void setMode(OperatingMode mode, SettingsManager& settings, ValveController& valve);
  OperatingMode getMode() const { return _mode; }

  // Manual actions
  bool manualOpen(ValveController& valve);
  bool manualClose(ValveController& valve);

  String getLastEvent() const { return _lastIrrigationEvent; }
  String toJson() const;

private:
  OperatingMode _mode = MODE_AUTO;
  String _lastIrrigationEvent = "Автоматизация готова";
  uint32_t _lastDecisionTime = 0;
  const uint32_t EVALUATION_INTERVAL_MS = 2500;

  bool shouldStartWatering(const SettingsManager& settings, const SensorManager& sensors);
  bool shouldStopWatering(const SettingsManager& settings, const SensorManager& sensors);
};
