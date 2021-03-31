#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stately.h"

enum input { INVALID, EVEN_INPUT, ODD_INPUT };
enum state { TRAP, START, EVEN_STATE, ODD_STATE };

struct request {
    int a;
    int b;
    int c;
};

int request_to_state(const void *req_ptr) {
    struct request req = *(struct request *)req_ptr;
    return (req.a + req.b + req.c) % 2 ? ODD_INPUT : EVEN_INPUT;
}

int main(void)
{
   /*************************************************************************
    * Moore machine that determines whether a running tally is even or odd  *
    *                           +---------------------+                     *
    *                           |                     |                     *
    *                           |                     |                     *
    *                   +-------+     START STATE     +-------+             *
    *                   |       |                     |       |             *
    *              EVEN |       |                     |       | ODD         *
    *                   |       +---------------------+       |             *
    *                   |                                     |             *
    *                   |                                     |             *
    *                  \|/                                   \|/            *
    *          +--------+----------+         +-------------+-----+          *
    *          |                   |         |                   |          *
    *          |                   |   ODD   |                   |          *
    *          |                   +-------> +                   |          *
    *          |                   |         |                   |          *
    * +------> +    EVEN STATE     |         |     ODD STATE     + <------+ *
    * |        |                   |  EVEN   |                   |        | *
    * |        |                   + <-------+                   |        | *
    * |        |                   |         |                   |        | *
    * |        |                   |         |                   |        | *
    * |        +--------+----------+         +----------+--------+        | *
    * |                 |                               |                 | *
    * |                 |                               |                 | *
    * |                 |                               |                 | *
    * +-----------------+                               +-----------------+ *
    *         ODD                                               EVEN        *
    ************************************************************************/

    struct state_machine machine = {
       
        // Start state
        .curr_state = START,

        // Input mapper
        .map = request_to_state,

        // States
        .state_table = {

            // Start state transitions
            [START] = {
                [EVEN_INPUT] = EVEN_STATE,
                [ODD_INPUT]  = ODD_STATE,
            },

            // Even state transitions
            [EVEN_STATE] = {
                [EVEN_INPUT] = EVEN_STATE,
                [ODD_INPUT]  = ODD_STATE,
            },

            // Odd state transitions
            [ODD_STATE] = {
                [EVEN_INPUT] = ODD_STATE,
                [ODD_INPUT]  = EVEN_STATE,
            },

        }

    };

    struct test_step {
        struct request request;
        int expected_result;
    };

    {
        puts("Test One");
        machine.curr_state = START;
        struct test_step steps[] = {
            { { 1, 2, 3 }, EVEN_STATE },
            { { 1, 9, 7 }, ODD_STATE  },
            { { 1, 9, 8 }, ODD_STATE  },
            { { 1, 2, 9 }, ODD_STATE  },
            { { 1, 2, 8 }, EVEN_STATE },
            { { 2, 0, 3 }, ODD_STATE  },
            { { 2, 0, 4 }, ODD_STATE  },
            { { 2, 8, 1 }, EVEN_STATE },
            { { 2, 8, 2 }, EVEN_STATE },
        };

        for (int i = 0; i < (int)(sizeof(steps) / sizeof(*steps)); i++) {
            printf("Applying state {%d, %d, %d} (%d), should be %s\n",
                steps[i].request.a, steps[i].request.b, steps[i].request.c,
                steps[i].request.a + steps[i].request.b + steps[i].request.c,
                steps[i].expected_result == EVEN_STATE ? "EVEN" : "ODD");
            (void)GET_NEXT_STATE(machine, &steps[i].request);
            assert(GET_STATE(machine) == steps[i].expected_result);
        }
    }

    {
        puts("Test Two");
        machine.curr_state = START;
        struct test_step steps[] = {
            { { 1, 2, 2 }, ODD_STATE  },
            { { 1, 9, 7 }, EVEN_STATE },
            { { 1, 9, 8 }, EVEN_STATE },
            { { 1, 2, 9 }, EVEN_STATE },
            { { 1, 2, 8 }, ODD_STATE  },
            { { 2, 0, 3 }, EVEN_STATE },
            { { 2, 0, 4 }, EVEN_STATE },
            { { 2, 8, 1 }, ODD_STATE  },
            { { 2, 8, 2 }, ODD_STATE  },
        };

        for (int i = 0; i < (int)(sizeof(steps) / sizeof(*steps)); i++) {
            printf("Applying state {%d, %d, %d} (%d), should be %s\n",
                steps[i].request.a, steps[i].request.b, steps[i].request.c,
                steps[i].request.a + steps[i].request.b + steps[i].request.c,
                steps[i].expected_result == EVEN_STATE ? "EVEN" : "ODD");
            (void)GET_NEXT_STATE(machine, &steps[i].request);
            assert(GET_STATE(machine) == steps[i].expected_result);
        }
    }

    puts("Complete");

    return 0;
}
