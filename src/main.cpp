#include <Arduino.h>
#include "Config.h"
#include "SettingsManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "ValveController.h"
#include "SafetyManager.h"
#include "IrrigationManager.h"
#include "WebServerManager.h"

// System Module Instances
SettingsManager settings;
WiFiManager wifiManager;
SensorManager sensorManager;
ValveController valveController;
SafetyManager safetyManager;
IrrigationManager irrigationManager;

WebServerManager webServer(settings,
                           wifiManager,
                           sensorManager,
                           valveController,
                           safetyManager,
                           irrigationManager);

uint32_t lastSerialLogTime = 0;
const uint32_t SERIAL_LOG_INTERVAL_MS = 5000;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("==================================================");
  Serial.println("🌱 Garden Watering Assistant - ESP32-C3 SuperMini");
  Serial.println("Firmware: Modular Architecture v2.0");
  Serial.println("==================================================");

  // 1. Initialize Settings (NVS Preferences)
  if (!settings.begin()) {
    Serial.println("[Setup] ERROR: Failed to initialize SettingsManager NVS!");
  } else {
    Serial.println("[Setup] Settings loaded from NVS.");
  }

  // 2. Initialize Valve Controller (Guarantees safe LOW levels on boot)
  valveController.begin(settings);

  // 3. Initialize Sensors (ADC1 and AHT10 I2C)
  sensorManager.begin(settings);

  // 4. Initialize Safety & Automation
  safetyManager.begin();
  irrigationManager.begin(settings);

  // 5. Initialize Wi-Fi (Station with Fallback to AP mode)
  const auto& cfg = settings.getSettings();
  wifiManager.begin(cfg.wifiSsid, cfg.wifiPassword);

  // 6. Start Web Server & REST API
  webServer.begin();

  Serial.println("[Setup] System initialization complete. Entering main loop.");
}

void loop() {
  // 1. Wi-Fi connection state management and auto-reconnect
  wifiManager.update();

  // 2. Non-blocking periodic sensor polling (HW-390 & AHT10)
  sensorManager.update(settings);

  // 3. Non-blocking valve pulse management and watering duration tracking
  valveController.update();

  // 4. Autonomous safety checks (max watering duration, sensor fault cutoffs)
  safetyManager.update(settings, sensorManager, valveController);

  // 5. Irrigation decision logic (Auto mode hysteresis / Manual mode)
  irrigationManager.update(settings, sensorManager, valveController, safetyManager);

  // 6. Web server request handling
  webServer.update();

  // 7. Periodic serial diagnostics output
  const uint32_t now = millis();
  if (now - lastSerialLogTime >= SERIAL_LOG_INTERVAL_MS) {
    lastSerialLogTime = now;

    Serial.printf("[Status] IP: %s | Mode: %s | Valve: %s (%us) | Soil Avg: %d%% | T: %.1fC | H: %.1f%%\n",
                  wifiManager.getIpAddress().c_str(),
                  irrigationManager.getMode() == MODE_AUTO ? "AUTO" : "MANUAL",
                  valveController.isOpen() ? "OPEN" : "CLOSED",
                  valveController.getWateringDurationSec(),
                  sensorManager.getAverageMoisture(),
                  sensorManager.getTemperature(),
                  sensorManager.getHumidity());
  }
}
