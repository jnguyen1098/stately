#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stately.h"

enum input { INVALID, ZERO_CHAR, ONE_CHAR };
enum state { REJECTING, ACCEPTING };

const char char_map[128] = {
    ['0'] = ZERO_CHAR,
    ['1'] = ONE_CHAR,
};

int map_chr(const void *chr) {
    return char_map[(int)*(const char *)chr];
}

int main(void)
{
   /***************************************
    *  DFA that accepts either the empty  *
    *  string, or any sequence of 1s.     *
    *                                     *
    *       1                    0,1      *
    *    +-----+               +----+     *
    *    |     |               |    |     *
    *    |    \ /              |   \ /    *
    * +--+---------+       +---+--------+ *
    * |            |       |            | *
    * | Accepting  |       |  Rejecting | *
    * |            |       |            | *
    * +-----+------+       +------------+ *
    *       |                    / \      *
    *       |                     |       *
    *       |                     |       *
    *       +---------------------+       *
    *                 0                   *
    **************************************/

    struct state_machine machine = {
       
        // Start state
        .curr_state = ACCEPTING,

        // Input mapper
        .map = map_chr,

        // States
        .state_table = {

            // Reject state transitions
            [REJECTING] = {
                [INVALID]   = REJECTING,
                [ZERO_CHAR] = REJECTING,
                [ONE_CHAR]  = REJECTING,
            },

            // Accept state transitions
            [ACCEPTING] = {
                [INVALID]   = REJECTING,
                [ZERO_CHAR] = REJECTING,
                [ONE_CHAR]  = ACCEPTING,
            }

        }

    };

    struct test_case {
        char input[16];
        int expected_result;
    };

    struct test_case tests[] = {
        { "111111111", ACCEPTING },
        { "         ", REJECTING },
        { "111111110", REJECTING },
        { "000000000", REJECTING },
        { "",          ACCEPTING },
    };

    for (int i = 0; i < (int)(sizeof(tests) / sizeof(*tests)); i++) {
        printf("Testing case '%s'\n", tests[i].input);
        SET_STATE(machine, ACCEPTING);
        for (int c = 0; tests[i].input[c]; c++) {
            (void)GET_NEXT_STATE(machine, &tests[i].input[c]);
        }
        assert(GET_STATE(machine) == tests[i].expected_result);
    }

    puts("Complete");

    return 0;
}
