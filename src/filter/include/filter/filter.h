#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>

// FIR which only looks behind

// For simplicity it's easier to have same size for history and for coefficients
#define LOOKUP_SIZE (5)

typedef struct _filter_t
{
    float coefficients[LOOKUP_SIZE];
    float previous[LOOKUP_SIZE];
} filter_t;

#ifdef __cplusplus
extern "C"
{
#endif

filter_t* filter_new();
void filter_free(filter_t* ctx);
float filter_process_single(filter_t* ctx, float sample);
void filter_process(filter_t* ctx, float* buffer, const size_t size);

#ifdef __cplusplus
}
#endif

#endif // FILTER_H
