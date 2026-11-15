/*
 * Garden watering assistant - feature test firmware
 * Target: ESP32-C3 SuperMini, Arduino framework
 *
 * Configure WIFI_SSID/WIFI_PASSWORD and RF_CODE before deployment.
 * RF_CODE is intentionally zero by default so a new installation cannot
 * operate an unknown relay until its learned 433 MHz code is supplied.
 */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "web_ui.h"

#define SOIL1 4
#define SOIL2 5
#define SOIL3 6
#define AHT10_SDA 8
#define AHT10_SCL 9
#define RF_TX 10

const char *WIFI_SSID = "YOUR_WIFI_SSID";
const char *WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Replace with the code learned from the CR-NAC100 remote. Zero disables RF.
const uint32_t RF_CODE = 0;
const uint8_t RF_BITS = 24;
const uint16_t RF_PULSE_US = 350;

const uint32_t SENSOR_INTERVAL_MS = 5000;
const uint32_t STATUS_INTERVAL_MS = 1000;
const uint32_t MAX_WATERING_MS = 10UL * 60UL * 1000UL;
const uint8_t MAX_BAD_READINGS = 3;

// Calibrate these values for the actual HW-390 sensors.
const int SOIL_DRY_ADC = 3200;
const int SOIL_WET_ADC = 1300;
const int START_WATERING_PERCENT = 35;
const int STOP_WATERING_PERCENT = 55;

WebServer server(80);
Preferences preferences;

struct SoilReading {
  int raw[3] = {0, 0, 0};
  int percent[3] = {0, 0, 0};
  int averagePercent = 0;
  bool valid = false;
};

SoilReading soil;
float airTemperature = NAN;
float airHumidity = NAN;
bool aht10Available = false;
bool automaticMode = true;
bool watering = false;
uint32_t wateringStartedAt = 0;
uint32_t manualStopAt = 0;
uint8_t badReadings = 0;
String lastEvent = "Booted";
uint32_t lastSensorRead = 0;
uint32_t lastStatusPrint = 0;

int soilPercent(int raw) {
  return constrain(map(raw, SOIL_DRY_ADC, SOIL_WET_ADC, 0, 100), 0, 100);
}

bool readSoilSensors() {
  soil.raw[0] = analogRead(SOIL1);
  soil.raw[1] = analogRead(SOIL2);
  soil.raw[2] = analogRead(SOIL3);

  soil.valid = true;
  int total = 0;
  for (uint8_t i = 0; i < 3; ++i) {
    // A saturated or disconnected ADC value is treated as a fault.
    if (soil.raw[i] < 20 || soil.raw[i] > 4080) soil.valid = false;
    soil.percent[i] = soilPercent(soil.raw[i]);
    total += soil.percent[i];
  }
  soil.averagePercent = total / 3;
  return soil.valid;
}

bool readAHT10() {
  Wire.beginTransmission(0x38);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) return false;

  delay(80);
  if (Wire.requestFrom(0x38, 6) != 6) return false;

  uint8_t data[6];
  for (uint8_t i = 0; i < 6; ++i) data[i] = Wire.read();
  if (data[0] & 0x80) return false; // still measuring

  uint32_t humidityRaw = ((uint32_t)data[1] << 12) |
                         ((uint32_t)data[2] << 4) | (data[3] >> 4);
  uint32_t temperatureRaw = (((uint32_t)data[3] & 0x0F) << 16) |
                            ((uint32_t)data[4] << 8) | data[5];
  airHumidity = (humidityRaw * 100.0f) / 1048576.0f;
  airTemperature = (temperatureRaw * 200.0f) / 1048576.0f - 50.0f;
  return airHumidity >= 0 && airHumidity <= 100 && airTemperature > -50 && airTemperature < 100;
}

void sendRfCode(bool on) {
  if (RF_CODE == 0) {
    lastEvent = on ? "Watering requested; RF code is not configured" : "Watering stopped; RF code is not configured";
    return;
  }

  // Generic 24-bit pulse-width OOK transmitter. Verify timing/protocol for
  // the actual CR-NAC100 before connecting the valve to mains power.
  uint32_t code = RF_CODE ^ (on ? 0 : 1);
  for (uint8_t repeat = 0; repeat < 5; ++repeat) {
    for (int8_t bit = RF_BITS - 1; bit >= 0; --bit) {
      bool one = code & (1UL << bit);
      digitalWrite(RF_TX, HIGH);
      delayMicroseconds(one ? RF_PULSE_US * 3 : RF_PULSE_US);
      digitalWrite(RF_TX, LOW);
      delayMicroseconds(one ? RF_PULSE_US : RF_PULSE_US * 3);
    }
    digitalWrite(RF_TX, LOW);
    delayMicroseconds(RF_PULSE_US * 10);
  }
  lastEvent = on ? "RF valve ON command sent" : "RF valve OFF command sent";
}

void startWatering(const char *reason) {
  if (watering) return;
  watering = true;
  wateringStartedAt = millis();
  sendRfCode(true);
  lastEvent = String("Watering started: ") + reason;
}

