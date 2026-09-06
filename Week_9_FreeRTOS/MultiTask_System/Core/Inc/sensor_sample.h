#ifndef SENSOR_SAMPLE_H
#define SENSOR_SAMPLE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float distance_cm;
    uint32_t tick_ms;
    bool valid;
} sensor_sample_t;

#endif /* SENSOR_SAMPLE_H */
