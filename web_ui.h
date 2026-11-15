#pragma once
#include <Arduino.h>

// Auto-generated from web/index.html
// Minimalistic web UI for Garden Watering Assistant
const char INDEX_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>Умный полив — Garden Assistant</title>
  <style>
    :root {
      --bg: #090d16;
      --card-bg: #121826;
      --card-subtle: #192234;
      --border: rgba(255, 255, 255, 0.08);
      --border-active: rgba(16, 185, 129, 0.4);
      --text-main: #f8fafc;
      --text-muted: #94a3b8;
      --text-dim: #64748b;
      --accent: #10b981;
      --accent-glow: rgba(16, 185, 129, 0.25);
      --accent-hover: #059669;
      --water-blue: #0ea5e9;
      --water-glow: rgba(14, 165, 233, 0.3);
      --warning: #f59e0b;
      --warning-glow: rgba(245, 158, 11, 0.2);
      --danger: #ef4444;
      --danger-glow: rgba(239, 68, 68, 0.25);
      --radius-sm: 8px;
      --radius-md: 14px;
      --radius-lg: 20px;
      --radius-full: 9999px;
      --shadow-sm: 0 2px 4px rgba(0, 0, 0, 0.2);
      --shadow-md: 0 8px 24px rgba(0, 0, 0, 0.3);
      --transition: all 0.25s cubic-bezier(0.4, 0, 0.2, 1);
    }

    [data-theme="light"] {
      --bg: #f3f6f9;
      --card-bg: #ffffff;
      --card-subtle: #f8fafc;
      --border: rgba(0, 0, 0, 0.08);
      --border-active: rgba(16, 185, 129, 0.5);
      --text-main: #0f172a;
      --text-muted: #475569;
      --text-dim: #94a3b8;
      --shadow-sm: 0 1px 3px rgba(0, 0, 0, 0.06);
      --shadow-md: 0 10px 25px rgba(0, 0, 0, 0.06);
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      -webkit-tap-highlight-color: transparent;
    }

    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Oxygen, Ubuntu, Cantarell, "Helvetica Neue", sans-serif;
      background-color: var(--bg);
      color: var(--text-main);
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 16px;
      transition: background-color 0.3s ease, color 0.3s ease;
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
      padding: 6px 2px;
    }

    .brand {
      display: flex;
      align-items: center;
      gap: 12px;
    }

    .logo-icon {
      width: 42px;
      height: 42px;
      background: linear-gradient(135deg, #10b981 0%, #059669 100%);
      border-radius: var(--radius-md);
      display: flex;
      align-items: center;
      justify-content: center;
      box-shadow: 0 4px 12px var(--accent-glow);
      color: white;
      font-size: 22px;
    }

    .brand-text h1 {
      font-size: 1.15rem;
      font-weight: 700;
      letter-spacing: -0.02em;
    }

    .brand-text p {
      font-size: 0.75rem;
      color: var(--text-dim);
      font-weight: 500;
    }

    .header-actions {
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .icon-btn {
      background: var(--card-bg);
      border: 1px solid var(--border);
      color: var(--text-muted);
      width: 38px;
      height: 38px;
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
      border-color: var(--border-active);
      transform: translateY(-1px);
    }

    /* Status connection pill */
    .status-pill {
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
      background: #94a3b8;
    }

    .status-dot.online {
      background: var(--accent);
      box-shadow: 0 0 10px var(--accent);
      animation: pulse 2s infinite;
    }

    .status-dot.offline {
      background: var(--danger);
    }

    .status-dot.demo {
      background: #a855f7;
      box-shadow: 0 0 8px #a855f7;
    }

    @keyframes pulse {
      0%, 100% { opacity: 1; transform: scale(1); }
      50% { opacity: 0.5; transform: scale(0.9); }
    }

    /* Cards */
    .card {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: var(--radius-lg);
      padding: 20px;
      box-shadow: var(--shadow-sm);
      transition: var(--transition);
      position: relative;
      overflow: hidden;
    }

    .card-title {
      font-size: 0.85rem;
      font-weight: 600;
      color: var(--text-dim);
      text-transform: uppercase;
      letter-spacing: 0.05em;
      margin-bottom: 14px;
      display: flex;
      align-items: center;
      justify-content: space-between;
    }

    /* Hero Watering Status */
    .hero-card {
      padding: 24px;
      display: flex;
      flex-direction: column;
      align-items: center;
      text-align: center;
      gap: 16px;
      transition: var(--transition);
      background: linear-gradient(180deg, var(--card-bg) 0%, var(--card-subtle) 100%);
    }

    .hero-card.is-watering {
      border-color: var(--water-blue);
      box-shadow: 0 0 25px var(--water-glow);
    }

    .hero-icon-wrap {
      width: 80px;
      height: 80px;
      border-radius: 50%;
      display: flex;
      align-items: center;
      justify-content: center;
      background: var(--card-subtle);
      border: 2px solid var(--border);
      position: relative;
      transition: var(--transition);
    }

    .hero-card.is-watering .hero-icon-wrap {
      background: rgba(14, 165, 233, 0.15);
      border-color: var(--water-blue);
      color: var(--water-blue);
    }

    .ripple-ring {
      position: absolute;
      inset: -8px;
      border-radius: 50%;
      border: 2px solid var(--water-blue);
      opacity: 0;
      pointer-events: none;
    }

    .hero-card.is-watering .ripple-ring {
      animation: ripple 2s infinite cubic-bezier(0, 0.2, 0.8, 1);
    }

    @keyframes ripple {
      0% { transform: scale(0.9); opacity: 0.8; }
      100% { transform: scale(1.4); opacity: 0; }
    }

    .hero-status-title {
      font-size: 1.35rem;
      font-weight: 700;
      letter-spacing: -0.01em;
    }

    .hero-badge {
      display: inline-flex;
      align-items: center;
      gap: 6px;
      padding: 4px 12px;
      border-radius: var(--radius-full);
      font-size: 0.8rem;
      font-weight: 600;
      background: var(--card-subtle);
      border: 1px solid var(--border);
      color: var(--text-muted);
    }

    .hero-badge.active {
      background: rgba(14, 165, 233, 0.15);
      border-color: rgba(14, 165, 233, 0.3);
      color: var(--water-blue);
    }

    /* Mode Selector */
    .mode-toggle {
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
      box-shadow: var(--shadow-sm);
      border: 1px solid var(--border);
    }

    .mode-description {
      font-size: 0.8rem;
      color: var(--text-muted);
      text-align: center;
      line-height: 1.4;
      margin-top: 4px;
    }

    /* Soil Moisture Section */
    .soil-main {
      display: flex;
      flex-direction: column;
      gap: 16px;
    }

    .average-moisture-box {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 16px;
      background: var(--card-subtle);
      border-radius: var(--radius-md);
      border: 1px solid var(--border);
    }

    .moisture-val-group {
      display: flex;
      flex-direction: column;
    }

    .moisture-percent-large {
      font-size: 2.5rem;
      font-weight: 800;
      letter-spacing: -0.03em;
      line-height: 1;
    }

    .moisture-state-chip {
      display: inline-flex;
      align-items: center;
      gap: 4px;
      font-size: 0.75rem;
      font-weight: 700;
      padding: 3px 10px;
      border-radius: var(--radius-full);
      margin-top: 6px;
      width: fit-content;
    }

    .state-dry {
      background: rgba(245, 158, 11, 0.15);
      color: var(--warning);
      border: 1px solid rgba(245, 158, 11, 0.3);
    }

    .state-optimal {
      background: rgba(16, 185, 129, 0.15);
      color: var(--accent);
      border: 1px solid rgba(16, 185, 129, 0.3);
    }

    .state-wet {
      background: rgba(14, 165, 233, 0.15);
      color: var(--water-blue);
      border: 1px solid rgba(14, 165, 233, 0.3);
    }

    /* Soil range scale bar */
    .moisture-scale {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .scale-bar-track {
      height: 10px;
      background: var(--card-subtle);
      border-radius: var(--radius-full);
      overflow: hidden;
      position: relative;
      border: 1px solid var(--border);
    }

    .scale-bar-fill {
      height: 100%;
      width: 0%;
      border-radius: var(--radius-full);
      transition: width 0.6s cubic-bezier(0.4, 0, 0.2, 1), background-color 0.4s ease;
      background: var(--accent);
    }

    .scale-markers {
      display: flex;
      justify-content: space-between;
      font-size: 0.7rem;
      color: var(--text-dim);
      font-weight: 500;
      padding: 0 2px;
    }

    .scale-markers span.active-threshold {
      color: var(--text-muted);
      font-weight: 700;
    }

    /* Individual Sensors Grid */
    .sensors-grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      margin-top: 4px;
    }

    .sensor-item {
      background: var(--card-subtle);
      border: 1px solid var(--border);
      border-radius: var(--radius-md);
      padding: 12px;
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .sensor-label {
      font-size: 0.75rem;
      color: var(--text-dim);
      font-weight: 600;
    }

    .sensor-val {
      font-size: 1.25rem;
      font-weight: 700;
    }

    .sensor-adc {
      font-size: 0.7rem;
      color: var(--text-dim);
    }

    .mini-progress {
      height: 4px;
      background: rgba(255, 255, 255, 0.05);
      border-radius: 2px;
      overflow: hidden;
      margin-top: 2px;
    }

    .mini-fill {
      height: 100%;
      width: 0%;
      background: var(--accent);
      border-radius: 2px;
      transition: width 0.4s ease;
    }

    /* Climate Grid */
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

    .climate-icon-wrap {
      width: 44px;
      height: 44px;
      border-radius: 12px;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 20px;
    }

    .temp-icon {
      background: rgba(245, 158, 11, 0.12);
      color: var(--warning);
    }

    .hum-icon {
      background: rgba(14, 165, 233, 0.12);
      color: var(--water-blue);
    }

    .climate-info p {
      font-size: 0.75rem;
      color: var(--text-dim);
      font-weight: 500;
    }

    .climate-info h3 {
      font-size: 1.35rem;
      font-weight: 700;
      letter-spacing: -0.02em;
    }

    /* Manual Control Actions */
    .control-section {
      display: flex;
      flex-direction: column;
      gap: 14px;
    }

    .duration-chips {
      display: flex;
      gap: 8px;
      overflow-x: auto;
      padding-bottom: 2px;
      scrollbar-width: none;
    }

    .duration-chips::-webkit-scrollbar {
      display: none;
    }

    .chip {
      background: var(--card-subtle);
      border: 1px solid var(--border);
      color: var(--text-muted);
      padding: 8px 14px;
      border-radius: var(--radius-full);
      font-size: 0.8rem;
      font-weight: 600;
      cursor: pointer;
      white-space: nowrap;
      transition: var(--transition);
    }

    .chip.active {
      background: var(--card-bg);
      border-color: var(--accent);
      color: var(--accent);
      box-shadow: 0 0 12px var(--accent-glow);
    }

    .slider-row {
      display: flex;
      align-items: center;
      gap: 12px;
    }

    .slider-row input[type="range"] {
      flex: 1;
      height: 6px;
      border-radius: 3px;
      accent-color: var(--accent);
      background: var(--card-subtle);
      cursor: pointer;
    }

    .duration-display {
      min-width: 60px;
      font-size: 0.85rem;
      font-weight: 700;
      color: var(--text-main);
      text-align: right;
    }

    .action-btn {
      width: 100%;
      padding: 15px;
      border: none;
      border-radius: var(--radius-md);
      font-size: 1rem;
      font-weight: 700;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 10px;
      transition: var(--transition);
      box-shadow: var(--shadow-sm);
    }

    .btn-start {
      background: linear-gradient(135deg, #10b981 0%, #059669 100%);
      color: white;
      box-shadow: 0 4px 14px var(--accent-glow);
    }

    .btn-start:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px var(--accent-glow);
    }

    .btn-stop {
      background: linear-gradient(135deg, #ef4444 0%, #dc2626 100%);
      color: white;
      box-shadow: 0 4px 14px var(--danger-glow);
    }

    .btn-stop:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px var(--danger-glow);
    }

    .action-btn:active {
      transform: translateY(0);
    }

    /* Event log & safety footer */
    .system-info-list {
      display: flex;
      flex-direction: column;
      gap: 10px;
      font-size: 0.8rem;
    }

    .info-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 6px 0;
      border-bottom: 1px solid var(--border);
    }

    .info-row:last-child {
      border-bottom: none;
    }

    .info-key {
      color: var(--text-dim);
      font-weight: 500;
      display: flex;
      align-items: center;
      gap: 6px;
    }

    .info-val {
      color: var(--text-muted);
      font-weight: 600;
      max-width: 60%;
      text-align: right;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
    }

    /* Modal */
    .modal-backdrop {
      position: fixed;
      inset: 0;
      background: rgba(0, 0, 0, 0.65);
      backdrop-filter: blur(4px);
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
      z-index: 100;
      opacity: 0;
      pointer-events: none;
      transition: opacity 0.25s ease;
    }

    .modal-backdrop.open {
      opacity: 1;
      pointer-events: auto;
    }

    .modal {
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: var(--radius-lg);
      padding: 24px;
      width: 100%;
      max-width: 440px;
      box-shadow: var(--shadow-md);
      display: flex;
      flex-direction: column;
      gap: 16px;
      transform: translateY(12px);
      transition: transform 0.25s ease;
    }

    .modal-backdrop.open .modal {
      transform: translateY(0);
    }

    .modal-header {
      display: flex;
      align-items: center;
      justify-content: space-between;
    }

    .modal-header h3 {
      font-size: 1.1rem;
      font-weight: 700;
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

    .toast-container {
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

    .toast {
      background: var(--card-bg);
      color: var(--text-main);
      border: 1px solid var(--border);
      border-radius: var(--radius-full);
      padding: 10px 20px;
      font-size: 0.85rem;
      font-weight: 600;
      box-shadow: var(--shadow-md);
      display: flex;
      align-items: center;
      gap: 8px;
      animation: toastIn 0.3s ease, toastOut 0.3s ease 2.7s forwards;
    }

    @keyframes toastIn {
      from { opacity: 0; transform: translateY(16px); }
      to { opacity: 1; transform: translateY(0); }
    }

    @keyframes toastOut {
      from { opacity: 1; transform: translateY(0); }
      to { opacity: 0; transform: translateY(16px); }
    }

    /* Sensor fault banner */
    .alert-banner {
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
        <div class="logo-icon">🌱</div>
        <div class="brand-text">
          <h1>Умный полив</h1>
          <p>ESP32-C3 SuperMini</p>
        </div>
      </div>
      <div class="header-actions">
        <div class="status-pill" id="statusPill" title="Статус подключения">
          <div class="status-dot offline" id="statusDot"></div>
          <span id="statusText">Офлайн</span>
        </div>
        <button class="icon-btn" id="themeBtn" title="Сменить тему">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <circle cx="12" cy="12" r="5"></circle>
            <line x1="12" y1="1" x2="12" y2="3"></line>
            <line x1="12" y1="21" x2="12" y2="23"></line>
            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line>
            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line>
            <line x1="1" y1="12" x2="3" y2="12"></line>
            <line x1="21" y1="12" x2="23" y2="12"></line>
            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line>
            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line>
          </svg>
        </button>
        <button class="icon-btn" id="settingsBtn" title="Настройки">
          <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
            <circle cx="12" cy="12" r="3"></circle>
            <path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path>
          </svg>
        </button>
      </div>
    </header>

    <!-- Fault Banner (hidden by default) -->
    <div class="alert-banner" id="sensorFaultBanner" style="display: none;">
      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
        <path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"></path>
        <line x1="12" y1="9" x2="12" y2="13"></line>
        <line x1="12" y1="17" x2="12.01" y2="17"></line>
      </svg>
      <div>Сбой датчиков почвы! Проверьте подключение HW-390 (GPIO 4, 5, 6).</div>
    </div>

    <!-- Hero Card: Watering Status -->
    <div class="card hero-card" id="heroCard">
      <div class="hero-icon-wrap" id="heroIconWrap">
        <div class="ripple-ring"></div>
        <svg id="heroWaterIcon" width="38" height="38" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
          <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path>
        </svg>
      </div>
      <div>
        <div class="hero-status-title" id="wateringTitle">Полив выключен</div>
        <div style="margin-top: 6px;">
          <span class="hero-badge" id="wateringModeBadge">Автоматический режим</span>
        </div>
      </div>
      <div class="mode-toggle">
        <button class="mode-btn active" id="btnModeAuto" onclick="setSystemMode('auto')">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="23 4 23 10 17 10"></polyline><polyline points="1 20 1 14 7 14"></polyline><path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"></path></svg>
          Авто
        </button>
        <button class="mode-btn" id="btnModeManual" onclick="setSystemMode('manual')">
          <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M18 11V6a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v0"></path><path d="M14 10V4a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v6"></path><path d="M10 10.5V6a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v8"></path><path d="M18 8a2 2 0 1 1 4 0v6a8 8 0 0 1-8 8h-2c-2.8 0-4.5-.86-5.99-2.34l-3.6-3.6a2 2 0 0 1 2.83-2.82L7 15"></path></svg>
          Ручной
        </button>
      </div>
      <div class="mode-description" id="modeDescription">
        Автополив включается при влажности ≤ 35% и отключается при ≥ 55%.
      </div>
    </div>

    <!-- Soil Moisture Card -->
    <div class="card">
      <div class="card-title">
        <span>Влажность почвы (HW-390)</span>
        <span id="soilAvgBadge" class="moisture-state-chip state-optimal">Оптимально</span>
      </div>
      <div class="soil-main">
        <div class="average-moisture-box">
          <div class="moisture-val-group">
            <span class="sensor-label">Средняя влажность</span>
            <div class="moisture-percent-large" id="avgMoistureVal">0%</div>
          </div>
          <div style="text-align: right;">
            <div class="sensor-label">Пороги автополива</div>
            <div style="font-size: 0.85rem; font-weight: 700; margin-top: 4px; color: var(--text-muted);">
              35% 💧 55%
            </div>
          </div>
        </div>

        <div class="moisture-scale">
          <div class="scale-bar-track">
            <div class="scale-bar-fill" id="avgMoistureFill"></div>
          </div>
          <div class="scale-markers">
            <span>0% (Сухо)</span>
            <span class="active-threshold">35% Старт</span>
            <span class="active-threshold">55% Стоп</span>
            <span>100% (Влажно)</span>
          </div>
        </div>

        <!-- 3 Sensors Grid -->
        <div class="sensors-grid">
          <div class="sensor-item">
            <span class="sensor-label">Датчик 1 (G4)</span>
            <div class="sensor-val" id="soil1Percent">--%</div>
            <div class="sensor-adc" id="soil1Adc">ADC: ----</div>
            <div class="mini-progress"><div class="mini-fill" id="soil1Bar"></div></div>
          </div>
          <div class="sensor-item">
            <span class="sensor-label">Датчик 2 (G5)</span>
            <div class="sensor-val" id="soil2Percent">--%</div>
            <div class="sensor-adc" id="soil2Adc">ADC: ----</div>
            <div class="mini-progress"><div class="mini-fill" id="soil2Bar"></div></div>
          </div>
          <div class="sensor-item">
            <span class="sensor-label">Датчик 3 (G6)</span>
            <div class="sensor-val" id="soil3Percent">--%</div>
            <div class="sensor-adc" id="soil3Adc">ADC: ----</div>
            <div class="mini-progress"><div class="mini-fill" id="soil3Bar"></div></div>
          </div>
        </div>
      </div>
    </div>

    <!-- Microclimate Card (AHT10) -->
    <div class="card">
      <div class="card-title">
        <span>Микроклимат (AHT10)</span>
        <span id="ahtStatusChip" style="font-size: 0.75rem; color: var(--accent); font-weight: 600;">I2C активен</span>
      </div>
      <div class="climate-grid">
        <div class="climate-box">
          <div class="climate-icon-wrap temp-icon">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"></path>
            </svg>
          </div>
          <div class="climate-info">
            <p>Температура</p>
            <h3 id="tempVal">-- °C</h3>
          </div>
        </div>
        <div class="climate-box">
          <div class="climate-icon-wrap hum-icon">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M12 2.69l5.66 5.66a8 8 0 1 1-11.31 0z"></path>
            </svg>
          </div>
          <div class="climate-info">
            <p>Влажность воздуха</p>
            <h3 id="humVal">-- %</h3>
          </div>
        </div>
      </div>
    </div>

    <!-- Manual Control Card -->
    <div class="card">
      <div class="card-title">
        <span>Управление клапаном</span>
        <span id="valveRelayState" style="font-size: 0.75rem; color: var(--text-dim);">433 МГц OOK</span>
      </div>
      <div class="control-section">
        <div style="font-size: 0.8rem; color: var(--text-muted); font-weight: 500;">
          Длительность полива:
        </div>
        <div class="duration-chips">
          <button class="chip" onclick="setDurationPreset(15)">15 сек</button>
          <button class="chip active" onclick="setDurationPreset(30)">30 сек</button>
          <button class="chip" onclick="setDurationPreset(60)">1 мин</button>
          <button class="chip" onclick="setDurationPreset(120)">2 мин</button>
          <button class="chip" onclick="setDurationPreset(300)">5 мин</button>
          <button class="chip" onclick="setDurationPreset(600)">10 мин</button>
        </div>

        <div class="slider-row">
          <input type="range" id="durationSlider" min="5" max="600" step="5" value="30" oninput="onDurationSliderChange(this.value)">
          <div class="duration-display" id="durationDisplay">30 сек</div>
        </div>

        <button class="action-btn btn-start" id="btnAction" onclick="handleActionButtonClick()">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
            <polygon points="5 3 19 12 5 21 5 3"></polygon>
          </svg>
          <span id="btnActionText">Запустить полив</span>
        </button>
      </div>
    </div>

    <!-- System Info & Safety -->
    <div class="card">
      <div class="card-title">
        <span>Статус системы</span>
      </div>
      <div class="system-info-list">
        <div class="info-row">
          <span class="info-key">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"></circle><polyline points="12 6 12 12 14 14"></polyline></svg>
            Последнее событие:
          </span>
          <span class="info-val" id="lastEventText">Загрузка...</span>
        </div>
        <div class="info-row">
          <span class="info-key">⏱️ Защита по времени:</span>
          <span class="info-val">Максимум 10 мин</span>
        </div>
        <div class="info-row">
          <span class="info-key">🛡️ Защита от сбоя:</span>
          <span class="info-val">3 плохих замера</span>
        </div>
        <div class="info-row">
          <span class="info-key">🔌 Реле клапана:</span>
          <span class="info-val">CR-NAC100 (220V NC)</span>
        </div>
      </div>
    </div>
  </div>

  <!-- Settings Modal -->
  <div class="modal-backdrop" id="settingsModal">
    <div class="modal">
      <div class="modal-header">
        <h3>Настройки</h3>
        <button class="icon-btn" onclick="toggleSettingsModal(false)">✕</button>
      </div>

      <div class="form-group">
        <label for="espHostInput">Адрес ESP32 (IP или домен):</label>
        <input type="text" id="espHostInput" class="form-input" placeholder="http://192.168.1.120 или оставьте пустым">
        <span style="font-size: 0.7rem; color: var(--text-dim);">Оставьте пустым, если страница загружена напрямую с ESP32.</span>
      </div>

      <div class="form-group">
        <label for="pollIntervalInput">Интервал опроса:</label>
        <select id="pollIntervalInput" class="form-input">
          <option value="1000">1 секунда</option>
          <option value="2000" selected>2 секунды</option>
          <option value="5000">5 секунд</option>
        </select>
      </div>

      <div class="form-group">
        <label style="display: flex; align-items: center; gap: 8px; cursor: pointer;">
          <input type="checkbox" id="demoModeCheckbox" onchange="toggleDemoMode(this.checked)">
          <span>Демо-режим (эмуляция датчиков)</span>
        </label>
        <span style="font-size: 0.7rem; color: var(--text-dim);">Используется для проверки интерфейса без подключенного контроллера.</span>
      </div>

      <button class="action-btn btn-start" onclick="saveSettings()">Сохранить настройки</button>
    </div>
  </div>

  <div class="toast-container" id="toastContainer"></div>

  <script>
    // System State
    const state = {
      mode: 'auto',
      watering: false,
      soilRaw: [0, 0, 0],
      soilPercent: [0, 0, 0],
      soilAverage: 0,
      temperature: null,
      humidity: null,
      sensorValid: true,
      lastEvent: 'Подключение...',
      espHost: localStorage.getItem('garden_esp_host') || '',
      pollInterval: parseInt(localStorage.getItem('garden_poll_interval') || '2000', 10),
      isDemo: localStorage.getItem('garden_demo_mode') === 'true',
      wateringSeconds: 30,
      theme: localStorage.getItem('garden_theme') || 'dark',
      manualStopTimeout: null,
      wateringStartTime: null,
      targetDuration: 30,
      countdownTimer: null
    };

    let pollTimer = null;

    // Initialize UI
    window.addEventListener('DOMContentLoaded', () => {
      initTheme();
      document.getElementById('espHostInput').value = state.espHost;
      document.getElementById('pollIntervalInput').value = state.pollInterval;
      document.getElementById('demoModeCheckbox').checked = state.isDemo;

      // Event listener for settings toggle
      document.getElementById('settingsBtn').addEventListener('click', () => toggleSettingsModal(true));
      document.getElementById('themeBtn').addEventListener('click', toggleTheme);

      // Start Polling or Demo
      startPolling();
    });

    // Theme Management
    function initTheme() {
      document.documentElement.setAttribute('data-theme', state.theme);
    }

    function toggleTheme() {
      state.theme = state.theme === 'dark' ? 'light' : 'dark';
      document.documentElement.setAttribute('data-theme', state.theme);
      localStorage.setItem('garden_theme', state.theme);
      showToast(`Тема изменена: ${state.theme === 'dark' ? 'Темная' : 'Светлая'}`);
    }

    // Modal
    function toggleSettingsModal(open) {
      const modal = document.getElementById('settingsModal');
      if (open) modal.classList.add('open');
      else modal.classList.remove('open');
    }

    function saveSettings() {
      let host = document.getElementById('espHostInput').value.trim();
      if (host && !host.startsWith('http://') && !host.startsWith('https://')) {
        host = 'http://' + host;
      }
      if (host.endsWith('/')) {
        host = host.slice(0, -1);
      }
      state.espHost = host;
      localStorage.setItem('garden_esp_host', host);

      state.pollInterval = parseInt(document.getElementById('pollIntervalInput').value, 10);
      localStorage.setItem('garden_poll_interval', state.pollInterval);

      toggleSettingsModal(false);
      showToast('Настройки сохранены');
      startPolling();
    }

    function toggleDemoMode(enabled) {
      state.isDemo = enabled;
      localStorage.setItem('garden_demo_mode', enabled ? 'true' : 'false');
      if (enabled) {
        showToast('Демо-режим включен');
      } else {
        showToast('Демо-режим выключен');
      }
      startPolling();
    }

    // Toast Notifications
    function showToast(message) {
      const container = document.getElementById('toastContainer');
      const toast = document.createElement('div');
      toast.className = 'toast';
      toast.innerHTML = message;
      container.appendChild(toast);
      setTimeout(() => {
        if (toast.parentNode) toast.remove();
      }, 3000);
    }

    // Polling & API
    function getApiUrl(path) {
      const base = state.espHost || window.location.origin;
      return `${base}${path}`;
    }

    function startPolling() {
      if (pollTimer) clearInterval(pollTimer);
      fetchStatus();
      pollTimer = setInterval(fetchStatus, state.pollInterval);
    }

    async function fetchStatus() {
      if (state.isDemo) {
        runDemoSimulation();
        updateUI();
        return;
      }

      try {
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 4000);
        const res = await fetch(getApiUrl('/status'), {
          signal: controller.signal,
          headers: { 'Accept': 'application/json' }
        });
        clearTimeout(timeoutId);

        if (!res.ok) throw new Error(`HTTP ${res.status}`);
        const data = await res.json();

        // Update state
        state.mode = data.mode || 'auto';
        state.watering = !!data.watering;
        state.soilRaw = data.soilRaw || [0, 0, 0];
        state.soilPercent = data.soilPercent || [0, 0, 0];
        state.soilAverage = data.soilAverage !== undefined ? data.soilAverage : 0;
        state.temperature = isNaN(data.temperature) ? null : data.temperature;
        state.humidity = isNaN(data.humidity) ? null : data.humidity;
        state.sensorValid = data.sensorValid !== undefined ? data.sensorValid : true;
        state.lastEvent = data.lastEvent || 'Готово';

        setConnectionStatus('online', 'Онлайн');
        updateUI();
      } catch (err) {
        setConnectionStatus('offline', 'Офлайн');
        // If connection fails and not in demo mode, update event text
        if (!state.isDemo) {
          document.getElementById('lastEventText').innerText = 'Нет связи с ESP32';
        }
      }
    }

    function setConnectionStatus(type, text) {
      const dot = document.getElementById('statusDot');
      const label = document.getElementById('statusText');
      dot.className = 'status-dot ' + (state.isDemo ? 'demo' : type);
      label.innerText = state.isDemo ? 'Демо-режим' : text;
    }

    // UI Updates
    function updateUI() {
      // 1. Watering & Hero
      const heroCard = document.getElementById('heroCard');
      const heroIconWrap = document.getElementById('heroIconWrap');
      const wateringTitle = document.getElementById('wateringTitle');
      const wateringModeBadge = document.getElementById('wateringModeBadge');
      const btnAction = document.getElementById('btnAction');
      const btnActionText = document.getElementById('btnActionText');

      if (state.watering) {
        heroCard.classList.add('is-watering');
        wateringTitle.innerText = 'Идёт полив сада...';
        wateringTitle.style.color = 'var(--water-blue)';
        wateringModeBadge.className = 'hero-badge active';
        wateringModeBadge.innerText = '💧 Клапан открыт';

        btnAction.className = 'action-btn btn-stop';
        btnActionText.innerText = 'Остановить полив';
      } else {
        heroCard.classList.remove('is-watering');
        wateringTitle.innerText = 'Полив выключен';
        wateringTitle.style.color = 'var(--text-main)';
        wateringModeBadge.className = 'hero-badge';
        wateringModeBadge.innerText = state.mode === 'auto' ? 'Автоматический режим' : 'Ручной режим';

        btnAction.className = 'action-btn btn-start';
        btnActionText.innerText = 'Запустить полив';
      }

      // 2. Mode buttons
      const btnAuto = document.getElementById('btnModeAuto');
      const btnManual = document.getElementById('btnModeManual');
      const modeDesc = document.getElementById('modeDescription');

      if (state.mode === 'auto') {
        btnAuto.classList.add('active');
        btnManual.classList.remove('active');
        modeDesc.innerText = 'Автополив включается при влажности ≤ 35% и отключается при ≥ 55%.';
      } else {
        btnManual.classList.add('active');
        btnAuto.classList.remove('active');
        modeDesc.innerText = 'Ручной режим: клапан включается по кнопке на заданное время (макс. 10 мин).';
      }

      // 3. Soil Moisture
      const avgVal = Math.round(state.soilAverage);
      const avgValEl = document.getElementById('avgMoistureVal');
      const avgFill = document.getElementById('avgMoistureFill');
      const avgBadge = document.getElementById('soilAvgBadge');

      avgValEl.innerText = `${avgVal}%`;
      avgFill.style.width = `${Math.min(100, Math.max(0, avgVal))}%`;

      if (avgVal <= 35) {
        avgBadge.className = 'moisture-state-chip state-dry';
        avgBadge.innerText = 'Сухо (<35%)';
        avgFill.style.backgroundColor = 'var(--warning)';
      } else if (avgVal >= 55) {
        avgBadge.className = 'moisture-state-chip state-wet';
        avgBadge.innerText = 'Влажно (>55%)';
        avgFill.style.backgroundColor = 'var(--water-blue)';
      } else {
        avgBadge.className = 'moisture-state-chip state-optimal';
        avgBadge.innerText = 'Оптимально';
        avgFill.style.backgroundColor = 'var(--accent)';
      }

      // 4. Individual Sensors
      for (let i = 0; i < 3; i++) {
        const p = state.soilPercent[i] !== undefined ? state.soilPercent[i] : 0;
        const raw = state.soilRaw[i] !== undefined ? state.soilRaw[i] : 0;
        const pEl = document.getElementById(`soil${i + 1}Percent`);
        const adcEl = document.getElementById(`soil${i + 1}Adc`);
        const barEl = document.getElementById(`soil${i + 1}Bar`);

        if (pEl) pEl.innerText = `${p}%`;
        if (adcEl) adcEl.innerText = `ADC: ${raw}`;
        if (barEl) {
          barEl.style.width = `${Math.min(100, Math.max(0, p))}%`;
          barEl.style.backgroundColor = p <= 35 ? 'var(--warning)' : (p >= 55 ? 'var(--water-blue)' : 'var(--accent)');
        }
      }

      // 5. Fault Banner
      const faultBanner = document.getElementById('sensorFaultBanner');
      faultBanner.style.display = state.sensorValid ? 'none' : 'flex';

      // 6. Climate
      const tempEl = document.getElementById('tempVal');
      const humEl = document.getElementById('humVal');
      const ahtChip = document.getElementById('ahtStatusChip');

      if (state.temperature !== null && !isNaN(state.temperature)) {
        tempEl.innerText = `${state.temperature.toFixed(1)} °C`;
        humEl.innerText = `${state.humidity.toFixed(1)} %`;
        ahtChip.innerText = 'I2C активен';
        ahtChip.style.color = 'var(--accent)';
      } else {
        tempEl.innerText = '-- °C';
        humEl.innerText = '-- %';
        ahtChip.innerText = 'Не подключен';
        ahtChip.style.color = 'var(--text-dim)';
      }

      // 7. System Info
      document.getElementById('lastEventText').innerText = state.lastEvent || '—';
    }

    // Actions & Handlers
    async function setSystemMode(mode) {
      if (state.isDemo) {
        state.mode = mode;
        state.lastEvent = `Режим изменён на ${mode === 'auto' ? 'автоматический' : 'ручной'}`;
        if (mode === 'auto' && state.watering) {
          state.watering = false;
        }
        updateUI();
        showToast(`Включен ${mode === 'auto' ? 'автоматический' : 'ручной'} режим`);
        return;
      }

      try {
        const res = await fetch(getApiUrl(`/mode?value=${mode}`));
        if (!res.ok) throw new Error(`Ошибка: ${res.statusText}`);
        const data = await res.json();
        state.mode = data.mode;
        state.watering = data.watering;
        state.lastEvent = data.lastEvent;
        updateUI();
        showToast(`Режим изменён: ${mode === 'auto' ? 'Автоматический' : 'Ручной'}`);
      } catch (err) {
        showToast(`Ошибка смены режима: ${err.message}`);
      }
    }

    function setDurationPreset(seconds) {
      state.wateringSeconds = seconds;
      document.getElementById('durationSlider').value = seconds;
      updateDurationDisplay(seconds);

      const chips = document.querySelectorAll('.duration-chips .chip');
      chips.forEach(chip => chip.classList.remove('active'));
      const activeChip = Array.from(chips).find(c => c.getAttribute('onclick')?.includes(`(${seconds})`));
      if (activeChip) activeChip.classList.add('active');
    }

    function onDurationSliderChange(val) {
      const seconds = parseInt(val, 10);
      state.wateringSeconds = seconds;
      updateDurationDisplay(seconds);

      const chips = document.querySelectorAll('.duration-chips .chip');
      chips.forEach(chip => chip.classList.remove('active'));
      const matchingChip = Array.from(chips).find(c => c.getAttribute('onclick')?.includes(`(${seconds})`));
      if (matchingChip) matchingChip.classList.add('active');
    }

    function updateDurationDisplay(sec) {
      const display = document.getElementById('durationDisplay');
      if (sec < 60) {
        display.innerText = `${sec} сек`;
      } else {
        const m = Math.floor(sec / 60);
        const s = sec % 60;
        display.innerText = s > 0 ? `${m}м ${s}с` : `${m} мин`;
      }
    }

    async function handleActionButtonClick() {
      if (state.watering) {
        // Stop watering
        stopWatering();
      } else {
        // Start watering
        if (state.mode === 'auto') {
          // Confirm switch to manual mode
          showToast('Переключение в ручной режим для полива...');
          await setSystemMode('manual');
        }
        startWatering(state.wateringSeconds);
      }
    }

    async function startWatering(seconds) {
      if (state.isDemo) {
        state.watering = true;
        state.targetDuration = seconds;
        state.wateringStartTime = Date.now();
        state.lastEvent = `Ручной полив запущен на ${seconds} сек`;
        updateUI();
        showToast(`Полив запущен на ${seconds} сек`);

        clearTimeout(state.manualStopTimeout);
        state.manualStopTimeout = setTimeout(() => {
          if (state.watering) {
            state.watering = false;
            state.lastEvent = 'Время полива истекло';
            updateUI();
            showToast('Полив завершён');
          }
        }, seconds * 1000);
        return;
      }

      try {
        const res = await fetch(getApiUrl(`/water?seconds=${seconds}`));
        if (!res.ok) {
          const msg = await res.text();
          throw new Error(msg || res.statusText);
        }
        const data = await res.json();
        state.watering = data.watering;
        state.lastEvent = data.lastEvent;
        updateUI();
        showToast(`Полив запущен на ${seconds} сек`);
      } catch (err) {
        showToast(`Не удалось запустить полив: ${err.message}`);
      }
    }

    async function stopWatering() {
      if (state.isDemo) {
        state.watering = false;
        clearTimeout(state.manualStopTimeout);
        state.lastEvent = 'Полив остановлен пользователем';
        updateUI();
        showToast('Полив остановлен');
        return;
      }

      try {
        const res = await fetch(getApiUrl('/stop'));
        if (!res.ok) throw new Error(`Ошибка: ${res.statusText}`);
        const data = await res.json();
        state.watering = data.watering;
        state.lastEvent = data.lastEvent;
        updateUI();
        showToast('Полив остановлен');
      } catch (err) {
        showToast(`Ошибка остановки полива: ${err.message}`);
      }
    }

    // Demo Mode Simulation
    function runDemoSimulation() {
      if (!state.isDemo) return;

      // Realistic soil values fluctuation
      if (state.soilRaw[0] === 0) {
        // Init demo
        state.soilPercent = [42, 40, 44];
        state.soilRaw = [2100, 2150, 2050];
        state.temperature = 23.6;
        state.humidity = 58.4;
      } else {
        // If watering, moisture goes up slowly
        if (state.watering) {
          for (let i = 0; i < 3; i++) {
            if (state.soilPercent[i] < 85) {
              state.soilPercent[i] += 1;
              state.soilRaw[i] -= 20;
            }
          }
        } else {
          // If not watering, slowly dries or stays near
          if (Math.random() > 0.6) {
            for (let i = 0; i < 3; i++) {
              state.soilPercent[i] = Math.max(20, state.soilPercent[i] + (Math.random() > 0.5 ? 0 : -1));
            }
          }
        }

        // Slight microclimate jitter
        state.temperature = parseFloat((23.5 + (Math.random() * 0.4 - 0.2)).toFixed(1));
        state.humidity = parseFloat((58.0 + (Math.random() * 0.8 - 0.4)).toFixed(1));
      }

      state.soilAverage = Math.round(
        (state.soilPercent[0] + state.soilPercent[1] + state.soilPercent[2]) / 3
      );

      // Auto mode triggers in demo
      if (state.mode === 'auto') {
        if (!state.watering && state.soilAverage <= 35) {
          state.watering = true;
          state.lastEvent = 'Автополив: почва сухая (<= 35%)';
        } else if (state.watering && state.soilAverage >= 55) {
          state.watering = false;
          state.lastEvent = 'Автополив: почва увлажнена (>= 55%)';
        }
      }
    }
  </script>
</body>
</html>
)rawliteral";
