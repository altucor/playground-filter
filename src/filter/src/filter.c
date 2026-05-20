#include "filter/filter.h"

#include <stdlib.h>

filter_t* filter_new()
{
    filter_t* ctx = calloc(1, sizeof(filter_t));

    // TODO: Play around with coefficients
    ctx->coefficients[0] = 0.2f;
    ctx->coefficients[1] = 0.2f;
    ctx->coefficients[2] = 0.2f;
    ctx->coefficients[3] = 0.2f;
    ctx->coefficients[4] = 0.2f;

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
    for (size_t i = LOOKUP_SIZE - 1; i > 0; i--)
    {
        ctx->previous[i] = ctx->previous[i - 1];
    }

    ctx->previous[0] = sample;

    // Calculate new value
    for (size_t i = 0; i < LOOKUP_SIZE; i++)
    {
        output += ctx->coefficients[i] * ctx->previous[i];
    }

    return output;
}

void filter_process(filter_t* ctx, float* buffer, const size_t size)
{
    //
}
