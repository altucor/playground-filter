#include "filter/filter.h"

#include <stdlib.h>
#include <math.h>

filter_t* filter_new()
{
    filter_t* ctx = calloc(1, sizeof(filter_t));
    if (!ctx)
    {
        return NULL;
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

const size_t filter_get_order(filter_t* ctx)
{
    if (!ctx)
    {
        return 0;
    }

    return ctx->order;
}

int filter_set_order(filter_t* ctx, const size_t order)
{
    if (!ctx || order < 1)
    {
        return -1;
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
        return -1;
    }

    ctx->previous = calloc(order + 1, sizeof(float));
    if (!ctx->previous)
    {
        return -1;
    }

    return 0;
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

int filter_init_moving_average(filter_t* ctx, const size_t order)
{
    if (!ctx || order < 1)
    {
        return -1;
    }

    if (0 != filter_set_order(ctx, order))
    {
        return -1;
    }

    const float kInitialTap = 1.0f / (float)(ctx->order + 1);
    for (size_t i = 0; i < ctx->order + 1; i++)
    {
        ctx->taps[i] = kInitialTap;
        ctx->previous[i] = 0.0f;
    }

    return 0;
}

static float cosine_window(const size_t harmonic, const size_t step, const size_t filterOrder)
{
    return cosf((2 * harmonic) * M_PI * step / filterOrder);
}

int filter_init_blackman_window(filter_t* ctx, const float cutoff_frequency)
{
    const float k_max_frequency = 1000.0f;
    const float k_normalized_transition_window = cutoff_frequency / k_max_frequency;

    // Need to be rounded to odd value for symmetric response
    size_t k_filter_order = 5.5f / k_normalized_transition_window;
    if (!(k_filter_order & 0x01) && k_filter_order > 1)
    {
        k_filter_order -= 1;
    }

    if (filter_set_order(ctx, k_filter_order) != 0)
    {
        return -1;
    }

    const size_t kCenterIndex = (k_filter_order - 1) / 2;

    const float kDcOffset = 0.42f;

    for (size_t i = 0; i < k_filter_order; i++)
    {
        // Sinc infinite impulse response
        const float temp = 2.0f * k_normalized_transition_window * (i - kCenterIndex);
        float sinc = sinf(M_PI * temp) / (M_PI * temp);
        if (i == kCenterIndex)
        {
            // Handle sinc specific center case explicitly
            sinc = 2.0f * k_normalized_transition_window;
        }

        // Should give -57db attenuation after cutoff frequency
        const float kBlackmanWindow =
            kDcOffset - 0.5 * cosine_window(1, i, k_filter_order - 1) + 0.08 * cosine_window(2, i, k_filter_order - 1);

        ctx->taps[i] = sinc * kBlackmanWindow;
    }

    float totalSum = 0.0f;
    for (size_t i = 0; i < k_filter_order; i++)
    {
        totalSum += ctx->taps[i];
    }

    for (size_t i = 0; i < k_filter_order; i++)
    {
        ctx->taps[i] /= totalSum;
    }

    return 0;
}
