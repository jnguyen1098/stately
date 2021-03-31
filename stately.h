#ifndef STATELY_H
#define STATELY_H

#ifndef MAX_ALPHABET_SIZE
# define MAX_ALPHABET_SIZE 256
#endif

#ifndef MAX_STATES
# define MAX_STATES 128
#endif

#define SET_STATE(machine, state)(machine.curr_state = state)
#define GET_STATE(machine)(machine.curr_state)
#define SUPPOSE_STATE(machine, state, input)(machine.state_table[machine.curr_state][machine.map(input)])
#define GET_NEXT_STATE(machine, input)(machine.curr_state = SUPPOSE_STATE(machine, machine.curr_state, input), GET_STATE(machine))

struct state_machine {
    int curr_state;
    int (*map)(const void *);
    int state_table[MAX_STATES][MAX_ALPHABET_SIZE + 1];
};

#endif
