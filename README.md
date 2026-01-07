# embedded-json-serializer
Arduino JSON serialization library
# JSON Serialization Library for Embedded w‑M‑Bus Firmware (Arduino + Wokwi)

## Project Overview

This project implements an **embedded‑friendly JSON serialization library** that converts structured smart‑meter data into a **strictly predefined JSON format**.

The assignment simulates a typical smart‑meter / w‑M‑Bus firmware data pipeline:

```
Measurement data → internal data structures → JSON serialization → gateway / backend
```

⚠️ Radio communication, OMS, encryption, and real meter interaction are **out of scope**.
The focus is on **clean architecture, deterministic behavior, and embedded suitability**.

---

## Chosen Platform & Language

### Platform

**Arduino framework using Wokwi online simulator (ESP32 target)**

**Justification:**

* Widely used embedded prototyping environment
* Hardware‑independent online simulation via Wokwi
* ESP32 provides sufficient RAM for JSON serialization
* Easy demonstration via Serial Monitor
* No OS or filesystem dependency

### Programming Language

**C++ (C‑style embedded programming)**

**Reasons:**

* Arduino core is C++ based
* Allows strict memory control
* No dynamic allocation required
* Suitable for production embedded firmware

---

## Project Structure

```
/
├── sketch.ino              # Demo application
├── meter_model.h           # Internal data model definitions
├── json_serializer.h       # Public serialization API
├── json_serializer.cpp     # Serialization implementation
├── diagram.json            # Wokwi simulator config (no hardware)
└── README.md
```

---

## Internal Data Model

### Gateway Metadata

```c
typedef struct {
    const char *gatewayId;
    const char *date;              // YYYY-MM-DD
    const char *deviceType;
    int interval_minutes;
    int total_readings;
} GatewayMeta;
```

### Data Point

```c
typedef struct {
    const char *timestamp;         // YYYY-MM-DD HH:MM
    const char *meter_datetime;
    double total_m3;
    const char *status;
} DataPoint;
```

### Device Reading

```c
typedef struct {
    const char *media;
    const char *meter;
    const char *deviceId;
    const char *unit;

    DataPoint *data;
    int data_count;
} DeviceReading;
```

### Values Container

```c
typedef struct {
    int device_count;
    DeviceReading *readings;
} Values;
```

### Top‑Level Data Model

```c
typedef struct {
    GatewayMeta meta;
    Values values;
} MeterData;
```

---

## Public API

### Function Prototype

```c
JsonStatus serialize_to_json(const MeterData *input,
                             char *output_buffer,
                             int buffer_size,
                             int *required_size);
```

### Behavior

* Serializes structured data into the **mandatory JSON format**
* Caller provides output buffer
* Prevents buffer overflow using `snprintf`
* Returns required JSON size
* Transport‑agnostic (no UART, MQTT, radio, etc.)

### Return Codes

```c
typedef enum {
    JSON_OK = 0,
    JSON_ERR_NULL_INPUT,
    JSON_ERR_BUFFER_TOO_SMALL
} JsonStatus;
```

---

## Mandatory JSON Output Format

The library generates **exactly** the following structure:

```json
[
  {
    "gatewayId": "gateway_1234",
    "date": "1970-01-01",
    "deviceType": "stromleser",
    "interval_minutes": 15,
    "total_readings": 1,
    "values": {
      "device_count": 1,
      "readings": [
        {
          "media": "water",
          "meter": "waterstarm",
          "deviceId": "stromleser_50898527",
          "unit": "m3",
          "data": [
            {
              "timestamp": "1970-01-01 00:00",
              "meter_datetime": "1970-01-01 00:00",
              "total_m3": 107.752,
              "status": "OK"
            }
          ]
        }
      ]
    }
  }
]
```

✔ Field names unchanged
✔ Structure unchanged
✔ Numbers serialized as numbers
✔ Outer element always an array

---

## Build & Run Instructions (Wokwi)

1. Open [https://wokwi.com](https://wokwi.com)
2. Create **New Arduino Project**
3. Select **ESP32 (Arduino)** board
4. Add the following files:

   * `meter_model.h`
   * `json_serializer.h`
   * `json_serializer.cpp`
5. Paste `sketch.ino` demo code
6. Click ▶ **Start Simulation**
7. Open **Serial Monitor**

The JSON output will appear every 4 seconds.

---

## Maximum JSON Size

The worst‑case JSON size depends on:

* Number of devices
* Number of data points per device
* Length of string fields

For the provided demo:

* Buffer size used: **1024 bytes**
* Required JSON size: ~**500 bytes**

The serializer reports the exact required size via `required_size`.

---

## Design Decisions & Assumptions

* No dynamic memory allocation (`malloc` avoided)
* No Arduino `String` class used
* `snprintf` ensures buffer safety
* Data provided via constant strings
* Library is fully transport‑agnostic

---

## Possible Extensions

* Support multiple devices and datapoints
* PROGMEM optimization for AVR boards
* Floating‑point formatting control
* Binary‑to‑JSON gateway integration
* Unit tests using native C++ environment

---

## License

This project is provided for educational and evaluation purposes.

---

## Author

**Embedded / Firmware Assignment – JSON Serialization**

Designed for smart‑meter and w‑M‑Bus firmware preparation.
