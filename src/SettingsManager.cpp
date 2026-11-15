#include "SettingsManager.h"

SettingsManager::SettingsManager() {
}

bool SettingsManager::begin() {
  if (_initialized) return true;
  if (!_prefs.begin("watering", false)) {
    return false;
  }
  _initialized = true;
  load();
  return true;
}

void SettingsManager::load() {
  if (!_initialized) return;

  _settings.wifiSsid = _prefs.getString("ssid", "");
  _settings.wifiPassword = _prefs.getString("pass", "");
  _settings.mode = (OperatingMode)_prefs.getUChar("mode", (uint8_t)MODE_AUTO);

  _settings.startPercent = _prefs.getInt("start_pct", DEFAULT_START_WATERING_PERCENT);
  _settings.stopPercent = _prefs.getInt("stop_pct", DEFAULT_STOP_WATERING_PERCENT);
  _settings.individualSensors = _prefs.getBool("indiv", false);

  _settings.individualStart[0] = _prefs.getInt("ind_s0", _settings.startPercent);
  _settings.individualStart[1] = _prefs.getInt("ind_s1", _settings.startPercent);
  _settings.individualStart[2] = _prefs.getInt("ind_s2", _settings.startPercent);

  _settings.individualStop[0] = _prefs.getInt("ind_e0", _settings.stopPercent);
  _settings.individualStop[1] = _prefs.getInt("ind_e1", _settings.stopPercent);
  _settings.individualStop[2] = _prefs.getInt("ind_e2", _settings.stopPercent);

  _settings.maxWateringSec = _prefs.getUInt("max_sec", DEFAULT_MAX_WATERING_SEC);
  _settings.pulseDurationMs = _prefs.getUShort("pulse_ms", DEFAULT_PULSE_DURATION_MS);

  _settings.pinValveOn = (int8_t)_prefs.getChar("pin_on", DEFAULT_PIN_VALVE_ON);
  _settings.pinValveOff = (int8_t)_prefs.getChar("pin_off", DEFAULT_PIN_VALVE_OFF);

  _settings.pinSoil[0] = (int8_t)_prefs.getChar("pin_s0", DEFAULT_PIN_SOIL1);
  _settings.pinSoil[1] = (int8_t)_prefs.getChar("pin_s1", DEFAULT_PIN_SOIL2);
  _settings.pinSoil[2] = (int8_t)_prefs.getChar("pin_s2", DEFAULT_PIN_SOIL3);

  _settings.soilDryAdc = _prefs.getInt("dry_adc", DEFAULT_SOIL_DRY_ADC);
  _settings.soilWetAdc = _prefs.getInt("wet_adc", DEFAULT_SOIL_WET_ADC);
}

bool SettingsManager::save() {
  if (!_initialized) return false;

  _prefs.putString("ssid", _settings.wifiSsid);
  _prefs.putString("pass", _settings.wifiPassword);
  _prefs.putUChar("mode", (uint8_t)_settings.mode);

  _prefs.putInt("start_pct", _settings.startPercent);
  _prefs.putInt("stop_pct", _settings.stopPercent);
  _prefs.putBool("indiv", _settings.individualSensors);

  _prefs.putInt("ind_s0", _settings.individualStart[0]);
  _prefs.putInt("ind_s1", _settings.individualStart[1]);
  _prefs.putInt("ind_s2", _settings.individualStart[2]);

  _prefs.putInt("ind_e0", _settings.individualStop[0]);
  _prefs.putInt("ind_e1", _settings.individualStop[1]);
  _prefs.putInt("ind_e2", _settings.individualStop[2]);

  _prefs.putUInt("max_sec", _settings.maxWateringSec);
  _prefs.putUShort("pulse_ms", _settings.pulseDurationMs);

  _prefs.putChar("pin_on", _settings.pinValveOn);
  _prefs.putChar("pin_off", _settings.pinValveOff);

  _prefs.putChar("pin_s0", _settings.pinSoil[0]);
  _prefs.putChar("pin_s1", _settings.pinSoil[1]);
  _prefs.putChar("pin_s2", _settings.pinSoil[2]);

  _prefs.putInt("dry_adc", _settings.soilDryAdc);
  _prefs.putInt("wet_adc", _settings.soilWetAdc);

  return true;
}

