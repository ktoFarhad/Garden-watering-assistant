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

## 2026-11-15

Implemented a responsive minimalistic web frontend for the system (`feature/web-ui`).

Features & Architecture:
- Created `web/index.html`: standalone single-file frontend with zero external dependencies (no CDNs, works completely offline in local Wi-Fi networks).
- Generated `web_ui.h`: embedded PROGMEM representation allowing ESP32-C3 to serve the UI directly from flash at `GET /`.
- Added CORS headers (`Access-Control-Allow-Origin: *`, `OPTIONS` preflight handling) across all API endpoints in `test1.cpp`.
- Hero Card: real-time watering state, animated wave indicator, active countdown timer.
- Mode switcher: seamless toggle between Automatic and Manual modes (`/mode?value=auto|manual`).
- Soil moisture monitoring:
  - Prominent average moisture display with threshold chips (Dry <35%, Optimal 35-55%, Wet >55%).
  - Visual hysteresis scale bar with 35% start and 55% stop markers.
  - Individual HW-390 sensor cards (GPIO 4, 5, 6) showing percentages, raw ADC values, and mini bars.
  - Sensor fault warning banner if any sensor fails.
- Microclimate monitoring: AHT10 air temperature and humidity cards with connection state.
- Valve controls: duration presets (15s, 30s, 1m, 2m, 5m, 10m) and continuous slider, with 1-click manual trigger and emergency stop.
- Safety & event logs: live `lastEvent` display and safety limits summary.
- Built-in Simulation / Demo Mode: enables testing full UI functionality, animations, and sensor fluctuations without physical hardware.
- Modern minimalistic design with Dark and Light themes (persisted in `localStorage`).


## 2026-11-15 (Modular Architecture & Full TZ Implementation)

Refactored the firmware and web interface into an 8-module non-blocking architecture matching the formal Technical Specification:

1. **Hardware & Optocouplers (CR-NAC100):**
   - Removed 433 MHz RF dependency.
   - Implemented `ValveController` driving 2 × 4N35 optocouplers with 250 ms non-blocking pulses.
   - Enforced mutual exclusion, 500 ms cooldown, and safe `LOW` boot state.
   - Left optocoupler GPIOs configurable with a safe default of `-1`.
   - Distinct unconfirmed valve state reported in API and UI.

2. **Wi-Fi Manager (`WiFiManager`):**
   - Station mode connection with automatic background reconnection.
   - Fallback Access Point `Garden-Watering-AP` (192.168.4.1) for zero-config onboarding.
   - Web configuration for network credentials.

3. **Autonomous Safety & Automation (`SafetyManager` & `IrrigationManager`):**
   - Autonomous operation: losing Wi-Fi does not interrupt irrigation logic.
   - 10-minute maximum watering duration hard limit.
   - 3 consecutive bad sensor readings cut off watering.
   - Configurable common or per-sensor moisture thresholds with hysteresis.

4. **REST API & Web Interface:**
   - Full REST API under `/api/*` (`/api/status`, `/api/sensors`, `/api/settings`, `/api/valve/open`, `/api/valve/close`, `/api/mode`, `/api/wifi`).
   - Dark theme minimalist UI matching reference specifications with real-time polling and simulator demo mode.
