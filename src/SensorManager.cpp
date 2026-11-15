#include "SensorManager.h"

SensorManager::SensorManager() {
  for (int i = 0; i < 3; i++) {
    _readings.soil[i].id = i + 1;
    _readings.soil[i].valid = false;
  }
}

bool SensorManager::begin(const SettingsManager& settings) {
  analogReadResolution(12);

  const auto& cfg = settings.getSettings();
  Wire.begin(cfg.pinAhtSda, cfg.pinAhtScl);

  // Initial read
  readSoilSensors(settings);
  _readings.climate.valid = readAHT10();

  _lastPollTime = millis();
  return true;
}

void SensorManager::update(const SettingsManager& settings) {
  const uint32_t now = millis();
  if (now - _lastPollTime >= DEFAULT_SENSOR_INTERVAL_MS || _lastPollTime == 0) {
    _lastPollTime = now;
    readSoilSensors(settings);
    _readings.climate.valid = readAHT10();
  }
}

int SensorManager::rawToPercent(int raw, int dryAdc, int wetAdc) {
  if (dryAdc == wetAdc) return 0;
  return constrain(map(raw, dryAdc, wetAdc, 0, 100), 0, 100);
}

void SensorManager::readSoilSensors(const SettingsManager& settings) {
  const auto& cfg = settings.getSettings();

  int totalPercent = 0;
  int validCount = 0;

  for (int i = 0; i < 3; i++) {
    int8_t pin = cfg.pinSoil[i];
    _readings.soil[i].pin = pin;

    if (pin < 0) {
      _readings.soil[i].raw = 0;
      _readings.soil[i].percent = 0;
      _readings.soil[i].valid = false;
      continue;
    }

    int raw = analogRead(pin);
    _readings.soil[i].raw = raw;

    // Check validity
    if (raw < ADC_MIN_VALID_VALUE || raw > ADC_MAX_VALID_VALUE) {
      _readings.soil[i].valid = false;
      _readings.soil[i].percent = 0;
    } else {
      _readings.soil[i].valid = true;
      _readings.soil[i].percent = rawToPercent(raw, cfg.soilDryAdc, cfg.soilWetAdc);
      totalPercent += _readings.soil[i].percent;
      validCount++;
    }
  }

  if (validCount > 0) {
    _readings.soilAverage = totalPercent / validCount;
    _readings.anySoilValid = true;
  } else {
    _readings.soilAverage = 0;
    _readings.anySoilValid = false;
  }
}

bool SensorManager::readAHT10() {
  Wire.beginTransmission(AHT10_I2C_ADDR);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) {
    _readings.climate.temperature = NAN;
    _readings.climate.humidity = NAN;
    return false;
  }

  delay(80); // Measurement delay for AHT10

  if (Wire.requestFrom((uint8_t)AHT10_I2C_ADDR, (uint8_t)6) != 6) {
    _readings.climate.temperature = NAN;
    _readings.climate.humidity = NAN;
    return false;
  }

  uint8_t data[6];
  for (uint8_t i = 0; i < 6; ++i) data[i] = Wire.read();

  if (data[0] & 0x80) {
    // Sensor still busy
    return false;
  }

  uint32_t humidityRaw = ((uint32_t)data[1] << 12) |
                         ((uint32_t)data[2] << 4) | (data[3] >> 4);
  uint32_t temperatureRaw = (((uint32_t)data[3] & 0x0F) << 16) |
                            ((uint32_t)data[4] << 8) | data[5];

  float hum = (humidityRaw * 100.0f) / 1048576.0f;
  float temp = (temperatureRaw * 200.0f) / 1048576.0f - 50.0f;

  if (hum >= 0.0f && hum <= 100.0f && temp >= -40.0f && temp <= 85.0f) {
    _readings.climate.humidity = hum;
    _readings.climate.temperature = temp;
    return true;
  }

  _readings.climate.temperature = NAN;
  _readings.climate.humidity = NAN;
  return false;
}

String SensorManager::toJson() const {
  String json = "{";
  json += "\"soil\":[";
  for (int i = 0; i < 3; i++) {
    if (i > 0) json += ",";
    json += "{\"id\":" + String(_readings.soil[i].id) + ",";
    json += "\"pin\":" + String(_readings.soil[i].pin) + ",";
    json += "\"raw\":" + String(_readings.soil[i].raw) + ",";
    json += "\"percent\":" + String(_readings.soil[i].percent) + ",";
    json += "\"valid\":" + String(_readings.soil[i].valid ? "true" : "false") + "}";
  }
  json += "],";
  json += "\"soilAverage\":" + String(_readings.soilAverage) + ",";
  json += "\"anySoilValid\":" + String(_readings.anySoilValid ? "true" : "false") + ",";
  json += "\"climate\":{";
  json += "\"temperature\":" + (isnan(_readings.climate.temperature) ? "null" : String(_readings.climate.temperature, 1)) + ",";
  json += "\"humidity\":" + (isnan(_readings.climate.humidity) ? "null" : String(_readings.climate.humidity, 1)) + ",";
  json += "\"valid\":" + String(_readings.climate.valid ? "true" : "false");
  json += "}}";
  return json;
}
