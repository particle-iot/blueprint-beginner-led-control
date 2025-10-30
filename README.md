# Blueprint – Beginner – LED Control (via Cloud Function)

**Repo:** `blueprint-beginner-led-control`  
**Difficulty:** Beginner  
**Estimated Time:** 15–25 minutes  
**Hardware Needed:** None (uses onboard LED)  
**Particle Features:** `Particle.function()`, `GPIO (digitalWrite)`, `Particle.publish()`, `Console`

---

## Overview
Control the onboard LED from the Particle Console with a cloud function, and publish the current LED state to the cloud.  
The function accepts `"ON"`, `"OFF"`, or `"TOGGLE"`; after updating the LED, the device publishes a private event `led_state` with `"ON"` or `"OFF"`.

---

## Tools & Materials
- Any Particle device (Photon 2, Boron, etc.)  
- USB cable  
- [Particle Workbench](https://docs.particle.io/workbench/) or [Particle CLI](https://docs.particle.io/tutorials/developer-tools/cli/)  
- A Particle account with a claimed device  

---

## Steps
1. **Clone this repository:**
   ```bash
   git clone https://github.com/<your-org>/blueprint-beginner-led-control.git
   cd blueprint-beginner-led-control
   ```
2. **Open the project** in Particle Workbench or your preferred editor.  
3. **Flash the firmware** to your device:
   ```bash
   particle flash <device-name>
   ```
4. **Call the cloud function** from the Particle Console:
   - Navigate to **Functions**, select `setLED`
   - Use one of: `ON`, `OFF`, or `TOGGLE`
5. **Verify LED behavior** on the device (onboard LED).  
6. **Check Console → Logs** to see the `led_state` event with `"ON"` or `"OFF"`.

---

## How It Works
- The firmware registers a cloud function `setLED` in `setup()`.  
- When called with `"ON"`, `"OFF"`, or `"TOGGLE"`, it updates the LED pin and internal `ledOn` flag.  
- It then publishes a private event `led_state` with the new state.  
- Using `SYSTEM_THREAD(ENABLED)` keeps OTA responsive.

---

## Source Code
Main firmware: [`/src/led-control.cpp`](src/led-control.cpp)  
Fully commented so it can serve as standalone documentation.

---

## Expected Output
**Console logs (Events):**
```
Event: led_state
Data: ON
```

**Serial logs (from `particle serial monitor --follow`):**
```
[app] INFO: LED state changed: ON
[app] INFO: Published led_state: ON
```

---

## Extensions
- Add a second function `getLED` that only publishes current state.  
- Debounce `TOGGLE` with a rate-limit to avoid rapid changes.  
- Mirror state to an RGB LED pattern.
