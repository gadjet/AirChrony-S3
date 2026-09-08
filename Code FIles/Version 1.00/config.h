#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ==============================================================================
// HARDWARE PIN DEFINITIONS FOR SEED STUDIO XIAO ESP32-S3
// ==============================================================================
// Seeed Studio XIAO ESP32-S3 Pin Mapping:
// D0 -> GPIO 1  (Assigned to: IR Emitters Ground Sink - LOW)
// D1 -> GPIO 2  (Assigned to: IR Emitters Power Control via 2x 150Ω resistors - HIGH)
// D2 -> GPIO 3
// D3 -> GPIO 4
// D4 -> GPIO 5
// D5 -> GPIO 6
// D6 -> GPIO 43 (TX)
// D7 -> GPIO 44 (RX)
// D8 -> GPIO 7  (SCL)
// D9 -> GPIO 8  (Assigned to: Gate 2 Stop Sensor DOUT)
// D10-> GPIO 9  (Assigned to: Gate 1 Start Sensor DOUT)
// Built-in User LED: GPIO 21 (Active LOW on XIAO ESP32-S3)
// ==============================================================================

#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN_GATE_1          10  // D10 on XIAO C3 (GPIO 10) - First optical IR sensor (Start)
  #define PIN_GATE_2          9   // D9  on XIAO C3 (GPIO 9)  - Second optical IR sensor (Stop)
  #define PIN_IR_EMITTER      3   // D1  on XIAO C3 (GPIO 3)  - Power feed (HIGH) to 2x IR emitter LEDs
  #define PIN_IR_EMITTER_GND  2   // D0  on XIAO C3 (GPIO 2)  - Ground sink (LOW) for 2x IR emitter LEDs
  #define PIN_STATUS_LED      10  // Built-in LED on XIAO C3
#else
  #define PIN_GATE_1          9   // D10 on XIAO S3 (GPIO 9)  - First optical IR sensor (Start)
  #define PIN_GATE_2          8   // D9  on XIAO S3 (GPIO 8)  - Second optical IR sensor (Stop)
  #define PIN_IR_EMITTER      2   // D1  on XIAO S3 (GPIO 2)  - Power feed (HIGH) to 2x IR emitter LEDs via 150Ω
  #define PIN_IR_EMITTER_GND  1   // D0  on XIAO S3 (GPIO 1)  - Ground sink (LOW) for 2x IR emitter LEDs
  #define PIN_STATUS_LED      21  // On-board LED for visual shot / AP indication (Active LOW on XIAO S3)
#endif

// Default trigger interrupt mode:
// Most digital IR receiver boards (phototransistor + LM393 comparator) output LOW
// when the IR beam is interrupted (falling edge) or HIGH (rising edge).
#define DEFAULT_TRIGGER_MODE FALLING 

// ==============================================================================
// DEFAULT MEASUREMENT & POWER SAVING PARAMETERS
// ==============================================================================
#define DEFAULT_GATE_DISTANCE_MM    50.0f    // Distance between Gate 1 and Gate 2 in millimeters
#define MIN_GATE_DISTANCE_MM        10.0f
#define MAX_GATE_DISTANCE_MM        300.0f

#define SHOT_TIMEOUT_MICROS         500000UL // 500ms timeout (allows slow gravity drop testing and low velocity projectiles)
#define MIN_SHOT_INTERVAL_MICROS    50000UL  // 50ms dead-time after a shot before arming next

#define DEFAULT_EMITTER_TIMEOUT_MIN 10       // Emitter power-saving sleep timeout in minutes (0 = always on)

// ==============================================================================
// DEFAULT PELLET SETTINGS
// ==============================================================================
#define DEFAULT_PELLET_BRAND        "JSB"
#define DEFAULT_PELLET_MODEL        "Exact Diabolo"
#define DEFAULT_PELLET_GRAINS       8.44f
#define DEFAULT_CALIBER             ".177 (4.5mm)"
#define DEFAULT_AVG_WINDOW          10       // Default rolling average shot window

// Unit Conversion Constants
#define GRAINS_TO_GRAMS             0.06479891f
#define GRAMS_TO_GRAINS             15.4323584f
#define MPS_TO_FPS                  3.280839895f
#define FPS_TO_MPS                  0.3048f
#define JOULES_TO_FTLBS             0.737562149f
#define FTLBS_TO_JOULES             1.355817948f

// ==============================================================================
// WIFI & ACCESS POINT CONFIGURATION
// ==============================================================================
#define AP_SSID                     "AirChrony-S3"
#define AP_PASSWORD                 ""          // Open network by default for convenience
#define AP_CHANNEL                  1
#define AP_MAX_CONN                 4

#define MDNS_HOSTNAME               "chronograph" // Accessible at http://chronograph.local

#define MAX_SHOTS_PER_STRING        250         // Memory safety buffer for single string
#define MAX_SAVED_STRINGS           50          // Max string files stored in LittleFS

#endif // CONFIG_H
