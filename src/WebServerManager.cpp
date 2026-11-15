#include "WebServerManager.h"
#include "WebInterface.h"

WebServerManager::WebServerManager(SettingsManager& settings,
                                   WiFiManager& wifi,
                                   SensorManager& sensors,
                                   ValveController& valve,
                                   SafetyManager& safety,
                                   IrrigationManager& irrigation)
    : _server(80),
      _settings(settings),
      _wifi(wifi),
      _sensors(sensors),
      _valve(valve),
      _safety(safety),
      _irrigation(irrigation) {
}

void WebServerManager::begin() {
  setupRoutes();
  _server.begin();
  Serial.println("[WebServer] HTTP REST server started on port 80.");
}

void WebServerManager::update() {
  _server.handleClient();
}

void WebServerManager::sendCorsHeaders() {
  _server.sendHeader("Access-Control-Allow-Origin", "*");
  _server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  _server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, Accept");
}

void WebServerManager::sendJsonResponse(int code, const String& body) {
  sendCorsHeaders();
  _server.send(code, "application/json; charset=utf-8", body);
}

void WebServerManager::handlePreflight() {
  sendCorsHeaders();
  _server.send(204);
}

void WebServerManager::setupRoutes() {
  // Static Web Interface
  _server.on("/", HTTP_GET, [this]() { handleRoot(); });

  // REST API Routes
  _server.on("/api/status", HTTP_GET, [this]() { handleGetStatus(); });
  _server.on("/api/status", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/sensors", HTTP_GET, [this]() { handleGetSensors(); });
  _server.on("/api/sensors", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/settings", HTTP_GET, [this]() { handleGetSettings(); });
  _server.on("/api/settings", HTTP_POST, [this]() { handlePostSettings(); });
  _server.on("/api/settings", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/valve/open", HTTP_POST, [this]() { handleValveOpen(); });
  _server.on("/api/valve/open", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/valve/close", HTTP_POST, [this]() { handleValveClose(); });
  _server.on("/api/valve/close", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/mode", HTTP_POST, [this]() { handlePostMode(); });
  _server.on("/api/mode", HTTP_OPTIONS, [this]() { handlePreflight(); });

  _server.on("/api/wifi", HTTP_GET, [this]() { handleGetWiFi(); });
  _server.on("/api/wifi", HTTP_POST, [this]() { handlePostWiFi(); });
  _server.on("/api/wifi", HTTP_OPTIONS, [this]() { handlePreflight(); });

  // Not found handler
  _server.onNotFound([this]() {
    if (_server.method() == HTTP_OPTIONS) {
      handlePreflight();
    } else {
      sendCorsHeaders();
      _server.send(404, "application/json", "{\"error\":\"Route not found\"}");
    }
  });
}

void WebServerManager::handleRoot() {
  sendCorsHeaders();
  _server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void WebServerManager::handleGetStatus() {
  String json = "{";
  json += "\"mode\":\"" + String(_irrigation.getMode() == MODE_AUTO ? "auto" : "manual") + "\",";
  json += "\"valve\":" + _valve.toJson() + ",";
  json += "\"wifi\":" + _wifi.toJson() + ",";
  json += "\"safety\":" + _safety.toJson() + ",";
  json += "\"soilAverage\":" + String(_sensors.getAverageMoisture()) + ",";
  json += "\"temperature\":" + (isnan(_sensors.getTemperature()) ? "null" : String(_sensors.getTemperature(), 1)) + ",";
  json += "\"humidity\":" + (isnan(_sensors.getHumidity()) ? "null" : String(_sensors.getHumidity(), 1)) + ",";
  json += "\"lastEvent\":\"" + _irrigation.getLastEvent() + "\"";
  json += "}";

  sendJsonResponse(200, json);
}

void WebServerManager::handleGetSensors() {
  sendJsonResponse(200, _sensors.toJson());
}

void WebServerManager::handleGetSettings() {
  sendJsonResponse(200, _settings.toJson());
}

void WebServerManager::handlePostSettings() {
  String body = _server.arg("plain");
  if (body.length() == 0) {
    sendJsonResponse(400, "{\"error\":\"Empty request body\"}");
    return;
  }

  bool updated = _settings.updateFromJson(body);
  if (updated) {
    // Reconfigure valve pins in case they changed
    _valve.begin(_settings);
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Настройки сохранены\",\"settings\":" + _settings.toJson() + "}");
  } else {
    sendJsonResponse(400, "{\"error\":\"Не удалось применить настройки\"}");
  }
}

void WebServerManager::handleValveOpen() {
  bool success = _irrigation.manualOpen(_valve);
  if (success) {
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Импульс ON подан на оптопару 1 (состояние клапана не подтверждено физически)\",\"valve\":" + _valve.toJson() + "}");
  } else {
    sendJsonResponse(409, "{\"success\":false,\"error\":\"" + _valve.getLastMessage() + "\"}");
  }
}

void WebServerManager::handleValveClose() {
  bool success = _irrigation.manualClose(_valve);
  if (success) {
    sendJsonResponse(200, "{\"success\":true,\"message\":\"Импульс OFF подан на оптопару 2 (состояние клапана не подтверждено физически)\",\"valve\":" + _valve.toJson() + "}");
  } else {
    sendJsonResponse(409, "{\"success\":false,\"error\":\"" + _valve.getLastMessage() + "\"}");
  }
}

void WebServerManager::handlePostMode() {
  String body = _server.arg("plain");
  String modeVal = "";

  if (body.length() > 0) {
    int keyIdx = body.indexOf("\"mode\"");
    if (keyIdx != -1) {
      int colonIdx = body.indexOf(':', keyIdx);
      if (colonIdx != -1) {
        int quote1 = body.indexOf('\"', colonIdx);
        int quote2 = body.indexOf('\"', quote1 + 1);
        if (quote1 != -1 && quote2 != -1) {
          modeVal = body.substring(quote1 + 1, quote2);
        }
      }
    }
  }

  if (modeVal.length() == 0) {
    modeVal = _server.arg("value");
  }

  if (modeVal == "auto") {
    _irrigation.setMode(MODE_AUTO, _settings, _valve);
    sendJsonResponse(200, "{\"success\":true,\"mode\":\"auto\",\"message\":\"Включен автоматический режим\"}");
  } else if (modeVal == "manual") {
    _irrigation.setMode(MODE_MANUAL, _settings, _valve);
    sendJsonResponse(200, "{\"success\":true,\"mode\":\"manual\",\"message\":\"Включен ручной режим\"}");
  } else {
    sendJsonResponse(400, "{\"error\":\"Параметр mode должен быть 'auto' или 'manual'\"}");
  }
}

void WebServerManager::handleGetWiFi() {
  sendJsonResponse(200, _wifi.toJson());
}

void WebServerManager::handlePostWiFi() {
  String body = _server.arg("plain");
  String ssid = "";
  String password = "";

  if (body.length() > 0) {
    int ssidIdx = body.indexOf("\"ssid\"");
    if (ssidIdx != -1) {
      int colonIdx = body.indexOf(':', ssidIdx);
      int q1 = body.indexOf('\"', colonIdx);
      int q2 = body.indexOf('\"', q1 + 1);
      if (q1 != -1 && q2 != -1) ssid = body.substring(q1 + 1, q2);
    }
    int passIdx = body.indexOf("\"password\"");
    if (passIdx != -1) {
      int colonIdx = body.indexOf(':', passIdx);
      int q1 = body.indexOf('\"', colonIdx);
      int q2 = body.indexOf('\"', q1 + 1);
      if (q1 != -1 && q2 != -1) password = body.substring(q1 + 1, q2);
    }
  }

  if (ssid.length() == 0) {
    ssid = _server.arg("ssid");
    password = _server.arg("password");
  }

  if (ssid.length() == 0) {
    sendJsonResponse(400, "{\"error\":\"SSID не может быть пустым\"}");
    return;
  }

  _settings.setWifiCredentials(ssid, password);
  _settings.save();
  _wifi.connectSta(ssid, password);

  sendJsonResponse(200, "{\"success\":true,\"message\":\"Настройки Wi-Fi сохранены, выполняется подключение...\",\"ssid\":\"" + ssid + "\"}");
}
