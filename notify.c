#include "notify.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

bool bn_notify(const char* notify_command) {
    if (system(notify_command) != 0) {
        fprintf(stderr, "ERROR: failed to execute command `%s`: %s\n", notify_command, strerror(errno));
        return false;
    }

    return true;
}
