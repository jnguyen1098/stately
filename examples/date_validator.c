#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stately.h"

// Inputs
enum input {
    INVALID, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, HYPHEN,
};

enum states {
    TRAP,                    // ([12]\d{3}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))
    FIRST_DIGIT,             //  [12]
    SECOND_DIGIT,            //      \d{3}
    THIRD_DIGIT,             //      \d{3}
    FOURTH_DIGIT,            //      \d{3}
    FIRST_HYPHEN,            //           -
    FIRST_DIGIT_OF_MONTH,    //            (0     |1     )
    SECOND_DIGIT_JAN_TO_SEP, //            ( [1-9]|      )
    SECOND_DIGIT_OCT_TO_DEC, //            (      | [0-2])
    SECOND_HYPHEN,           //                           -
    FIRST_DIGIT_OF_DAY,      //                            (0     |[12]  |3    )
    SECOND_DIGIT_ZERO,       //                            ( [1-9]|      |     )
    SECOND_DIGIT_ONE_TWO,    //                            (      |    \d|     )
    SECOND_DIGIT_THREE,      //                            (      |      | [01])
    ACCEPT                   // ACCEPT
};

char *texts[] = {
	"TRAP", "FIRST_DIGIT", "SECOND_DIGIT", "THIRD_DIGIT", "FOURTH_DIGIT",
	"FIRST_HYPHEN", "FIRST_DIGIT_OF_MONTH", "SECOND_DIGIT_JAN_TO_SEP",
	"SECOND_DIGIT_OCT_TO_DEC", "SECOND_HYPHEN", "FIRST_DIGIT_OF_DAY",
	"SECOND_DIGIT_ZERO", "SECOND_DIGIT_ONE_TWO", "SECOND_DIGIT_THREE"
};

// Lookup table
const int char_map[128] = {
    ['0'] = _0, ['1'] = _1, ['2'] = _2, ['3'] = _3, ['4'] = _4,
    ['5'] = _5, ['6'] = _6, ['7'] = _7, ['8'] = _8, ['9'] = _9,
    ['-'] = HYPHEN
};

// Mapping function
int map_chr(const void *chr) {
    return char_map[(int)*(char *)chr];
}

