#include "sequence.h"

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>

sequence_t* sequence_new()
{
    sequence_t* ctx = calloc(1, sizeof(sequence_t));
    if (!ctx)
    {
        return NULL;
    }

    return ctx;
}

void sequence_free(sequence_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->points)
    {
        free(ctx->points);
        ctx->points = NULL;
    }

    ctx->point_count = 0;

    free(ctx);
}

const size_t sequence_size(sequence_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    return ctx->point_count;
}

sequence_error_e sequence_from_csv(sequence_t* ctx, const char* path)
{
    int ret_code = SEQUENCE_ERROR_OK;
    FILE* file = NULL;
    bool header_seen = false;
    point_t point = {0};
    char* line = NULL;
    size_t size = 0;
    size_t line_size = 0;

    if (file = fopen(path, "r"), file == NULL)
    {
        ret_code = SEQUENCE_ERROR_FAILED_TO_OPEN_FILE;
        goto exit;
    }

    while (line_size = getline(&line, &size, file), (line_size > 0 && line && strlen(line)))
    {
        if (!header_seen)
        {
            free(line);
            line = NULL;
            header_seen = true;
            continue;
        }

        point_reset(&point);

        if (point_unmarshal(&point, line) != POINT_ERROR_OK)
        {
            ret_code = SEQUENCE_ERROR_FAILED_TO_UNMARSHAL_POINT;
            goto exit;
        }

        ctx->point_count++;
        ctx->points = realloc(ctx->points, ctx->point_count * sizeof(point_t));
        ctx->points[ctx->point_count - 1] = point;

        free(line);
        line = NULL;
    }

exit:
    if (file)
    {
        fclose(file);
        file = NULL;
    }

    if (line)
    {
        free(line);
        line = NULL;
    }

    return ret_code;
}

sequence_error_e sequence_to_csv(sequence_t* ctx, const char* path)
{
    sequence_error_e ret_code = SEQUENCE_ERROR_OK;

    FILE* file = NULL;
    char* out_line = NULL;

    file = fopen(path, "w");
    if (!file)
    {
        ret_code = SEQUENCE_ERROR_FAILED_TO_MARSHAL_POINT;
        goto exit;
    }

    const char* header = "t,raw,filtered\r\n";
    if (fwrite(header, strlen(header), 1, file) != 1)
    {
        ret_code = SEQUENCE_ERROR_FAILED_TO_MARSHAL_POINT;
        goto exit;
    }

    for (size_t i = 0; i < ctx->point_count; i++)
    {
        if (out_line = point_marshal(&ctx->points[i]), !out_line)
        {
            ret_code = SEQUENCE_ERROR_FAILED_TO_MARSHAL_POINT;
            goto exit;
        }

        const size_t written = fwrite(out_line, strlen(out_line), 1, file);
        if (written != 1)
        {
            ret_code = SEQUENCE_ERROR_FAILED_TO_MARSHAL_POINT;
            goto exit;
        }

        free(out_line);
        out_line = NULL;
    }

exit:
    if (file)
    {
        fclose(file);
        file = NULL;
    }

    if (out_line)
    {
        free(out_line);
        out_line = NULL;
    }

    return ret_code;
}

sequence_error_e sequence_filter_with(sequence_t* ctx, filter_t* filter)
{
    if (!ctx || !filter)
    {
        return SEQUENCE_ERROR_FILTERING;
    }

    for (size_t i = 0; i < ctx->point_count; i++)
    {
        ctx->points[i].output = filter_process_single(filter, ctx->points[i].input);
    }

    return SEQUENCE_ERROR_OK;
}