void SettingsManager::resetToDefaults() {
  _settings = SystemSettings();
  save();
}

void SettingsManager::setMode(OperatingMode mode) {
  _settings.mode = mode;
  if (_initialized) _prefs.putUChar("mode", (uint8_t)_settings.mode);
}

void SettingsManager::setWifiCredentials(const String& ssid, const String& password) {
  _settings.wifiSsid = ssid;
  _settings.wifiPassword = password;
  if (_initialized) {
    _prefs.putString("ssid", _settings.wifiSsid);
    _prefs.putString("pass", _settings.wifiPassword);
  }
}

void SettingsManager::setThresholds(int startPercent, int stopPercent) {
  _settings.startPercent = constrain(startPercent, 5, 95);
  _settings.stopPercent = constrain(stopPercent, _settings.startPercent + 5, 100);
  if (_initialized) {
    _prefs.putInt("start_pct", _settings.startPercent);
    _prefs.putInt("stop_pct", _settings.stopPercent);
  }
}

void SettingsManager::setPins(int8_t valveOn, int8_t valveOff, int8_t soil1, int8_t soil2, int8_t soil3) {
  _settings.pinValveOn = valveOn;
  _settings.pinValveOff = valveOff;
  _settings.pinSoil[0] = soil1;
  _settings.pinSoil[1] = soil2;
  _settings.pinSoil[2] = soil3;
  if (_initialized) {
    _prefs.putChar("pin_on", _settings.pinValveOn);
    _prefs.putChar("pin_off", _settings.pinValveOff);
    _prefs.putChar("pin_s0", _settings.pinSoil[0]);
    _prefs.putChar("pin_s1", _settings.pinSoil[1]);
    _prefs.putChar("pin_s2", _settings.pinSoil[2]);
  }
}

void SettingsManager::setTimings(uint32_t maxWateringSec, uint16_t pulseDurationMs) {
  _settings.maxWateringSec = constrain(maxWateringSec, 10, 3600);
  _settings.pulseDurationMs = constrain(pulseDurationMs, 50, 1000);
  if (_initialized) {
    _prefs.putUInt("max_sec", _settings.maxWateringSec);
    _prefs.putUShort("pulse_ms", _settings.pulseDurationMs);
  }
}

String SettingsManager::toJson() const {
  String json = "{";
  json += "\"mode\":\"" + String(_settings.mode == MODE_AUTO ? "auto" : "manual") + "\",";
  json += "\"wifi\":{\"ssid\":\"" + _settings.wifiSsid + "\",\"hasPassword\":" + (_settings.wifiPassword.length() > 0 ? "true" : "false") + "},";
  json += "\"thresholds\":{";
  json += "\"start\":" + String(_settings.startPercent) + ",";
  json += "\"stop\":" + String(_settings.stopPercent) + ",";
  json += "\"individual\":" + String(_settings.individualSensors ? "true" : "false") + ",";
  json += "\"indStart\":[" + String(_settings.individualStart[0]) + "," + String(_settings.individualStart[1]) + "," + String(_settings.individualStart[2]) + "],";
  json += "\"indStop\":[" + String(_settings.individualStop[0]) + "," + String(_settings.individualStop[1]) + "," + String(_settings.individualStop[2]) + "]";
  json += "},";
  json += "\"timing\":{";
  json += "\"maxDurationSec\":" + String(_settings.maxWateringSec) + ",";
  json += "\"pulseDurationMs\":" + String(_settings.pulseDurationMs);
  json += "},";
  json += "\"pins\":{";
  json += "\"valveOn\":" + String(_settings.pinValveOn) + ",";
  json += "\"valveOff\":" + String(_settings.pinValveOff) + ",";
  json += "\"soil\":[" + String(_settings.pinSoil[0]) + "," + String(_settings.pinSoil[1]) + "," + String(_settings.pinSoil[2]) + "],";
  json += "\"ahtSda\":" + String(_settings.pinAhtSda) + ",";
  json += "\"ahtScl\":" + String(_settings.pinAhtScl);
  json += "},";
  json += "\"calibration\":{";
  json += "\"soilDryAdc\":" + String(_settings.soilDryAdc) + ",";
  json += "\"soilWetAdc\":" + String(_settings.soilWetAdc);
  json += "}}";
  return json;
}

