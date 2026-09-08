#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include "config.h"
#include "ChronoEngine.h"
#include "StorageManager.h"
#include "WebPages.h"

// Core Engines
ChronoEngine engine;
StorageManager storage;

// Networking
WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;

// Helper to extract JSON parameter from body
String getJsonField(const String& body, const String& field) {
    int idx = body.indexOf("\"" + field + "\"");
    if (idx == -1) return "";
    int colon = body.indexOf(':', idx);
    if (colon == -1) return "";
    int start = colon + 1;
    while (start < (int)body.length() && (body[start] == ' ' || body[start] == '\t' || body[start] == '\n' || body[start] == '\r')) start++;
    if (start >= (int)body.length()) return "";

    if (body[start] == '\"') {
        int end = body.indexOf('\"', start + 1);
        if (end == -1) return "";
        return body.substring(start + 1, end);
    } else {
        int end = start;
        while (end < (int)body.length() && body[end] != ',' && body[end] != '}' && body[end] != ' ' && body[end] != '\n' && body[end] != '\r') end++;
        return body.substring(start, end);
    }
}

// HTTP Route Handlers
void handleRoot() {
    engine.refreshActivity();
    server.send_P(200, "text/html", PAGE_INDEX);
}

void handleGetState() {
    String json = storage.stringToJson(engine);
    server.send(200, "application/json", json);
}

void handleGetLive() {
    String json = "{";
    json += "\"totalShots\":" + String(engine.shots.size()) + ",";
    json += "\"state\":" + String((int)engine.getState()) + ",";
    json += "\"emittersPowered\":" + String(engine.emittersPowered ? "true" : "false");
    json += "}";
    server.send(200, "application/json", json);
}

void handlePostPellet() {
    engine.refreshActivity();
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        
        String brand = getJsonField(body, "brand");
        if (brand.length() > 0) engine.pellet.brand = brand;

        String model = getJsonField(body, "model");
        if (model.length() > 0) engine.pellet.model = model;

        String cal = getJsonField(body, "caliber");
        if (cal.length() > 0) engine.pellet.caliber = cal;

        String gr = getJsonField(body, "weightGrains");
        if (gr.length() > 0) engine.pellet.setWeightGrains(gr.toFloat());

        String win = getJsonField(body, "avgWindowSize");
        if (win.length() > 0) engine.avgWindowSize = win.toInt();

        engine.recalculateStatistics();
        storage.saveSettings(engine);

        server.send(200, "application/json", "{\"status\":\"ok\"}");
        return;
    }
    server.send(400, "application/json", "{\"error\":\"Missing body\"}");
}

void handlePostHardware() {
    engine.refreshActivity();
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        
        String dist = getJsonField(body, "gateDistanceMm");
        if (dist.length() > 0) engine.gateDistanceMm = dist.toFloat();

        String trig = getJsonField(body, "triggerMode");
        if (trig.length() > 0) {
            engine.triggerMode = trig.toInt();
            engine.attachGateInterrupts();
        }

        String emitTimeout = getJsonField(body, "emitterTimeoutMin");
        if (emitTimeout.length() > 0) {
            engine.emitterTimeoutMin = emitTimeout.toInt();
        }

        storage.saveSettings(engine);
        server.send(200, "application/json", "{\"status\":\"ok\"}");
        return;
    }
    server.send(400, "application/json", "{\"error\":\"Missing body\"}");
}

void handlePostEmitters() {
    engine.refreshActivity();
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        String en = getJsonField(body, "enabled");
        if (en == "true") {
            engine.setEmitters(true);
        } else if (en == "false") {
            engine.setEmitters(false);
        } else {
            // Toggle
            engine.setEmitters(!engine.emittersPowered);
        }
    } else {
        engine.setEmitters(!engine.emittersPowered);
    }
    String json = "{\"status\":\"ok\",\"emittersPowered\":" + String(engine.emittersPowered ? "true" : "false") + "}";
    server.send(200, "application/json", json);
}

void handleGetSensors() {
    engine.refreshActivity();
    // Read direct digital pin states for sensor alignment test
    bool g1 = (digitalRead(PIN_GATE_1) == (engine.triggerMode == FALLING ? LOW : HIGH));
    bool g2 = (digitalRead(PIN_GATE_2) == (engine.triggerMode == FALLING ? LOW : HIGH));

    String json = "{";
    json += "\"gate1\":" + String(g1 ? "true" : "false") + ",";
    json += "\"gate2\":" + String(g2 ? "true" : "false") + ",";
    json += "\"emittersPowered\":" + String(engine.emittersPowered ? "true" : "false");
    json += "}";
    server.send(200, "application/json", json);
}

void handleSimulateShot() {
    engine.simulateShot(785.0f, 6.0f);
    server.send(200, "application/json", "{\"status\":\"simulated\"}");
}

void handleDeleteShot() {
    engine.refreshActivity();
    if (server.hasArg("index")) {
        int idx = server.arg("index").toInt();
        engine.deleteShot(idx);
        server.send(200, "application/json", "{\"status\":\"deleted\"}");
        return;
    }
    server.send(400, "application/json", "{\"error\":\"Missing index\"}");
}

