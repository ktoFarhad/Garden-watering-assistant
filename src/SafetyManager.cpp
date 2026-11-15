#include "SafetyManager.h"

SafetyManager::SafetyManager() {
}

void SafetyManager::begin() {
  _status.maxDurationReached = false;
  _status.sensorFault = false;
  _status.badReadingCount = 0;
  _status.emergencyStopped = false;
  _lastSafetyEvent = "Безопасность активна";
}

void SafetyManager::update(const SettingsManager& settings, const SensorManager& sensors, ValveController& valve) {
  const auto& cfg = settings.getSettings();

  // 1. Check Maximum Continuous Watering Duration
  if (valve.isOpen()) {
    uint32_t duration = valve.getWateringDurationSec();
    if (duration >= cfg.maxWateringSec) {
      Serial.println("[SafetyManager] ALERT: Maximum watering time reached. Forcing valve OFF.");
      valve.closeValve("Защита: лимит времени полива исчерпан");
      _status.maxDurationReached = true;
      _lastSafetyEvent = "Защита: отключение по таймауту полива";
    }
  } else {
    _status.maxDurationReached = false;
  }

  // 2. Check Consecutive Soil Sensor Faults
  if (!sensors.isSoilValid()) {
    _status.badReadingCount++;
    if (_status.badReadingCount >= MAX_BAD_READINGS_COUNT) {
      _status.sensorFault = true;
      if (valve.isOpen()) {
        Serial.println("[SafetyManager] ALERT: Consecutive sensor faults detected. Forcing valve OFF.");
        valve.closeValve("Защита: отказ датчиков почвы");
        _lastSafetyEvent = "Защита: отключение из-за сбоя датчиков почвы";
      }
    }
  } else {
    _status.badReadingCount = 0;
    _status.sensorFault = false;
  }
}

void SafetyManager::clearFaults() {
  _status.maxDurationReached = false;
  _status.sensorFault = false;
  _status.badReadingCount = 0;
  _status.emergencyStopped = false;
  _lastSafetyEvent = "Ошибки безопасности сброшены";
}

String SafetyManager::toJson() const {
  String json = "{";
  json += "\"maxDurationReached\":" + String(_status.maxDurationReached ? "true" : "false") + ",";
  json += "\"sensorFault\":" + String(_status.sensorFault ? "true" : "false") + ",";
  json += "\"badReadingCount\":" + String(_status.badReadingCount) + ",";
  json += "\"emergencyStopped\":" + String(_status.emergencyStopped ? "true" : "false") + ",";
  json += "\"lastEvent\":\"" + _lastSafetyEvent + "\"";
  json += "}";
  return json;
}
