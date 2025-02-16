#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "envargs.h"
#include "battery.h"
#include "notify.h"

int main() {
    struct bn_envargs envargs;
    if (!bn_envargs_load(&envargs)) {
        return 1;
    }

    fprintf(stderr, "INFO: battery path: %s\n", envargs.battery_path);
    fprintf(stderr, "INFO: adapter path: %s\n", envargs.adapter_path);
    fprintf(stderr, "INFO: threshold: %d\n", envargs.threshold);
    fprintf(stderr, "INFO: period: %u\n", envargs.period_secs);
    fprintf(stderr, "INFO: notify command: %s\n", envargs.notify_command);

    struct timespec interval = { .tv_sec = envargs.period_secs, .tv_nsec = 0 };
    struct timespec next = { 0 };

    clock_gettime(CLOCK_MONOTONIC, &next);
    while (true) {
        struct bn_battery battery;
        if (!bn_battery_poll(envargs.battery_path, envargs.adapter_path, &battery)) {
            return 1;
        }

        fprintf(stderr, "INFO: battery level: %d\n", battery.level);
        fprintf(stderr, "INFO: battery charging: %s\n", battery.charging ? "yes" : "no");

        if (!battery.charging && battery.level <= envargs.threshold) {
            fprintf(stderr, "INFO: low battery level, notifying...\n");
            bn_notify(envargs.notify_command);
        }

        next.tv_sec += interval.tv_sec;
        next.tv_nsec += interval.tv_nsec;
        if (next.tv_nsec >= 1000000000) {
            next.tv_sec++;
            next.tv_nsec -= 1000000000;
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }

    return 0;
}