void handleResetSession() {
    engine.resetSession();
    server.send(200, "application/json", "{\"status\":\"reset\"}");
}

void handleSaveString() {
    engine.refreshActivity();
    String name = "";
    if (server.hasArg("plain")) {
        name = getJsonField(server.arg("plain"), "name");
    }
    String id = storage.saveShotString(engine, name);
    if (id.length() > 0) {
        server.send(200, "application/json", "{\"status\":\"saved\",\"id\":\"" + id + "\"}");
    } else {
        server.send(500, "application/json", "{\"error\":\"Save failed\"}");
    }
}

void handleListStrings() {
    engine.refreshActivity();
    String listJson = storage.listSavedStrings();
    server.send(200, "application/json", listJson);
}

void handleLoadString() {
    engine.refreshActivity();
    if (server.hasArg("id")) {
        String id = server.arg("id");
        String json = storage.getShotStringJson(id);
        if (json.length() > 2) {
            server.send(200, "application/json", json);
            return;
        }
    }
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
}

void handleDeleteSavedString() {
    engine.refreshActivity();
    if (server.hasArg("id")) {
        String id = server.arg("id");
        storage.deleteShotString(id);
        server.send(200, "application/json", "{\"status\":\"deleted\"}");
        return;
    }
    server.send(400, "application/json", "{\"error\":\"Missing id\"}");
}

void handleExportActiveCSV() {
    engine.refreshActivity();
    String csv = storage.generateCSV(engine);
    server.sendHeader("Content-Disposition", "attachment; filename=\"active_shot_string.csv\"");
    server.send(200, "text/csv", csv);
}

void handleExportSavedCSV() {
    engine.refreshActivity();
    if (server.hasArg("id")) {
        String id = server.arg("id");
        String json = storage.getShotStringJson(id);
        server.sendHeader("Content-Disposition", "attachment; filename=\"string_" + id + ".json\"");
        server.send(200, "application/json", json);
        return;
    }
    server.send(400, "text/plain", "Missing string id");
}

void handleNotFound() {
    String host = server.hostHeader();
    if (host != "192.168.4.1" && host != "chronograph.local") {
        server.sendHeader("Location", "http://192.168.4.1/", true);
        server.send(302, "text/plain", "");
        return;
    }
    server.send(404, "text/plain", "Not Found");
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println();
    Serial.println(F("========================================"));
    Serial.println(F("  AirChrony S3 Precision Chronograph    "));
    Serial.println(F("  Gate 1: D10 (GPIO9) | Gate 2: D9 (GPIO8) "));
    Serial.println(F("  IR Emitters: D1 (GPIO2) via 2x 150R   "));
    Serial.println(F("========================================"));

    // Initialize Storage
    storage.begin();

    // Initialize Chronograph Engine & Sensors
    engine.begin();
    storage.loadSettings(engine);

    // Initialize WiFi in Access Point mode
    WiFi.mode(WIFI_AP);
    IPAddress local_ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL, 0, AP_MAX_CONN);

    Serial.print(F("[WiFi] Access Point Created: "));
    Serial.println(AP_SSID);
    Serial.print(F("[WiFi] Connect to: http://"));
    Serial.println(WiFi.softAPIP());

    // Start DNS Captive Portal
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", local_ip);

    // Start mDNS Responder
    if (MDNS.begin(MDNS_HOSTNAME)) {
        Serial.println(F("[mDNS] Responding at http://chronograph.local"));
        MDNS.addService("http", "tcp", 80);
    }

    // Configure WebServer Routes
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api/state", HTTP_GET, handleGetState);
    server.on("/api/live", HTTP_GET, handleGetLive);
    server.on("/api/pellet", HTTP_POST, handlePostPellet);
    server.on("/api/hardware", HTTP_POST, handlePostHardware);
    server.on("/api/emitters", HTTP_POST, handlePostEmitters);
    server.on("/api/sensors", HTTP_GET, handleGetSensors);
    server.on("/api/simulate", HTTP_POST, handleSimulateShot);
    server.on("/api/shot/delete", HTTP_POST, handleDeleteShot);
    server.on("/api/reset", HTTP_POST, handleResetSession);
    server.on("/api/string/save", HTTP_POST, handleSaveString);
    server.on("/api/string/list", HTTP_GET, handleListStrings);
    server.on("/api/string/load", HTTP_GET, handleLoadString);
    server.on("/api/string/delete", HTTP_POST, handleDeleteSavedString);
    server.on("/api/export/csv", HTTP_GET, handleExportActiveCSV);
    server.on("/api/string/csv", HTTP_GET, handleExportSavedCSV);

    // Captive Portal probe endpoints (iOS, Android, Windows)
    server.on("/hotspot-detect.html", HTTP_GET, handleRoot);
    server.on("/generate_204", HTTP_GET, handleRoot);
    server.on("/canonical.html", HTTP_GET, handleRoot);
    server.on("/ncsi.txt", HTTP_GET, handleRoot);

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println(F("[WebServer] Chronograph Web Server is live!"));
}

void loop() {
    // Real-time chronograph interrupt state machine & sleep management
    engine.update();

    // DNS captive portal handling
    dnsServer.processNextRequest();

    // Handle incoming web client requests
    server.handleClient();
}
