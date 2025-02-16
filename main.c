#include <stdio.h>

#include "envargs.h"
#include "battery.h"
#include "notify.h"

int main() {
    struct bn_envargs envargs;
    if (!bn_envargs_load(&envargs)) {
        return 1;
    }

    printf("battery_path: %s\n", envargs.battery_path);
    printf("adapter_path: %s\n", envargs.adapter_path);
    printf("threshold: %d\n", envargs.threshold);
    printf("period_secs: %u\n", envargs.period_secs);
    printf("notify_command: %s\n", envargs.notify_command);

    struct bn_battery battery;
    if (!bn_battery_poll(envargs.battery_path, envargs.adapter_path, &battery)) {
        return 1;
    }

    printf("level: %d\n", battery.level);
    printf("charging: %d\n", battery.charging);

    bn_notify(envargs.notify_command);

    return 0;
}