// Lightweight JSON field extraction helper
static String getJsonValue(const String& json, const String& key) {
  int keyIndex = json.indexOf("\"" + key + "\"");
  if (keyIndex == -1) return "";
  int colonIndex = json.indexOf(':', keyIndex);
  if (colonIndex == -1) return "";
  int startIndex = colonIndex + 1;
  while (startIndex < (int)json.length() && (json[startIndex] == ' ' || json[startIndex] == '\t')) startIndex++;
  if (startIndex >= (int)json.length()) return "";

  if (json[startIndex] == '\"') {
    int endIndex = json.indexOf('\"', startIndex + 1);
    if (endIndex != -1) return json.substring(startIndex + 1, endIndex);
  } else {
    int endIndex = startIndex;
    while (endIndex < (int)json.length() && json[endIndex] != ',' && json[endIndex] != '}' && json[endIndex] != ']' && json[endIndex] != '\r' && json[endIndex] != '\n') {
      endIndex++;
    }
    String val = json.substring(startIndex, endIndex);
    val.trim();
    return val;
  }
  return "";
}

bool SettingsManager::updateFromJson(const String& jsonString) {
  bool changed = false;

  String modeStr = getJsonValue(jsonString, "mode");
  if (modeStr == "auto") { _settings.mode = MODE_AUTO; changed = true; }
  else if (modeStr == "manual") { _settings.mode = MODE_MANUAL; changed = true; }

  String startStr = getJsonValue(jsonString, "start");
  if (startStr.length() > 0) {
    _settings.startPercent = constrain(startStr.toInt(), 5, 95);
    changed = true;
  }

  String stopStr = getJsonValue(jsonString, "stop");
  if (stopStr.length() > 0) {
    _settings.stopPercent = constrain(stopStr.toInt(), _settings.startPercent + 5, 100);
    changed = true;
  }

  String maxSecStr = getJsonValue(jsonString, "maxDurationSec");
  if (maxSecStr.length() > 0) {
    _settings.maxWateringSec = constrain(maxSecStr.toInt(), 10, 3600);
    changed = true;
  }

  String pulseMsStr = getJsonValue(jsonString, "pulseDurationMs");
  if (pulseMsStr.length() > 0) {
    _settings.pulseDurationMs = constrain(pulseMsStr.toInt(), 50, 1000);
    changed = true;
  }

  String valveOnStr = getJsonValue(jsonString, "valveOn");
  if (valveOnStr.length() > 0) {
    _settings.pinValveOn = (int8_t)valveOnStr.toInt();
    changed = true;
  }

  String valveOffStr = getJsonValue(jsonString, "valveOff");
  if (valveOffStr.length() > 0) {
    _settings.pinValveOff = (int8_t)valveOffStr.toInt();
    changed = true;
  }

  String dryAdcStr = getJsonValue(jsonString, "soilDryAdc");
  if (dryAdcStr.length() > 0) {
    _settings.soilDryAdc = dryAdcStr.toInt();
    changed = true;
  }

  String wetAdcStr = getJsonValue(jsonString, "soilWetAdc");
  if (wetAdcStr.length() > 0) {
    _settings.soilWetAdc = wetAdcStr.toInt();
    changed = true;
  }

  if (changed) {
    save();
  }
  return changed;
}
