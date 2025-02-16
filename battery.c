#include "battery.h"

#include <linux/limits.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "util.h"

bool bn_battery_poll(const char* battery_path, const char* adapter_path, struct bn_battery *out) {
    bool status = true;

    char battery_capacity_path[PATH_MAX + 1] = { 0 };
    char adapter_online_path[PATH_MAX + 1] = { 0 };
    FILE* battery_capacity_file = NULL;
    FILE* adapter_online_file = NULL;
    char battery_capacity_string[3 + 1] = { 0 }; // strlen("100") + null
    char adapter_online_string[1 + 1] = { 0 }; // strlen("1") + null
    int64_t battery_capacity;
    int64_t adapter_online;

    if (snprintf(battery_capacity_path, sizeof(battery_capacity_path), "%s/capacity", battery_path) < 0) {
        fprintf(stderr, "ERROR: failed to create path: %s\n", strerror(errno));
        status = false;
        goto end;
    }
    if (snprintf(adapter_online_path, sizeof(adapter_online_path), "%s/online", adapter_path) < 0) {
        fprintf(stderr, "ERROR: failed to create path: %s\n", strerror(errno));
        status = false;
        goto end;
    }

    battery_capacity_file = fopen(battery_capacity_path, "r");
    adapter_online_file = fopen(adapter_online_path, "r");

    if (battery_capacity_file == NULL) {
        fprintf(stderr, "ERROR: failed to open file `%s`: %s\n", battery_capacity_path, strerror(errno));
        status = false;
        goto end;
    }
    if (adapter_online_file == NULL) {
        fprintf(stderr, "ERROR: failed to open file `%s`: %s\n", adapter_online_path, strerror(errno));
        status = false;
        goto end;
    }

    if (fscanf(battery_capacity_file, "%3s", battery_capacity_string) == EOF) {
        fprintf(stderr, "ERROR: failed to read from file `%s`: %s\n", battery_capacity_path, strerror(errno));
        status = false;
        goto end;
    }
    if (fscanf(adapter_online_file, "%1s", adapter_online_string) == EOF) {
        fprintf(stderr, "ERROR: failed to read from file `%s`: %s\n", adapter_online_path, strerror(errno));
        status = false;
        goto end;
    }

    if (!bn_parse_num(battery_capacity_string, &battery_capacity)) {
        fprintf(stderr, "ERROR: failed to parse battery capacity from `%s`\n", battery_capacity_string);
        status = false;
        goto end;
    }
    if (!bn_parse_num(adapter_online_string, &adapter_online)) {
        fprintf(stderr, "ERROR: failed to parse adapter state from `%s`\n", adapter_online_string);
        status = false;
        goto end;
    }

    battery_capacity_string[strlen(battery_capacity_string) - 2] = '\0';
    adapter_online_string[strlen(adapter_online_string) - 2] = '\0';

    if (battery_capacity < 0 || battery_capacity > 100) {
        fprintf(stderr, "ERROR: invalid battery capacity `%s`\n", battery_capacity_string);
        status = false;
        goto end;
    }
    if (adapter_online < 0 || adapter_online > 1) {
        fprintf(stderr, "ERROR: invalid adapter state `%s`\n", adapter_online_string);
        status = false;
        goto end;
    }

    out->level = battery_capacity;
    out->charging = adapter_online;

end:
    if (battery_capacity_file != NULL) fclose(battery_capacity_file);
    if (adapter_online_file != NULL) fclose(adapter_online_file);
    return status;
}
