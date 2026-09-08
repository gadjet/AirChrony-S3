#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <Arduino.h>

const char PAGE_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>AirChrony S3 Precision Chronograph</title>
  <style>
    :root {
      --bg-base: #0a0f1d;
      --bg-card: rgba(18, 26, 47, 0.85);
      --bg-card-border: rgba(56, 189, 248, 0.18);
      --text-main: #f1f5f9;
      --text-muted: #94a3b8;
      --accent-cyan: #00f2fe;
      --accent-blue: #4facfe;
      --accent-emerald: #10b981;
      --accent-amber: #f59e0b;
      --accent-rose: #f43f5e;
      --accent-purple: #a855f7;
      --glow-cyan: 0 0 20px rgba(0, 242, 254, 0.35);
      --glow-emerald: 0 0 20px rgba(16, 185, 129, 0.35);
      --font-mono: 'SF Mono', Consolas, 'Liberation Mono', Menlo, Courier, monospace;
      --font-sans: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    }

    /* Light Theme Overrides */
    body.light-theme {
      --bg-base: #f1f5f9;
      --bg-card: rgba(255, 255, 255, 0.95);
      --bg-card-border: rgba(203, 213, 225, 0.9);
      --text-main: #0f172a;
      --text-muted: #64748b;
      --accent-cyan: #0284c7;
      --accent-blue: #2563eb;
      --accent-emerald: #059669;
      --accent-amber: #d97706;
      --accent-rose: #e11d48;
      --accent-purple: #7c3aed;
      --glow-cyan: 0 0 15px rgba(2, 132, 199, 0.2);
      --glow-emerald: 0 0 15px rgba(5, 150, 105, 0.2);
      background: #f1f5f9;
      background-image: 
        radial-gradient(circle at 10% 20%, rgba(2, 132, 199, 0.06) 0%, transparent 40%),
        radial-gradient(circle at 90% 80%, rgba(124, 58, 237, 0.06) 0%, transparent 40%);
    }

    * { box-sizing: border-box; margin: 0; padding: 0; }
    
    body {
      background: var(--bg-base);
      background-image: 
        radial-gradient(circle at 10% 20%, rgba(14, 165, 233, 0.08) 0%, transparent 40%),
        radial-gradient(circle at 90% 80%, rgba(168, 85, 247, 0.08) 0%, transparent 40%);
      color: var(--text-main);
      font-family: var(--font-sans);
      min-height: 100vh;
      padding-bottom: 30px;
      transition: background-color 0.3s ease, color 0.3s ease;
    }

    .container {
      max-width: 1100px;
      margin: 0 auto;
      padding: 16px;
    }

    /* Header */
    header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 12px 18px;
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      border-radius: 16px;
      backdrop-filter: blur(12px);
      margin-bottom: 16px;
      box-shadow: 0 8px 32px rgba(0,0,0,0.12);
    }

    .logo-group {
      display: flex;
      align-items: center;
      gap: 12px;
    }

    .logo-icon {
      width: 34px;
      height: 34px;
      background: linear-gradient(135deg, var(--accent-cyan), var(--accent-blue));
      border-radius: 10px;
      display: flex;
      align-items: center;
      justify-content: center;
      color: #fff;
      font-weight: 900;
      font-size: 17px;
      box-shadow: var(--glow-cyan);
    }

    .logo-title {
      font-size: 19px;
      font-weight: 800;
      letter-spacing: -0.5px;
      background: linear-gradient(90deg, #ffffff, #94a3b8);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    body.light-theme .logo-title {
      background: linear-gradient(90deg, #0f172a, #334155);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
    }

    .header-actions {
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }

    .status-pill {
      display: flex;
      align-items: center;
      gap: 8px;
      font-size: 12px;
      padding: 6px 14px;
      border-radius: 20px;
      background: rgba(16, 185, 129, 0.12);
      border: 1px solid rgba(16, 185, 129, 0.3);
      color: var(--accent-emerald);
      font-weight: 600;
    }

    body.light-theme .status-pill {
      background: rgba(5, 150, 105, 0.1);
      border-color: rgba(5, 150, 105, 0.3);
      color: #059669;
    }

    .pulse-dot {
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: var(--accent-emerald);
      box-shadow: var(--glow-emerald);
      animation: pulse 2s infinite;
    }

    @keyframes pulse {
      0%, 100% { opacity: 1; transform: scale(1); }
      50% { opacity: 0.4; transform: scale(0.8); }
    }

    /* Tabs */
    .tabs-nav {
      display: flex;
      gap: 8px;
      margin-bottom: 16px;
      overflow-x: auto;
      padding-bottom: 4px;
    }

    .tab-btn {
      background: rgba(30, 41, 59, 0.6);
      border: 1px solid rgba(255, 255, 255, 0.08);
      color: var(--text-muted);
      padding: 10px 18px;
      border-radius: 12px;
      font-size: 14px;
      font-weight: 600;
      cursor: pointer;
      display: flex;
      align-items: center;
      gap: 8px;
      transition: all 0.2s ease;
      white-space: nowrap;
    }

    body.light-theme .tab-btn {
      background: rgba(255, 255, 255, 0.85);
      border-color: rgba(203, 213, 225, 0.9);
      color: #475569;
    }

    .tab-btn.active, .tab-btn:hover {
      background: linear-gradient(135deg, rgba(0, 242, 254, 0.15), rgba(79, 172, 254, 0.15));
      border-color: var(--accent-cyan);
      color: var(--accent-cyan);
      box-shadow: 0 0 15px rgba(0, 242, 254, 0.15);
    }

    body.light-theme .tab-btn.active, body.light-theme .tab-btn:hover {
      background: linear-gradient(135deg, rgba(2, 132, 199, 0.12), rgba(37, 99, 235, 0.12));
      border-color: #0284c7;
      color: #0284c7;
    }

    .tab-content { display: none; }
    .tab-content.active { display: block; }

    /* Live HUD Cards - Main Focus at top */
    .hud-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(260px, 1fr));
      gap: 16px;
      margin-bottom: 16px;
    }

    .hud-card {
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      border-radius: 18px;
      padding: 20px 22px;
      position: relative;
      overflow: hidden;
      box-shadow: 0 8px 24px rgba(0, 0, 0, 0.08);
      transition: all 0.2s ease;
    }

    .hud-card::before {
      content: '';
      position: absolute;
      top: 0; left: 0; right: 0;
      height: 3px;
      background: linear-gradient(90deg, var(--accent-cyan), var(--accent-blue));
    }

    .hud-card.energy-card::before {
      background: linear-gradient(90deg, var(--accent-amber), var(--accent-rose));
    }

    .hud-card.avg-card::before {
      background: linear-gradient(90deg, var(--accent-emerald), var(--accent-cyan));
    }

    .hud-label {
      font-size: 11px;
      text-transform: uppercase;
      letter-spacing: 1.5px;
      color: var(--text-muted);
      font-weight: 700;
      margin-bottom: 8px;
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .hud-value-group {
      display: flex;
      align-items: baseline;
      gap: 8px;
      margin-bottom: 6px;
    }

    .hud-big-value {
      font-size: 48px;
      font-family: var(--font-mono);
      font-weight: 800;
      letter-spacing: -1px;
      color: #fff;
      text-shadow: 0 0 20px rgba(255, 255, 255, 0.15);
    }

    body.light-theme .hud-big-value {
      color: #0f172a;
      text-shadow: none;
    }

    .hud-unit {
      font-size: 17px;
      font-weight: 800;
      color: var(--accent-cyan);
    }

    .hud-unit.energy-unit {
      color: var(--accent-amber);
    }

    .hud-unit.avg-unit {
      color: var(--accent-emerald);
    }

    .hud-subvalue {
      font-size: 13px;
      color: var(--text-muted);
      font-family: var(--font-mono);
    }

    /* Stats Ribbon */
    .stats-card {
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      border-radius: 16px;
      padding: 16px;
      margin-bottom: 16px;
    }

    .stats-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
      gap: 10px;
      text-align: center;
    }

    .stat-box {
      background: rgba(15, 23, 42, 0.6);
      padding: 10px;
      border-radius: 12px;
      border: 1px solid rgba(255, 255, 255, 0.05);
    }

    body.light-theme .stat-box {
      background: #f8fafc;
      border: 1px solid #e2e8f0;
    }

    .stat-title {
      font-size: 10px;
      color: var(--text-muted);
      text-transform: uppercase;
      font-weight: 700;
      margin-bottom: 4px;
      letter-spacing: 0.5px;
    }

    .stat-val {
      font-size: 17px;
      font-family: var(--font-mono);
      font-weight: 700;
      color: #fff;
    }

    body.light-theme .stat-val {
      color: #0f172a;
    }

    /* Compact Session Bar */
    .session-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      padding: 10px 16px;
      border-radius: 14px;
      margin-bottom: 16px;
      flex-wrap: wrap;
      gap: 10px;
    }

    .session-info {
      font-size: 13px;
      color: var(--text-main);
      display: flex;
      align-items: center;
      gap: 8px;
      flex-wrap: wrap;
    }

    .pellet-badge {
      background: rgba(16, 185, 129, 0.15);
      color: var(--accent-emerald);
      padding: 3px 8px;
      border-radius: 6px;
      font-weight: 700;
      font-size: 12px;
      font-family: var(--font-mono);
    }

    .action-group {
      display: flex;
      gap: 8px;
      flex-wrap: wrap;
    }

    .btn {
      background: rgba(30, 41, 59, 0.8);
      color: #fff;
      border: 1px solid rgba(255, 255, 255, 0.12);
      padding: 8px 14px;
      border-radius: 10px;
      font-size: 12px;
      font-weight: 600;
      cursor: pointer;
      display: inline-flex;
      align-items: center;
      gap: 6px;
      transition: all 0.2s ease;
    }

    body.light-theme .btn {
      background: #ffffff;
      color: #1e293b;
      border: 1px solid #cbd5e1;
    }

    .btn:hover {
      background: rgba(51, 65, 85, 0.9);
      border-color: rgba(255, 255, 255, 0.25);
    }

    body.light-theme .btn:hover {
      background: #f1f5f9;
      border-color: #94a3b8;
    }

    .btn-primary {
      background: linear-gradient(135deg, #0284c7, #0369a1);
      border-color: #38bdf8;
      box-shadow: 0 4px 14px rgba(2, 132, 199, 0.35);
      color: #fff !important;
    }

    .btn-primary:hover {
      background: linear-gradient(135deg, #0369a1, #075985);
    }

    .btn-success {
      background: linear-gradient(135deg, #059669, #047857);
      border-color: #34d399;
      box-shadow: 0 4px 14px rgba(5, 150, 105, 0.35);
      color: #fff !important;
    }

    .btn-danger {
      background: rgba(239, 68, 68, 0.15);
      border-color: rgba(239, 68, 68, 0.4);
      color: #ef4444;
    }

    .btn-danger:hover {
      background: rgba(239, 68, 68, 0.25);
    }

    /* Chart Container */
    .chart-card {
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      border-radius: 16px;
      padding: 18px;
      margin-bottom: 16px;
    }

    .chart-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 12px;
    }

    .chart-title {
      font-size: 14px;
      font-weight: 700;
      color: var(--text-main);
    }

    .chart-wrapper {
      width: 100%;
      height: 250px;
      position: relative;
    }

    canvas#chronoChart {
      width: 100%;
      height: 100%;
    }

    /* Shot Table */
    .table-card {
      background: var(--bg-card);
      border: 1px solid var(--bg-card-border);
      border-radius: 16px;
      padding: 18px;
      overflow-x: auto;
    }

    table {
      width: 100%;
      border-collapse: collapse;
      font-size: 13px;
    }

    th {
      text-align: left;
      padding: 10px 14px;
      color: var(--text-muted);
      border-bottom: 1px solid rgba(255, 255, 255, 0.08);
      text-transform: uppercase;
      font-size: 11px;
      letter-spacing: 0.8px;
    }

    body.light-theme th {
      border-bottom: 1px solid #e2e8f0;
    }

    td {
      padding: 10px 14px;
      border-bottom: 1px solid rgba(255, 255, 255, 0.04);
      font-family: var(--font-mono);
      color: var(--text-main);
    }

    body.light-theme td {
      border-bottom: 1px solid #f1f5f9;
    }

    tr:hover td {
      background: rgba(255, 255, 255, 0.03);
    }

    body.light-theme tr:hover td {
      background: rgba(0, 0, 0, 0.02);
    }

    .delta-pos { color: #10b981; }
    .delta-neg { color: #ef4444; }
    .delta-zero { color: var(--text-muted); }

    /* Form Styles */
    .form-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
      gap: 16px;
      margin-bottom: 20px;
    }

    .form-group {
      display: flex;
      flex-direction: column;
      gap: 6px;
    }

    .form-group label {
      font-size: 12px;
      font-weight: 600;
      color: var(--text-muted);
      text-transform: uppercase;
    }

    .form-control {
      background: rgba(15, 23, 42, 0.7);
      border: 1px solid rgba(255, 255, 255, 0.12);
      border-radius: 10px;
      padding: 10px 14px;
      color: #fff;
      font-size: 14px;
      outline: none;
      transition: border-color 0.2s;
    }

    body.light-theme .form-control {
      background: #ffffff;
      border: 1px solid #cbd5e1;
      color: #0f172a;
    }

    .form-control:focus {
      border-color: var(--accent-cyan);
      box-shadow: 0 0 10px rgba(0, 242, 254, 0.2);
    }

    .preset-chips {
      display: flex;
      gap: 8px;
      flex-wrap: wrap;
      margin-top: 10px;
      margin-bottom: 20px;
    }

    .chip {
      background: rgba(30, 41, 59, 0.6);
      border: 1px solid rgba(255, 255, 255, 0.08);
      color: #cbd5e1;
      padding: 6px 12px;
      border-radius: 20px;
      font-size: 12px;
      cursor: pointer;
      transition: all 0.2s;
    }

    body.light-theme .chip {
      background: #ffffff;
      border: 1px solid #cbd5e1;
      color: #334155;
    }

    .chip:hover {
      background: rgba(56, 189, 248, 0.15);
      border-color: var(--accent-cyan);
      color: var(--accent-cyan);
    }

    /* Modal */
    .modal-overlay {
      display: none;
      position: fixed;
      top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(0, 0, 0, 0.75);
      backdrop-filter: blur(6px);
      z-index: 1000;
      align-items: center;
      justify-content: center;
      padding: 16px;
    }

    .modal-overlay.active { display: flex; }

    .modal-card {
      background: #0f172a;
      border: 1px solid var(--accent-cyan);
      border-radius: 18px;
      padding: 24px;
      width: 100%;
      max-width: 500px;
      box-shadow: 0 20px 50px rgba(0, 0, 0, 0.6);
    }

    body.light-theme .modal-card {
      background: #ffffff;
      border: 1px solid #0284c7;
      box-shadow: 0 20px 50px rgba(0, 0, 0, 0.15);
    }

    .modal-title {
      font-size: 18px;
      font-weight: 700;
      margin-bottom: 14px;
      color: var(--text-main);
    }

    /* Sensor Diagnostics Indicator */
    .sensor-box {
      display: flex;
      gap: 20px;
      align-items: center;
      background: rgba(15, 23, 42, 0.7);
      padding: 16px;
      border-radius: 12px;
      margin-top: 12px;
      flex-wrap: wrap;
    }

    body.light-theme .sensor-box {
      background: #ffffff;
      border: 1px solid #e2e8f0;
    }

    .sensor-item {
      display: flex;
      align-items: center;
      gap: 10px;
    }

    .sensor-light {
      width: 16px;
      height: 16px;
      border-radius: 50%;
      background: #334155;
      border: 2px solid #64748b;
      transition: all 0.2s ease;
    }

    .sensor-light.active {
      background: var(--accent-emerald);
      border-color: #34d399;
      box-shadow: var(--glow-emerald);
    }

    .sensor-light.emitter-on {
      background: var(--accent-amber);
      border-color: #f59e0b;
      box-shadow: 0 0 15px rgba(245, 158, 11, 0.4);
    }

    @media (max-width: 768px) {
      .hud-big-value { font-size: 38px; }
      .container { padding: 10px; }
      header { flex-direction: column; gap: 10px; align-items: flex-start; }
      .header-actions { width: 100%; justify-content: space-between; }
    }
  </style>
</head>
<body>

<div class="container">
  <!-- Header -->
  <header>
    <div class="logo-group">
      <div class="logo-icon">&#x26A1;</div>
      <div>
        <div class="logo-title">AirChrony S3</div>
        <div style="font-size: 11px; color: var(--text-muted)">ESP32-S3 High-Speed Optical Chronograph</div>
      </div>
    </div>
    <div class="header-actions">
      <button class="btn" style="padding: 6px 12px; font-size: 12px;" id="voiceToggleBtn" onclick="toggleVoice()" title="Toggle Voice Prompts">
        &#x1F50A; Voice ON
      </button>
      <button class="btn" style="padding: 6px 12px; font-size: 12px;" id="themeToggleBtn" onclick="toggleTheme()" title="Switch Light/Dark Theme">
        &#x2600;&#xFE0F; Light
      </button>
      <div class="status-pill">
        <div class="pulse-dot"></div>
        <span id="connStatus">GATE READY</span>
      </div>
    </div>
  </header>

  <!-- Navigation Tabs -->
  <nav class="tabs-nav">
    <button class="tab-btn active" onclick="switchTab('liveTab')">&#x1F4CA; Live Chrono</button>
    <button class="tab-btn" onclick="switchTab('pelletTab')">&#x1F3AF; Pellet Profile</button>
    <button class="tab-btn" onclick="switchTab('historyTab')">&#x1F4DA; Saved Strings</button>
    <button class="tab-btn" onclick="switchTab('settingsTab')">&#x2699;&#xFE0F; Settings & Test</button>
  </nav>

  <!-- ==================== TAB 1: LIVE CHRONO (SHOT-FOCUSED HUD) ==================== -->
  <div id="liveTab" class="tab-content active">
    
    <!-- TOP PRIMARY READOUTS (CLEAN & CLUTTER-FREE) -->
    <div class="hud-grid">
      <!-- Speed Card -->
      <div class="hud-card">
        <div class="hud-label">
          <span>Last Shot Speed</span>
          <span style="font-size: 11px; color: var(--accent-cyan); font-weight: 800;">LIVE</span>
        </div>
        <div class="hud-value-group">
          <span class="hud-big-value" id="dispSpeed">0.0</span>
          <span class="hud-unit" id="dispSpeedUnit">FPS</span>
        </div>
        <div class="hud-subvalue" id="dispSpeedDelta">Delta to Avg: 0.0</div>
      </div>

      <!-- Power / Energy Card -->
      <div class="hud-card energy-card">
        <div class="hud-label">
          <span>Last Shot Energy</span>
          <span style="font-size: 11px; color: var(--accent-amber); font-weight: 800;">MUZZLE</span>
        </div>
        <div class="hud-value-group">
          <span class="hud-big-value" id="dispEnergy">0.00</span>
          <span class="hud-unit energy-unit" id="dispEnergyUnit">FT-LBS</span>
        </div>
        <div class="hud-subvalue" id="dispEnergyDesc">Kinetic Energy</div>
      </div>

      <!-- Rolling Average Card -->
      <div class="hud-card avg-card">
        <div class="hud-label">
          <span>Rolling Average (<span id="dispAvgWindow">10</span> Shots)</span>
          <span style="font-size: 11px; color: var(--accent-emerald); font-weight: 800;">ACTIVE</span>
        </div>
        <div class="hud-value-group">
          <span class="hud-big-value" id="dispRollingAvg" style="color: var(--accent-emerald)">0.0</span>
          <span class="hud-unit avg-unit" id="dispRollingAvgUnit">FPS</span>
        </div>
        <div class="hud-subvalue" id="dispRollingAvgEnergy">Avg Energy: 0.00 FT-LBS</div>
      </div>
    </div>

    <!-- Statistics Summary Ribbon -->
    <div class="stats-card">
      <div class="stats-grid">
        <div class="stat-box">
          <div class="stat-title">Shot Count</div>
          <div class="stat-val" id="statCount">0</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Session Avg</div>
          <div class="stat-val" id="statAvg">0.0</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Min Speed</div>
          <div class="stat-val" id="statMin">0.0</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Max Speed</div>
          <div class="stat-val" id="statMax">0.0</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Spread (ES)</div>
          <div class="stat-val" id="statES" style="color: var(--accent-amber)">0.0</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Std Dev (SD)</div>
          <div class="stat-val" id="statSD" style="color: var(--accent-purple)">0.00</div>
        </div>
        <div class="stat-box">
          <div class="stat-title">Avg Energy</div>
          <div class="stat-val" id="statAvgEnergy">0.00</div>
        </div>
      </div>
    </div>

    <!-- Compact Session Info & Action Toolbar -->
    <div class="session-bar">
      <div class="session-info">
        <span>Pellet: <strong id="bannerPelletName">JSB Exact Diabolo</strong></span>
        <span class="pellet-badge" id="bannerPelletWeight">8.44 gr</span>
        <button class="btn" style="padding: 3px 8px; font-size: 11px;" onclick="switchTab('pelletTab')">Edit</button>
      </div>
      <div class="action-group">
        <button class="btn btn-primary" onclick="openSaveModal()">&#x1F4BE; Save</button>
        <button class="btn" onclick="exportCSV()">&#x1F4E5; CSV</button>
        <button class="btn" onclick="copySummaryText()">&#x1F4DD; Summary</button>
        <button class="btn btn-danger" onclick="clearActiveString()">&#x1F5D1;&#xFE0F; Clear</button>
        <button class="btn" onclick="simulateShot()">&#x1F52B; Test Shot</button>
      </div>
    </div>

    <!-- Progression Chart -->
    <div class="chart-card">
      <div class="chart-header">
        <div class="chart-title">&#x1F4C8; Shot Progression (<span class="unitLabelSpeed">FPS</span>)</div>
        <div style="font-size: 12px; color: var(--text-muted)">
          <span style="color: var(--accent-cyan);">&#x25CF; Speed</span> &nbsp;
          <span style="color: var(--accent-emerald);">&#x25AC; Average</span> &nbsp;
          <span style="color: rgba(245, 158, 11, 0.4);">&#x25A0; Spread Band</span>
        </div>
      </div>
      <div class="chart-wrapper">
        <canvas id="chronoChart"></canvas>
      </div>
    </div>

    <!-- Shot Table -->
    <div class="table-card">
      <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 12px;">
        <h3 style="font-size: 14px; color: var(--text-main);">Shot Details Log</h3>
        <span style="font-size: 12px; color: var(--text-muted);" id="tableSubtitle">0 shots recorded</span>
      </div>
      <table>
        <thead>
          <tr>
            <th>#</th>
            <th>Speed (<span class="unitLabelSpeed">FPS</span>)</th>
            <th>Energy (<span class="unitLabelEnergy">FT-LBS</span>)</th>
            <th>Delta Avg</th>
            <th>Gate Time</th>
            <th>Action</th>
          </tr>
        </thead>
        <tbody id="shotTableBody">
          <tr>
            <td colspan="6" style="text-align: center; color: var(--text-muted); padding: 30px;">
              Ready for fire. Shoot through chronograph or click "Test Shot".
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>

  <!-- ==================== TAB 2: PELLET PROFILE ==================== -->
  <div id="pelletTab" class="tab-content">
    <div class="hud-card">
      <h2 style="font-size: 18px; color: var(--text-main); margin-bottom: 16px;">Pellet & Caliber Configuration</h2>
      <p style="font-size: 13px; color: var(--text-muted); margin-bottom: 20px;">
        Enter your pellet details to accurately calculate muzzle kinetic energy.
      </p>

      <div class="form-grid">
        <div class="form-group">
          <label>Pellet Brand</label>
          <input type="text" id="inputBrand" class="form-control" placeholder="e.g. JSB, H&N, RWS">
        </div>
        <div class="form-group">
          <label>Pellet Model</label>
          <input type="text" id="inputModel" class="form-control" placeholder="e.g. Exact, Field Target Trophy">
        </div>
        <div class="form-group">
          <label>Caliber</label>
          <select id="inputCaliber" class="form-control">
            <option value=".177 (4.5mm)">.177 (4.5mm)</option>
            <option value=".22 (5.5mm)">.22 (5.5mm)</option>
            <option value=".25 (6.35mm)">.25 (6.35mm)</option>
            <option value=".30 (7.62mm)">.30 (7.62mm)</option>
          </select>
        </div>
        <div class="form-group">
          <label>Weight (Grains - gr)</label>
          <input type="number" step="0.01" id="inputGrains" class="form-control" oninput="syncWeightFromGrains(this.value)">
        </div>
        <div class="form-group">
          <label>Weight (Grams - g)</label>
          <input type="number" step="0.001" id="inputGrams" class="form-control" oninput="syncWeightFromGrams(this.value)">
        </div>
        <div class="form-group">
          <label>Rolling Average Window (N Shots)</label>
          <input type="number" min="2" max="100" id="inputAvgWindow" class="form-control" value="10">
        </div>
      </div>

      <div style="font-size: 12px; font-weight: 700; color: var(--text-muted); text-transform: uppercase;">Quick Pellet Presets:</div>
      <div class="preset-chips">
        <div class="chip" onclick="applyPreset('JSB', 'Exact Diabolo', '.177 (4.5mm)', 8.44)">JSB Exact .177 (8.44gr)</div>
        <div class="chip" onclick="applyPreset('JSB', 'Exact Heavy', '.177 (4.5mm)', 10.34)">JSB Heavy .177 (10.34gr)</div>
        <div class="chip" onclick="applyPreset('H&N', 'Field Target Trophy', '.177 (4.5mm)', 8.64)">H&N FTT .177 (8.64gr)</div>
        <div class="chip" onclick="applyPreset('RWS', 'Superdome', '.177 (4.5mm)', 8.30)">RWS Superdome .177 (8.30gr)</div>
        <div class="chip" onclick="applyPreset('JSB', 'Jumbo Exact', '.22 (5.5mm)', 15.89)">JSB Jumbo .22 (15.89gr)</div>
        <div class="chip" onclick="applyPreset('H&N', 'Baracuda Match', '.22 (5.5mm)', 21.14)">H&N Baracuda .22 (21.14gr)</div>
      </div>

      <div style="margin-top: 20px;">
        <button class="btn btn-success" onclick="savePelletProfile()">&#x2714;&#xFE0F; Save Profile Settings</button>
      </div>
    </div>
  </div>

  <!-- ==================== TAB 3: SAVED STRINGS ==================== -->
  <div id="historyTab" class="tab-content">
    <div class="hud-card">
      <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 16px;">
        <h2 style="font-size: 18px; color: var(--text-main);">Saved Shot Strings (ESP32 Flash)</h2>
        <div style="display: flex; gap: 8px;">
          <button class="btn" onclick="fetchSavedStrings()">&#x1F504; Refresh</button>
          <button class="btn" onclick="triggerImportJson()">&#x1F4E5; Import File</button>
          <input type="file" id="jsonFileInput" style="display: none;" accept=".json" onchange="handleImportFile(event)">
        </div>
      </div>

      <div id="savedStringsList">
        <!-- Dynamic list of saved strings -->
      </div>
    </div>
  </div>

  <!-- ==================== TAB 4: SETTINGS & DIAGNOSTICS ==================== -->
  <div id="settingsTab" class="tab-content">
    
    <!-- Audio & Voice Callout Settings -->
    <div class="hud-card" style="margin-bottom: 16px;">
      <h2 style="font-size: 18px; color: var(--text-main); margin-bottom: 16px;">&#x1F50A; Voice Audio Callout (Browser Speech)</h2>
      <p style="font-size: 13px; color: var(--text-muted); margin-bottom: 18px;">
        Hear your shot count, velocity, and power spoken aloud through your phone speaker or Bluetooth ear defenders without looking at the screen.
      </p>

      <div class="form-grid">
        <div class="form-group">
          <label>Voice Announcer</label>
          <select id="selectVoiceEnabled" class="form-control" onchange="setVoiceEnabled(this.value)">
            <option value="true">&#x1F50A; Enabled (Speak After Every Shot)</option>
            <option value="false">&#x1F507; Disabled (Mute Voice)</option>
          </select>
        </div>
        <div class="form-group">
          <label>Spoken Format</label>
          <select id="selectVoiceFormat" class="form-control" onchange="setVoiceFormat(this.value)">
            <option value="shot_speed">"Shot 5, 784 FPS" (Default)</option>
            <option value="count_only">"Shot 5" (Shot Count Only)</option>
            <option value="speed_only">"784" (Speed Only - Fast)</option>
            <option value="shot_speed_energy">"Shot 5, 784 FPS, 11.5 foot-pounds"</option>
            <option value="speed_full">"784 feet per second"</option>
          </select>
        </div>
        <div class="form-group">
          <label>Speech Speed (Rate)</label>
          <select id="selectVoiceRate" class="form-control" onchange="setVoiceRate(this.value)">
            <option value="1.0">1.0x (Normal Pace)</option>
            <option value="1.2" selected>1.2x (Brisk / Shooting Pace)</option>
            <option value="1.4">1.4x (Fast)</option>
          </select>
        </div>
      </div>

      <div style="margin-top: 10px;">
        <button class="btn" onclick="testVoiceCallout()">&#x1F50A; Test Voice Callout</button>
      </div>
    </div>

    <!-- Units & Theme -->
    <div class="hud-card" style="margin-bottom: 16px;">
      <h2 style="font-size: 18px; color: var(--text-main); margin-bottom: 16px;">Measurement Units & Display</h2>
      
      <div class="form-grid">
        <div class="form-group">
          <label>Speed / Velocity Unit</label>
          <select id="selectSpeedUnit" class="form-control" onchange="setSpeedUnit(this.value)">
            <option value="fps">Feet per Second (FPS)</option>
            <option value="mps">Meters per Second (m/s)</option>
          </select>
        </div>
        <div class="form-group">
          <label>Energy / Power Unit</label>
          <select id="selectEnergyUnit" class="form-control" onchange="setEnergyUnit(this.value)">
            <option value="ftlbs">Foot-pounds (FT-LBS)</option>
            <option value="joules">Joules (J)</option>
          </select>
        </div>
        <div class="form-group">
          <label>UI Color Theme</label>
          <select id="selectTheme" class="form-control" onchange="setTheme(this.value)">
            <option value="dark">&#x1F319; Dark Mode (Tactical HUD)</option>
            <option value="light">&#x2600;&#xFE0F; Light Mode (Outdoor High-Contrast)</option>
          </select>
        </div>
      </div>
    </div>

    <!-- Hardware Configuration -->
    <div class="hud-card">
      <h2 style="font-size: 18px; color: var(--text-main); margin-bottom: 16px;">Hardware & Optical Gate Setup</h2>

      <div class="form-grid">
        <div class="form-group">
          <label>Sensor Spacing Distance (mm)</label>
          <input type="number" step="0.1" id="inputGateDist" class="form-control" value="50.0">
          <small style="color: var(--text-muted); font-size: 11px;">Measured distance between Gate 1 (D10) and Gate 2 (D9)</small>
        </div>
        <div class="form-group">
          <label>Trigger Edge Polarity</label>
          <select id="inputTriggerMode" class="form-control">
            <option value="2">FALLING (Active Low / Beam Interrupted)</option>
            <option value="1">RISING (Active High / Beam Interrupted)</option>
          </select>
        </div>
        <div class="form-group">
          <label>Auto-Power Save Inactivity Sleep</label>
          <select id="inputEmitterTimeout" class="form-control">
            <option value="5">5 Minutes Inactive</option>
            <option value="10" selected>10 Minutes Inactive</option>
            <option value="15">15 Minutes Inactive</option>
            <option value="30">30 Minutes Inactive</option>
            <option value="0">Disabled (Always Powered)</option>
          </select>
          <small style="color: var(--text-muted); font-size: 11px;">Turns off IR emitters on D1 (+)/D0 (GND) to save battery</small>
        </div>
      </div>

      <div style="margin-top: 14px; display: flex; gap: 10px; align-items: center; flex-wrap: wrap;">
        <button class="btn btn-primary" onclick="saveHardwareSettings()">Save Hardware Config</button>
        <button class="btn" id="btnEmitterToggle" onclick="toggleEmitters()">&#x1F4A1; Toggle Emitters</button>
      </div>

      <hr style="border: 0; border-top: 1px solid var(--bg-card-border); margin: 24px 0;">

      <h3 style="font-size: 15px; color: var(--text-main); margin-bottom: 10px;">Live Sensor Alignment Diagnostic</h3>
      <p style="font-size: 13px; color: var(--text-muted);">
        Place a card or projectile in front of each optical sensor to verify alignment and digital pin response:
      </p>

      <div class="sensor-box">
        <div class="sensor-item">
          <div class="sensor-light" id="gate1Light"></div>
          <div>
            <div style="font-weight: 700; font-size: 13px;">Gate 1 (Pin D10 / GPIO9)</div>
            <div style="font-size: 11px; color: var(--text-muted);" id="gate1StateText">Clear / Armed</div>
          </div>
        </div>
        <div class="sensor-item">
          <div class="sensor-light" id="gate2Light"></div>
          <div>
            <div style="font-weight: 700; font-size: 13px;">Gate 2 (Pin D9 / GPIO8)</div>
            <div style="font-size: 11px; color: var(--text-muted);" id="gate2StateText">Clear / Armed</div>
          </div>
        </div>
        <div class="sensor-item">
          <div class="sensor-light emitter-on" id="emitterLight"></div>
          <div>
            <div style="font-weight: 700; font-size: 13px;">IR Emitters (D1 +VCC / D0 GND)</div>
            <div style="font-size: 11px; color: var(--text-muted);" id="emitterStateText">Powered ON (D1=HIGH, D0=LOW)</div>
          </div>
        </div>
        <button class="btn" style="margin-left: auto; padding: 6px 12px; font-size: 12px;" onclick="pollSensors()">Poll Sensors</button>
      </div>
    </div>
  </div>
</div>

<!-- Save String Modal -->
<div id="saveModal" class="modal-overlay">
  <div class="modal-card">
    <div class="modal-title">Save Shot String</div>
    <div class="form-group" style="margin-bottom: 16px;">
      <label>String Name / Session Title</label>
      <input type="text" id="saveStringName" class="form-control" placeholder="e.g. HW97k JSB Exact Test 1">
    </div>
    <div style="display: flex; justify-content: flex-end; gap: 10px;">
      <button class="btn" onclick="closeSaveModal()">Cancel</button>
      <button class="btn btn-primary" onclick="confirmSaveString()">Save to Flash</button>
    </div>
  </div>
</div>

<script>
// Application State
let appData = {
  theme: 'dark', // 'dark' or 'light'
  speedUnit: 'fps', // 'fps' or 'mps'
  energyUnit: 'ftlbs', // 'ftlbs' or 'joules'
  voiceEnabled: true,
  voiceFormat: 'shot_speed', // 'shot_speed', 'speed_only', 'shot_speed_energy', 'speed_full'
  voiceRate: 1.2,
  shots: [],
  stats: {},
  pellet: {
    brand: 'JSB',
    model: 'Exact Diabolo',
    caliber: '.177 (4.5mm)',
    weightGrains: 8.44,
    weightGrams: 0.547
  },
  gateDistanceMm: 50.0,
  avgWindowSize: 10,
  emitterTimeoutMin: 10,
  emittersPowered: true,
  stringName: 'Session 1'
};

let lastSpokenShotCount = 0;
let isInitialLoad = true;

// Unit Conversions
const GRAINS_TO_GRAMS = 0.06479891;
const GRAMS_TO_GRAINS = 15.4323584;
const FPS_TO_MPS = 0.3048;
const MPS_TO_FPS = 3.28084;
const JOULES_TO_FTLBS = 0.737562;
const FTLBS_TO_JOULES = 1.355818;

// Initialize on Load
window.addEventListener('DOMContentLoaded', () => {
  initPreferences();
  fetchFullState();
  initCanvasChart();
  setInterval(pollLiveStatus, 1200);
});

// Preferences Management (Theme, Units, Voice)
function initPreferences() {
  const savedTheme = localStorage.getItem('airchrony_theme') || 'dark';
  const savedSpeed = localStorage.getItem('airchrony_speedUnit') || 'fps';
  const savedEnergy = localStorage.getItem('airchrony_energyUnit') || 'ftlbs';
  const savedVoiceEn = localStorage.getItem('airchrony_voiceEnabled');
  const savedVoiceFmt = localStorage.getItem('airchrony_voiceFormat') || 'shot_speed';
  const savedVoiceRate = localStorage.getItem('airchrony_voiceRate') || '1.2';
  
  appData.speedUnit = savedSpeed;
  appData.energyUnit = savedEnergy;
  appData.voiceEnabled = (savedVoiceEn === null || savedVoiceEn === 'true');
  appData.voiceFormat = savedVoiceFmt;
  appData.voiceRate = parseFloat(savedVoiceRate) || 1.2;
  
  const speedSel = document.getElementById('selectSpeedUnit');
  if (speedSel) speedSel.value = savedSpeed;

  const energySel = document.getElementById('selectEnergyUnit');
  if (energySel) energySel.value = savedEnergy;

  const voiceEnSel = document.getElementById('selectVoiceEnabled');
  if (voiceEnSel) voiceEnSel.value = appData.voiceEnabled ? 'true' : 'false';

  const voiceFmtSel = document.getElementById('selectVoiceFormat');
  if (voiceFmtSel) voiceFmtSel.value = appData.voiceFormat;

  const voiceRateSel = document.getElementById('selectVoiceRate');
  if (voiceRateSel) voiceRateSel.value = String(appData.voiceRate);

  updateVoiceButtonUI();
  setTheme(savedTheme);
  updateUnitLabels();
}

function setSpeedUnit(unit) {
  appData.speedUnit = unit;
  localStorage.setItem('airchrony_speedUnit', unit);
  updateUnitLabels();
  renderHUD();
  renderTable();
  drawChart();
}

function setEnergyUnit(unit) {
  appData.energyUnit = unit;
  localStorage.setItem('airchrony_energyUnit', unit);
  updateUnitLabels();
  renderHUD();
  renderTable();
}

function updateUnitLabels() {
  const isFps = appData.speedUnit === 'fps';
  const isFtLbs = appData.energyUnit === 'ftlbs';

  document.querySelectorAll('.unitLabelSpeed').forEach(el => el.innerText = isFps ? 'FPS' : 'M/S');
  document.querySelectorAll('.unitLabelEnergy').forEach(el => el.innerText = isFtLbs ? 'FT-LBS' : 'JOULES');
  
  document.getElementById('dispSpeedUnit').innerText = isFps ? 'FPS' : 'M/S';
  document.getElementById('dispEnergyUnit').innerText = isFtLbs ? 'FT-LBS' : 'JOULES';
  document.getElementById('dispRollingAvgUnit').innerText = isFps ? 'FPS' : 'M/S';
}

// Voice Prompts Management
function setVoiceEnabled(val) {
  appData.voiceEnabled = (val === 'true' || val === true);
  localStorage.setItem('airchrony_voiceEnabled', String(appData.voiceEnabled));
  updateVoiceButtonUI();
}

function setVoiceFormat(fmt) {
  appData.voiceFormat = fmt;
  localStorage.setItem('airchrony_voiceFormat', fmt);
}

function setVoiceRate(rate) {
  appData.voiceRate = parseFloat(rate) || 1.2;
  localStorage.setItem('airchrony_voiceRate', String(appData.voiceRate));
}

function toggleVoice() {
  setVoiceEnabled(!appData.voiceEnabled);
  const voiceEnSel = document.getElementById('selectVoiceEnabled');
  if (voiceEnSel) voiceEnSel.value = appData.voiceEnabled ? 'true' : 'false';
  if (appData.voiceEnabled) {
    speakText("Voice prompts enabled");
  }
}

function updateVoiceButtonUI() {
  const btn = document.getElementById('voiceToggleBtn');
  if (!btn) return;
  if (appData.voiceEnabled) {
    btn.innerHTML = '&#x1F50A; Voice ON';
    btn.style.color = '#34d399';
    btn.style.borderColor = 'rgba(52, 211, 153, 0.4)';
  } else {
    btn.innerHTML = '&#x1F507; Voice OFF';
    btn.style.color = '#94a3b8';
    btn.style.borderColor = 'rgba(255, 255, 255, 0.12)';
  }
}

function speakShot(shot) {
  if (!appData.voiceEnabled || !('speechSynthesis' in window)) return;
  if (!shot) return;

  const isFps = appData.speedUnit === 'fps';
  const isFtLbs = appData.energyUnit === 'ftlbs';
  const speed = (isFps ? shot.fps : shot.mps).toFixed(1);
  const energy = (isFtLbs ? shot.ftLbs : shot.joules).toFixed(1);
  const speedUnitSpoken = isFps ? "feet per second" : "meters per second";
  const energyUnitSpoken = isFtLbs ? "foot pounds" : "joules";

  let phrase = "";
  switch (appData.voiceFormat) {
    case 'count_only':
      phrase = `Shot ${shot.num}`;
      break;
    case 'speed_only':
      phrase = `${Math.round(isFps ? shot.fps : shot.mps)}`;
      break;
    case 'shot_speed_energy':
      phrase = `Shot ${shot.num}, ${speed} ${isFps ? 'FPS' : 'm per second'}, ${energy} ${energyUnitSpoken}`;
      break;
    case 'speed_full':
      phrase = `${speed} ${speedUnitSpoken}`;
      break;
    case 'shot_speed':
    default:
      phrase = `Shot ${shot.num}, ${speed}`;
      break;
  }

  speakText(phrase);
}

function speakText(text) {
  if (!('speechSynthesis' in window)) return;
  window.speechSynthesis.cancel(); // cancel any trailing utterance
  const utterance = new SpeechSynthesisUtterance(text);
  utterance.rate = appData.voiceRate || 1.2;
  utterance.pitch = 1.0;
  window.speechSynthesis.speak(utterance);
}

function testVoiceCallout() {
  const dummyShot = {
    num: (appData.shots.length > 0 ? appData.shots.length : 1),
    fps: 785.4,
    mps: 239.4,
    ftLbs: 11.56,
    joules: 15.67
  };
  speakShot(dummyShot);
}

function setTheme(theme) {
  appData.theme = theme;
  localStorage.setItem('airchrony_theme', theme);
  const themeBtn = document.getElementById('themeToggleBtn');
  const themeSelect = document.getElementById('selectTheme');

  if (theme === 'light') {
    document.body.classList.add('light-theme');
    if (themeBtn) themeBtn.innerHTML = '&#x1F319; Dark';
    if (themeSelect) themeSelect.value = 'light';
  } else {
    document.body.classList.remove('light-theme');
    if (themeBtn) themeBtn.innerHTML = '&#x2600;&#xFE0F; Light';
    if (themeSelect) themeSelect.value = 'dark';
  }
  drawChart();
}

function toggleTheme() {
  const newTheme = appData.theme === 'dark' ? 'light' : 'dark';
  setTheme(newTheme);
}

// Tab Navigation
function switchTab(tabId) {
  document.querySelectorAll('.tab-content').forEach(el => el.classList.remove('active'));
  document.querySelectorAll('.tab-btn').forEach(el => el.classList.remove('active'));
  
  const target = document.getElementById(tabId);
  if (target) target.classList.add('active');
  
  // Safely activate the corresponding tab navigation button
  document.querySelectorAll('.tab-btn').forEach(btn => {
    const clickAttr = btn.getAttribute('onclick') || '';
    if (clickAttr.includes("'" + tabId + "'") || clickAttr.includes('"' + tabId + '"')) {
      btn.classList.add('active');
    }
  });

  if (tabId === 'historyTab') fetchSavedStrings();
  if (tabId === 'liveTab') setTimeout(drawChart, 50);
}

// Data Fetching & Sync
async function fetchFullState() {
  try {
    const res = await fetch('/api/state');
    if (res.ok) {
      const data = await res.json();
      const prevCount = appData.shots.length;
      appData.shots = data.shots || [];
      appData.stats = data.stats || {};
      if (data.pellet) appData.pellet = data.pellet;
      if (data.gateDistanceMm) appData.gateDistanceMm = data.gateDistanceMm;
      if (data.avgWindowSize) appData.avgWindowSize = data.avgWindowSize;
      if (data.emitterTimeoutMin !== undefined) appData.emitterTimeoutMin = data.emitterTimeoutMin;
      if (data.emittersPowered !== undefined) appData.emittersPowered = data.emittersPowered;
      if (data.stringName) appData.stringName = data.stringName;

      updatePelletFormUI();
      updateHardwareUI();
      renderHUD();
      renderTable();
      drawChart();

      // Check if new shot fired
      if (!isInitialLoad && appData.shots.length > prevCount && appData.shots.length > lastSpokenShotCount) {
        const latestShot = appData.shots[appData.shots.length - 1];
        lastSpokenShotCount = appData.shots.length;
        speakShot(latestShot);
      } else if (isInitialLoad) {
        lastSpokenShotCount = appData.shots.length;
        isInitialLoad = false;
      }
    }
  } catch (e) {
    console.error('Error fetching state:', e);
  }
}

async function pollLiveStatus() {
  try {
    const res = await fetch('/api/live');
    if (res.ok) {
      const data = await res.json();
      if (data.totalShots !== appData.shots.length) {
        fetchFullState();
      }
      if (data.emittersPowered !== undefined && data.emittersPowered !== appData.emittersPowered) {
        appData.emittersPowered = data.emittersPowered;
        updateHardwareUI();
      }
    }
  } catch (e) {}
}

// Render HUD & Stats
function renderHUD() {
  const isFps = appData.speedUnit === 'fps';
  const isFtLbs = appData.energyUnit === 'ftlbs';
  const shots = appData.shots;
  const stats = appData.stats;

  if (shots.length > 0) {
    const lastShot = shots[shots.length - 1];
    const speed = isFps ? lastShot.fps : lastShot.mps;
    const energy = isFtLbs ? lastShot.ftLbs : lastShot.joules;
    const delta = isFps ? lastShot.deltaFps : lastShot.deltaMps;

    document.getElementById('dispSpeed').innerText = speed.toFixed(1);
    document.getElementById('dispEnergy').innerText = energy.toFixed(2);
    
    let deltaSign = delta > 0.05 ? '+' : '';
    document.getElementById('dispSpeedDelta').innerText = `Delta to Avg: ${deltaSign}${delta.toFixed(1)} ${isFps ? 'FPS' : 'm/s'}`;
    document.getElementById('dispEnergyDesc').innerText = `${energy.toFixed(2)} ${isFtLbs ? 'FT-LBS' : 'JOULES'} muzzle energy`;
  } else {
    document.getElementById('dispSpeed').innerText = '0.0';
    document.getElementById('dispEnergy').innerText = '0.00';
    document.getElementById('dispSpeedDelta').innerText = 'Ready for shot';
    document.getElementById('dispEnergyDesc').innerText = 'Kinetic energy readout';
  }

  // Rolling Avg
  document.getElementById('dispAvgWindow').innerText = appData.avgWindowSize || 10;
  document.getElementById('dispRollingAvg').innerText = (isFps ? (stats.rollingAvgFps || 0) : (stats.rollingAvgMps || 0)).toFixed(1);
  document.getElementById('dispRollingAvgEnergy').innerText = `Avg Energy: ${(isFtLbs ? (stats.rollingAvgFtLbs || 0) : (stats.rollingAvgJoules || 0)).toFixed(2)} ${isFtLbs ? 'FT-LBS' : 'J'}`;

  // Stats Ribbon
  document.getElementById('statCount').innerText = stats.totalShots || 0;
  document.getElementById('statAvg').innerText = (isFps ? (stats.avgFps || 0) : (stats.avgMps || 0)).toFixed(1);
  document.getElementById('statMin').innerText = (isFps ? (stats.minFps || 0) : (stats.minMps || 0)).toFixed(1);
  document.getElementById('statMax').innerText = (isFps ? (stats.maxFps || 0) : (stats.maxMps || 0)).toFixed(1);
  document.getElementById('statES').innerText = (isFps ? (stats.esFps || 0) : (stats.esMps || 0)).toFixed(1);
  document.getElementById('statSD').innerText = (isFps ? (stats.sdFps || 0) : (stats.sdMps || 0)).toFixed(2);
  document.getElementById('statAvgEnergy').innerText = (isFtLbs ? (stats.avgEnergyFtLbs || 0) : (stats.avgEnergyJoules || 0)).toFixed(2);

  // Compact Session Bar
  document.getElementById('bannerPelletName').innerText = `${appData.pellet.brand} ${appData.pellet.model}`;
  document.getElementById('bannerPelletWeight').innerText = `${appData.pellet.weightGrains.toFixed(2)} gr (${appData.pellet.weightGrams.toFixed(3)} g)`;
}

// Render Table
function renderTable() {
  const tbody = document.getElementById('shotTableBody');
  const shots = appData.shots;
  const isFps = appData.speedUnit === 'fps';
  const isFtLbs = appData.energyUnit === 'ftlbs';

  document.getElementById('tableSubtitle').innerText = `${shots.length} shots recorded`;

  if (shots.length === 0) {
    tbody.innerHTML = `<tr><td colspan="6" style="text-align: center; color: var(--text-muted); padding: 30px;">Ready for fire. Shoot through chronograph or click "Test Shot".</td></tr>`;
    return;
  }

  let html = '';
  // Show most recent shots at the top
  for (let i = shots.length - 1; i >= 0; i--) {
    const s = shots[i];
    const speed = isFps ? s.fps : s.mps;
    const energy = isFtLbs ? s.ftLbs : s.joules;
    const delta = isFps ? s.deltaFps : s.deltaMps;

    let deltaClass = 'delta-zero';
    let deltaSign = '';
    if (delta > 0.05) { deltaClass = 'delta-pos'; deltaSign = '+'; }
    else if (delta < -0.05) { deltaClass = 'delta-neg'; }

    html += `
      <tr>
        <td><strong style="color: var(--accent-cyan);">#${s.num}</strong></td>
        <td><strong>${speed.toFixed(1)}</strong></td>
        <td>${energy.toFixed(2)}</td>
        <td class="${deltaClass}">${deltaSign}${delta.toFixed(1)}</td>
        <td style="color: var(--text-muted);">${(s.timeUs || 0).toFixed(1)} &mu;s</td>
        <td>
          <button class="btn btn-danger" style="padding: 2px 8px; font-size: 11px;" onclick="deleteShot(${i})">&#x2715;</button>
        </td>
      </tr>
    `;
  }
  tbody.innerHTML = html;
}

// Pure Canvas Charting (Zero Internet / CDN Dependencies)
function initCanvasChart() {
  window.addEventListener('resize', drawChart);
  drawChart();
}

function drawChart() {
  const canvas = document.getElementById('chronoChart');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const rect = canvas.parentElement.getBoundingClientRect();
  canvas.width = rect.width * window.devicePixelRatio;
  canvas.height = rect.height * window.devicePixelRatio;
  ctx.scale(window.devicePixelRatio, window.devicePixelRatio);

  const w = rect.width;
  const h = rect.height;
  ctx.clearRect(0, 0, w, h);

  const isLight = document.body.classList.contains('light-theme');
  const shots = appData.shots;
  if (shots.length === 0) {
    ctx.fillStyle = isLight ? '#64748b' : '#94a3b8';
    ctx.font = '13px ' + getComputedStyle(document.body).fontFamily;
    ctx.textAlign = 'center';
    ctx.fillText('No shot data recorded yet. Fire shots to populate graph.', w / 2, h / 2);
    return;
  }

  const isFps = appData.speedUnit === 'fps';
  const values = shots.map(s => isFps ? s.fps : s.mps);
  const avg = isFps ? appData.stats.avgFps : appData.stats.avgMps;
  const minVal = isFps ? appData.stats.minFps : appData.stats.minMps;
  const maxVal = isFps ? appData.stats.maxFps : appData.stats.maxMps;

  const padLeft = 46;
  const padRight = 20;
  const padTop = 20;
  const padBottom = 30;

  const chartW = w - padLeft - padRight;
  const chartH = h - padTop - padBottom;

  let yMin = Math.floor(minVal - 15);
  let yMax = Math.ceil(maxVal + 15);
  if (yMax - yMin < 20) { yMin -= 10; yMax += 10; }

  // Draw Grid Lines & Y-axis labels
  ctx.strokeStyle = isLight ? 'rgba(0, 0, 0, 0.08)' : 'rgba(255, 255, 255, 0.06)';
  ctx.fillStyle = isLight ? '#64748b' : '#94a3b8';
  ctx.font = '11px monospace';
  ctx.textAlign = 'right';

  const gridSteps = 4;
  for (let i = 0; i <= gridSteps; i++) {
    const yVal = yMin + ((yMax - yMin) / gridSteps) * i;
    const yPos = padTop + chartH - (i / gridSteps) * chartH;
    
    ctx.beginPath();
    ctx.moveTo(padLeft, yPos);
    ctx.lineTo(w - padRight, yPos);
    ctx.stroke();

    ctx.fillText(yVal.toFixed(0), padLeft - 8, yPos + 4);
  }

  // Draw Average Line
  if (shots.length > 1) {
    const avgY = padTop + chartH - ((avg - yMin) / (yMax - yMin)) * chartH;
    ctx.strokeStyle = isLight ? '#059669' : '#10b981';
    ctx.setLineDash([4, 4]);
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.moveTo(padLeft, avgY);
    ctx.lineTo(w - padRight, avgY);
    ctx.stroke();
    ctx.setLineDash([]);
  }

  // Draw Line Plot
  const points = [];
  for (let i = 0; i < values.length; i++) {
    const x = values.length === 1 ? padLeft + chartW / 2 : padLeft + (i / (values.length - 1)) * chartW;
    const y = padTop + chartH - ((values[i] - yMin) / (yMax - yMin)) * chartH;
    points.push({ x, y, val: values[i], num: shots[i].num });
  }

  if (points.length > 1) {
    ctx.strokeStyle = isLight ? '#0284c7' : '#00f2fe';
    ctx.lineWidth = 2.5;
    ctx.beginPath();
    ctx.moveTo(points[0].x, points[0].y);
    for (let i = 1; i < points.length; i++) {
      ctx.lineTo(points[i].x, points[i].y);
    }
    ctx.stroke();
  }

  // Draw Points & Tooltips
  points.forEach(p => {
    ctx.fillStyle = isLight ? '#ffffff' : '#0a0f1d';
    ctx.strokeStyle = isLight ? '#0284c7' : '#00f2fe';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(p.x, p.y, 4.5, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();

    // X-axis label
    ctx.fillStyle = isLight ? '#475569' : '#94a3b8';
    ctx.font = '10px monospace';
    ctx.textAlign = 'center';
    ctx.fillText(`#${p.num}`, p.x, h - 10);
  });
}

// Pellet Form Handling
function updatePelletFormUI() {
  document.getElementById('inputBrand').value = appData.pellet.brand;
  document.getElementById('inputModel').value = appData.pellet.model;
  document.getElementById('inputCaliber').value = appData.pellet.caliber;
  document.getElementById('inputGrains').value = appData.pellet.weightGrains;
  document.getElementById('inputGrams').value = appData.pellet.weightGrams;
  document.getElementById('inputAvgWindow').value = appData.avgWindowSize;
}

function syncWeightFromGrains(gr) {
  const num = parseFloat(gr) || 0;
  document.getElementById('inputGrams').value = (num * GRAINS_TO_GRAMS).toFixed(3);
}

function syncWeightFromGrams(g) {
  const num = parseFloat(g) || 0;
  document.getElementById('inputGrains').value = (num * GRAMS_TO_GRAINS).toFixed(2);
}

function applyPreset(brand, model, caliber, gr) {
  document.getElementById('inputBrand').value = brand;
  document.getElementById('inputModel').value = model;
  document.getElementById('inputCaliber').value = caliber;
  document.getElementById('inputGrains').value = gr;
  syncWeightFromGrains(gr);
}

async function savePelletProfile() {
  const profile = {
    brand: document.getElementById('inputBrand').value,
    model: document.getElementById('inputModel').value,
    caliber: document.getElementById('inputCaliber').value,
    weightGrains: parseFloat(document.getElementById('inputGrains').value) || 8.44,
    weightGrams: parseFloat(document.getElementById('inputGrams').value) || 0.547,
    avgWindowSize: parseInt(document.getElementById('inputAvgWindow').value) || 10
  };

  try {
    const res = await fetch('/api/pellet', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(profile)
    });
    if (res.ok) {
      alert('Pellet Profile Saved Successfully!');
      fetchFullState();
      switchTab('liveTab');
    }
  } catch (e) {
    alert('Error saving profile: ' + e.message);
  }
}

// Hardware & Sensor Settings
function updateHardwareUI() {
  const distInp = document.getElementById('inputGateDist');
  if (distInp && appData.gateDistanceMm) distInp.value = appData.gateDistanceMm;

  const timeoutSel = document.getElementById('inputEmitterTimeout');
  if (timeoutSel && appData.emitterTimeoutMin !== undefined) timeoutSel.value = appData.emitterTimeoutMin;

  const emitBtn = document.getElementById('btnEmitterToggle');
  const emitLight = document.getElementById('emitterLight');
  const emitText = document.getElementById('emitterStateText');

  if (appData.emittersPowered) {
    if (emitBtn) emitBtn.innerHTML = '&#x1F4A1; Emitters: ON';
    if (emitLight) emitLight.classList.add('emitter-on');
    if (emitText) emitText.innerText = 'Powered ON (Active)';
  } else {
    if (emitBtn) emitBtn.innerHTML = '&#x1F4A4; Emitters: OFF (Sleep)';
    if (emitLight) emitLight.classList.remove('emitter-on');
    if (emitText) emitText.innerText = 'Powered OFF (Sleep Mode)';
  }
}

async function saveHardwareSettings() {
  const dist = parseFloat(document.getElementById('inputGateDist').value) || 50.0;
  const trig = parseInt(document.getElementById('inputTriggerMode').value) || 2;
  const timeout = parseInt(document.getElementById('inputEmitterTimeout').value) || 10;

  try {
    const res = await fetch('/api/hardware', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ gateDistanceMm: dist, triggerMode: trig, emitterTimeoutMin: timeout })
    });
    if (res.ok) {
      alert('Hardware settings updated!');
      fetchFullState();
    }
  } catch (e) {
    alert('Error: ' + e.message);
  }
}

