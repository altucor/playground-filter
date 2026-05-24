#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KV_CLEAN                                                                                                       \
    free(key);                                                                                                         \
    key = NULL;                                                                                                        \
    free(value);                                                                                                       \
    value = NULL;

void config_cleanup(config_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    if (ctx->input_path)
    {
        free(ctx->input_path);
        ctx->input_path = NULL;
    }

    if (ctx->output_path)
    {
        free(ctx->output_path);
        ctx->output_path = NULL;
    }
}

static int extract_kv(const char* line, char** key, char** value)
{
    // "--k=v" - minimum expected 5 elements
    if (!line || strlen(line) < 5 || !key || !value)
    {
        return -1;
    }

    if (line[0] != '-' && line[1] != '-')
    {
        return -1;
    }

    char* kv_delimiter = strchr(line, '=');
    if (!kv_delimiter)
    {
        return -1;
    }

    const size_t key_size = kv_delimiter - line - 2;
    *key = calloc(1, key_size + 1);
    strncpy(*key, line + 2, key_size);

    const size_t value_start_offset = kv_delimiter + 1 - line;
    const size_t value_size = strlen(line) - value_start_offset;
    *value = calloc(1, value_size + 1);
    strncpy(*value, line + value_start_offset, value_size);

    return 0;
}

int config_from_args(config_t* ctx, int argc, char* argv[])
{
    for (size_t i = 0; i < argc; i++)
    {
        char* key = NULL;
        char* value = NULL;

        if (-1 == extract_kv(argv[i], &key, &value))
        {
            continue;
        }

        if (0 == strcmp(key, "input"))
        {
            ctx->input_path = value;
            free(key);
            key = NULL;
            continue;
        }

        if (0 == strcmp(key, "output"))
        {
            ctx->output_path = value;
            free(key);
            key = NULL;
            continue;
        }

        if (0 == strcmp(key, "type"))
        {
            // Default if nothing set is "moving-average"
            ctx->filter_type = FILTER_TYPE_FIR_MOVING_AVERAGE;

            if (0 == strcmp(value, "blackman"))
            {
                ctx->filter_type = FILTER_TYPE_FIR_BLACKMAN;
            }

            KV_CLEAN
            continue;
        }

        if (0 == strcmp(key, "order"))
        {
            int temp = atoi(value);
            if (temp < 0)
            {
                temp = 2;
            }

            ctx->order = temp;

            KV_CLEAN
            continue;
        }

        if (0 == strcmp(key, "cutoff"))
        {
            long long temp = atoll(value);
            if (temp < 0 || temp > 500)
            {
                temp = 500;
            }

            ctx->cutoff_frequency = temp;

            KV_CLEAN
            continue;
        }

        // Catch freeing resources which doesnt match any argument
        if (key)
        {
            free(key);
            key = NULL;
        }

        if (value)
        {
            free(value);
            value = NULL;
        }
    }

    return 0;
}

void config_print(config_t* ctx)
{
    printf("App configuration is:\n");
    printf("Input path: %s\n", ctx->input_path);
    printf("Output path: %s\n", ctx->output_path);

    char* filter_type_string = NULL;
    switch (ctx->filter_type)
    {
        default:
        case FILTER_TYPE_FIR_MOVING_AVERAGE:
        {
            filter_type_string = "moving-average";
            break;
        }

        case FILTER_TYPE_FIR_BLACKMAN:
        {
            filter_type_string = "blackman";
            break;
        }
    }

    printf("Filter type: %s\n", filter_type_string);
    printf("Filter order: %zu\n", ctx->order);
    printf("Filter cutoff frequency: %zu\n", ctx->cutoff_frequency);
}
