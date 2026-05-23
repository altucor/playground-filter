#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdint.h>

typedef enum _filter_type_e
{
    FILTER_TYPE_FIR_MOVING_AVERAGE = 0,
    FILTER_TYPE_FIR_BLACKMAN,
} filter_type_e;

typedef struct _config_t
{
    char* input_path;
    char* output_path;
    filter_type_e filter_type;
    size_t order;
    size_t cutoff_frequency;
} config_t;

void config_cleanup(config_t* ctx);
int config_from_args(config_t* ctx, int argc, char* argv[]);
void config_print(config_t* ctx);

#endif // CONFIG_H