async function toggleEmitters() {
  try {
    const res = await fetch('/api/emitters', { method: 'POST' });
    if (res.ok) {
      const data = await res.json();
      appData.emittersPowered = data.emittersPowered;
      updateHardwareUI();
    }
  } catch (e) {}
}

async function pollSensors() {
  try {
    const res = await fetch('/api/sensors');
    if (res.ok) {
      const data = await res.json();
      const g1 = document.getElementById('gate1Light');
      const g2 = document.getElementById('gate2Light');
      const g1T = document.getElementById('gate1StateText');
      const g2T = document.getElementById('gate2StateText');

      if (data.gate1) { g1.classList.add('active'); g1T.innerText = 'BEAM BLOCKED / TRIGGERED'; }
      else { g1.classList.remove('active'); g1T.innerText = 'Clear / Armed'; }

      if (data.gate2) { g2.classList.add('active'); g2T.innerText = 'BEAM BLOCKED / TRIGGERED'; }
      else { g2.classList.remove('active'); g2T.innerText = 'Clear / Armed'; }

      if (data.emittersPowered !== undefined) {
        appData.emittersPowered = data.emittersPowered;
        updateHardwareUI();
      }
    }
  } catch (e) {}
}

// Simulated Test Shot
async function simulateShot() {
  try {
    await fetch('/api/simulate', { method: 'POST' });
    fetchFullState();
  } catch (e) {}
}

