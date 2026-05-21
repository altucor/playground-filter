#include "filter/filter.h"

#include <stdlib.h>

filter_t* filter_new()
{
    filter_t* ctx = calloc(1, sizeof(filter_t));

    const float kInitialTap = 0.2f;
    for (size_t i = 0; i < FILTER_ORDER; i++)
    {
        ctx->taps[i] = kInitialTap;
        ctx->previous[i] = 0.0f;
    }

    return ctx;
}

void filter_free(filter_t* ctx)
{
    if (!ctx)
    {
        return;
    }

    free(ctx);
}

float filter_process_single(filter_t* ctx, float sample)
{
    float output = 0;

    // Move old samples
    for (size_t i = FILTER_ORDER - 1; i > 0; i--)
    {
        ctx->previous[i] = ctx->previous[i - 1];
    }

    ctx->previous[0] = sample;

    // Calculate new value
    for (size_t i = 0; i < FILTER_ORDER; i++)
    {
        output += ctx->taps[i] * ctx->previous[i];
    }

    return output;
}
