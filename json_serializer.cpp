#include <stdio.h>
#include "json_serializer.h"

JsonStatus serialize_to_json(const MeterData *input,
                             char *output_buffer,
                             int buffer_size,
                             int *required_size)
{
    if (!input || !output_buffer || !required_size)
        return JSON_ERR_NULL_INPUT;

    int idx = 0;

    int written = snprintf(output_buffer + idx,
                           buffer_size - idx,
                           "[{"
                           "\"gatewayId\":\"%s\","
                           "\"date\":\"%s\","
                           "\"deviceType\":\"%s\","
                           "\"interval_minutes\":%d,"
                           "\"total_readings\":%d,"
                           "\"values\":{"
                           "\"device_count\":%d,"
                           "\"readings\":[",
                           input->meta.gatewayId,
                           input->meta.date,
                           input->meta.deviceType,
                           input->meta.interval_minutes,
                           input->meta.total_readings,
                           input->values.device_count);

    idx += written;

    for (int d = 0; d < input->values.device_count; d++)
    {
        DeviceReading *dev = &input->values.readings[d];

        written = snprintf(output_buffer + idx,
                           buffer_size - idx,
                           "{"
                           "\"media\":\"%s\","
                           "\"meter\":\"%s\","
                           "\"deviceId\":\"%s\","
                           "\"unit\":\"%s\","
                           "\"data\":[",
                           dev->media,
                           dev->meter,
                           dev->deviceId,
                           dev->unit);

        idx += written;

        for (int p = 0; p < dev->data_count; p++)
        {
            DataPoint *pt = &dev->data[p];

            written = snprintf(output_buffer + idx,
                               buffer_size - idx,
                               "{"
                               "\"timestamp\":\"%s\","
                               "\"meter_datetime\":\"%s\","
                               "\"total_m3\":%.3f,"
                               "\"status\":\"%s\""
                               "}",
                               pt->timestamp,
                               pt->meter_datetime,
                               pt->total_m3,
                               pt->status);

            idx += written;

            if (p < dev->data_count - 1)
                idx += snprintf(output_buffer + idx,
                                buffer_size - idx,
                                ",");
        }

        idx += snprintf(output_buffer + idx,
                        buffer_size - idx,
                        "]}");

        if (d < input->values.device_count - 1)
            idx += snprintf(output_buffer + idx,
                            buffer_size - idx,
                            ",");
    }

    idx += snprintf(output_buffer + idx,
                    buffer_size - idx,
                    "]} } }]");

    *required_size = idx;

    if (idx >= buffer_size)
        return JSON_ERR_BUFFER_TOO_SMALL;

    return JSON_OK;
}
