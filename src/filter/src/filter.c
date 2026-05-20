#include "filter/filter.h"

#include <stdlib.h>

filter_t* filter_new()
{
    filter_t* ctx = calloc(1, sizeof(filter_t));
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

void filter_process(filter_t* ctx, uint8_t* buffer, const size_t size)
{
    //
}
