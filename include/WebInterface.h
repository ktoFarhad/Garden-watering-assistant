#pragma once
#include <Arduino.h>

// Auto-generated WebInterface header for ESP32-C3 SuperMini
// Embedded minimalist dark-theme UI with REST API integration
const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Система полива — ESP32-C3</title>
  <style>
    :root {
      --bg: #090d16;
      --card-bg: #121826;
      --card-subtle: #192234;
      --card-highlight: #1f2a40;
      --border: rgba(255, 255, 255, 0.08);
      --border-focus: rgba(16, 185, 129, 0.5);
      --text-main: #f8fafc;
      --text-muted: #94a3b8;
      --text-dim: #64748b;
      --accent: #10b981;
      --accent-hover: #059669;
      --accent-glow: rgba(16, 185, 129, 0.25);
      --water-blue: #0ea5e9;
      --water-glow: rgba(14, 165, 233, 0.3);
      --warning: #f59e0b;
      --danger: #ef4444;
      --danger-hover: #dc2626;
      --radius-sm: 8px;
      --radius-md: 14px;
      --radius-lg: 20px;
      --radius-full: 9999px;
      --shadow: 0 4px 20px rgba(0, 0, 0, 0.35);
      --transition: all 0.25s cubic-bezier(0.4, 0, 0.2, 1);
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      -webkit-tap-highlight-color: transparent;
    }

    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", sans-serif;
      background-color: var(--bg);
      color: var(--text-main);
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 16px;
      line-height: 1.5;
    }

    .container {
      width: 100%;
      max-width: 580px;
      display: flex;
      flex-direction: column;
      gap: 16px;
    }

    /* Header */
    header {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 4px 2px;
    }

    .brand {
      display: flex;
      align-items: center;
      gap: 12px;
    }

    .logo-badge {
      width: 44px;
      height: 44px;
      background: linear-gradient(135deg, #10b981 0%, #059669 100%);
      border-radius: var(--radius-md);
      display: flex;
      align-items: center;
      justify-content: center;
      box-shadow: 0 4px 14px var(--accent-glow);
      font-size: 24px;
    }

    .brand-title h1 {
      font-size: 1.15rem;
      font-weight: 700;
      letter-spacing: -0.01em;
    }

    .brand-title p {
      font-size: 0.75rem;
      color: var(--text-dim);
      font-weight: 500;
    }

    .header-actions {
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .pill-badge {
      display: flex;
      align-items: center;
      gap: 6px;
      font-size: 0.75rem;
      font-weight: 600;
      padding: 6px 12px;
      border-radius: var(--radius-full);
      background: var(--card-bg);
      border: 1px solid var(--border);
    }

    .status-dot {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: var(--text-dim);
    }

    .status-dot.online {
      background: var(--accent);
      box-shadow: 0 0 10px var(--accent);
    }

    .status-dot.ap {
      background: var(--water-blue);
      box-shadow: 0 0 8px var(--water-blue);
    }

    .status-dot.offline {
      background: var(--danger);
    }

    .status-dot.demo {
      background: #a855f7;
      box-shadow: 0 0 8px #a855f7;
    }

    .icon-btn {
      background: var(--card-bg);
      border: 1px solid var(--border);
      color: var(--text-muted);
      width: 40px;
      height: 40px;
      border-radius: var(--radius-full);
      display: flex;
      align-items: center;
      justify-content: center;
      cursor: pointer;
      transition: var(--transition);
      outline: none;
    }

    .icon-btn:hover {
      color: var(--text-main);
      border-color: var(--border-focus);
      transform: translateY(-1px);
    }

    /* Cards */
    .card {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: var(--radius-lg);
      padding: 20px;
      box-shadow: var(--shadow);
      display: flex;
      flex-direction: column;
      gap: 14px;
      transition: var(--transition);
    }

    .card-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
    }

    .card-title {
      font-size: 0.85rem;
      font-weight: 600;
      color: var(--text-dim);
      text-transform: uppercase;
      letter-spacing: 0.05em;
    }

    /* Hero Valve Card */
    .hero-card {
      align-items: center;
      text-align: center;
      padding: 26px 20px;
      background: linear-gradient(180deg, var(--card-bg) 0%, var(--card-subtle) 100%);
      position: relative;
      overflow: hidden;
    }

    .hero-card.is-open {
      border-color: var(--water-blue);
      box-shadow: 0 0 30px var(--water-glow);
    }

    .valve-icon-wrap {
      width: 84px;
      height: 84px;
      border-radius: 50%;
      display: flex;
      align-items: center;
      justify-content: center;
      background: var(--card-subtle);
      border: 2px solid var(--border);
      position: relative;
      transition: var(--transition);
      font-size: 36px;
    }

    .hero-card.is-open .valve-icon-wrap {
      background: rgba(14, 165, 233, 0.15);
      border-color: var(--water-blue);
      color: var(--water-blue);
    }

    .water-ripple {
      position: absolute;
      inset: -10px;
      border-radius: 50%;
      border: 2px solid var(--water-blue);
      opacity: 0;
      pointer-events: none;
    }

    .hero-card.is-open .water-ripple {
      animation: rippleAnim 2s infinite cubic-bezier(0, 0.2, 0.8, 1);
    }

    @keyframes rippleAnim {
      0% { transform: scale(0.9); opacity: 0.8; }
      100% { transform: scale(1.4); opacity: 0; }
    }

    .valve-status-title {
      font-size: 1.4rem;
      font-weight: 800;
      letter-spacing: -0.01em;
    }

    .unconfirmed-note {
      font-size: 0.75rem;
      color: var(--warning);
      background: rgba(245, 158, 11, 0.12);
      border: 1px solid rgba(245, 158, 11, 0.25);
      padding: 4px 10px;
      border-radius: var(--radius-full);
      display: inline-flex;
      align-items: center;
      gap: 5px;
    }

    .timer-badge {
      font-size: 0.85rem;
      font-weight: 700;
      color: var(--text-muted);
      background: var(--card-subtle);
      border: 1px solid var(--border);
      padding: 4px 14px;
      border-radius: var(--radius-full);
    }

    /* Mode Selector Buttons */
    .mode-switch-box {
      display: grid;
      grid-template-columns: 1fr 1fr;
      background: var(--card-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 4px;
      gap: 4px;
      width: 100%;
    }

    .mode-btn {
      padding: 10px;
      border: none;
      background: transparent;
      color: var(--text-muted);
      font-size: 0.9rem;
      font-weight: 600;
      border-radius: 10px;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      transition: var(--transition);
    }

    .mode-btn.active {
      background: var(--card-bg);
      color: var(--text-main);
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.3);
      border: 1px solid var(--border);
    }

    /* Valve Control Buttons */
    .valve-actions-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
      width: 100%;
    }

    .btn-action {
      padding: 14px 12px;
      border: none;
      border-radius: var(--radius-md);
      font-size: 0.95rem;
      font-weight: 700;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      transition: var(--transition);
      box-shadow: var(--shadow);
    }

    .btn-open {
      background: linear-gradient(135deg, #10b981 0%, #059669 100%);
      color: white;
      box-shadow: 0 4px 14px var(--accent-glow);
    }

    .btn-open:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px var(--accent-glow);
    }

    .btn-close {
      background: linear-gradient(135deg, #ef4444 0%, #dc2626 100%);
      color: white;
      box-shadow: 0 4px 14px rgba(239, 68, 68, 0.25);
    }

    .btn-close:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px rgba(239, 68, 68, 0.35);
    }

    .btn-action:active {
      transform: translateY(0);
    }

    /* Soil Moisture Section */
    .moisture-average-box {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 16px;
      background: var(--card-subtle);
      border-radius: var(--radius-md);
      border: 1px solid var(--border);
    }

    .large-percent {
      font-size: 2.6rem;
      font-weight: 800;
      letter-spacing: -0.03em;
      line-height: 1;
    }

    .state-badge {
      display: inline-flex;
      align-items: center;
      gap: 5px;
      font-size: 0.75rem;
      font-weight: 700;
      padding: 4px 10px;
      border-radius: var(--radius-full);
      margin-top: 6px;
    }

    .state-dry { background: rgba(245, 158, 11, 0.15); color: var(--warning); border: 1px solid rgba(245, 158, 11, 0.3); }
    .state-optimal { background: rgba(16, 185, 129, 0.15); color: var(--accent); border: 1px solid rgba(16, 185, 129, 0.3); }
    .state-wet { background: rgba(14, 165, 233, 0.15); color: var(--water-blue); border: 1px solid rgba(14, 165, 233, 0.3); }

    .scale-bar-wrap {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .scale-bar-track {
      height: 10px;
      background: var(--card-subtle);
      border-radius: var(--radius-full);
      overflow: hidden;
      border: 1px solid var(--border);
    }

    .scale-bar-fill {
      height: 100%;
      width: 0%;
      border-radius: var(--radius-full);
      transition: width 0.6s cubic-bezier(0.4, 0, 0.2, 1), background-color 0.4s ease;
      background: var(--accent);
    }

    .scale-labels {
      display: flex;
      justify-content: space-between;
      font-size: 0.7rem;
      color: var(--text-dim);
      font-weight: 600;
    }

    /* 3 Sensors Grid */
    .sensors-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
    }

    .sensor-item {
      background: var(--card-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 12px;
      display: flex;
      flex-direction: column;
      gap: 4px;
    }

    .sensor-title {
      font-size: 0.75rem;
      color: var(--text-dim);
      font-weight: 600;
    }

    .sensor-val {
      font-size: 1.25rem;
      font-weight: 700;
    }

    .sensor-raw {
      font-size: 0.7rem;
      color: var(--text-dim);
    }

    .mini-progress-track {
      height: 4px;
      background: rgba(255, 255, 255, 0.06);
      border-radius: 2px;
      overflow: hidden;
      margin-top: 4px;
    }

    .mini-progress-fill {
      height: 100%;
      width: 0%;
      border-radius: 2px;
      background: var(--accent);
      transition: width 0.4s ease;
    }

    /* Climate Card */
    .climate-grid {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
    }

    .climate-box {
      background: var(--card-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 16px;
      display: flex;
      align-items: center;
      gap: 14px;
    }

    .climate-icon {
      width: 44px;
      height: 44px;
      border-radius: 12px;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 22px;
    }

    .climate-temp { background: rgba(245, 158, 11, 0.12); color: var(--warning); }
    .climate-hum { background: rgba(14, 165, 233, 0.12); color: var(--water-blue); }

    .climate-val {
      font-size: 1.35rem;
      font-weight: 800;
      letter-spacing: -0.02em;
    }

    /* System rows */
    .system-rows {
      display: flex;
      flex-direction: column;
      gap: 8px;
      font-size: 0.8rem;
    }

    .system-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 6px 0;
      border-bottom: 1px solid var(--border);
    }

    .system-row:last-child {
      border-bottom: none;
    }

    .system-key {
      color: var(--text-dim);
      font-weight: 500;
    }

    .system-val {
      color: var(--text-muted);
      font-weight: 600;
      text-align: right;
    }

    /* Modal Backdrop & Window */
    .modal-overlay {
      position: fixed;
      inset: 0;
      background: rgba(0, 0, 0, 0.7);
      backdrop-filter: blur(5px);
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 16px;
      z-index: 100;
      opacity: 0;
      pointer-events: none;
      transition: opacity 0.25s ease;
    }

    .modal-overlay.active {
      opacity: 1;
      pointer-events: auto;
    }

    .modal-content {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: var(--radius-lg);
      padding: 24px;
      width: 100%;
      max-width: 480px;
      max-height: 90vh;
      overflow-y: auto;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5);
      display: flex;
      flex-direction: column;
      gap: 16px;
      transform: translateY(16px);
      transition: transform 0.25s ease;
    }

    .modal-overlay.active .modal-content {
      transform: translateY(0);
    }

    .modal-tabs {
      display: flex;
      gap: 6px;
      border-bottom: 1px solid var(--border);
      padding-bottom: 8px;
    }

    .tab-btn {
      padding: 6px 14px;
      background: transparent;
      border: none;
      color: var(--text-dim);
      font-size: 0.85rem;
      font-weight: 600;
      border-radius: var(--radius-full);
      cursor: pointer;
      transition: var(--transition);
    }

    .tab-btn.active {
      background: var(--card-subtle);
      color: var(--accent);
    }

    .tab-pane {
      display: none;
      flex-direction: column;
      gap: 14px;
    }

    .tab-pane.active {
      display: flex;
    }

    .form-group {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .form-group label {
      font-size: 0.8rem;
      color: var(--text-dim);
      font-weight: 600;
    }

    .form-input {
      background: var(--card-subtle);
      border: 1px solid var(--border);
      color: var(--text-main);
      padding: 10px 14px;
      border-radius: var(--radius-md);
      font-size: 0.9rem;
      outline: none;
      transition: var(--transition);
    }

    .form-input:focus {
      border-color: var(--accent);
      box-shadow: 0 0 0 3px var(--accent-glow);
    }

    .form-row {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 12px;
    }

    /* Toast */
    .toast-box {
      position: fixed;
      bottom: 24px;
      left: 50%;
      transform: translateX(-50%);
      display: flex;
      flex-direction: column;
      gap: 8px;
      z-index: 200;
      pointer-events: none;
    }

    .toast-msg {
      background: var(--card-bg);
      color: var(--text-main);
      border: 1px solid var(--border);
      border-radius: var(--radius-full);
      padding: 10px 20px;
      font-size: 0.85rem;
      font-weight: 600;
      box-shadow: 0 4px 20px rgba(0, 0, 0, 0.4);
      display: flex;
      align-items: center;
      gap: 8px;
      animation: toastShow 0.3s ease, toastHide 0.3s ease 2.7s forwards;
    }

    @keyframes toastShow {
      from { opacity: 0; transform: translateY(16px); }
      to { opacity: 1; transform: translateY(0); }
    }

    @keyframes toastHide {
      from { opacity: 1; transform: translateY(0); }
      to { opacity: 0; transform: translateY(16px); }
    }

    .alert-box {
      background: rgba(239, 68, 68, 0.12);
      border: 1px solid rgba(239, 68, 68, 0.3);
      color: var(--danger);
      padding: 12px 16px;
      border-radius: var(--radius-md);
      font-size: 0.8rem;
      font-weight: 600;
      display: flex;
      align-items: center;
      gap: 10px;
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Header -->
    <header>
      <div class="brand">
        <div class="logo-badge">🌱</div>
        <div class="brand-title">
          <h1>Автополив</h1>
          <p id="subTitleText">ESP32-C3 SuperMini</p>
        </div>
      </div>
      <div class="header-actions">
        <div class="pill-badge" id="wifiBadge" title="Состояние Wi-Fi">
          <div class="status-dot offline" id="wifiDot"></div>
          <span id="wifiStatusText">Офлайн</span>
        </div>
        <button class="icon-btn" id="btnSettings" title="Настройки">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <circle cx="12" cy="12" r="3"></circle>
            <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
          </svg>
        </button>
      </div>
    </header>

    <!-- Alert Banner (hidden if sensors ok) -->
    <div class="alert-box" id="sensorAlertBox" style="display: none;">
      <span>⚠️</span>
      <div id="sensorAlertMsg">Сбой датчиков влажности почвы! Автополив заблокирован.</div>
    </div>

    <!-- Hero Card: Valve Status & Mode -->
    <div class="card hero-card" id="heroValveCard">
      <div class="valve-icon-wrap" id="valveIconWrap">
        <div class="water-ripple"></div>
        <span id="valveIconEmoji">💧</span>
      </div>
      <div>
        <div class="valve-status-title" id="valveStatusText">Клапан закрыт</div>
        <div style="margin-top: 6px;">
          <span class="unconfirmed-note" title="CR-NAC100 управляется импульсом оптопары без концевика">
            ⚠️ Состояние физически не подтверждено
          </span>
        </div>
      </div>

      <div class="timer-badge" id="wateringTimerBadge">
        Длительность: 00:00 (макс. 10 мин)
      </div>

      <!-- Mode Switcher -->
      <div class="mode-switch-box">
        <button class="mode-btn active" id="btnModeAuto" onclick="switchMode('auto')">
          🔄 Автоматический
        </button>
        <button class="mode-btn" id="btnModeManual" onclick="switchMode('manual')">
          ✋ Ручной
        </button>
      </div>

      <!-- Separate Push Button Controls for Optocouplers 1 & 2 -->
      <div class="valve-actions-grid">
        <button class="btn-action btn-open" onclick="postValveAction('open')">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><polygon points="5 3 19 12 5 21 5 3"></polygon></svg>
          Открыть клапан (ON)
        </button>
        <button class="btn-action btn-close" onclick="postValveAction('close')">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><rect x="6" y="6" width="12" height="12"></rect></svg>
          Закрыть клапан (OFF)
        </button>
      </div>
    </div>

    <!-- Soil Moisture Card (HW-390) -->
    <div class="card">
      <div class="card-header">
        <span class="card-title">Влажность почвы (HW-390)</span>
        <span id="soilAvgBadge" class="state-badge state-optimal">Оптимально</span>
      </div>

      <div class="moisture-average-box">
        <div>
          <span class="sensor-title">Средняя влажность</span>
          <div class="large-percent" id="soilAvgPercent">0%</div>
        </div>
        <div style="text-align: right;">
          <span class="sensor-title">Пороги автополива</span>
          <div style="font-size: 0.9rem; font-weight: 700; margin-top: 4px; color: var(--text-muted);" id="thresholdDisplay">
            35% 💧 55%
          </div>
        </div>
      </div>

      <div class="scale-bar-wrap">
        <div class="scale-bar-track">
          <div class="scale-bar-fill" id="soilAvgFill"></div>
        </div>
        <div class="scale-labels">
          <span>0% (Сухо)</span>
          <span id="scaleStartLabel">35% Старт</span>
          <span id="scaleStopLabel">55% Стоп</span>
          <span>100% (Влажно)</span>
        </div>
      </div>

      <div class="sensors-grid">
        <div class="sensor-item">
          <span class="sensor-title" id="soilLabel1">Датчик 1 (G4)</span>
          <div class="sensor-val" id="soil1Val">--%</div>
          <div class="sensor-raw" id="soil1Raw">ADC: ----</div>
          <div class="mini-progress-track"><div class="mini-progress-fill" id="soil1Fill"></div></div>
        </div>
        <div class="sensor-item">
          <span class="sensor-title" id="soilLabel2">Датчик 2 (G5)</span>
          <div class="sensor-val" id="soil2Val">--%</div>
          <div class="sensor-raw" id="soil2Raw">ADC: ----</div>
          <div class="mini-progress-track"><div class="mini-progress-fill" id="soil2Fill"></div></div>
        </div>
        <div class="sensor-item">
          <span class="sensor-title" id="soilLabel3">Датчик 3 (G1)</span>
          <div class="sensor-val" id="soil3Val">--%</div>
          <div class="sensor-raw" id="soil3Raw">ADC: ----</div>
          <div class="mini-progress-track"><div class="mini-progress-fill" id="soil3Fill"></div></div>
        </div>
      </div>
    </div>

    <!-- Microclimate Card (AHT10) -->
    <div class="card">
      <div class="card-header">
        <span class="card-title">Микроклимат (AHT10)</span>
        <span id="ahtBadge" style="font-size: 0.75rem; color: var(--accent); font-weight: 600;">I2C активен</span>
      </div>
      <div class="climate-grid">
        <div class="climate-box">
          <div class="climate-icon climate-temp">🌡️</div>
          <div>
            <p class="sensor-title">Температура</p>
            <div class="climate-val" id="tempVal">-- °C</div>
          </div>
        </div>
        <div class="climate-box">
          <div class="climate-icon climate-hum">💧</div>
          <div>
            <p class="sensor-title">Влажность воздуха</p>
            <div class="climate-val" id="humVal">-- %</div>
          </div>
        </div>
      </div>
    </div>

    <!-- System Status & Safety Card -->
    <div class="card">
      <div class="card-header">
        <span class="card-title">Системные параметры</span>
      </div>
      <div class="system-rows">
        <div class="system-row">
          <span class="system-key">IP-адрес:</span>
          <span class="system-val" id="sysIpVal">0.0.0.0</span>
        </div>
        <div class="system-row">
          <span class="system-key">Wi-Fi сеть:</span>
          <span class="system-val" id="sysSsidVal">—</span>
        </div>
        <div class="system-row">
          <span class="system-key">Реле CR-NAC100:</span>
          <span class="system-val">2× 4N35 оптопары (импульс)</span>
        </div>
        <div class="system-row">
          <span class="system-key">Последнее событие:</span>
          <span class="system-val" id="sysEventVal">Инициализация...</span>
        </div>
      </div>
    </div>
  </div>

  <!-- Modal Settings -->
  <div class="modal-overlay" id="settingsModal">
    <div class="modal-content">
      <div class="card-header">
        <h3 style="font-size: 1.1rem; font-weight: 700;">Настройки системы</h3>
        <button class="icon-btn" onclick="toggleSettingsModal(false)">✕</button>
      </div>

      <div class="modal-tabs">
        <button class="tab-btn active" onclick="switchTab('tabWifi')">Wi-Fi</button>
        <button class="tab-btn" onclick="switchTab('tabThresholds')">Пороги</button>
        <button class="tab-btn" onclick="switchTab('tabPins')">GPIO / Аппаратура</button>
        <button class="tab-btn" onclick="switchTab('tabDemo')">Демо</button>
      </div>

      <!-- Tab 1: Wi-Fi -->
      <div class="tab-pane active" id="tabWifi">
        <div class="form-group">
          <label>Имя сети (SSID):</label>
          <input type="text" id="cfgWifiSsid" class="form-input" placeholder="Название домашней сети">
        </div>
        <div class="form-group">
          <label>Пароль Wi-Fi:</label>
          <input type="password" id="cfgWifiPass" class="form-input" placeholder="Пароль сети">
        </div>
        <button class="btn-action btn-open" onclick="saveWifiSettings()">Сохранить и подключиться к Wi-Fi</button>
      </div>

      <!-- Tab 2: Thresholds & Timing -->
      <div class="tab-pane" id="tabThresholds">
        <div class="form-row">
          <div class="form-group">
            <label>Порог старта полива (%):</label>
            <input type="number" id="cfgStartPct" class="form-input" min="5" max="95" value="35">
          </div>
          <div class="form-group">
            <label>Порог стопа полива (%):</label>
            <input type="number" id="cfgStopPct" class="form-input" min="10" max="100" value="55">
          </div>
        </div>
        <div class="form-group">
          <label>Макс. время полива (секунд):</label>
          <input type="number" id="cfgMaxSec" class="form-input" min="10" max="3600" value="600">
        </div>
        <button class="btn-action btn-open" onclick="saveThresholdSettings()">Сохранить пороги</button>
      </div>

      <!-- Tab 3: Pins & Hardware -->
      <div class="tab-pane" id="tabPins">
        <div class="form-row">
          <div class="form-group">
            <label>GPIO Оптопары ON:</label>
            <input type="number" id="cfgPinOn" class="form-input" placeholder="-1 (не назначен)">
          </div>
          <div class="form-group">
            <label>GPIO Оптопары OFF:</label>
            <input type="number" id="cfgPinOff" class="form-input" placeholder="-1 (не назначен)">
          </div>
        </div>
        <div class="form-group">
          <label>Длительность импульса оптопары (мс):</label>
          <input type="number" id="cfgPulseMs" class="form-input" min="50" max="1000" value="250">
        </div>
        <button class="btn-action btn-open" onclick="savePinSettings()">Сохранить конфигурацию GPIO</button>
      </div>

      <!-- Tab 4: Demo / Connection Target -->
      <div class="tab-pane" id="tabDemo">
        <div class="form-group">
          <label>Адрес контроллера (если запущен не с ESP32):</label>
          <input type="text" id="cfgHostUrl" class="form-input" placeholder="http://192.168.1.120 или пусто">
        </div>
        <div class="form-group" style="flex-direction: row; align-items: center; gap: 10px; margin-top: 8px;">
          <input type="checkbox" id="cfgDemoMode" onchange="toggleDemoMode(this.checked)">
          <label for="cfgDemoMode" style="cursor: pointer;">Включить Демо-режим (симулятор датчиков)</label>
        </div>
        <button class="btn-action btn-open" onclick="saveDemoSettings()">Применить</button>
      </div>
    </div>
  </div>

  <div class="toast-box" id="toastContainer"></div>

  <script>
    // System State
    const state = {
      mode: 'auto',
      valve: { state: 'closed', isConfirmed: false, pulseActive: false, wateringDurationSec: 0, lastMessage: '' },
      wifi: { status: 'disconnected', mode: 'STA', ssid: '', ip: '0.0.0.0', rssi: 0, apActive: false },
      safety: { maxDurationReached: false, sensorFault: false, lastEvent: '' },
      soil: [
        { id: 1, pin: 4, raw: 0, percent: 0, valid: false },
        { id: 2, pin: 5, raw: 0, percent: 0, valid: false },
        { id: 3, pin: 1, raw: 0, percent: 0, valid: false }
      ],
      soilAverage: 0,
      climate: { temperature: null, humidity: null, valid: false },
      settings: {
        thresholds: { start: 35, stop: 55 },
        timing: { maxDurationSec: 600, pulseDurationMs: 250 },
        pins: { valveOn: -1, valveOff: -1, soil: [4, 5, 1] }
      },
      lastEvent: 'Инициализация...',
      targetHost: localStorage.getItem('garden_host') || '',
      isDemo: localStorage.getItem('garden_demo') === 'true'
    };

    let pollInterval = null;

    window.addEventListener('DOMContentLoaded', () => {
      document.getElementById('cfgHostUrl').value = state.targetHost;
      document.getElementById('cfgDemoMode').checked = state.isDemo;
      document.getElementById('btnSettings').addEventListener('click', () => toggleSettingsModal(true));

      startPolling();
    });

    function getApiUrl(endpoint) {
      const base = state.targetHost || window.location.origin;
      return `${base}${endpoint}`;
    }

    function toggleSettingsModal(open) {
      const modal = document.getElementById('settingsModal');
      if (open) {
        modal.classList.add('active');
        fetchSettings();
      } else {
        modal.classList.remove('active');
      }
    }

    function switchTab(tabId) {
      document.querySelectorAll('.tab-pane').forEach(el => el.classList.remove('active'));
      document.querySelectorAll('.tab-btn').forEach(el => el.classList.remove('active'));
      document.getElementById(tabId).classList.add('active');
      event.target.classList.add('active');
    }

    function showToast(msg) {
      const box = document.getElementById('toastContainer');
      const toast = document.createElement('div');
      toast.className = 'toast-msg';
      toast.innerHTML = msg;
      box.appendChild(toast);
      setTimeout(() => { if (toast.parentNode) toast.remove(); }, 3000);
    }

    function startPolling() {
      if (pollInterval) clearInterval(pollInterval);
      pollData();
      pollInterval = setInterval(pollData, 2000);
    }

    async function pollData() {
      if (state.isDemo) {
        simulateDemo();
        updateUI();
        return;
      }

      try {
        const [resStatus, resSensors] = await Promise.all([
          fetch(getApiUrl('/api/status'), { headers: { 'Accept': 'application/json' } }),
          fetch(getApiUrl('/api/sensors'), { headers: { 'Accept': 'application/json' } })
        ]);

        if (resStatus.ok && resSensors.ok) {
          const statusData = await resStatus.json();
          const sensorsData = await resSensors.json();

          state.mode = statusData.mode || 'auto';
          state.valve = statusData.valve || state.valve;
          state.wifi = statusData.wifi || state.wifi;
          state.safety = statusData.safety || state.safety;
          state.lastEvent = statusData.lastEvent || 'Норма';

          state.soil = sensorsData.soil || state.soil;
          state.soilAverage = sensorsData.soilAverage !== undefined ? sensorsData.soilAverage : 0;
          state.climate = sensorsData.climate || state.climate;

          updateUI();
        } else {
          setOfflineState();
        }
      } catch (e) {
        setOfflineState();
      }
    }

    function setOfflineState() {
      const dot = document.getElementById('wifiDot');
      const text = document.getElementById('wifiStatusText');
      dot.className = 'status-dot offline';
      text.innerText = 'Офлайн';
    }

    function updateUI() {
      // 1. Wi-Fi Badge
      const wifiDot = document.getElementById('wifiDot');
      const wifiText = document.getElementById('wifiStatusText');
      if (state.isDemo) {
        wifiDot.className = 'status-dot demo';
        wifiText.innerText = 'Демо-режим';
      } else if (state.wifi.status === 'connected') {
        wifiDot.className = 'status-dot online';
        wifiText.innerText = `${state.wifi.ip}`;
      } else if (state.wifi.apActive) {
        wifiDot.className = 'status-dot ap';
        wifiText.innerText = 'AP: 192.168.4.1';
      } else {
        wifiDot.className = 'status-dot offline';
        wifiText.innerText = 'Отключено';
      }

      // 2. Valve Status
      const heroCard = document.getElementById('heroValveCard');
      const valveText = document.getElementById('valveStatusText');
      const timerBadge = document.getElementById('wateringTimerBadge');
      const isOpen = state.valve.state === 'open';

      if (isOpen) {
        heroCard.classList.add('is-open');
        valveText.innerText = 'Клапан открыт (подан импульс ON)';
        valveText.style.color = 'var(--water-blue)';
        const sec = state.valve.wateringDurationSec || 0;
        const m = Math.floor(sec / 60);
        const s = sec % 60;
        timerBadge.innerText = `Идёт полив: ${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;
        timerBadge.style.color = 'var(--water-blue)';
      } else {
        heroCard.classList.remove('is-open');
        valveText.innerText = 'Клапан закрыт';
        valveText.style.color = 'var(--text-main)';
        timerBadge.innerText = `Полив выключен (макс. ${Math.round(state.settings.timing.maxDurationSec / 60)} мин)`;
        timerBadge.style.color = 'var(--text-muted)';
      }

      // 3. Mode Buttons
      document.getElementById('btnModeAuto').classList.toggle('active', state.mode === 'auto');
      document.getElementById('btnModeManual').classList.toggle('active', state.mode === 'manual');

      // 4. Soil Moisture
      const avg = Math.round(state.soilAverage);
      document.getElementById('soilAvgPercent').innerText = `${avg}%`;
      const fill = document.getElementById('soilAvgFill');
      fill.style.width = `${Math.min(100, Math.max(0, avg))}%`;

      const badge = document.getElementById('soilAvgBadge');
      const startThresh = state.settings.thresholds.start;
      const stopThresh = state.settings.thresholds.stop;

      if (avg <= startThresh) {
        badge.className = 'state-badge state-dry';
        badge.innerText = `Сухо (≤ ${startThresh}%)`;
        fill.style.backgroundColor = 'var(--warning)';
      } else if (avg >= stopThresh) {
        badge.className = 'state-badge state-wet';
        badge.innerText = `Влажно (≥ ${stopThresh}%)`;
        fill.style.backgroundColor = 'var(--water-blue)';
      } else {
        badge.className = 'state-badge state-optimal';
        badge.innerText = 'Оптимально';
        fill.style.backgroundColor = 'var(--accent)';
      }

      // 3 Sensors
      for (let i = 0; i < 3; i++) {
        const s = state.soil[i] || { raw: 0, percent: 0, pin: -1 };
        const valEl = document.getElementById(`soil${i + 1}Val`);
        const rawEl = document.getElementById(`soil${i + 1}Raw`);
        const fillEl = document.getElementById(`soil${i + 1}Fill`);
        const labelEl = document.getElementById(`soilLabel${i + 1}`);

        if (labelEl && s.pin >= 0) labelEl.innerText = `Датчик ${i + 1} (G${s.pin})`;
        if (valEl) valEl.innerText = `${s.percent}%`;
        if (rawEl) rawEl.innerText = `ADC: ${s.raw}`;
        if (fillEl) fillEl.style.width = `${Math.min(100, Math.max(0, s.percent))}%`;
      }

      // Sensor Alert Banner
      const alertBox = document.getElementById('sensorAlertBox');
      if (state.safety.sensorFault) {
        alertBox.style.display = 'flex';
      } else {
        alertBox.style.display = 'none';
      }

      // 5. Climate
      const t = state.climate.temperature;
      const h = state.climate.humidity;
      document.getElementById('tempVal').innerText = (t !== null && !isNaN(t)) ? `${t.toFixed(1)} °C` : '-- °C';
      document.getElementById('humVal').innerText = (h !== null && !isNaN(h)) ? `${h.toFixed(1)} %` : '-- %';

      // 6. System rows
      document.getElementById('sysIpVal').innerText = state.wifi.ip || '0.0.0.0';
      document.getElementById('sysSsidVal').innerText = state.wifi.ssid || '—';
      document.getElementById('sysEventVal').innerText = state.lastEvent || '—';
    }

    async function switchMode(newMode) {
      if (state.isDemo) {
        state.mode = newMode;
        state.lastEvent = `Режим изменён на ${newMode}`;
        updateUI();
        showToast(`Включен ${newMode === 'auto' ? 'автоматический' : 'ручной'} режим`);
        return;
      }

      try {
        const res = await fetch(getApiUrl('/api/mode'), {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ mode: newMode })
        });
        const data = await res.json();
        if (res.ok) {
          state.mode = newMode;
          updateUI();
          showToast(data.message || 'Режим обновлен');
        } else {
          showToast(`Ошибка: ${data.error || 'Не удалось сменить режим'}`);
        }
      } catch (e) {
        showToast(`Ошибка сети: ${e.message}`);
      }
    }

    async function postValveAction(action) {
      if (state.isDemo) {
        if (action === 'open') {
          state.valve.state = 'open';
          state.valve.wateringDurationSec = 1;
          state.lastEvent = 'Демо: подан импульс ON (клапан открыт)';
          showToast('Импульс ON подан на оптопару 1');
        } else {
          state.valve.state = 'closed';
          state.valve.wateringDurationSec = 0;
          state.lastEvent = 'Демо: подан импульс OFF (клапан закрыт)';
          showToast('Импульс OFF подан на оптопару 2');
        }
        updateUI();
        return;
      }

      try {
        const res = await fetch(getApiUrl(`/api/valve/${action}`), {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' }
        });
        const data = await res.json();
        if (res.ok) {
          showToast(data.message);
          pollData();
        } else {
          showToast(`Ошибка: ${data.error || 'Команда отклонена'}`);
        }
      } catch (e) {
        showToast(`Ошибка: ${e.message}`);
      }
    }

    async function fetchSettings() {
      if (state.isDemo) return;
      try {
        const res = await fetch(getApiUrl('/api/settings'));
        if (res.ok) {
          const cfg = await res.json();
          state.settings = cfg;
          document.getElementById('cfgStartPct').value = cfg.thresholds.start;
          document.getElementById('cfgStopPct').value = cfg.thresholds.stop;
          document.getElementById('cfgMaxSec').value = cfg.timing.maxDurationSec;
          document.getElementById('cfgPulseMs').value = cfg.timing.pulseDurationMs;
          document.getElementById('cfgPinOn').value = cfg.pins.valveOn;
          document.getElementById('cfgPinOff').value = cfg.pins.valveOff;
          document.getElementById('cfgWifiSsid').value = cfg.wifi.ssid || '';
        }
      } catch (e) {}
    }

    async function saveWifiSettings() {
      const ssid = document.getElementById('cfgWifiSsid').value.trim();
      const pass = document.getElementById('cfgWifiPass').value;

      if (!ssid) {
        showToast('Введите SSID сети');
        return;
      }

      if (state.isDemo) {
        state.wifi.ssid = ssid;
        showToast('Демо: настройки Wi-Fi сохранены');
        toggleSettingsModal(false);
        return;
      }

      try {
        const res = await fetch(getApiUrl('/api/wifi'), {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ ssid: ssid, password: pass })
        });
        const data = await res.json();
        if (res.ok) {
          showToast(data.message);
          toggleSettingsModal(false);
        } else {
          showToast(data.error);
        }
      } catch (e) {
        showToast(e.message);
      }
    }

    async function saveThresholdSettings() {
      const start = parseInt(document.getElementById('cfgStartPct').value, 10);
      const stop = parseInt(document.getElementById('cfgStopPct').value, 10);
      const maxSec = parseInt(document.getElementById('cfgMaxSec').value, 10);

      const payload = { start: start, stop: stop, maxDurationSec: maxSec };

      if (state.isDemo) {
        state.settings.thresholds.start = start;
        state.settings.thresholds.stop = stop;
        state.settings.timing.maxDurationSec = maxSec;
        showToast('Пороги сохранены');
        toggleSettingsModal(false);
        updateUI();
        return;
      }

      try {
        const res = await fetch(getApiUrl('/api/settings'), {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(payload)
        });
        if (res.ok) {
          showToast('Настройки сохранены');
          toggleSettingsModal(false);
          pollData();
        }
      } catch (e) { showToast(e.message); }
    }

    async function savePinSettings() {
      const pinOn = parseInt(document.getElementById('cfgPinOn').value, 10);
      const pinOff = parseInt(document.getElementById('cfgPinOff').value, 10);
      const pulseMs = parseInt(document.getElementById('cfgPulseMs').value, 10);

      const payload = { valveOn: pinOn, valveOff: pinOff, pulseDurationMs: pulseMs };

      if (state.isDemo) {
        state.settings.pins.valveOn = pinOn;
        state.settings.pins.valveOff = pinOff;
        state.settings.timing.pulseDurationMs = pulseMs;
        showToast('GPIO оптопар обновлены');
        toggleSettingsModal(false);
        return;
      }

      try {
        const res = await fetch(getApiUrl('/api/settings'), {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(payload)
        });
        if (res.ok) {
          showToast('Конфигурация GPIO сохранена');
          toggleSettingsModal(false);
          pollData();
        }
      } catch (e) { showToast(e.message); }
    }

    function saveDemoSettings() {
      let host = document.getElementById('cfgHostUrl').value.trim();
      if (host && !host.startsWith('http://') && !host.startsWith('https://')) host = 'http://' + host;
      if (host.endsWith('/')) host = host.slice(0, -1);
      state.targetHost = host;
      localStorage.setItem('garden_host', host);

      toggleSettingsModal(false);
      showToast('Настройки обновлены');
      startPolling();
    }

    function toggleDemoMode(enabled) {
      state.isDemo = enabled;
      localStorage.setItem('garden_demo', enabled ? 'true' : 'false');
      showToast(enabled ? 'Демо-режим включен' : 'Демо-режим выключен');
    }

    function simulateDemo() {
      if (state.soil[0].raw === 0) {
        state.soil = [
          { id: 1, pin: 4, raw: 2150, percent: 42, valid: true },
          { id: 2, pin: 5, raw: 2200, percent: 40, valid: true },
          { id: 3, pin: 1, raw: 2100, percent: 44, valid: true }
        ];
        state.climate = { temperature: 24.2, humidity: 59.1, valid: true };
      }

      if (state.valve.state === 'open') {
        state.valve.wateringDurationSec += 2;
        for (let i = 0; i < 3; i++) {
          if (state.soil[i].percent < 80) state.soil[i].percent += 1;
        }
      } else {
        if (Math.random() > 0.6) {
          for (let i = 0; i < 3; i++) {
            if (state.soil[i].percent > 20) state.soil[i].percent -= (Math.random() > 0.5 ? 1 : 0);
          }
        }
      }

      state.soilAverage = Math.round((state.soil[0].percent + state.soil[1].percent + state.soil[2].percent) / 3);

      // Auto mode triggers in demo
      if (state.mode === 'auto') {
        if (state.valve.state !== 'open' && state.soilAverage <= state.settings.thresholds.start) {
          state.valve.state = 'open';
          state.valve.wateringDurationSec = 1;
          state.lastEvent = 'Демо: автополив включен (влажность ниже порога)';
        } else if (state.valve.state === 'open' && state.soilAverage >= state.settings.thresholds.stop) {
          state.valve.state = 'closed';
          state.valve.wateringDurationSec = 0;
          state.lastEvent = 'Демо: автополив отключен (целевая влажность достигнута)';
        }
      }
    }
  </script>
</body>
</html>
)rawliteral";
