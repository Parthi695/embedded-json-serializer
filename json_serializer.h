#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include "meter_model.h"

JsonStatus serialize_to_json(const MeterData *input,
                             char *output_buffer,
                             int buffer_size,
                             int *required_size);

#endif
