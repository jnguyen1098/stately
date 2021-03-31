#ifndef STATELY_H
#define STATELY_H

#define ALPHABET_SIZE 2
#define NUM_CHAR 128
#define NUM_STATE 2
#define LABEL_SIZE 128

struct state_machine {
    int curr_state;
    int (*map)(const void *);
    int state_table[NUM_STATE][ALPHABET_SIZE + 1];
};

#define GET_STATE(machine)(machine.curr_state)
#define SUPPOSE_STATE(machine, state, input)(machine.state_table[machine.curr_state][machine.map(input)])
#define GET_NEXT_STATE(machine, input)(machine.curr_state = SUPPOSE_STATE(machine, machine.curr_state, input), GET_STATE(machine))

#endif
