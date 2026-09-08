#ifndef CHRONO_ENGINE_H
#define CHRONO_ENGINE_H

#include <Arduino.h>
#include <vector>
#include <cmath>
#include "driver/gpio.h"
#include "config.h"

// Gate State Machine Status
enum ChronoState {
    CHRONO_READY = 0,
    CHRONO_GATE1_TRIGGERED,
    CHRONO_SHOT_CAPTURED,
    CHRONO_TIMEOUT,
    CHRONO_SLEEP
};

struct ShotData {
    uint16_t shotNumber;
    uint32_t timestampMs;       // Session relative time
    uint32_t deltaCycles;       // ESP32 240MHz cycle count between gates
    float timeMicros;           // Time between gates in microseconds
    float speedMps;             // Meters per second
    float speedFps;             // Feet per second
    float energyJoules;         // Joules
    float energyFtLbs;          // Foot-pounds
    float deltaAvgFps;          // Difference from current string average
    float deltaAvgMps;          // Difference from current string average
};

struct PelletProfile {
    String brand;
    String model;
    String caliber;
    float weightGrains;
    float weightGrams;

    void setWeightGrains(float gr) {
        weightGrains = gr;
        weightGrams = gr * GRAINS_TO_GRAMS;
    }

    void setWeightGrams(float g) {
        weightGrams = g;
        weightGrains = g * GRAMS_TO_GRAINS;
    }
};

struct StringStatistics {
    uint16_t totalShots;
    float avgFps;
    float avgMps;
    float minFps;
    float minMps;
    float maxFps;
    float maxMps;
    float esFps;             // Extreme Spread (Max - Min)
    float esMps;
    float sdFps;             // Standard Deviation
    float sdMps;
    float avgEnergyFtLbs;
    float avgEnergyJoules;
    float rollingAvgFps;     // Rolling window average
    float rollingAvgMps;
    float rollingAvgFtLbs;
    float rollingAvgJoules;
};

// Global volatile ISR variables (prevents Xtensa literal pool relocation issues)
static volatile ChronoState chronoState = CHRONO_READY;
static volatile uint32_t chronoGate1Cycles = 0;
static volatile uint32_t chronoGate2Cycles = 0;
static volatile uint64_t chronoGate1Micros = 0;
static volatile uint64_t chronoGate2Micros = 0;
static volatile bool chronoNewShotAvailable = false;

// Standalone ISR functions with ARDUINO_ISR_ATTR
static void ARDUINO_ISR_ATTR isrGate1Handler() {
    if (chronoState == CHRONO_READY) {
        chronoGate1Cycles = ESP.getCycleCount();
        chronoGate1Micros = esp_timer_get_time();
        chronoState = CHRONO_GATE1_TRIGGERED;
    }
}

static void ARDUINO_ISR_ATTR isrGate2Handler() {
    if (chronoState == CHRONO_GATE1_TRIGGERED) {
        chronoGate2Cycles = ESP.getCycleCount();
        chronoGate2Micros = esp_timer_get_time();
        chronoState = CHRONO_SHOT_CAPTURED;
        chronoNewShotAvailable = true;
    }
}

class ChronoEngine {
public:
    PelletProfile pellet;
    float gateDistanceMm;
    int triggerMode;
    uint16_t avgWindowSize;
    uint16_t emitterTimeoutMin;
    bool emittersPowered;
    uint32_t lastActivityMs;
    uint64_t lastCaptureTimeMicros;
    ChronoState lastReportedState;
    String stringName;
    uint32_t stringStartTime;

    std::vector<ShotData> shots;
    StringStatistics stats;

    ChronoEngine() {
        gateDistanceMm = DEFAULT_GATE_DISTANCE_MM;
        triggerMode = DEFAULT_TRIGGER_MODE;
        avgWindowSize = DEFAULT_AVG_WINDOW;
        emitterTimeoutMin = DEFAULT_EMITTER_TIMEOUT_MIN;
        emittersPowered = true;
        lastActivityMs = 0;
        lastCaptureTimeMicros = 0;
        lastReportedState = CHRONO_READY;
        stringName = "Session 1";
        stringStartTime = 0;

        pellet.brand = DEFAULT_PELLET_BRAND;
        pellet.model = DEFAULT_PELLET_MODEL;
        pellet.caliber = DEFAULT_CALIBER;
        pellet.setWeightGrains(DEFAULT_PELLET_GRAINS);

        resetStatistics();
    }

