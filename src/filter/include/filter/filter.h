#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stddef.h>

typedef struct _filter_t
{
    int value;
} filter_t;

#ifdef __cplusplus
extern "C"
{
#endif

filter_t* filter_new();
void filter_free(filter_t* ctx);
void filter_process(filter_t* ctx, uint8_t* buffer, const size_t size);

#ifdef __cplusplus
}
#endif

#endif // FILTER_H
