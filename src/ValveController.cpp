#include "ValveController.h"

ValveController::ValveController() {
  _info.state = VALVE_CLOSED;
  _info.isConfirmed = false;
  _info.pulseActive = false;
  _info.wateringDurationSec = 0;
  _info.wateringStartedAtMs = 0;
  _info.lastCommandMessage = "Клапан закрыт при инициализации";
}

bool ValveController::begin(const SettingsManager& settings) {
  const auto& cfg = settings.getSettings();
  _pinOn = cfg.pinValveOn;
  _pinOff = cfg.pinValveOff;
  _pulseDurationMs = cfg.pulseDurationMs;

  // Initialize configured pins safely to LOW
  if (_pinOn >= 0) {
    pinMode(_pinOn, OUTPUT);
    digitalWrite(_pinOn, LOW);
  }
  if (_pinOff >= 0) {
    pinMode(_pinOff, OUTPUT);
    digitalWrite(_pinOff, LOW);
  }

  Serial.println("[ValveController] Initialized.");
  if (_pinOn < 0 || _pinOff < 0) {
    Serial.println("[ValveController] WARNING: Optocoupler pins not configured (-1). Configure pins in settings.");
  }
  return true;
}

bool ValveController::openValve(const char* reason) {
  return triggerPulse(true, reason);
}

bool ValveController::closeValve(const char* reason) {
  return triggerPulse(false, reason);
}

bool ValveController::triggerPulse(bool openCommand, const char* reason) {
  const uint32_t now = millis();

  // 1. Check if GPIOs are configured
  int8_t targetPin = openCommand ? _pinOn : _pinOff;
  if (targetPin < 0) {
    _info.lastCommandMessage = "Ошибка: GPIO оптопары не назначен (-1)";
    Serial.println("[ValveController] Command rejected: Target GPIO is not configured.");
    return false;
  }

  // 2. Check mutual exclusion and active pulse
  if (_pulseState != PULSE_IDLE) {
    _info.lastCommandMessage = "Защита: активен предыдущий импульс";
    Serial.println("[ValveController] Command rejected: Another pulse is already active.");
    return false;
  }

  // 3. Check cooldown between commands
  if (_lastPulseEndTime != 0 && (now - _lastPulseEndTime < DEFAULT_PULSE_COOLDOWN_MS)) {
    _info.lastCommandMessage = "Защита: интервал между командами (cooldown)";
    Serial.println("[ValveController] Command rejected: Cooldown active.");
    return false;
  }

  // 4. Ensure opposing pin is strictly LOW
  int8_t opposingPin = openCommand ? _pinOff : _pinOn;
  if (opposingPin >= 0) {
    digitalWrite(opposingPin, LOW);
  }

  // 5. Fire pulse
  digitalWrite(targetPin, HIGH);
  _pulseStartTime = now;
  _pulseState = openCommand ? PULSE_ON_ACTIVE : PULSE_OFF_ACTIVE;
  _info.pulseActive = true;

  if (openCommand) {
    _info.state = VALVE_OPEN;
    _info.wateringStartedAtMs = now;
    _info.lastCommandMessage = String("Импульс ON отправлен (") + reason + ") [физически не подтверждено]";
  } else {
    _info.state = VALVE_CLOSED;
    _info.wateringDurationSec = 0;
    _info.wateringStartedAtMs = 0;
    _info.lastCommandMessage = String("Импульс OFF отправлен (") + reason + ") [физически не подтверждено]";
  }

  Serial.println(_info.lastCommandMessage);
  return true;
}

void ValveController::update() {
  const uint32_t now = millis();

  // Handle pulse completion (non-blocking)
  if (_pulseState != PULSE_IDLE) {
    if (now - _pulseStartTime >= _pulseDurationMs) {
      completePulse();
    }
  }

  // Update ongoing watering duration
  if (_info.state == VALVE_OPEN && _info.wateringStartedAtMs != 0) {
    _info.wateringDurationSec = (now - _info.wateringStartedAtMs) / 1000UL;
  }
}

void ValveController::completePulse() {
  if (_pinOn >= 0) digitalWrite(_pinOn, LOW);
  if (_pinOff >= 0) digitalWrite(_pinOff, LOW);

  _lastPulseEndTime = millis();
  _pulseState = PULSE_IDLE;
  _info.pulseActive = false;
  Serial.println("[ValveController] Pulse completed, pins returned to LOW.");
}

uint32_t ValveController::getWateringDurationSec() const {
  if (_info.state == VALVE_OPEN && _info.wateringStartedAtMs != 0) {
    return (millis() - _info.wateringStartedAtMs) / 1000UL;
  }
  return 0;
}

String ValveController::toJson() const {
  String json = "{";
  json += "\"state\":\"" + String(_info.state == VALVE_OPEN ? "open" : "closed") + "\",";
  json += "\"isConfirmed\":false,"; // Always unconfirmed because CR-NAC100 has no physical feedback
  json += "\"pulseActive\":" + String(_info.pulseActive ? "true" : "false") + ",";
  json += "\"wateringDurationSec\":" + String(getWateringDurationSec()) + ",";
  json += "\"pinOn\":" + String(_pinOn) + ",";
  json += "\"pinOff\":" + String(_pinOff) + ",";
  json += "\"lastMessage\":\"" + _info.lastCommandMessage + "\"";
  json += "}";
  return json;
}
