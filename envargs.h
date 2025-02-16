#ifndef BATTERY_NOTIFIER_ENVARGS_H
#define BATTERY_NOTIFIER_ENVARGS_H

#include <stdint.h>
#include <stdbool.h>

struct bn_envargs {
    const char* battery_path;
    const char* adapter_path;
    uint8_t threshold;
    const char* notify_command;
    uint32_t period_secs;
};

bool bn_envargs_load(struct bn_envargs* envargs);

#endif // BATTERY_NOTIFIER_ENVARGS_H
