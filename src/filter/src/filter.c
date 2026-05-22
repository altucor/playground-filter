#include "filter/filter.h"

#include <stdlib.h>
#include <stdio.h>

filter_t* filter_new(const size_t order)
{
    filter_t* ctx = calloc(1, sizeof(filter_t));
    if (!ctx)
    {
        return NULL;
    }

    filter_set_order(ctx, order);

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

void filter_set_order(filter_t* ctx, const size_t order)
{
    if (!ctx || order < 1)
    {
        return;
    }

    if (ctx->taps)
    {
        free(ctx->taps);
        ctx->taps = NULL;
    }

    if (ctx->previous)
    {
        free(ctx->previous);
        ctx->previous = NULL;
    }

    ctx->order = order;

    ctx->taps = calloc(order + 1, sizeof(float));
    if (!ctx->taps)
    {
        return;
    }

    ctx->previous = calloc(order + 1, sizeof(float));
    if (!ctx->previous)
    {
        return;
    }

    const float kInitialTap = 1.0f / (float)(order + 1);
    for (size_t i = 0; i < order + 1; i++)
    {
        ctx->taps[i] = kInitialTap;
        ctx->previous[i] = 0.0f;
    }
}

float filter_process_single(filter_t* ctx, float sample)
{
    if (!ctx)
    {
        return 0.0f;
    }

    // Move old samples
    for (size_t i = ctx->order; i > 0; i--)
    {
        ctx->previous[i] = ctx->previous[i - 1];
    }

    ctx->previous[0] = sample;

    float output = 0;
    // Calculate new value
    for (size_t i = 0; i < ctx->order + 1; i++)
    {
        output += ctx->taps[i] * ctx->previous[i];
    }

    return output;
}

void filter_process(filter_t* ctx, const float* input, float* output, const size_t size)
{
    if (!ctx || !input || !output || !size)
    {
        return;
    }

    for (size_t i = 0; i < size; i += ctx->order)
    {
    }
}
