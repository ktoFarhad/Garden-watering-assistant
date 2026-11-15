#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "Config.h"
#include "SettingsManager.h"

class SensorManager {
public:
  SensorManager();
  bool begin(const SettingsManager& settings);
  void update(const SettingsManager& settings);

  const SensorReadings& getReadings() const { return _readings; }
  bool isSoilValid() const { return _readings.anySoilValid; }
  int getAverageMoisture() const { return _readings.soilAverage; }
  float getTemperature() const { return _readings.climate.temperature; }
  float getHumidity() const { return _readings.climate.humidity; }

  String toJson() const;

private:
  SensorReadings _readings;
  uint32_t _lastPollTime = 0;
  bool _ahtInitialized = false;

  void readSoilSensors(const SettingsManager& settings);
  bool readAHT10();
  int rawToPercent(int raw, int dryAdc, int wetAdc);
};
