#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "envargs.h"
#include "battery.h"
#include "notify.h"

#ifndef BN_VERSION
#define BN_VERSION "unknown"
#endif

static const char* VERSION_TEXT = "battery-notifier version " BN_VERSION "\n";

static const char* HELP_TEXT =
    "battery-notifier version " BN_VERSION "\n"
    "\n"
    "USAGE: battery-notifier\n"
    "\n"
    "ARGUMENTS:\n"
    "    -h, --help, help - Display this help message\n"
    "    -v, --version, version - Display version information\n"
    "\n"
    "CONFIGURATION:\n"
    "    battery-notifier is configured using environment variables.\n"
    "    The following environment variables are used:\n"
    "    BN_BATTERY_PATH - Path to the battery device directory\n"
    "        Default: '/sys/class/power_supply/BAT0'\n"
    "    BN_ADAPTER_PATH - Path to the power adapter device directory\n"
    "        Default: '/sys/class/power_supply/AC0'\n"
    "    BN_THRESHOLD - Battery level threshold\n"
    "        Default: 15\n"
    "    BN_PERIOD_SECS - Period of battery state polling in seconds\n"
    "        Default: 30\n"
    "    BN_NOTIFY_COMMAND - Command to execute when battery level is below the threshold\n"
    "        Default: 'notify-send \"The battery is low\" \"Please connect the charger\"'\n";

static inline void _advance_time(struct timespec* curr, const struct timespec* interval) {
    curr->tv_sec += interval->tv_sec;
    curr->tv_nsec += interval->tv_nsec;
    if (curr->tv_nsec >= 1000000000) {
        curr->tv_sec++;
        curr->tv_nsec -= 1000000000;
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "help") == 0 ||
            strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0) {
            printf("%s", HELP_TEXT);
            return 0;
        }
        if (strcmp(argv[1], "version") == 0 ||
            strcmp(argv[1], "--version") == 0 ||
            strcmp(argv[1], "-v") == 0) {
            printf("%s", VERSION_TEXT);
            return 0;
        }
    }

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
    struct timespec now = { 0 };

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
            if (!bn_notify(envargs.notify_command)) {
                return 1;
            }
        }

        _advance_time(&next, &interval);

        clock_gettime(CLOCK_MONOTONIC, &now);

        if ((now.tv_sec > next.tv_sec) ||
            (now.tv_sec == next.tv_sec && now.tv_nsec > next.tv_nsec)) {
            next = now;

            _advance_time(&next, &interval);
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }

    return 0;
}
