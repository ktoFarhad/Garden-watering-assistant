#pragma once

#include <Arduino.h>

// =============================================================================
// Garden Watering Assistant - System Configuration & Defaults
// Target: ESP32-C3 SuperMini
// =============================================================================

// Default Wi-Fi Access Point settings (fallback mode for initial setup)
#define DEFAULT_AP_SSID "Garden-Watering-AP"
#define DEFAULT_AP_PASS "" // Open AP by default for initial setup
#define DEFAULT_AP_IP IPAddress(192, 168, 4, 1)
#define DEFAULT_AP_GATEWAY IPAddress(192, 168, 4, 1)
#define DEFAULT_AP_SUBNET IPAddress(255, 255, 255, 0)

// Default GPIO Pin Assignments
// CRITICAL: Optocoupler GPIOs are NOT assigned by default (-1).
// User must explicitly configure pins in settings to prevent spurious relay triggers.
#define DEFAULT_PIN_VALVE_ON   -1  // Optocoupler #1 (ON button pulse)
#define DEFAULT_PIN_VALVE_OFF  -1  // Optocoupler #2 (OFF button pulse)

// Soil Sensors (HW-390): ESP32-C3 ADC1 channels:
// ADC1_CH4 = GPIO4, ADC1_CH3 = GPIO3, ADC1_CH1 = GPIO1 (safe ADC1 pins with active Wi-Fi)
#define DEFAULT_PIN_SOIL1      4
#define DEFAULT_PIN_SOIL2      5   // ADC2_CH0 (or configurable to GPIO3 / GPIO1)
#define DEFAULT_PIN_SOIL3      1   // Safe ADC1_CH1 (was GPIO6 in earlier prototype, but GPIO6 lacks ADC on C3)

// AHT10 I2C Pins
#define DEFAULT_PIN_AHT_SDA    8
#define DEFAULT_PIN_AHT_SCL    9
#define AHT10_I2C_ADDR         0x38

// Timing & Threshold Defaults
#define DEFAULT_START_WATERING_PERCENT  35
#define DEFAULT_STOP_WATERING_PERCENT   55
#define DEFAULT_MAX_WATERING_SEC        600     // 10 minutes maximum duration
#define DEFAULT_PULSE_DURATION_MS       250     // 250 ms safe pulse for 4N35 optocoupler
#define DEFAULT_PULSE_COOLDOWN_MS       500     // Minimum delay between consecutive pulses
#define DEFAULT_SENSOR_INTERVAL_MS      2000    // Read sensors every 2 seconds
#define DEFAULT_WIFI_CHECK_INTERVAL_MS  10000   // Check Wi-Fi state every 10 seconds
#define MAX_BAD_READINGS_COUNT          3       // Cutoff watering after 3 consecutive sensor faults

// Sensor ADC Calibration Constants
#define DEFAULT_SOIL_DRY_ADC   3200
#define DEFAULT_SOIL_WET_ADC   1300
#define ADC_MIN_VALID_VALUE    20
#define ADC_MAX_VALID_VALUE    4080

// System Enums
enum OperatingMode {
  MODE_AUTO = 0,
  MODE_MANUAL = 1
};

enum ValveCommandState {
  VALVE_CLOSED = 0,
  VALVE_OPEN = 1
};

enum PulseState {
  PULSE_IDLE = 0,
  PULSE_ON_ACTIVE = 1,
  PULSE_OFF_ACTIVE = 2
};

enum WiFiStateEnum {
  WIFI_STATE_DISCONNECTED = 0,
  WIFI_STATE_CONNECTING = 1,
  WIFI_STATE_CONNECTED = 2,
  WIFI_STATE_AP_MODE = 3
};

// Data Structures
struct SoilSensorReading {
  int id = 1;
  int pin = -1;
  int raw = 0;
  int percent = 0;
  bool valid = false;
};

struct ClimateReading {
  float temperature = NAN;
  float humidity = NAN;
  bool valid = false;
};

struct SensorReadings {
  SoilSensorReading soil[3];
  int soilAverage = 0;
  bool anySoilValid = false;
  ClimateReading climate;
};

struct ValveStateInfo {
  ValveCommandState state = VALVE_CLOSED;
  bool isConfirmed = false;              // Always false: CR-NAC100 does not provide physical feedback
  bool pulseActive = false;
  uint32_t wateringDurationSec = 0;
  uint32_t wateringStartedAtMs = 0;
  String lastCommandMessage = "Клапан закрыт при запуске";
};

struct SafetyStatus {
  bool maxDurationReached = false;
  bool sensorFault = false;
  uint8_t badReadingCount = 0;
  bool emergencyStopped = false;
};

struct SystemSettings {
  String wifiSsid = "";
  String wifiPassword = "";
  OperatingMode mode = MODE_AUTO;
  
  // Thresholds
  int startPercent = DEFAULT_START_WATERING_PERCENT;
  int stopPercent = DEFAULT_STOP_WATERING_PERCENT;
  bool individualSensors = false;
  int individualStart[3] = {35, 35, 35};
  int individualStop[3] = {55, 55, 55};

  // Timings
  uint32_t maxWateringSec = DEFAULT_MAX_WATERING_SEC;
  uint16_t pulseDurationMs = DEFAULT_PULSE_DURATION_MS;

  // Pin Configuration
  int8_t pinValveOn = DEFAULT_PIN_VALVE_ON;
  int8_t pinValveOff = DEFAULT_PIN_VALVE_OFF;
  int8_t pinSoil[3] = {DEFAULT_PIN_SOIL1, DEFAULT_PIN_SOIL2, DEFAULT_PIN_SOIL3};
  int8_t pinAhtSda = DEFAULT_PIN_AHT_SDA;
  int8_t pinAhtScl = DEFAULT_PIN_AHT_SCL;

  // Calibration
  int soilDryAdc = DEFAULT_SOIL_DRY_ADC;
  int soilWetAdc = DEFAULT_SOIL_WET_ADC;
};
