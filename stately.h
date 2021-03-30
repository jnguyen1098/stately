#ifndef STATELY_H
#define STATELY_H

#define ALPHABET_SIZE 2
#define NUM_CHAR 128
#define NUM_STATE 2

enum input {
    INVALID,
    ZERO_CHAR,
    ONE_CHAR,
};

enum state {
    REJECTING,
    ACCEPTING,
};

struct state_machine {
    enum input char_map[NUM_CHAR];
    enum state state_table[NUM_STATE][ALPHABET_SIZE + 1];
};

#endif
