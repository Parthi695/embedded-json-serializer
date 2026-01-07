#ifndef METER_MODEL_H
#define METER_MODEL_H

typedef struct {
    const char *gatewayId;
    const char *date;        // YYYY-MM-DD
    const char *deviceType;
    int interval_minutes;
    int total_readings;
} GatewayMeta;

typedef struct {
    const char *timestamp;      
    const char *meter_datetime;
    double total_m3;
    const char *status;
} DataPoint;

typedef struct {
    const char *media;
    const char *meter;
    const char *deviceId;
    const char *unit;

    DataPoint *data;
    int data_count;
} DeviceReading;

typedef struct {
    int device_count;
    DeviceReading *readings;
} Values;

typedef struct {
    GatewayMeta meta;
    Values values;
} MeterData;

typedef enum {
    JSON_OK = 0,
    JSON_ERR_NULL_INPUT,
    JSON_ERR_BUFFER_TOO_SMALL
} JsonStatus;

#endif