// Delete Single Shot
async function deleteShot(index) {
  if (confirm(`Delete shot #${appData.shots[index].num}?`)) {
    try {
      await fetch(`/api/shot/delete?index=${index}`, { method: 'POST' });
      fetchFullState();
    } catch (e) {}
  }
}

// Clear Active String
async function clearActiveString() {
  if (confirm('Clear current shot session? Make sure to save if needed!')) {
    try {
      await fetch('/api/reset', { method: 'POST' });
      lastSpokenShotCount = 0;
      fetchFullState();
    } catch (e) {}
  }
}

// Save Modal & Persistence
function openSaveModal() {
  document.getElementById('saveStringName').value = `${appData.pellet.brand} ${appData.pellet.model} - ${new Date().toLocaleDateString()}`;
  document.getElementById('saveModal').classList.add('active');
}

function closeSaveModal() {
  document.getElementById('saveModal').classList.remove('active');
}

async function confirmSaveString() {
  const name = document.getElementById('saveStringName').value || 'Shot String';
  try {
    const res = await fetch('/api/string/save', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ name })
    });
    if (res.ok) {
      closeSaveModal();
      alert('String saved successfully to ESP32 Flash!');
    }
  } catch (e) {
    alert('Error saving string: ' + e.message);
  }
}

// Saved Strings Tab Management
async function fetchSavedStrings() {
  try {
    const res = await fetch('/api/string/list');
    if (res.ok) {
      const list = await res.json();
      const container = document.getElementById('savedStringsList');
      if (list.length === 0) {
        container.innerHTML = '<div style="color: var(--text-muted); text-align: center; padding: 20px;">No saved strings found in Flash memory.</div>';
        return;
      }
      let html = '';
      list.forEach(item => {
        html += `
          <div style="display: flex; justify-content: space-between; align-items: center; background: var(--bg-card); padding: 14px 18px; border-radius: 12px; margin-bottom: 10px; border: 1px solid var(--bg-card-border); flex-wrap: wrap; gap: 10px;">
            <div>
              <div style="font-weight: 700; font-size: 15px; color: var(--text-main);">${item.name}</div>
              <div style="font-size: 12px; color: var(--text-muted); margin-top: 4px;">
                ${item.pellet} &bull; <strong>${item.count} shots</strong> &bull; Avg: <strong>${item.avgFps} fps</strong> &bull; ES: <strong>${item.esFps} fps</strong>
              </div>
            </div>
            <div style="display: flex; gap: 8px;">
              <button class="btn" style="padding: 6px 12px; font-size: 12px;" onclick="recallString('${item.id}')">&#x1F441;&#xFE0F; Recall</button>
              <button class="btn" style="padding: 6px 12px; font-size: 12px;" onclick="downloadSavedCSV('${item.id}', '${item.name}')">&#x1F4E5; CSV</button>
              <button class="btn btn-danger" style="padding: 6px 12px; font-size: 12px;" onclick="deleteSavedString('${item.id}')">&#x1F5D1;&#xFE0F;</button>
            </div>
          </div>
        `;
      });
      container.innerHTML = html;
    }
  } catch (e) {}
}

