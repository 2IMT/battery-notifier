#ifndef BATTERY_NOTIFIER_UTIL_H
#define BATTERY_NOTIFIER_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

static inline bool bn_parse_num(const char* str, int64_t* out) {
    char *endptr;
    errno = 0;

    int64_t value = (int64_t)strtoll(str, &endptr, 10);

    if (errno == ERANGE || endptr == str || *endptr != '\0') {
        return false;
    }

    *out = value;
    return true;
}

#endif // BATTERY_NOTIFIER_UTIL_H
