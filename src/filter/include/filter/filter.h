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

#ifdef __cplusplus
extern "C"
{
#endif

filter_t* filter_new(const size_t order);
void filter_free(filter_t* ctx);
void filter_set_order(filter_t* ctx, const size_t order);
float filter_process_single(filter_t* ctx, float sample);
void filter_process(filter_t* ctx, const float* input, float* output, const size_t size);

#ifdef __cplusplus
}
#endif

#endif // FILTER_H
