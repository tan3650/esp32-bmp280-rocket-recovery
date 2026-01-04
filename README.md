# esp32-bmp280-rocket-recovery
A basic rocket apogee detection and recovery algorithm using ESP32 and BMP280 pressure sensor. The code tracks atmospheric pressure changes to detect the highest point (apogee) in a small rocket's flight, and triggers a pin to deploy a recovery system (e.g. parachute).

# Overview
- Uses atmospheric pressure readings to detect ascent and descent.
- Averages pressure differences to reduce sensor noise.
- Detects apogee when pressure trend switches from decreasing to increasing.
- Activates recovery system using a digital output pin.
- Outputs real-time altitude data over serial.

# Hardware Required
- ESP32 Dev Module
- BMP280 (I2C)
- Recovery system
- LED for visual indication (optional)
- Buzzer for audio indication

# Wiring
| BMP280 Pin | ESP32 Pin |
|------------|-----------|
| VCC        | 3.3V      |
| GND        | GND       |
| SCL        | GPIO 22   |
| SDA        | GPIO 21   |
- presspin (GPIO 21): triggers the recovery system
- ledpin (GPIO 35): LED indicator

# Libraries Required (Arduino IDE)
- Adafruit BMP280
- Adafruit Unified Sensor
- Wire.h


# How It Works
1. Pressure Sampling: Reads and averages pressure differences during ascent and descent phases.
2. Apogee Detection:
   - During ascent, pressure drops steadily.
   - During descent, pressure starts increasing.
   - If average pressure difference reverses and crosses a defined threshold, the system assumes apogee has been reached.
3.Recovery Trigger:
   - Once apogee is confirmed over a set duration, the ESP32 activates `presspin` and `ledpin` for a few seconds to deploy the recovery system.

# Parameters You Can Tune
| Variable  | Value | Purpose |
|-----------|-------|---------|
| constred  | 65    | Number of readings to average |
| a         | 60    | Minimum pressure difference threshold |
| t         | 30    | Number of confirmations before triggering recovery |

# Serial Output
- Altitude readings are printed via 'Serial' at 115200 baud.
- Debugging messages (like "apogee detected") are printed.
