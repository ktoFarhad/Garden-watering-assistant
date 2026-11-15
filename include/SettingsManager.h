#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"

class SettingsManager {
public:
  SettingsManager();
  bool begin();
  void load();
  bool save();
  void resetToDefaults();

  const SystemSettings& getSettings() const { return _settings; }
  SystemSettings& getSettingsMutable() { return _settings; }

  void setMode(OperatingMode mode);
  void setWifiCredentials(const String& ssid, const String& password);
  void setThresholds(int startPercent, int stopPercent);
  void setPins(int8_t valveOn, int8_t valveOff, int8_t soil1, int8_t soil2, int8_t soil3);
  void setTimings(uint32_t maxWateringSec, uint16_t pulseDurationMs);

  String toJson() const;
  bool updateFromJson(const String& jsonString);

private:
  Preferences _prefs;
  SystemSettings _settings;
  bool _initialized = false;
};