int main(void)
{
   /*****************************************************
    * DFA that validates a YYYY-MM-dd date of the form: *
    * ([12]\d{3}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])) *
    ****************************************************/
    struct state_machine machine = {
       
        // Start state
        .curr_state = FIRST_DIGIT,

        // Input mapper
        // ([12]\d{3}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))
        .map = map_chr,

        // States
        .state_table = {

            // Immediate death
            [TRAP] = {},

            [FIRST_DIGIT] = {
            	[_1] = SECOND_DIGIT,
            	[_2] = SECOND_DIGIT,
            },

            [SECOND_DIGIT] = {
            	[_0] = THIRD_DIGIT,
            	[_1] = THIRD_DIGIT,
            	[_2] = THIRD_DIGIT,
            	[_3] = THIRD_DIGIT,
            	[_4] = THIRD_DIGIT,
            	[_5] = THIRD_DIGIT,
            	[_6] = THIRD_DIGIT,
            	[_7] = THIRD_DIGIT,
            	[_8] = THIRD_DIGIT,
            	[_9] = THIRD_DIGIT,
            },

            [THIRD_DIGIT] = {
            	[_0] = FOURTH_DIGIT,
            	[_1] = FOURTH_DIGIT,
            	[_2] = FOURTH_DIGIT,
            	[_3] = FOURTH_DIGIT,
            	[_4] = FOURTH_DIGIT,
            	[_5] = FOURTH_DIGIT,
            	[_6] = FOURTH_DIGIT,
            	[_7] = FOURTH_DIGIT,
            	[_8] = FOURTH_DIGIT,
            	[_9] = FOURTH_DIGIT,
            },

            [FOURTH_DIGIT] = {
            	[_0] = FIRST_HYPHEN,
            	[_1] = FIRST_HYPHEN,
            	[_2] = FIRST_HYPHEN,
            	[_3] = FIRST_HYPHEN,
            	[_4] = FIRST_HYPHEN,
            	[_5] = FIRST_HYPHEN,
            	[_6] = FIRST_HYPHEN,
            	[_7] = FIRST_HYPHEN,
            	[_8] = FIRST_HYPHEN,
            	[_9] = FIRST_HYPHEN,
            },

            [FIRST_HYPHEN] = {
            	[HYPHEN] = FIRST_DIGIT_OF_MONTH,
            },

            [FIRST_DIGIT_OF_MONTH] = {
            	[_0] = SECOND_DIGIT_JAN_TO_SEP,
            	[_1] = SECOND_DIGIT_OCT_TO_DEC,
            },

            [SECOND_DIGIT_JAN_TO_SEP] = {
            	[_0] = SECOND_HYPHEN,
            	[_1] = SECOND_HYPHEN,
            	[_2] = SECOND_HYPHEN,
            	[_3] = SECOND_HYPHEN,
            	[_4] = SECOND_HYPHEN,
            	[_5] = SECOND_HYPHEN,
            	[_6] = SECOND_HYPHEN,
            	[_7] = SECOND_HYPHEN,
            	[_8] = SECOND_HYPHEN,
            	[_9] = SECOND_HYPHEN,
            },

            [SECOND_DIGIT_OCT_TO_DEC] = {
            	[_0] = SECOND_HYPHEN,
            	[_1] = SECOND_HYPHEN,
            	[_2] = SECOND_HYPHEN,
            	[_3] = SECOND_HYPHEN,
            	[_4] = SECOND_HYPHEN,
            	[_5] = SECOND_HYPHEN,
            	[_6] = SECOND_HYPHEN,
            	[_7] = SECOND_HYPHEN,
            	[_8] = SECOND_HYPHEN,
            	[_9] = SECOND_HYPHEN,
            },

            [SECOND_HYPHEN] = {
            	[HYPHEN] = FIRST_DIGIT_OF_DAY,
            },

            [FIRST_DIGIT_OF_DAY] = {
            	[_0] = SECOND_DIGIT_ZERO,
            	[_1] = SECOND_DIGIT_ONE_TWO,
            	[_2] = SECOND_DIGIT_ONE_TWO,
            	[_3] = SECOND_DIGIT_THREE,
            },

            [SECOND_DIGIT_ZERO] = {
            	[_1] = ACCEPT,
            	[_2] = ACCEPT,
            	[_3] = ACCEPT,
            	[_4] = ACCEPT,
            	[_5] = ACCEPT,
            	[_6] = ACCEPT,
            	[_7] = ACCEPT,
            	[_8] = ACCEPT,
            	[_9] = ACCEPT,
            },

            [SECOND_DIGIT_ONE_TWO] = {
            	[_0] = ACCEPT,
            	[_1] = ACCEPT,
            	[_2] = ACCEPT,
            	[_3] = ACCEPT,
            	[_4] = ACCEPT,
            	[_5] = ACCEPT,
            	[_6] = ACCEPT,
            	[_7] = ACCEPT,
            	[_8] = ACCEPT,
            	[_9] = ACCEPT,
            },

            [SECOND_DIGIT_THREE] = {
            	[_0] = ACCEPT,
            	[_1] = ACCEPT,
            },

        }

    };

    struct test_case {
        char input[16];
        int expected_result;
    };

    struct test_case tests[] = {
    	// Assert initial state
    	{"", FIRST_DIGIT},

    	// FIRST_DIGIT -> SECOND_DIGIT
    	{"1", SECOND_DIGIT},
    	{"2", SECOND_DIGIT},
    	{"-", TRAP},
    	{"0", TRAP},
    	{"3", TRAP},
    	{"4", TRAP},
    	{"5", TRAP},
    	{"6", TRAP},
    	{"7", TRAP},
    	{"8", TRAP},
    	{"9", TRAP},

    	// SECOND_DIGIT -> THIRD_DIGIT
    	{"19", THIRD_DIGIT},
    	{"29", THIRD_DIGIT},
    	{"10", THIRD_DIGIT},
    	{"20", THIRD_DIGIT},
    	{"15", THIRD_DIGIT},
    	{"25", THIRD_DIGIT},
    	{"2-", TRAP},

    	// THIRD_DIGIT -> FOURTH_DIGIT
    	{"190", FOURTH_DIGIT},
    	{"290", FOURTH_DIGIT},
    	{"199", FOURTH_DIGIT},
    	{"299", FOURTH_DIGIT},
    	{"19-", TRAP},

    	// FOURTH_DIGIT -> FIRST_HYPHEN
    	{"1990", FIRST_HYPHEN},
    	{"2990", FIRST_HYPHEN},
		{"1999", FIRST_HYPHEN},
    	{"2999", FIRST_HYPHEN},
    	{"299-", TRAP},

    	// FIRST_HYPHEN -> FIRST_DIGIT_OF_MONTH
    	{"1999-", FIRST_DIGIT_OF_MONTH},
    	{"2000-", FIRST_DIGIT_OF_MONTH},
    	{"19993", TRAP},
    	{"20003", TRAP},

    	// FIRST_DIGIT_OF_MONTH -> SECOND_DIGIT_JAN_TO_SEP
    	// FIRST_DIGIT_OF_MONTH -> SECOND_DIGIT_OCT_TO_DEC
    	{"1999-0", SECOND_DIGIT_JAN_TO_SEP},
    	{"2000-0", SECOND_DIGIT_JAN_TO_SEP},
		{"2000-1", SECOND_DIGIT_OCT_TO_DEC},
    	{"1999-1", SECOND_DIGIT_OCT_TO_DEC},
    	{"1999-2", TRAP},
    	{"2000-2", TRAP},
    	{"1999--", TRAP},
    	{"2000--", TRAP},
    	{"1999-9", TRAP},
    	{"2000-9", TRAP},

    	// SECOND_DIGIT_JAN_TO_SEP -> SECOND_HYPHEN
    	// SECOND_DIGIT_OCT_TO_DEC -> SECOND_HYPHEN
    	{"1999-01", SECOND_HYPHEN},
		{"1999-02", SECOND_HYPHEN},
		{"1999-03", SECOND_HYPHEN},
		{"1999-04", SECOND_HYPHEN},
		{"1999-05", SECOND_HYPHEN},
		{"1999-06", SECOND_HYPHEN},
		{"1999-07", SECOND_HYPHEN},
		{"1999-08", SECOND_HYPHEN},
		{"1999-09", SECOND_HYPHEN},
		{"1999-0-", TRAP},

    	{"1999-10", SECOND_HYPHEN},
    	{"1999-11", SECOND_HYPHEN},
    	{"1999-12", SECOND_HYPHEN},
    	{"1999-1-", TRAP},

    	{"2000-01", SECOND_HYPHEN},
    	{"2000-02", SECOND_HYPHEN},
    	{"2000-03", SECOND_HYPHEN},
    	{"2000-04", SECOND_HYPHEN},
    	{"2000-05", SECOND_HYPHEN},
    	{"2000-06", SECOND_HYPHEN},
    	{"2000-07", SECOND_HYPHEN},
    	{"2000-08", SECOND_HYPHEN},
    	{"2000-09", SECOND_HYPHEN},
    	{"2000-0-", TRAP},

    	{"2000-10", SECOND_HYPHEN},
    	{"2000-11", SECOND_HYPHEN},
    	{"2000-12", SECOND_HYPHEN},
    	{"2000-1-", TRAP},

    	// SECOND_HYPHEN -> FIRST_DIGIT_OF_DAY
    	{"1999-01-", FIRST_DIGIT_OF_DAY},
		{"1999-02-", FIRST_DIGIT_OF_DAY},
		{"1999-03-", FIRST_DIGIT_OF_DAY},
		{"1999-04-", FIRST_DIGIT_OF_DAY},
		{"1999-05-", FIRST_DIGIT_OF_DAY},
		{"1999-06-", FIRST_DIGIT_OF_DAY},
		{"1999-07-", FIRST_DIGIT_OF_DAY},
		{"1999-08-", FIRST_DIGIT_OF_DAY},
		{"1999-09-", FIRST_DIGIT_OF_DAY},
		{"1999-09-", FIRST_DIGIT_OF_DAY},
		{"1999-090", TRAP},
		{"1999-099", TRAP},

    	{"1999-10-", FIRST_DIGIT_OF_DAY},
    	{"1999-11-", FIRST_DIGIT_OF_DAY},
    	{"1999-12-", FIRST_DIGIT_OF_DAY},
    	{"1999-120", TRAP},
    	{"1999-129", TRAP},

    	{"2000-01-", FIRST_DIGIT_OF_DAY},
    	{"2000-02-", FIRST_DIGIT_OF_DAY},
    	{"2000-03-", FIRST_DIGIT_OF_DAY},
    	{"2000-04-", FIRST_DIGIT_OF_DAY},
    	{"2000-05-", FIRST_DIGIT_OF_DAY},
    	{"2000-06-", FIRST_DIGIT_OF_DAY},
    	{"2000-07-", FIRST_DIGIT_OF_DAY},
    	{"2000-08-", FIRST_DIGIT_OF_DAY},
    	{"2000-09-", FIRST_DIGIT_OF_DAY},
    	{"2000-090", TRAP},
    	{"2000-099", TRAP},

    	{"2000-10-", FIRST_DIGIT_OF_DAY},
    	{"2000-11-", FIRST_DIGIT_OF_DAY},
    	{"2000-12-", FIRST_DIGIT_OF_DAY},
    	{"2000-120", TRAP},
    	{"2000-129", TRAP},

    	// FIRST_DIGIT_OF_DAY -> SECOND_DIGIT_ZERO
    	{"1999-01-0", SECOND_DIGIT_ZERO},
		{"1999-02-0", SECOND_DIGIT_ZERO},
		{"1999-03-0", SECOND_DIGIT_ZERO},
		{"1999-04-0", SECOND_DIGIT_ZERO},
		{"1999-05-0", SECOND_DIGIT_ZERO},
		{"1999-06-0", SECOND_DIGIT_ZERO},
		{"1999-07-0", SECOND_DIGIT_ZERO},
		{"1999-08-0", SECOND_DIGIT_ZERO},
		{"1999-09-0", SECOND_DIGIT_ZERO},
		{"1999-09-4", TRAP},
		{"1999-09-9", TRAP},
		{"1999-09--", TRAP},	

    	{"1999-10-0", SECOND_DIGIT_ZERO},
    	{"1999-11-0", SECOND_DIGIT_ZERO},
    	{"1999-12-0", SECOND_DIGIT_ZERO},
    	{"1999-12-4", TRAP},
    	{"1999-12-9", TRAP},
    	{"1999-12--", TRAP},

    	{"2000-01-0", SECOND_DIGIT_ZERO},
    	{"2000-02-0", SECOND_DIGIT_ZERO},
    	{"2000-03-0", SECOND_DIGIT_ZERO},
    	{"2000-04-0", SECOND_DIGIT_ZERO},
    	{"2000-05-0", SECOND_DIGIT_ZERO},
    	{"2000-06-0", SECOND_DIGIT_ZERO},
    	{"2000-07-0", SECOND_DIGIT_ZERO},
    	{"2000-08-0", SECOND_DIGIT_ZERO},
    	{"2000-09-0", SECOND_DIGIT_ZERO},
    	{"2000-09-4", TRAP},
    	{"2000-09-9", TRAP},
    	{"2000-09--", TRAP},

    	{"2000-10-0", SECOND_DIGIT_ZERO},
    	{"2000-11-0", SECOND_DIGIT_ZERO},
    	{"2000-12-0", SECOND_DIGIT_ZERO},
    	{"2000-12-4", TRAP},
    	{"2000-12-9", TRAP},
    	{"2000-12--", TRAP},

    	// FIRST_DIGIT_OF_DAY -> SECOND_DIGIT_ONE_TWO
    	{"1999-01-1", SECOND_DIGIT_ONE_TWO},
		{"1999-02-1", SECOND_DIGIT_ONE_TWO},
		{"1999-03-1", SECOND_DIGIT_ONE_TWO},
		{"1999-04-1", SECOND_DIGIT_ONE_TWO},
		{"1999-05-1", SECOND_DIGIT_ONE_TWO},
		{"1999-06-1", SECOND_DIGIT_ONE_TWO},
		{"1999-07-1", SECOND_DIGIT_ONE_TWO},
		{"1999-08-1", SECOND_DIGIT_ONE_TWO},
		{"1999-09-1", SECOND_DIGIT_ONE_TWO},
		{"1999-09-4", TRAP},
		{"1999-09-9", TRAP},
		{"1999-09--", TRAP},

    	{"1999-10-1", SECOND_DIGIT_ONE_TWO},
    	{"1999-11-1", SECOND_DIGIT_ONE_TWO},
    	{"1999-12-1", SECOND_DIGIT_ONE_TWO},
    	{"1999-12-4", TRAP},
    	{"1999-12-9", TRAP},
    	{"1999-12--", TRAP},

    	{"2000-01-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-02-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-03-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-04-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-05-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-06-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-07-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-08-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-09-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-09-4", TRAP},
    	{"2000-09-9", TRAP},
    	{"2000-09--", TRAP},

    	{"2000-10-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-11-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-12-1", SECOND_DIGIT_ONE_TWO},
    	{"2000-12-4", TRAP},
    	{"2000-12-9", TRAP},
    	{"2000-12--", TRAP},

    	{"1999-01-2", SECOND_DIGIT_ONE_TWO},
		{"1999-02-2", SECOND_DIGIT_ONE_TWO},
		{"1999-03-2", SECOND_DIGIT_ONE_TWO},
		{"1999-04-2", SECOND_DIGIT_ONE_TWO},
		{"1999-05-2", SECOND_DIGIT_ONE_TWO},
		{"1999-06-2", SECOND_DIGIT_ONE_TWO},
		{"1999-07-2", SECOND_DIGIT_ONE_TWO},
		{"1999-08-2", SECOND_DIGIT_ONE_TWO},
		{"1999-09-2", SECOND_DIGIT_ONE_TWO},
		{"1999-09-4", TRAP},
		{"1999-09-9", TRAP},
		{"1999-09--", TRAP},

    	{"1999-10-2", SECOND_DIGIT_ONE_TWO},
    	{"1999-11-2", SECOND_DIGIT_ONE_TWO},
    	{"1999-12-2", SECOND_DIGIT_ONE_TWO},
    	{"1999-12-4", TRAP},
    	{"1999-12-9", TRAP},
    	{"1999-12--", TRAP},

    	{"2000-01-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-02-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-03-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-04-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-05-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-06-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-07-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-08-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-09-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-09-4", TRAP},
    	{"2000-09-9", TRAP},
    	{"2000-09--", TRAP},

    	{"2000-10-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-11-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-12-2", SECOND_DIGIT_ONE_TWO},
    	{"2000-12-4", TRAP},
    	{"2000-12-9", TRAP},
    	{"2000-12--", TRAP},

    	// FIRST_DIGIT_OF_DAY -> SECOND_DIGIT_THREE
    	{"1999-01-3", SECOND_DIGIT_THREE},
		{"1999-02-3", SECOND_DIGIT_THREE},
		{"1999-03-3", SECOND_DIGIT_THREE},
		{"1999-04-3", SECOND_DIGIT_THREE},
		{"1999-05-3", SECOND_DIGIT_THREE},
		{"1999-06-3", SECOND_DIGIT_THREE},
		{"1999-07-3", SECOND_DIGIT_THREE},
		{"1999-08-3", SECOND_DIGIT_THREE},
		{"1999-09-3", SECOND_DIGIT_THREE},
		{"1999-09-4", TRAP},
		{"1999-09-9", TRAP},
		{"1999-09--", TRAP},

    	{"1999-10-3", SECOND_DIGIT_THREE},
    	{"1999-11-3", SECOND_DIGIT_THREE},
    	{"1999-12-3", SECOND_DIGIT_THREE},
    	{"1999-12-4", TRAP},
    	{"1999-12-9", TRAP},
    	{"1999-12--", TRAP},

    	{"2000-01-3", SECOND_DIGIT_THREE},
    	{"2000-02-3", SECOND_DIGIT_THREE},
    	{"2000-03-3", SECOND_DIGIT_THREE},
    	{"2000-04-3", SECOND_DIGIT_THREE},
    	{"2000-05-3", SECOND_DIGIT_THREE},
    	{"2000-06-3", SECOND_DIGIT_THREE},
    	{"2000-07-3", SECOND_DIGIT_THREE},
    	{"2000-08-3", SECOND_DIGIT_THREE},
    	{"2000-09-3", SECOND_DIGIT_THREE},
    	{"2000-09-4", TRAP},
    	{"2000-09-9", TRAP},
    	{"2000-09--", TRAP},

    	{"2000-10-3", SECOND_DIGIT_THREE},
    	{"2000-11-3", SECOND_DIGIT_THREE},
    	{"2000-12-3", SECOND_DIGIT_THREE},
    	{"2000-12-4", TRAP},
    	{"2000-12-9", TRAP},
    	{"2000-12--", TRAP},

    	// SECOND_DIGIT_ZERO -> ACCEPT
        {"1999-01-01", ACCEPT},
        {"1999-02-02", ACCEPT},
        {"1999-03-03", ACCEPT},
        {"1999-04-04", ACCEPT},
        {"1999-05-05", ACCEPT},
        {"1999-06-06", ACCEPT},
        {"1999-07-07", ACCEPT},
        {"1999-08-08", ACCEPT},
        {"1999-09-09", ACCEPT},
        {"1999-09-00", TRAP},
        {"1999-09-0-", TRAP},

        {"1999-10-01", ACCEPT},
        {"1999-11-02", ACCEPT},
        {"1999-12-03", ACCEPT},
        {"1999-12-00", TRAP},
        {"1999-12-0-", TRAP},

        {"2000-01-01", ACCEPT},
        {"2000-02-02", ACCEPT},
        {"2000-03-03", ACCEPT},
        {"2000-04-04", ACCEPT},
        {"2000-05-05", ACCEPT},
        {"2000-06-06", ACCEPT},
        {"2000-07-07", ACCEPT},
        {"2000-08-08", ACCEPT},
        {"2000-09-09", ACCEPT},
        {"2000-09-00", TRAP},
        {"2000-09-0-", TRAP},

        {"2000-10-01", ACCEPT},
        {"2000-11-02", ACCEPT},
        {"2000-12-03", ACCEPT},
        {"2000-12-00", TRAP},
        {"2000-12-0-", TRAP},

        // SECOND_DIGIT_ONE_TWO -> ACCEPT
        {"1999-01-10", ACCEPT},
        {"1999-02-11", ACCEPT},
        {"1999-03-12", ACCEPT},
        {"1999-04-13", ACCEPT},
        {"1999-05-14", ACCEPT},
        {"1999-06-15", ACCEPT},
        {"1999-07-16", ACCEPT},
        {"1999-08-17", ACCEPT},
        {"1999-09-18", ACCEPT},
        {"1999-09-1-", TRAP},

        {"1999-10-19", ACCEPT},
        {"1999-11-10", ACCEPT},
        {"1999-12-11", ACCEPT},
        {"1999-12-1-", TRAP},

        {"2000-01-12", ACCEPT},
        {"2000-02-13", ACCEPT},
        {"2000-03-14", ACCEPT},
        {"2000-04-15", ACCEPT},
        {"2000-05-16", ACCEPT},
        {"2000-06-17", ACCEPT},
        {"2000-07-18", ACCEPT},
        {"2000-08-19", ACCEPT},
        {"2000-09-10", ACCEPT},
        {"2000-09-1-", TRAP},

        {"2000-10-11", ACCEPT},
        {"2000-11-12", ACCEPT},
        {"2000-12-13", ACCEPT},
        {"2000-12-1-", TRAP},

        {"1999-01-24", ACCEPT},
        {"1999-02-25", ACCEPT},
        {"1999-03-26", ACCEPT},
        {"1999-04-27", ACCEPT},
        {"1999-05-28", ACCEPT},
        {"1999-06-29", ACCEPT},
        {"1999-07-20", ACCEPT},
        {"1999-08-21", ACCEPT},
        {"1999-09-22", ACCEPT},
        {"1999-09-2-", TRAP},

        {"1999-10-23", ACCEPT},
        {"1999-11-24", ACCEPT},
        {"1999-12-25", ACCEPT},
        {"1999-12-2-", TRAP},

        {"2000-01-26", ACCEPT},
        {"2000-02-27", ACCEPT},
        {"2000-03-28", ACCEPT},
        {"2000-04-29", ACCEPT},
        {"2000-05-20", ACCEPT},
        {"2000-06-21", ACCEPT},
        {"2000-07-22", ACCEPT},
        {"2000-08-23", ACCEPT},
        {"2000-09-24", ACCEPT},
        {"2000-09-2-", TRAP},

        {"2000-10-25", ACCEPT},
        {"2000-11-26", ACCEPT},
        {"2000-12-27", ACCEPT},
        {"2000-12-2-", TRAP},

        // SECOND_DIGIT_THREE -> ACCEPT
        {"1999-01-30", ACCEPT},
        {"1999-02-31", ACCEPT},
        {"1999-03-30", ACCEPT},
        {"1999-04-31", ACCEPT},
        {"1999-05-30", ACCEPT},
        {"1999-06-31", ACCEPT},
        {"1999-07-30", ACCEPT},
        {"1999-08-31", ACCEPT},
        {"1999-09-30", ACCEPT},
        {"1999-09-3-", TRAP},
        {"1999-09-32", TRAP},
        {"1999-09-39", TRAP},

        {"1999-10-30", ACCEPT},
        {"1999-11-31", ACCEPT},
        {"1999-12-30", ACCEPT},
        {"1999-12-3-", TRAP},
        {"1999-12-32", TRAP},
        {"1999-12-39", TRAP},

        {"2000-01-31", ACCEPT},
        {"2000-02-30", ACCEPT},
        {"2000-03-31", ACCEPT},
        {"2000-04-30", ACCEPT},
        {"2000-05-31", ACCEPT},
        {"2000-06-30", ACCEPT},
        {"2000-07-31", ACCEPT},
        {"2000-08-30", ACCEPT},
        {"2000-09-31", ACCEPT},
        {"2000-09-3-", TRAP},
        {"2000-09-32", TRAP},
        {"2000-09-39", TRAP},

        {"2000-10-30", ACCEPT},
        {"2000-11-31", ACCEPT},
        {"2000-12-30", ACCEPT},
        {"2000-12-3-", TRAP},
        {"2000-12-32", TRAP},
        {"2000-12-39", TRAP},

        // Fuzzing
        {"},;/,:?}<?:", TRAP},
        {"\x1", TRAP}, {"\x2", TRAP}, {"\x3", TRAP}, {"\x4", TRAP},
        {"\x5", TRAP}, {"\x6", TRAP}, {"\x7", TRAP}, {"\x8", TRAP},
        {"\x9", TRAP}, {"\xa", TRAP}, {"\xb", TRAP}, {"\xc", TRAP},
        {"\xd", TRAP}, {"\xe", TRAP}, {"\xf", TRAP}, {"\x10", TRAP},
        {"\x11", TRAP}, {"\x12", TRAP}, {"\x13", TRAP}, {"\x14", TRAP},
        {"\x15", TRAP}, {"\x16", TRAP}, {"\x17", TRAP}, {"\x18", TRAP},
        {"\x19", TRAP}, {"\x1a", TRAP}, {"\x1b", TRAP}, {"\x1c", TRAP},
        {"\x1d", TRAP}, {"\x1e", TRAP}, {"\x1f", TRAP}, {"\x20", TRAP},
        {"\x21", TRAP}, {"\x22", TRAP}, {"\x23", TRAP}, {"\x24", TRAP},
        {"\x25", TRAP}, {"\x26", TRAP}, {"\x27", TRAP}, {"\x28", TRAP},
        {"\x29", TRAP}, {"\x2a", TRAP}, {"\x2b", TRAP}, {"\x2c", TRAP},
        {"\x2d", TRAP}, {"\x2e", TRAP}, {"\x2f", TRAP}, {"\x30", TRAP},
        {"\x33", TRAP}, {"\x34", TRAP}, {"\x35", TRAP}, {"\x36", TRAP},
        {"\x37", TRAP}, {"\x38", TRAP}, {"\x39", TRAP}, {"\x3a", TRAP},
        {"\x3b", TRAP}, {"\x3c", TRAP}, {"\x3d", TRAP}, {"\x3e", TRAP},
        {"\x3f", TRAP}, {"\x40", TRAP}, {"\x41", TRAP}, {"\x42", TRAP},
        {"\x43", TRAP}, {"\x44", TRAP}, {"\x45", TRAP}, {"\x46", TRAP},
        {"\x47", TRAP}, {"\x48", TRAP}, {"\x49", TRAP}, {"\x4a", TRAP},
        {"\x4b", TRAP}, {"\x4c", TRAP}, {"\x4d", TRAP}, {"\x4e", TRAP},
        {"\x4f", TRAP}, {"\x50", TRAP}, {"\x51", TRAP}, {"\x52", TRAP},
        {"\x53", TRAP}, {"\x54", TRAP}, {"\x55", TRAP}, {"\x56", TRAP},
        {"\x57", TRAP}, {"\x58", TRAP}, {"\x59", TRAP}, {"\x5a", TRAP},
        {"\x5b", TRAP}, {"\x5c", TRAP}, {"\x5d", TRAP}, {"\x5e", TRAP},
        {"\x5f", TRAP}, {"\x60", TRAP}, {"\x61", TRAP}, {"\x62", TRAP},
        {"\x63", TRAP}, {"\x64", TRAP}, {"\x65", TRAP}, {"\x66", TRAP},
        {"\x67", TRAP}, {"\x68", TRAP}, {"\x69", TRAP}, {"\x6a", TRAP},
        {"\x6b", TRAP}, {"\x6c", TRAP}, {"\x6d", TRAP}, {"\x6e", TRAP},
        {"\x6f", TRAP}, {"\x70", TRAP}, {"\x71", TRAP}, {"\x72", TRAP},
        {"\x73", TRAP}, {"\x74", TRAP}, {"\x75", TRAP}, {"\x76", TRAP},
        {"\x77", TRAP}, {"\x78", TRAP}, {"\x79", TRAP}, {"\x7a", TRAP},
        {"\x7b", TRAP}, {"\x7c", TRAP}, {"\x7d", TRAP}, {"\x7e", TRAP},
        {"\x7f", TRAP},

    };

    for (int i = 0; i < (int)(sizeof(tests) / sizeof(*tests)); i++) {
        printf("Testing case '%s'\n", tests[i].input);
        SET_STATE(machine, FIRST_DIGIT);
        for (int c = 0; tests[i].input[c]; c++) {
            (void)GET_NEXT_STATE(machine, &tests[i].input[c]);
        }
        if (GET_STATE(machine) != tests[i].expected_result) {
        	puts("");
        	printf("    Expected %s but got %s\n", texts[tests[i].expected_result], texts[GET_STATE(machine)]);
        	puts("");
        	return 1;
        }
    }

    return 0;
}
