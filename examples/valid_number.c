#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stately.h"

// Inputs
enum input { INVALID, DIGIT, SCIENTIFIC_E, PLUS_MINUS, PERIOD };

// Mapping function
int map_chr(const void *chr) {
    char c = *(char *)chr;
    if (c >= '0' && c <= '9')
        return DIGIT;
    if (c == 'E' || c == 'e')
        return SCIENTIFIC_E;
    if (c == '+' || c == '-')
        return PLUS_MINUS;
    if (c == '.')
        return PERIOD;
    return INVALID;
}

int main(void)
{
   /************************************************
    *  DFA that validates a number to the rules of *
    *  LeetCode problem #0065 (Valid Number)       *
    *  https://leetcode.com/problems/valid-number/ *
    ***********************************************/

    struct state_machine machine = {
       
        // Start state
        .curr_state = 1,

        // Input mapper
        .map = map_chr,

        // States
        .state_table = {

            [1] = {
                [PLUS_MINUS] = 2,
                [DIGIT]      = 3,
                [PERIOD]     = 4,
            },

            [2] = {
                [DIGIT]  = 3,
                [PERIOD] = 4,
            },

            [3] = {
                [DIGIT]        = 3,
                [PERIOD]       = 5,
                [SCIENTIFIC_E] = 6,
            },

            [4] = {
                [DIGIT] = 5,
            },

            [5] = {
                [DIGIT]        = 5,
                [SCIENTIFIC_E] = 6,
            },

            [6] = {
                [PLUS_MINUS] = 7,
                [DIGIT]      = 8,
            },

            [7] = {
                [DIGIT] = 8,
            },

            [8] = {
                [DIGIT] = 8,
            },

        }

    };

    struct test_case {
        char input[16];
        int expected_result;
    };

    struct test_case tests[] = {
        { "1234567", 3 },
        { "EeEeEeEeE", 0 },
        { "45345e4", 8 },
        { "+3.", 5 },
        { "3e+", 7 },
        { "0089", 3 },
        { "-0.1", 5 },
        { "+3.14", 5 },
        { "-.9", 5 },
        { "-90E3", 8 },
        { "-123.456e789", 8 },

    };

    for (int i = 0; i < (int)(sizeof(tests) / sizeof(*tests)); i++) {
        printf("Testing case '%s'\n", tests[i].input);
        SET_STATE(machine, 1);
        for (int c = 0; tests[i].input[c]; c++) {
            (void)GET_NEXT_STATE(machine, &tests[i].input[c]);
        }
        assert(GET_STATE(machine) == tests[i].expected_result);
    }

    return 0;
}