async function recallString(id) {
  try {
    const res = await fetch(`/api/string/load?id=${id}`);
    if (res.ok) {
      const data = await res.json();
      appData.shots = data.shots || [];
      appData.stats = data.stats || {};
      if (data.pellet) appData.pellet = data.pellet;
      if (data.stringName) appData.stringName = data.stringName;
      lastSpokenShotCount = appData.shots.length;
      updatePelletFormUI();
      renderHUD();
      renderTable();
      drawChart();
      switchTab('liveTab');
    }
  } catch (e) {}
}

async function deleteSavedString(id) {
  if (confirm('Delete this saved string from ESP32 Flash?')) {
    try {
      await fetch(`/api/string/delete?id=${id}`, { method: 'POST' });
      fetchSavedStrings();
    } catch (e) {}
  }
}

function downloadSavedCSV(id, name) {
  window.open(`/api/string/csv?id=${id}`, '_blank');
}

// Data Export & Import
function exportCSV() {
  window.open('/api/export/csv', '_blank');
}

function exportJSON() {
  const jsonStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(appData, null, 2));
  const dlAnchor = document.createElement('a');
  dlAnchor.setAttribute("href", jsonStr);
  dlAnchor.setAttribute("download", `${(appData.stringName || 'ChronoSession').replace(/\s+/g, '_')}.json`);
  dlAnchor.click();
}

