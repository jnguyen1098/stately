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

int _char_map[NUM_CHAR] = {
    ['0'] = ZERO_CHAR,
    ['1'] = ONE_CHAR,
};

int map_char(const void *ch)
{
    return _char_map[*(char *)ch];

    char c = *(char *)ch;
    if (c == '0') {
        return ZERO_CHAR;
    }
    if (c == '1') {
        return ONE_CHAR;
    }
    return INVALID;
}

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

    assert(map_str("0") == ZERO_CHAR);
    assert(map_str("1") == ONE_CHAR);

    const char *input[] = {
        "1", "1", "1", "1", NULL
    };

    int state = get_state(&machine);
    for (int i = 0; input[i]; i++) {
        state = get_next_state(&machine, input[i]);
    }

    if (state == ACCEPTING) {
        puts("Accept");
    } else {
        puts("Reject");
    }

    return 0;
}

#if 0

enum input char_map[NUM_CHAR] = {
    ['0'] = ZERO_CHAR,
    ['1'] = ONE_CHAR,
};

enum input input_mapper(char input) {
    return char_map[input];
}

enum input _input_mapper(char input)
{
    if (input == '0') {
        return ZERO_CHAR;
    }

    if (input == '1') {
        return ONE_CHAR;
    }

    return INVALID;
}

int main(void)
{
    struct state_machine machine = {
        .char_map = {
            ['0'] = ZERO_CHAR,
            ['1'] = ONE_CHAR,
        },
        .state_table = {
            [REJECTING] = {
                [INVALID]   = REJECTING,
                [ONE_CHAR]  = REJECTING,
                [ZERO_CHAR] = REJECTING,
            },
            [ACCEPTING] = {
                [INVALID]   = REJECTING,
                [ZERO_CHAR] = REJECTING,
                [ONE_CHAR]  = ACCEPTING,
            },
        },
    };
    char input[] = "";
    int state = 1;

    for (int i = 0; input[i]; i++) {
        state = machine.state_table[state][machine.char_map[input[i]]];
    }

    printf("Final state is %d\n", state);

    return 0;
}

#endif
