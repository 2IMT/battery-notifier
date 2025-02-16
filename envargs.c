#include "envargs.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static const struct bn_envargs _ENVARGS_DEFAULT = {
    .battery_path = "/sys/class/power_supply/BAT0",
    .adapter_path = "/sys/class/power_supply/AC0",
    .threshold = 15,
    .notify_command = "notify-send \"The battery is low\" \"Please connect the charger\"",
    .period_secs = 30,
};

bool bn_envargs_load(struct bn_envargs* envargs) {
    *envargs = _ENVARGS_DEFAULT;

    const char *value;
    if ((value = getenv("BN_BATTERY_PATH"))) {
        envargs->battery_path = value;
    }
    if ((value = getenv("BN_ADAPTER_PATH"))) {
        envargs->adapter_path = value;
    }
    if ((value = getenv("BN_THRESHOLD"))) {
        int64_t threshold;
        if (!bn_parse_num(value, &threshold)) {
            fprintf(stderr, "ERROR: unparsable threshold value `%s` (set by BN_THRESHOLD)\n", value);
            return false;
        }

        if (threshold < 0 || threshold > 100) {
            fprintf(stderr, "ERROR: threshold value `%s` out of range (set by BN_THRESHOLD)\n", value);
            return false;
        }

        envargs->threshold = (uint8_t)threshold;
    }
    if ((value = getenv("BN_NOTIFY_COMMAND"))) {
        envargs->notify_command = value;
    }
    if ((value = getenv("BN_PERIOD_SECS"))) {
        int64_t period_secs;
        if (!bn_parse_num(value, &period_secs)) {
            fprintf(stderr, "ERROR: unparsable period value `%s` (set by BN_PERIOD_SECS)\n", value);
            return false;
        }

        if (period_secs < 0 || period_secs > UINT32_MAX) {
            fprintf(stderr, "ERROR: period value `%s` out of range (set by BN_PERIOD_SECS)\n", value);
            return false;
        }

        envargs->period_secs = (uint32_t)period_secs;
    }

    return true;
}