function copySummaryText() {
  const s = appData.stats;
  const p = appData.pellet;
  const isFps = appData.speedUnit === 'fps';
  const isFtLbs = appData.energyUnit === 'ftlbs';
  const text = `--- AirChrony S3 Report ---
Session: ${appData.stringName}
Pellet: ${p.brand} ${p.model} (${p.weightGrains} gr / ${p.weightGrams} g)
Shots Count: ${s.totalShots}
Average: ${(isFps ? s.avgFps : s.avgMps).toFixed(1)} ${isFps ? 'FPS' : 'm/s'}
Min / Max: ${(isFps ? s.minFps : s.minMps).toFixed(1)} / ${(isFps ? s.maxFps : s.maxMps).toFixed(1)} ${isFps ? 'FPS' : 'm/s'}
Extreme Spread (ES): ${(isFps ? s.esFps : s.esMps).toFixed(1)} ${isFps ? 'FPS' : 'm/s'}
Std Dev (SD): ${(isFps ? s.sdFps : s.sdMps).toFixed(2)}
Avg Energy: ${(isFtLbs ? s.avgEnergyFtLbs : s.avgEnergyJoules).toFixed(2)} ${isFtLbs ? 'ft-lbs' : 'J'}`;

  navigator.clipboard.writeText(text).then(() => {
    alert('Summary copied to clipboard!');
  }).catch(() => {
    prompt('Copy summary:', text);
  });
}

function triggerImportJson() {
  document.getElementById('jsonFileInput').click();
}

function handleImportFile(event) {
  const file = event.target.files[0];
  if (!file) return;
  const reader = new FileReader();
  reader.onload = (e) => {
    try {
      const data = JSON.parse(e.target.result);
      if (data.shots) {
        appData.shots = data.shots;
        appData.stats = data.stats || {};
        if (data.pellet) appData.pellet = data.pellet;
        if (data.stringName) appData.stringName = data.stringName;
        lastSpokenShotCount = appData.shots.length;
        renderHUD();
        renderTable();
        drawChart();
        switchTab('liveTab');
        alert('String imported successfully!');
      }
    } catch (err) {
      alert('Invalid JSON file format: ' + err.message);
    }
  };
  reader.readAsText(file);
}
</script>
</body>
</html>
)rawliteral";

#endif // WEB_PAGES_H
