/*
 * Project: LED Control via Cloud Function
 * Repo: blueprint-beginner-led-control
 * Author: Particle
 * Date: 2025-04-20
 *
 * Description:
 *  Beginner blueprint that shows how to control an LED from the Particle Console.
 *  The device registers a cloud function `setLED` that accepts "ON", "OFF", or
 *  "TOGGLE". After updating the LED, the device publishes the current LED state
 *  to the cloud as a private event `led_state` with data "ON" or "OFF".
 *
 * Quickstart:
 *   1) Flash this firmware to your device.
 *   2) In the Console → Functions, call `setLED` with "ON", "OFF", or "TOGGLE".
 *   3) Observe the LED change and check Console → Logs for event `led_state`.
 *
 * Docs:
 *   - Cloud Function reference:
 *     https://docs.particle.io/reference/device-os/firmware/#particle-function-
 *   - Publish reference:
 *     https://docs.particle.io/reference/device-os/firmware/#particle-publish-
 *   - GPIO reference:
 *     https://docs.particle.io/reference/device-os/firmware/#digitalwrite-
 */

#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);   // Manage cloud connection automatically
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED); // System thread defaults to on in 6.2.0 and later and this line is not required
#endif
SerialLogHandler logHandler(LOG_LEVEL_INFO); // View logs with: particle serial monitor --follow

// -------------------------------------------------------------------
// Hardware configuration
// -------------------------------------------------------------------
// Use the onboard LED connected to D7 if available or change it to a different pin
#define LED_PIN D7

// Track current LED state in firmware (true = ON, false = OFF)
bool ledOn = false;

// Forward declaration for cloud function handler
int setLedHandler(String command);

// Helper: publish current LED state as "ON" or "OFF"
void publishLedState() {
    const char* state = ledOn ? "ON" : "OFF";
    // PRIVATE so it won't appear on the public firehose
    Particle.publish("led_state", state);
    Log.info("Published led_state: %s", state);
}

// -------------------------------------------------------------------
// setup() — runs once on boot
// -------------------------------------------------------------------
void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledOn = false;

    // Register cloud function available in Console → Functions
    Particle.function("setLED", setLedHandler);

    Log.info("Setup complete. Cloud function 'setLED' ready. LED initialized OFF.");
    // Optional: publish initial state for visibility
    publishLedState();
}

// -------------------------------------------------------------------
// loop() — runs repeatedly
// -------------------------------------------------------------------
void loop() {
    // No continuous work required; we react to cloud function calls.
    delay(50);
}

// -------------------------------------------------------------------
// setLedHandler() — Cloud Function
// -------------------------------------------------------------------
// Accepts: "ON", "OFF", "TOGGLE" (case-insensitive)
// Behavior:
//   - Updates LED output accordingly
//   - Publishes `led_state` event with "ON"/"OFF"
// Returns:
//   1  on success
//  -1  on invalid argument
int setLedHandler(String command) {
    // Normalize command to uppercase for robust parsing
    command.trim();
    command.toUpperCase();

    if (command == "ON") {
        ledOn = true;
        digitalWrite(LED_PIN, HIGH);
    } else if (command == "OFF") {
        ledOn = false;
        digitalWrite(LED_PIN, LOW);
    } else if (command == "TOGGLE") {
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    } else {
        Log.warn("Invalid setLED argument: %s (expected ON|OFF|TOGGLE)", command.c_str());
        return -1;
    }

    Log.info("LED state changed: %s", ledOn ? "ON" : "OFF");
    publishLedState();
    return 1;
}
