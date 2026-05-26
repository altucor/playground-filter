#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "point.h"
#include "filter/filter.h"

#include <stddef.h>

typedef enum _sequence_error_e
{
    SEQUENCE_ERROR_OK = 0,
    SEQUENCE_ERROR_FAILED_TO_OPEN_FILE = -1,
    SEQUENCE_ERROR_FAILED_TO_CREATE_NEW_POINT = -2,
    SEQUENCE_ERROR_FAILED_TO_UNMARSHAL_POINT = -3,
    SEQUENCE_ERROR_FAILED_TO_MARSHAL_POINT = -4,
    SEQUENCE_ERROR_FILTERING = -5,
} sequence_error_e;

typedef struct _sequence_t
{
    size_t point_count;
    point_t* points;
} sequence_t;

sequence_t* sequence_new();
void sequence_free(sequence_t* ctx);
const size_t sequence_size(sequence_t* ctx);
sequence_error_e sequence_from_csv(sequence_t* ctx, const char* path);
sequence_error_e sequence_to_csv(sequence_t* ctx, const char* path);
sequence_error_e sequence_filter_with(sequence_t* ctx, filter_t* filter);

#endif // SEQUENCE_H
