#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"
#include "ChronoEngine.h"

class StorageManager {
public:
    bool isMounted;

    StorageManager() {
        isMounted = false;
    }

    bool begin() {
        if (!LittleFS.begin(true)) { // true = format if mount fails
            Serial.println(F("[Storage] LittleFS Mount Failed"));
            isMounted = false;
            return false;
        }
        isMounted = true;
        Serial.println(F("[Storage] LittleFS Mounted successfully."));

        if (!LittleFS.exists("/strings")) {
            LittleFS.mkdir("/strings");
        }
        return true;
    }

    // Save active configuration to /config.json
    bool saveSettings(const ChronoEngine& engine) {
        if (!isMounted) return false;

        File file = LittleFS.open("/config.json", "w");
        if (!file) return false;

        String json = "{\n";
        json += "  \"gateDistanceMm\": " + String(engine.gateDistanceMm, 2) + ",\n";
        json += "  \"triggerMode\": " + String(engine.triggerMode) + ",\n";
        json += "  \"avgWindowSize\": " + String(engine.avgWindowSize) + ",\n";
        json += "  \"emitterTimeoutMin\": " + String(engine.emitterTimeoutMin) + ",\n";
        json += "  \"pellet\": {\n";
        json += "    \"brand\": \"" + escapeJson(engine.pellet.brand) + "\",\n";
        json += "    \"model\": \"" + escapeJson(engine.pellet.model) + "\",\n";
        json += "    \"caliber\": \"" + escapeJson(engine.pellet.caliber) + "\",\n";
        json += "    \"weightGrains\": " + String(engine.pellet.weightGrains, 3) + ",\n";
        json += "    \"weightGrams\": " + String(engine.pellet.weightGrams, 4) + "\n";
        json += "  }\n";
        json += "}";

        file.print(json);
        file.close();
        return true;
    }

    // Load configuration from /config.json
    bool loadSettings(ChronoEngine& engine) {
        if (!isMounted || !LittleFS.exists("/config.json")) return false;

        File file = LittleFS.open("/config.json", "r");
        if (!file) return false;

        String content = file.readString();
        file.close();

        // Lightweight parser for config fields
        String distStr = extractJsonValue(content, "gateDistanceMm");
        if (distStr.length() > 0) engine.gateDistanceMm = distStr.toFloat();

        String trigStr = extractJsonValue(content, "triggerMode");
        if (trigStr.length() > 0) engine.triggerMode = trigStr.toInt();

        String winStr = extractJsonValue(content, "avgWindowSize");
        if (winStr.length() > 0) engine.avgWindowSize = winStr.toInt();

        String emitStr = extractJsonValue(content, "emitterTimeoutMin");
        if (emitStr.length() > 0) engine.emitterTimeoutMin = emitStr.toInt();

        String brand = extractJsonValue(content, "brand");
        if (brand.length() > 0) engine.pellet.brand = brand;

        String model = extractJsonValue(content, "model");
        if (model.length() > 0) engine.pellet.model = model;

        String caliber = extractJsonValue(content, "caliber");
        if (caliber.length() > 0) engine.pellet.caliber = caliber;

        String grStr = extractJsonValue(content, "weightGrains");
        if (grStr.length() > 0) engine.pellet.setWeightGrains(grStr.toFloat());

        engine.attachGateInterrupts();
        return true;
    }

    // Save shot string as JSON file
    String saveShotString(const ChronoEngine& engine, String customName = "") {
        if (!isMounted) return "";

        String stringId = String(millis());
        String filename = "/strings/" + stringId + ".json";

        File file = LittleFS.open(filename, "w");
        if (!file) return "";

        String name = customName.length() > 0 ? customName : engine.stringName;
        String json = stringToJson(engine, stringId, name);

        file.print(json);
        file.close();

        updateIndex(stringId, name, engine.shots.size(), engine.stats.avgFps, engine.stats.esFps, engine.pellet.brand + " " + engine.pellet.model);

        return stringId;
    }

    // Get list of saved strings in JSON format
    String listSavedStrings() {
        if (!isMounted || !LittleFS.exists("/strings/index.json")) {
            return "[]";
        }
        File file = LittleFS.open("/strings/index.json", "r");
        if (!file) return "[]";
        String content = file.readString();
        file.close();
        return content.length() > 0 ? content : "[]";
    }

