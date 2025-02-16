#include <stdio.h>

#include "envargs.h"

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

    return 0;
}
