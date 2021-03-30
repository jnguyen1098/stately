#ifndef STATELY_H
#define STATELY_H

#define ALPHABET_SIZE 2
#define NUM_CHAR 128
#define NUM_STATE 2
#define LABEL_SIZE 128

struct state_machine {
    int state;
    int (*map)(const void *);
    int state_table[NUM_STATE][ALPHABET_SIZE + 1];
};

int get_state(struct state_machine *machine)
{
    return machine->state;
}

int get_next_state(struct state_machine *machine, const void *input)
{
    machine->state = machine->state_table[machine->state][machine->map(input)];
    return get_state(machine);
}

#endif
