#pragma once

#include <Arduino.h>
#include "Config.h"
#include "SettingsManager.h"

class ValveController {
public:
  ValveController();
  bool begin(const SettingsManager& settings);
  void update();

  // Pulse Commands
  bool openValve(const char* reason = "Manual Command");
  bool closeValve(const char* reason = "Manual Command");

  // State Getters
  bool isOpen() const { return _info.state == VALVE_OPEN; }
  ValveCommandState getState() const { return _info.state; }
  bool isConfirmed() const { return false; } // Hardware lacks physical feedback
  bool isPulseActive() const { return _pulseState != PULSE_IDLE; }
  uint32_t getWateringDurationSec() const;
  const ValveStateInfo& getInfo() const { return _info; }
  String getLastMessage() const { return _info.lastCommandMessage; }

  String toJson() const;

private:
  ValveStateInfo _info;
  PulseState _pulseState = PULSE_IDLE;
  uint32_t _pulseStartTime = 0;
  uint32_t _lastPulseEndTime = 0;
  uint16_t _pulseDurationMs = DEFAULT_PULSE_DURATION_MS;
  int8_t _pinOn = DEFAULT_PIN_VALVE_ON;
  int8_t _pinOff = DEFAULT_PIN_VALVE_OFF;

  bool triggerPulse(bool openCommand, const char* reason);
  void completePulse();
};
