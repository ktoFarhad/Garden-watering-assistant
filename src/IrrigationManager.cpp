#include "IrrigationManager.h"

IrrigationManager::IrrigationManager() {
}

void IrrigationManager::begin(SettingsManager& settings) {
  _mode = settings.getSettings().mode;
  _lastIrrigationEvent = String("Режим: ") + (_mode == MODE_AUTO ? "Автоматический" : "Ручной");
}

void IrrigationManager::setMode(OperatingMode mode, SettingsManager& settings, ValveController& valve) {
  _mode = mode;
  settings.setMode(mode);

  if (_mode == MODE_AUTO) {
    _lastIrrigationEvent = "Включен автоматический режим";
  } else {
    _lastIrrigationEvent = "Включен ручной режим";
  }
  Serial.print("[IrrigationManager] ");
  Serial.println(_lastIrrigationEvent);
}

bool IrrigationManager::manualOpen(ValveController& valve) {
  bool success = valve.openValve("Команда пользователя через Web API");
  if (success) {
    _lastIrrigationEvent = "Ручной запуск: подан импульс ON";
  }
  return success;
}

bool IrrigationManager::manualClose(ValveController& valve) {
  bool success = valve.closeValve("Команда пользователя через Web API");
  if (success) {
    _lastIrrigationEvent = "Ручная остановка: подан импульс OFF";
  }
  return success;
}

bool IrrigationManager::shouldStartWatering(const SettingsManager& settings, const SensorManager& sensors) {
  const auto& cfg = settings.getSettings();
  const auto& readings = sensors.getReadings();

  if (!readings.anySoilValid) return false;

  if (cfg.individualSensors) {
    for (int i = 0; i < 3; i++) {
      if (readings.soil[i].valid && readings.soil[i].percent <= cfg.individualStart[i]) {
        return true;
      }
    }
    return false;
  } else {
    return (readings.soilAverage <= cfg.startPercent);
  }
}

bool IrrigationManager::shouldStopWatering(const SettingsManager& settings, const SensorManager& sensors) {
  const auto& cfg = settings.getSettings();
  const auto& readings = sensors.getReadings();

  if (!readings.anySoilValid) return true; // Stop if sensors become invalid

  if (cfg.individualSensors) {
    bool allSatisfied = true;
    for (int i = 0; i < 3; i++) {
      if (readings.soil[i].valid && readings.soil[i].percent < cfg.individualStop[i]) {
        allSatisfied = false;
        break;
      }
    }
    return allSatisfied;
  } else {
    return (readings.soilAverage >= cfg.stopPercent);
  }
}

void IrrigationManager::update(const SettingsManager& settings, const SensorManager& sensors, ValveController& valve, const SafetyManager& safety) {
  const uint32_t now = millis();

  // Non-blocking periodic evaluation
  if (now - _lastDecisionTime < EVALUATION_INTERVAL_MS) {
    return;
  }
  _lastDecisionTime = now;

  // In Manual Mode, do not execute automated watering decisions
  if (_mode == MODE_MANUAL) {
    return;
  }

  // In Auto Mode:
  // If safety fault is active, do not trigger watering
  if (safety.isSafetyFault()) {
    return;
  }

  bool watering = valve.isOpen();

  if (!watering) {
    if (shouldStartWatering(settings, sensors)) {
      Serial.println("[IrrigationManager] Auto condition met: starting irrigation.");
      bool ok = valve.openValve("Автополив: влажность почвы ниже порога");
      if (ok) {
        _lastIrrigationEvent = "Автополив: запущен по порогу влажности";
      }
    }
  } else {
    if (shouldStopWatering(settings, sensors)) {
      Serial.println("[IrrigationManager] Auto condition met: stopping irrigation.");
      bool ok = valve.closeValve("Автополив: целевая влажность достигнута");
      if (ok) {
        _lastIrrigationEvent = "Автополив: остановлен по целевой влажности";
      }
    }
  }
}

String IrrigationManager::toJson() const {
  String json = "{";
  json += "\"mode\":\"" + String(_mode == MODE_AUTO ? "auto" : "manual") + "\",";
  json += "\"lastEvent\":\"" + _lastIrrigationEvent + "\"";
  json += "}";
  return json;
}
