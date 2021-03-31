#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stately.h"

enum input {
    INVALID,
    ZERO_CHAR,
    ONE_CHAR,
};

enum state {
    REJECTING,
    ACCEPTING,
};

int map_str(const void *str)
{
    char *string = (char *)str;

    if (!strcmp(string, "0")) {
        return ZERO_CHAR;
    }

    if (!strcmp(string, "1")) {
        return ONE_CHAR;
    }

    return INVALID;
}

int main(void)
{
    struct state_machine machine = {
        .curr_state = ACCEPTING,
        .map = map_str,
        .state_table = {
            [REJECTING] = {
                [INVALID] = REJECTING,
                [ZERO_CHAR] = REJECTING,
                [ONE_CHAR] = REJECTING,
            },
            [ACCEPTING] = {
                [INVALID] = REJECTING,
                [ZERO_CHAR] = REJECTING,
                [ONE_CHAR] = ACCEPTING,
            }
        }
    };

    const char *input[] = {
        "1", "1", "1", "1", NULL
    };

    int state = GET_STATE(machine);
    for (int i = 0; input[i]; i++) {
        state = GET_NEXT_STATE(machine, input[i]);
    }

    if (state == ACCEPTING) {
        puts("Accept");
    } else {
        puts("Reject");
    }

    return 0;
}
