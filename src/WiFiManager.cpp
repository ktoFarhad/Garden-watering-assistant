#include "WiFiManager.h"

WiFiManager::WiFiManager() {
}

void WiFiManager::begin(const String& ssid, const String& password) {
  _currentSsid = ssid;
  _currentPass = password;

  if (_currentSsid.length() == 0) {
    // No saved network, start Access Point immediately
    startAp();
  } else {
    connectSta(_currentSsid, _currentPass);
  }
}

void WiFiManager::startAp() {
  WiFi.mode(WIFI_AP);
  if (strlen(DEFAULT_AP_PASS) > 0) {
    WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASS);
  } else {
    WiFi.softAP(DEFAULT_AP_SSID);
  }
  WiFi.softAPConfig(DEFAULT_AP_IP, DEFAULT_AP_GATEWAY, DEFAULT_AP_SUBNET);
  _apActive = true;
  _state = WIFI_STATE_AP_MODE;

  Serial.println("[WiFi] Access Point started: " DEFAULT_AP_SSID);
  Serial.print("[WiFi] AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void WiFiManager::connectSta(const String& ssid, const String& password) {
  _currentSsid = ssid;
  _currentPass = password;
  _connectAttempts = 0;
  _lastConnectionAttempt = millis();
  _state = WIFI_STATE_CONNECTING;

  // Start in AP+STA mode so server remains reachable while attempting connection
  WiFi.mode(WIFI_AP_STA);
  if (strlen(DEFAULT_AP_PASS) > 0) {
    WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASS);
  } else {
    WiFi.softAP(DEFAULT_AP_SSID);
  }
  _apActive = true;

  Serial.print("[WiFi] Connecting to Station: ");
  Serial.println(_currentSsid);
  WiFi.begin(_currentSsid.c_str(), _currentPass.c_str());
}

void WiFiManager::update() {
  const uint32_t now = millis();

  if (_state == WIFI_STATE_CONNECTING) {
    if (WiFi.status() == WL_CONNECTED) {
      _state = WIFI_STATE_CONNECTED;
      // Once connected to STA successfully, disable AP to save power/radio bandwidth
      WiFi.mode(WIFI_STA);
      _apActive = false;
      Serial.print("[WiFi] Connected! IP: ");
      Serial.println(WiFi.localIP());
    } else if (now - _lastConnectionAttempt >= STA_TIMEOUT_MS) {
      // Timeout connecting to STA, fallback to AP mode
      Serial.println("[WiFi] Connection failed / timeout. Falling back to AP mode.");
      startAp();
    }
  } else if (_state == WIFI_STATE_CONNECTED) {
    if (now - _lastCheckTime >= DEFAULT_WIFI_CHECK_INTERVAL_MS) {
      _lastCheckTime = now;
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WiFi] Connection lost. Reconnecting...");
        _state = WIFI_STATE_CONNECTING;
        _lastConnectionAttempt = now;
        WiFi.reconnect();
      }
    }
  }
}

bool WiFiManager::isConnected() const {
  return _state == WIFI_STATE_CONNECTED && WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIpAddress() const {
  if (isConnected()) {
    return WiFi.localIP().toString();
  } else if (_apActive) {
    return WiFi.softAPIP().toString();
  }
  return "0.0.0.0";
}

int8_t WiFiManager::getRssi() const {
  if (isConnected()) {
    return WiFi.RSSI();
  }
  return 0;
}

String WiFiManager::toJson() const {
  String json = "{";
  json += "\"status\":\"" + String(isConnected() ? "connected" : (_apActive ? "ap_mode" : "disconnected")) + "\",";
  json += "\"mode\":\"" + String(_apActive && isConnected() ? "AP_STA" : (_apActive ? "AP" : "STA")) + "\",";
  json += "\"ssid\":\"" + (_apActive && !isConnected() ? String(DEFAULT_AP_SSID) : _currentSsid) + "\",";
  json += "\"ip\":\"" + getIpAddress() + "\",";
  json += "\"rssi\":" + String(getRssi()) + ",";
  json += "\"apActive\":" + String(_apActive ? "true" : "false");
  json += "}";
  return json;
}
