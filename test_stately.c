#include <stdio.h>

#include "stately.h"

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
