# Implementation log

## 2026-11-14

Created `test1.cpp` as the feature-test firmware for the ESP32-C3 SuperMini.

Implemented:

- Three HW-390 analog soil sensor readings on GPIO 4, 5, and 6.
- 12-bit ADC setup and configurable dry/wet calibration values.
- Soil moisture conversion to percentages and average moisture calculation.
- AHT10 temperature and humidity readings over I2C on GPIO 8/9.
- Automatic watering with dry-start and wet-stop thresholds.
- Hysteresis between the start threshold (35%) and stop threshold (55%).
- Maximum watering duration of 10 minutes.
- Sensor-fault detection for invalid ADC values.
- Automatic shutdown after three consecutive invalid soil readings.
- Manual and automatic operating modes, persisted in ESP32 preferences.
- Generic 433 MHz OOK transmitter support on GPIO 10.
- Safe RF default: transmission is disabled until `RF_CODE` is configured.
- Wi-Fi station connection using configurable credentials.
- HTTP status and controls:
  - `/status` returns JSON sensor and watering status.
  - `/mode?value=auto` or `/mode?value=manual` changes the mode.
  - `/water?seconds=30` starts a manual watering cycle.
  - `/stop` stops watering.
- Manual watering honors the requested duration while remaining capped by the
  ten-minute maximum watering safety limit.
- Serial status output at 115200 baud.

Before hardware deployment, calibrate the soil ADC limits, configure Wi-Fi,
learn and verify the CR-NAC100 RF code, and test the mains section with proper
electrical isolation and protection.