void stopWatering(const char *reason) {
  if (!watering) return;
  watering = false;
  manualStopAt = 0;
  sendRfCode(false);
  lastEvent = String("Watering stopped: ") + reason;
}

void evaluateAutomaticWatering() {
  if (!automaticMode) return;
  if (!soil.valid) {
    ++badReadings;
    if (badReadings >= MAX_BAD_READINGS) stopWatering("sensor fault");
    return;
  }
  badReadings = 0;

  if (watering && millis() - wateringStartedAt >= MAX_WATERING_MS) {
    stopWatering("maximum watering time reached");
  } else if (!watering && soil.averagePercent <= START_WATERING_PERCENT) {
    startWatering("soil is dry");
  } else if (watering && soil.averagePercent >= STOP_WATERING_PERCENT) {
    stopWatering("soil is wet");
  }
}

String jsonStatus() {
  String json = "{";
  json += "\"mode\":\"" + String(automaticMode ? "auto" : "manual") + "\",";
  json += "\"watering\":" + String(watering ? "true" : "false") + ",";
  json += "\"soilRaw\":[" + String(soil.raw[0]) + "," + String(soil.raw[1]) + "," + String(soil.raw[2]) + "],";
  json += "\"soilPercent\":[" + String(soil.percent[0]) + "," + String(soil.percent[1]) + "," + String(soil.percent[2]) + "],";
  json += "\"soilAverage\":" + String(soil.averagePercent) + ",";
  json += "\"temperature\":" + String(airTemperature, 1) + ",\"humidity\":" + String(airHumidity, 1) + ",";
  json += "\"sensorValid\":" + String(soil.valid ? "true" : "false") + ",";
  json += "\"lastEvent\":\"" + lastEvent + "\"}";
  return json;
}

void sendJsonResponse(int code, const String &body) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send(code, "application/json", body);
}

void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "*");
  server.send(204);
}

void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleMode() {
  String value = server.arg("value");
  if (value == "auto") automaticMode = true;
  else if (value == "manual") automaticMode = false;
  else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "value must be auto or manual");
    return;
  }
  preferences.putBool("auto", automaticMode);
  if (automaticMode) stopWatering("automatic mode enabled");
  lastEvent = String("Mode changed to ") + (automaticMode ? "automatic" : "manual");
  sendJsonResponse(200, jsonStatus());
}

void handleWater() {
  if (automaticMode) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(409, "text/plain", "switch to manual mode first");
    return;
  }
  uint32_t seconds = server.arg("seconds").toInt();
  if (seconds == 0 || seconds > 600) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "seconds must be 1..600");
    return;
  }
  startWatering("manual command");
  manualStopAt = millis() + seconds * 1000UL;
  sendJsonResponse(200, jsonStatus());
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, []() { sendJsonResponse(200, jsonStatus()); });
  server.on("/status", HTTP_OPTIONS, handleCORS);
  server.on("/mode", HTTP_GET, handleMode);
  server.on("/mode", HTTP_OPTIONS, handleCORS);
  server.on("/water", HTTP_GET, handleWater);
  server.on("/water", HTTP_OPTIONS, handleCORS);
  server.on("/stop", HTTP_GET, []() { stopWatering("manual command"); sendJsonResponse(200, jsonStatus()); });
  server.on("/stop", HTTP_OPTIONS, handleCORS);
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      handleCORS();
    } else {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(404, "text/plain", "not found");
    }
  });
  server.begin();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(RF_TX, OUTPUT);
  digitalWrite(RF_TX, LOW);
  analogReadResolution(12);
  Wire.begin(AHT10_SDA, AHT10_SCL);
  preferences.begin("watering", false);
  automaticMode = preferences.getBool("auto", true);
  aht10Available = readAHT10();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  uint8_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts++ < 20) delay(500);
  if (WiFi.status() == WL_CONNECTED) setupWebServer();

  Serial.println("Garden watering assistant ready");
  Serial.println(WiFi.status() == WL_CONNECTED ? WiFi.localIP() : IPAddress(0, 0, 0, 0));
}

void loop() {
  const uint32_t now = millis();
  if (now - lastSensorRead >= SENSOR_INTERVAL_MS || lastSensorRead == 0) {
    lastSensorRead = now;
    bool soilOk = readSoilSensors();
    aht10Available = readAHT10();
    if (!aht10Available) { airTemperature = NAN; airHumidity = NAN; }
    if (!soilOk) lastEvent = "Soil sensor fault detected";
    evaluateAutomaticWatering();
  }
  if (watering && !automaticMode && manualStopAt != 0 && (int32_t)(now - manualStopAt) >= 0) {
    stopWatering("manual watering duration reached");
  } else if (watering && now - wateringStartedAt >= MAX_WATERING_MS) {
    stopWatering("maximum watering time reached");
  }
  if (now - lastStatusPrint >= STATUS_INTERVAL_MS) {
    lastStatusPrint = now;
    Serial.println(jsonStatus());
  }
  if (WiFi.status() == WL_CONNECTED) server.handleClient();
}
