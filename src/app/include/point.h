#ifndef POINT_H
#define POINT_H

#include <float.h>
#include <stdint.h>

typedef enum _point_error_e
{
    POINT_ERROR_OK = 0,
    POINT_ERROR_FAILED_TO_UNMARSHAL = -1,
    POINT_ERROR_FAILED_TO_MARSHAL = -2,
} point_error_e;

typedef struct _point_t
{
    float time;
    float input;
    float output;
} point_t;

point_t* point_new();
void point_free(point_t* ctx);
point_error_e point_unmarshal(point_t* ctx, const char* line);
char* point_marshal(point_t* ctx);

#endif // POINT_H
