#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>

typedef struct _filter_t
{
    size_t order;
    float* taps;
    float* previous;
} filter_t;

filter_t* filter_new();
void filter_free(filter_t* ctx);
const size_t filter_get_order(filter_t* ctx);
int filter_set_order(filter_t* ctx, const size_t order);
float filter_process_single(filter_t* ctx, float sample);
void filter_process(filter_t* ctx, const float* input, float* output, const size_t size);
int filter_init_moving_average(filter_t* ctx, const size_t order);
int filter_init_blackman_window(filter_t* ctx, const float cutoffFrequency);

#endif // FILTER_H
