#include "envargs.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static bool _parse_num(const char* str, int64_t* out) {
    char *endptr;
    errno = 0;

    int64_t value = (int64_t)strtoll(str, &endptr, 10);

    if (errno == ERANGE || endptr == str || *endptr != '\0') {
        return false;
    }

    *out = value;
    return true;
}

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
        if (!_parse_num(value, &threshold)) {
            fprintf(stderr, "ERROR: unparsable threshold value `%s` (set by BN_THRESHOLD)\n", value);
            return false;
        }

        if (threshold < 1 || threshold > 99) {
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
        if (!_parse_num(value, &period_secs)) {
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