    // Load full shot string JSON by ID
    String getShotStringJson(String stringId) {
        if (!isMounted) return "{}";
        String filename = "/strings/" + stringId + ".json";
        if (!LittleFS.exists(filename)) return "{}";

        File file = LittleFS.open(filename, "r");
        if (!file) return "{}";
        String content = file.readString();
        file.close();
        return content;
    }

    // Delete saved shot string
    bool deleteShotString(String stringId) {
        if (!isMounted) return false;
        String filename = "/strings/" + stringId + ".json";
        if (LittleFS.exists(filename)) {
            LittleFS.remove(filename);
        }
        removeFromIndex(stringId);
        return true;
    }

    // Format active string into CSV text
    String generateCSV(const ChronoEngine& engine) {
        String csv = "Shot,Speed (FPS),Speed (m/s),Energy (ft-lbs),Energy (J),Delta Avg (FPS),Time (us)\r\n";
        for (size_t i = 0; i < engine.shots.size(); i++) {
            const ShotData& s = engine.shots[i];
            csv += String(s.shotNumber) + ",";
            csv += String(s.speedFps, 2) + ",";
            csv += String(s.speedMps, 2) + ",";
            csv += String(s.energyFtLbs, 2) + ",";
            csv += String(s.energyJoules, 2) + ",";
            csv += String(s.deltaAvgFps, 2) + ",";
            csv += String(s.timeMicros, 1) + "\r\n";
        }
        return csv;
    }

    // Build JSON representation of active or recorded string
    String stringToJson(const ChronoEngine& engine, String id = "", String name = "") {
        String json = "{\n";
        json += "  \"id\": \"" + (id.length() > 0 ? id : String(millis())) + "\",\n";
        json += "  \"name\": \"" + escapeJson(name.length() > 0 ? name : engine.stringName) + "\",\n";
        json += "  \"timestamp\": " + String(millis()) + ",\n";
        json += "  \"gateDistanceMm\": " + String(engine.gateDistanceMm, 2) + ",\n";
        json += "  \"avgWindowSize\": " + String(engine.avgWindowSize) + ",\n";
        json += "  \"emitterTimeoutMin\": " + String(engine.emitterTimeoutMin) + ",\n";
        json += "  \"emittersPowered\": " + String(engine.emittersPowered ? "true" : "false") + ",\n";
        
        json += "  \"pellet\": {\n";
        json += "    \"brand\": \"" + escapeJson(engine.pellet.brand) + "\",\n";
        json += "    \"model\": \"" + escapeJson(engine.pellet.model) + "\",\n";
        json += "    \"caliber\": \"" + escapeJson(engine.pellet.caliber) + "\",\n";
        json += "    \"weightGrains\": " + String(engine.pellet.weightGrains, 3) + ",\n";
        json += "    \"weightGrams\": " + String(engine.pellet.weightGrams, 4) + "\n";
        json += "  },\n";

        json += "  \"stats\": {\n";
        json += "    \"totalShots\": " + String(engine.stats.totalShots) + ",\n";
        json += "    \"avgFps\": " + String(engine.stats.avgFps, 2) + ",\n";
        json += "    \"avgMps\": " + String(engine.stats.avgMps, 2) + ",\n";
        json += "    \"minFps\": " + String(engine.stats.minFps, 2) + ",\n";
        json += "    \"minMps\": " + String(engine.stats.minMps, 2) + ",\n";
        json += "    \"maxFps\": " + String(engine.stats.maxFps, 2) + ",\n";
        json += "    \"maxMps\": " + String(engine.stats.maxMps, 2) + ",\n";
        json += "    \"esFps\": " + String(engine.stats.esFps, 2) + ",\n";
        json += "    \"esMps\": " + String(engine.stats.esMps, 2) + ",\n";
        json += "    \"sdFps\": " + String(engine.stats.sdFps, 2) + ",\n";
        json += "    \"sdMps\": " + String(engine.stats.sdMps, 2) + ",\n";
        json += "    \"avgEnergyFtLbs\": " + String(engine.stats.avgEnergyFtLbs, 2) + ",\n";
        json += "    \"avgEnergyJoules\": " + String(engine.stats.avgEnergyJoules, 2) + ",\n";
        json += "    \"rollingAvgFps\": " + String(engine.stats.rollingAvgFps, 2) + ",\n";
        json += "    \"rollingAvgMps\": " + String(engine.stats.rollingAvgMps, 2) + ",\n";
        json += "    \"rollingAvgFtLbs\": " + String(engine.stats.rollingAvgFtLbs, 2) + ",\n";
        json += "    \"rollingAvgJoules\": " + String(engine.stats.rollingAvgJoules, 2) + "\n";
        json += "  },\n";

        json += "  \"shots\": [\n";
        for (size_t i = 0; i < engine.shots.size(); i++) {
            const ShotData& s = engine.shots[i];
            json += "    {";
            json += "\"num\":" + String(s.shotNumber) + ",";
            json += "\"fps\":" + String(s.speedFps, 2) + ",";
            json += "\"mps\":" + String(s.speedMps, 2) + ",";
            json += "\"ftLbs\":" + String(s.energyFtLbs, 2) + ",";
            json += "\"joules\":" + String(s.energyJoules, 2) + ",";
            json += "\"deltaFps\":" + String(s.deltaAvgFps, 2) + ",";
            json += "\"deltaMps\":" + String(s.deltaAvgMps, 2) + ",";
            json += "\"timeUs\":" + String(s.timeMicros, 1);
            json += "}";
            if (i + 1 < engine.shots.size()) json += ",";
            json += "\n";
        }
        json += "  ]\n";
        json += "}";
        return json;
    }

private:
    String escapeJson(String s) {
        s.replace("\\", "\\\\");
        s.replace("\"", "\\\"");
        s.replace("\n", "\\n");
        s.replace("\r", "\\r");
        return s;
    }

