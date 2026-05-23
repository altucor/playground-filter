#include "point.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VALUE_BUFFER_SIZE (128)

point_t* point_new()
{
    point_t* ctx = calloc(1, sizeof(point_t));
    if (!ctx)
    {
        return NULL;
    }

    return ctx;
}

void point_free(point_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

static int parse_value(const char* line, const size_t begin, const size_t end, float* out)
{
    if (!line || !strlen(line) || end >= strlen(line) || begin >= strlen(line) || end <= begin || !out)
    {
        return -1;
    }

    char temp[VALUE_BUFFER_SIZE];
    memset(&temp, 0x00, VALUE_BUFFER_SIZE);

    strncpy((char*)&temp, line + begin, end - begin);
    *out = atof((char*)&temp);

    return 0;
}

point_error_e point_unmarshal(point_t* ctx, const char* line)
{
    point_error_e ret_code = POINT_ERROR_OK;

    if (!line || strlen(line) < 1)
    {
        ret_code = POINT_ERROR_FAILED_TO_UNMARSHAL;
        goto exit;
    }

    char* delimiter = NULL;

    delimiter = strchr(line, ',');

    if (!delimiter)
    {
        ret_code = POINT_ERROR_FAILED_TO_UNMARSHAL;
        goto exit;
    }

    const size_t k_crlf_size = 2;
    if (parse_value(line, 0, delimiter - line, &ctx->time))
    {
        ret_code = POINT_ERROR_FAILED_TO_UNMARSHAL;
        goto exit;
    }

    if (parse_value(line, delimiter - line + 1, strlen(line) - k_crlf_size, &ctx->input))
    {
        ret_code = POINT_ERROR_FAILED_TO_UNMARSHAL;
        goto exit;
    }

exit:
    return ret_code;
}

char* point_marshal(point_t* ctx)
{
    // Always contant size
    // 3 = float values count
    // 2+6 = float value decimal, delimiter and floating
    // 2 = separators
    // 2 = CRLF
    // 1 = EOL
    // 2 = reserve for minus signs for values
    static const size_t buffer_size = 3 * (2 + 6) + 2 + 2 + 1 + 2;
    char* line = calloc(buffer_size, sizeof(char));
    size_t written = snprintf(line, buffer_size, "%.06f,%.06f,%.06f\r\n", ctx->time, ctx->input, ctx->output);
    if (written >= buffer_size)
    {
        free(line);
        return NULL;
    }

    return line;
}
