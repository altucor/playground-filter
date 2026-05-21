#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>

// FIR which only looks behind

// For simplicity it's easier to have same size for history and for coefficients
#define FILTER_ORDER (5 - 1)

typedef struct _filter_t
{
    float taps[FILTER_ORDER];
    float previous[FILTER_ORDER];
} filter_t;

#ifdef __cplusplus
extern "C"
{
#endif

filter_t* filter_new();
void filter_free(filter_t* ctx);
float filter_process_single(filter_t* ctx, float sample);

#ifdef __cplusplus
}
#endif

#endif // FILTER_H