    void begin() {
        pinMode(PIN_GATE_1, INPUT);
        pinMode(PIN_GATE_2, INPUT);
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, HIGH); // LED off (active low)

        pinMode(PIN_IR_EMITTER, OUTPUT);
        pinMode(PIN_IR_EMITTER_GND, OUTPUT);
        gpio_set_drive_capability((gpio_num_t)PIN_IR_EMITTER, GPIO_DRIVE_CAP_3);
        gpio_set_drive_capability((gpio_num_t)PIN_IR_EMITTER_GND, GPIO_DRIVE_CAP_3);
        digitalWrite(PIN_IR_EMITTER_GND, LOW); // D0 provides ground (LOW) for emitter LEDs
        setEmitters(true); // Power on IR emitters on boot

        attachGateInterrupts();
        resetSession();
    }

    void setEmitters(bool enable) {
        emittersPowered = enable;
        digitalWrite(PIN_IR_EMITTER, enable ? HIGH : LOW);
        digitalWrite(PIN_IR_EMITTER_GND, LOW); // Maintain D0 as ground return
        if (enable) {
            refreshActivity();
            if (chronoState == CHRONO_SLEEP) {
                chronoState = CHRONO_READY;
            }
        } else {
            chronoState = CHRONO_SLEEP;
        }
    }

    void refreshActivity() {
        lastActivityMs = millis();
        if (!emittersPowered) {
            setEmitters(true);
        }
    }

    void attachGateInterrupts() {
        detachInterrupt(digitalPinToInterrupt(PIN_GATE_1));
        detachInterrupt(digitalPinToInterrupt(PIN_GATE_2));
        
        attachInterrupt(digitalPinToInterrupt(PIN_GATE_1), isrGate1Handler, triggerMode);
        attachInterrupt(digitalPinToInterrupt(PIN_GATE_2), isrGate2Handler, triggerMode);
    }

    ChronoState getState() const {
        return chronoState;
    }

    void update() {
        uint64_t nowMicros = esp_timer_get_time();

        // Debug report when Gate 1 is triggered
        if (chronoState == CHRONO_GATE1_TRIGGERED && lastReportedState != CHRONO_GATE1_TRIGGERED) {
            Serial.println(F("[CHRONO] Gate 1 triggered -> timer started, awaiting Gate 2..."));
            lastReportedState = CHRONO_GATE1_TRIGGERED;
        }

        // Check for gate 1 trigger timeout (e.g. projectile didn't hit gate 2 or drop was too slow)
        if (chronoState == CHRONO_GATE1_TRIGGERED) {
            if (nowMicros - chronoGate1Micros > SHOT_TIMEOUT_MICROS) {
                Serial.printf("[CHRONO] Timeout: Gate 2 not reached within %lu ms. Discarding.\n",
                              (unsigned long)(SHOT_TIMEOUT_MICROS / 1000UL));
                chronoState = CHRONO_TIMEOUT;
                lastCaptureTimeMicros = nowMicros;
                lastReportedState = CHRONO_TIMEOUT;
            }
        }

        // Process newly captured shot
        if (chronoNewShotAvailable && chronoState == CHRONO_SHOT_CAPTURED) {
            processCapturedShot();
            chronoNewShotAvailable = false;
            refreshActivity();
            lastCaptureTimeMicros = esp_timer_get_time();
            lastReportedState = CHRONO_SHOT_CAPTURED;
            
            // LED flash to give physical feedback
            digitalWrite(PIN_STATUS_LED, LOW);
            delayMicroseconds(2000);
            digitalWrite(PIN_STATUS_LED, HIGH);
        }

        // Re-arm state machine only when dead-time has elapsed AND both optical gates are clear
        if (chronoState == CHRONO_SHOT_CAPTURED || chronoState == CHRONO_TIMEOUT) {
            bool deadtimePassed = (nowMicros - lastCaptureTimeMicros >= MIN_SHOT_INTERVAL_MICROS);
            bool g1Clear = (digitalRead(PIN_GATE_1) != (triggerMode == FALLING ? LOW : HIGH));
            bool g2Clear = (digitalRead(PIN_GATE_2) != (triggerMode == FALLING ? LOW : HIGH));
            bool safetyTimeout = (nowMicros - lastCaptureTimeMicros >= 1500000ULL); // 1.5s safety override

            if ((deadtimePassed && g1Clear && g2Clear) || safetyTimeout) {
                chronoGate1Cycles = 0;
                chronoGate2Cycles = 0;
                chronoState = CHRONO_READY;
                if (lastReportedState != CHRONO_READY) {
                    Serial.println(F("[CHRONO] Gates clear & armed. Ready for next shot."));
                    lastReportedState = CHRONO_READY;
                }
            }
        }

        // Power-saving idle timeout check
        if (emitterTimeoutMin > 0 && emittersPowered) {
            if (millis() - lastActivityMs > (emitterTimeoutMin * 60000UL)) {
                setEmitters(false); // turn off emitters to save power
            }
        }
    }

    void processCapturedShot() {
        uint32_t deltaCycles = 0;
        if (chronoGate2Cycles >= chronoGate1Cycles) {
            deltaCycles = chronoGate2Cycles - chronoGate1Cycles;
        } else {
            // Cycle counter rollover (32-bit uint)
            deltaCycles = (0xFFFFFFFF - chronoGate1Cycles) + chronoGate2Cycles + 1;
        }

        // ESP32 CPU frequency (typically 240 MHz on S3)
        float cpuFreqHz = (float)ESP.getCpuFreqMHz() * 1000000.0f;
        float elapsedSeconds = (float)deltaCycles / cpuFreqHz;
        float timeMicros = elapsedSeconds * 1000000.0f;

        // Fallback sanity check using esp_timer if cycle count looks anomalous
        if (elapsedSeconds <= 0.000005f || elapsedSeconds > 0.5f) {
            timeMicros = (float)(chronoGate2Micros - chronoGate1Micros);
            elapsedSeconds = timeMicros / 1000000.0f;
        }

        if (elapsedSeconds <= 0.000005f) return; // Prevent division by zero or unrealistic speeds

        float distanceMeters = gateDistanceMm / 1000.0f;
        float speedMps = distanceMeters / elapsedSeconds;
        float speedFps = speedMps * MPS_TO_FPS;

        // Energy calculations:
        // E = 0.5 * m * v^2
        // In metric: mass in kg, velocity in m/s -> Joules
        float massKg = pellet.weightGrams / 1000.0f;
        float energyJoules = 0.5f * massKg * (speedMps * speedMps);

        // In imperial: (grains * fps * fps) / 450240 -> ft-lbs
        float energyFtLbs = (pellet.weightGrains * speedFps * speedFps) / 450240.0f;

        Serial.printf("[CHRONO] Shot #%u captured! Time: %.1f us (%.2f ms) | Speed: %.1f FPS (%.2f m/s)\n",
                      (unsigned int)(shots.size() + 1), timeMicros, timeMicros / 1000.0f, speedFps, speedMps);

        ShotData shot;
        shot.shotNumber = shots.size() + 1;
        shot.timestampMs = millis() - stringStartTime;
        shot.deltaCycles = deltaCycles;
        shot.timeMicros = timeMicros;
        shot.speedMps = speedMps;
        shot.speedFps = speedFps;
        shot.energyJoules = energyJoules;
        shot.energyFtLbs = energyFtLbs;
        shot.deltaAvgFps = 0.0f;
        shot.deltaAvgMps = 0.0f;

        addShot(shot);
    }

    void addShot(ShotData shot) {
        if (shots.size() >= MAX_SHOTS_PER_STRING) {
            shots.erase(shots.begin()); // keep buffer bounded
        }
        shots.push_back(shot);
        recalculateStatistics();
    }

    void deleteShot(uint16_t index) {
        if (index < shots.size()) {
            shots.erase(shots.begin() + index);
            // Re-number remaining shots
            for (size_t i = 0; i < shots.size(); i++) {
                shots[i].shotNumber = i + 1;
            }
            recalculateStatistics();
        }
    }

    void resetSession(String newName = "") {
        shots.clear();
        stringStartTime = millis();
        if (newName.length() > 0) {
            stringName = newName;
        }
        resetStatistics();
        refreshActivity();
        lastCaptureTimeMicros = esp_timer_get_time();
        lastReportedState = CHRONO_READY;
        chronoState = CHRONO_READY;
    }

    void resetStatistics() {
        stats.totalShots = 0;
        stats.avgFps = 0;
        stats.avgMps = 0;
        stats.minFps = 0;
        stats.minMps = 0;
        stats.maxFps = 0;
        stats.maxMps = 0;
        stats.esFps = 0;
        stats.esMps = 0;
        stats.sdFps = 0;
        stats.sdMps = 0;
        stats.avgEnergyFtLbs = 0;
        stats.avgEnergyJoules = 0;
        stats.rollingAvgFps = 0;
        stats.rollingAvgMps = 0;
        stats.rollingAvgFtLbs = 0;
        stats.rollingAvgJoules = 0;
    }

    void recalculateStatistics() {
        size_t n = shots.size();
        stats.totalShots = n;

        if (n == 0) {
            resetStatistics();
            return;
        }

        float sumFps = 0;
        float sumMps = 0;
        float sumFtLbs = 0;
        float sumJoules = 0;
        float minF = shots[0].speedFps;
        float maxF = shots[0].speedFps;
        float minM = shots[0].speedMps;
        float maxM = shots[0].speedMps;

        for (size_t i = 0; i < n; i++) {
            float f = shots[i].speedFps;
            float m = shots[i].speedMps;
            sumFps += f;
            sumMps += m;
            sumFtLbs += shots[i].energyFtLbs;
            sumJoules += shots[i].energyJoules;

            if (f < minF) minF = f;
            if (f > maxF) maxF = f;
            if (m < minM) minM = m;
            if (m > maxM) maxM = m;
        }

        stats.avgFps = sumFps / n;
        stats.avgMps = sumMps / n;
        stats.minFps = minF;
        stats.maxFps = maxF;
        stats.minMps = minM;
        stats.maxMps = maxM;
        stats.esFps = maxF - minF;
        stats.esMps = maxM - minM;
        stats.avgEnergyFtLbs = sumFtLbs / n;
        stats.avgEnergyJoules = sumJoules / n;

        // Calculate Standard Deviation (Sample SD)
        if (n > 1) {
            float varianceFps = 0;
            float varianceMps = 0;
            for (size_t i = 0; i < n; i++) {
                varianceFps += (shots[i].speedFps - stats.avgFps) * (shots[i].speedFps - stats.avgFps);
                varianceMps += (shots[i].speedMps - stats.avgMps) * (shots[i].speedMps - stats.avgMps);
            }
            stats.sdFps = sqrt(varianceFps / (n - 1));
            stats.sdMps = sqrt(varianceMps / (n - 1));
        } else {
            stats.sdFps = 0;
            stats.sdMps = 0;
        }

        // Calculate Rolling Window Average (last N shots)
        size_t window = avgWindowSize;
        if (window > n) window = n;
        float rollingSumFps = 0;
        float rollingSumMps = 0;
        float rollingSumFtLbs = 0;
        float rollingSumJoules = 0;
        
        for (size_t i = n - window; i < n; i++) {
            rollingSumFps += shots[i].speedFps;
            rollingSumMps += shots[i].speedMps;
            rollingSumFtLbs += shots[i].energyFtLbs;
            rollingSumJoules += shots[i].energyJoules;
        }
        stats.rollingAvgFps = rollingSumFps / window;
        stats.rollingAvgMps = rollingSumMps / window;
        stats.rollingAvgFtLbs = rollingSumFtLbs / window;
        stats.rollingAvgJoules = rollingSumJoules / window;

        // Update delta to average for each shot
        for (size_t i = 0; i < n; i++) {
            shots[i].deltaAvgFps = shots[i].speedFps - stats.avgFps;
            shots[i].deltaAvgMps = shots[i].speedMps - stats.avgMps;
        }
    }

    // Software simulated shot for end-to-end testing without shooting pellets
    void simulateShot(float targetFps = 780.0f, float varianceFps = 5.0f) {
        refreshActivity();
        float randVar = ((float)(random(-100, 101)) / 100.0f) * varianceFps;
        float simFps = targetFps + randVar;
        float simMps = simFps * FPS_TO_MPS;
        float distMeters = gateDistanceMm / 1000.0f;
        float elapsedSec = distMeters / simMps;
        float timeMicros = elapsedSec * 1000000.0f;

        float massKg = pellet.weightGrams / 1000.0f;
        float energyJoules = 0.5f * massKg * (simMps * simMps);
        float energyFtLbs = (pellet.weightGrains * simFps * simFps) / 450240.0f;

        ShotData shot;
        shot.shotNumber = shots.size() + 1;
        shot.timestampMs = millis() - stringStartTime;
        shot.deltaCycles = (uint32_t)(elapsedSec * (float)ESP.getCpuFreqMHz() * 1000000.0f);
        shot.timeMicros = timeMicros;
        shot.speedMps = simMps;
        shot.speedFps = simFps;
        shot.energyJoules = energyJoules;
        shot.energyFtLbs = energyFtLbs;
        shot.deltaAvgFps = 0.0f;
        shot.deltaAvgMps = 0.0f;

        addShot(shot);
    }
};

#endif // CHRONO_ENGINE_H
