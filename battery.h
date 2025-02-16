#ifndef BATTERY_NOTIFIER_BATTERY_H
#define BATTERY_NOTIFIER_BATTERY_H

#include <stdint.h>
#include <stdbool.h>

struct bn_battery {
    uint8_t level;
    bool charging;
};

bool bn_battery_poll(const char* battery_path, const char* adapter_path, struct bn_battery *state);

#endif // BATTERY_NOTIFIER_BATTERY_H
