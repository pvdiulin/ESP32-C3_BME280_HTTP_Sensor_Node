# ESP32-C3 BME280 HTTP Sensor Node

A lightweight IoT firmware for **ESP32-C3 SuperMini** that reads environmental data from a **BME280 sensor** and periodically sends measurements as JSON to a remote HTTP endpoint.

This project is designed for simple environmental monitoring scenarios such as balcony/weather nodes, indoor climate tracking, or distributed sensor systems.

---

## Features

- Temperature, humidity, and pressure measurement
- I²C BME280 sensor integration
- Wi-Fi auto connection with timeout protection
- HTTP JSON telemetry upload
- Retry logic for failed transmissions
- Sensor error detection
- Lightweight and stable runtime behavior
- PlatformIO build system

---

## Hardware Requirements

### Components

- ESP32-C3 SuperMini
- BME280 environmental sensor (I²C module)

### Wiring

| BME280 | ESP32-C3 |
|------------|--------------|
| VIN        | 5V  |
| GND        | GND |
| SDA        | GPIO 8 |
| SCL        | GPIO 9 |

> Most BME280 modules support both 3.3V and 5V input, but 3.3V is safest for ESP32 logic.

---

## Software Requirements

- VS Code
- PlatformIO extension
- ESP32 USB drivers

---

## ⚙ Installation

### 1. Clone the repository

```bash
git clone https://github.com/pvdiulin/thmetrix.git
cd thmetrix
```

### 2. Configure credentials

Create the file:

```
src/config.h
```

Example configuration:

```cpp
#pragma once

#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"

#define API_ENDPOINT "http://your-server/api/sensor"

#define LOCATION "location"

#define SEND_INTERVAL 30000
```

### 3. Build firmware

```bash
pio run
```

### 4. Upload to device

```bash
pio run -t upload
```

### 5. Monitor serial output

```bash
pio device monitor
```

---

## Example JSON Payload

```json
{
  "location": "balcony",
  "temperature": 23.6,
  "humidity": 44.8,
  "pressure": 1012.9
}
```

---

## Troubleshooting

### Sensor not detected

- Check wiring
- Confirm I²C address (0x76 or 0x77)
- Use 3.3V power
- Verify SDA/SCL pins

### Wi-Fi connection issues

- Confirm credentials
- Check signal strength
- Watch serial logs

### HTTP failures

- Verify API endpoint
- Ensure server is reachable
- Check firewall/network rules

---

## Possible Extensions

- MQTT telemetry publishing
- Deep sleep battery operation
- OTA firmware updates
- Web dashboard integration
- Home Assistant support
- Data buffering and batching

---

t
MIT License — free to use, modify, and distribute.

---

## Acknowledgements

- ESP32 Arduino framework
- Adafruit BME280 library
- ArduinoJson
- PlatformIO ecosystem

![ESP32-C3_SuperMini__BME280_Modules](https://raw.githubusercontent.com/pvdiulin/thmetrix/refs/heads/master/files/ESPBME.jpg)
![ESP32-C3_SuperMini__BME280_Modules_Reverse](https://raw.githubusercontent.com/pvdiulin/thmetrix/refs/heads/master/files/ESPBME_Reverse.jpg)
