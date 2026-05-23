
#include "config.h"
#include "sequence.h"
#include "filter/filter.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help(const char* app_name)
{
    printf("Usage: %s <--key=value>\n", app_name);
    printf("Available keys:\n");
    printf("--input=<file-path> // Should exist\n");
    printf("--output=<file-path> // Can be existing or now, anyway will be created or wiped\n");
    printf("--type=<filter-type> // Available: \"moving-average\", \"blackman\" \n");
    printf("--order=<decimal> // Allows to set FIR filter order for \"moving-average\" type\n");
    printf(
        "--cutoff=<decimal> // Allows to set cutoff frequency for \"blackman\" type. NOTE: Sample rate is 1000 samples "
        "per second\n");
}

int main(int argc, char* argv[])
{
    int ret_code = 0;
    config_t config = {0};

    sequence_t* sequence = NULL;
    filter_t* filter = NULL;

    if (-1 == config_from_args(&config, argc, argv))
    {
        help(argv[0]);
        ret_code = -1;
        goto exit;
    }

    config_print(&config);

    sequence = sequence_new();
    if (!sequence)
    {
        printf("Error allocating sequence\n");
        ret_code = -1;
        goto exit;
    }

    if (ret_code = sequence_from_csv(sequence, config.input_path), ret_code < 0)
    {
        printf("Sequence import from csv failed\n");
        goto exit;
    }

    filter = filter_new();
    if (!filter)
    {
        printf("Error allocating filter\n");
        ret_code = -1;
        goto exit;
    }

    switch (config.filter_type)
    {
        default:
        case FILTER_TYPE_FIR_MOVING_AVERAGE:
        {
            if (filter_init_moving_average(filter, config.order) != 0)
            {
                printf("Error initializing filter moving average\n");
                ret_code = -1;
                goto exit;
            }
            break;
        }

        case FILTER_TYPE_FIR_BLACKMAN:
        {
            if (filter_init_blackman_window(filter, config.cutoff_frequency) != 0)
            {
                printf("Error initializing filter blackman\n");
                ret_code = -1;
                goto exit;
            }
            break;
        }
    }

    printf("Filter order is: %zu\n", filter_get_order(filter));

    if (ret_code = sequence_filter_with(sequence, filter), ret_code < 0)
    {
        printf("Sequence filtering failed\n");
        goto exit;
    }

    if (ret_code = sequence_to_csv(sequence, config.output_path), ret_code < 0)
    {
        printf("Sequence export to csv failed\n");
        goto exit;
    }

    printf("App finished without errors\n");

exit:
    config_cleanup(&config);
    sequence_free(sequence);
    filter_free(filter);

    return ret_code;
}