    String extractJsonValue(const String& json, const String& key) {
        int keyIndex = json.indexOf("\"" + key + "\"");
        if (keyIndex == -1) return "";
        int colonIndex = json.indexOf(':', keyIndex);
        if (colonIndex == -1) return "";
        
        int startVal = colonIndex + 1;
        while (startVal < (int)json.length() && (json[startVal] == ' ' || json[startVal] == '\t' || json[startVal] == '\n' || json[startVal] == '\r')) {
            startVal++;
        }

        if (startVal >= (int)json.length()) return "";

        if (json[startVal] == '\"') {
            int endVal = json.indexOf('\"', startVal + 1);
            if (endVal == -1) return "";
            return json.substring(startVal + 1, endVal);
        } else {
            int endVal = startVal;
            while (endVal < (int)json.length() && json[endVal] != ',' && json[endVal] != '}' && json[endVal] != '\n' && json[endVal] != '\r' && json[endVal] != ' ') {
                endVal++;
            }
            return json.substring(startVal, endVal);
        }
    }

    void updateIndex(String id, String name, int count, float avgFps, float esFps, String pelletSummary) {
        String indexStr = listSavedStrings();
        // Remove trailing ']' if present
        indexStr.trim();
        if (indexStr.startsWith("[") && indexStr.endsWith("]")) {
            indexStr = indexStr.substring(1, indexStr.length() - 1);
            indexStr.trim();
        }

        String item = "{";
        item += "\"id\":\"" + id + "\",";
        item += "\"name\":\"" + escapeJson(name) + "\",";
        item += "\"count\":" + String(count) + ",";
        item += "\"avgFps\":" + String(avgFps, 1) + ",";
        item += "\"esFps\":" + String(esFps, 1) + ",";
        item += "\"pellet\":\"" + escapeJson(pelletSummary) + "\",";
        item += "\"time\":" + String(millis());
        item += "}";

        if (indexStr.length() > 0) {
            indexStr = "[" + item + "," + indexStr + "]";
        } else {
            indexStr = "[" + item + "]";
        }

        File file = LittleFS.open("/strings/index.json", "w");
        if (file) {
            file.print(indexStr);
            file.close();
        }
    }

    void removeFromIndex(String id) {
        String indexStr = listSavedStrings();
        if (indexStr == "[]" || indexStr.length() == 0) return;

        int startPos = 0;
        String newIndex = "[";
        bool first = true;

        while ((startPos = indexStr.indexOf("{\"id\":\"", startPos)) != -1) {
            int endPos = indexStr.indexOf("}", startPos);
            if (endPos == -1) break;
            String item = indexStr.substring(startPos, endPos + 1);
            if (item.indexOf("\"id\":\"" + id + "\"") == -1) {
                if (!first) newIndex += ",";
                newIndex += item;
                first = false;
            }
            startPos = endPos + 1;
        }
        newIndex += "]";

        File file = LittleFS.open("/strings/index.json", "w");
        if (file) {
            file.print(newIndex);
            file.close();
        }
    }
};

#endif // STORAGE_MANAGER_H
