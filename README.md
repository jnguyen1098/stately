<p align="center">
  <img src="https://i.imgur.com/scNvoFm.png">
</p>

# stately

Polymorphic template-based finite-state machine library written in C, delivered through a single header file. I made this for my game programming class (we had to implement state machine AI) and figured I'd probably make good use of it in other projects, so here it is.

Thanks to C's initialization quirks, invalid `char` inputs and trap (rejecting) states are handled automatically.

States are implemented internally as `int`s, but as you see in the `examples` folder, `enum`s can (and should) be used for readability.

A `state_machine` `struct` is declared as follows:

```c
struct state_machine {
    int curr_state;
    int (*map)(const void *);
    int state_table[MAX_STATES][MAX_ALPHABET_SIZE + 1];
};
```

Where:

* `curr_state` is the current state of the machine

* `map()` is a template function allowing the caller to map an arbitrary input to a state (`int`). This is analogous to the `cmp()` parameter in libc `qsort`. Just like `qsort`'s `cmp()`, `map()` takes in a `const void *` and outputs an `int`.

* `state_table[MAX_STATES][MAX_ALPHABET_SIZE + 1]` is the table used to map the states to each other by means of transitions. It is a 2-dimensional array of `int`s, each row representing the possible transitions from each state and each column representing the input symbol it takes in.

In actually manipulating the machine, the following interface is exposed (as macros):

* `SET_STATE(machine, state)` will set the machine's `curr_state` to argument `state`

* `GET_STATE(machine)` will retrieve the machine's `curr_state`

* `GET_NEXT_STATE(machine, input)` will feed the machine argument `input`, mutate its `curr_state` member and then return the `curr_state` just as you would with `GET_STATE(machine)`

* `SUPPOSE_STATE(machine, state, input)` is a purely functional macro that returns a given next state without changing or relying on the current state (`curr_state`). You specify the machine, the _supposed_ state, and the hypothetical input you would give it, and it would give you the _hypothetical_ output in return.

Taking a look at a simple example, we examine this DFA program that feeds a string to the finite-state machine character-by-character:

```c
   /***************************************
    *  DFA that accepts either the empty  *
    *  string, or any sequence of 1s.     *
    *                                     *
    *       1                    0,1      *
    *    +-----+               +----+     *
    *    |     |               |    |     *
    *    |    \|/              |   \|/    *
    * +--+---------+       +---+--------+ *
    * |            |       |            | *
    * | Accepting  |       |    TRAP    | *
    * |            |       |            | *
    * +-----+------+       +------------+ *
    *       |                    /|\      *
    *       |                     |       *
    *       |                     |       *
    *       +---------------------+       *
    *                 0                   *
    **************************************/

    enum input { INVALID, ZERO_CHAR, ONE_CHAR };
    enum state { TRAP, ACCEPTING };

    const char char_map[128] = {
        ['0'] = ZERO_CHAR,
        ['1'] = ONE_CHAR,
    };

    int map_chr(const void *chr) {
        return char_map[(int)*(const char *)chr];
    }

    struct state_machine machine = {
       
        // Start state
        .curr_state = ACCEPTING,

        // Input mapper
        .map = map_chr,

        // States
        .state_table = {

            // Reject state transitions
            [TRAP] = {
                [INVALID]   = TRAP,
                [ZERO_CHAR] = TRAP,
                [ONE_CHAR]  = TRAP,
            },

            // Accept state transitions
            [ACCEPTING] = {
                [INVALID]   = TRAP,
                [ZERO_CHAR] = TRAP,
                [ONE_CHAR]  = ACCEPTING,
            }

        }

    };
```

Examining the code bit-by-bit, first we see two enum declarations. One for input, one for state. The fact that we have two declarations is done for readability. In reality all we need are the labels.

```c
    enum input { INVALID, ZERO_CHAR, ONE_CHAR };
    enum state { TRAP, ACCEPTING };
```

Here we map characters to states. When we iterate a string, we feed individual characters to the DFA, and each character corresponds to an input. For the sake of readability and maintaining a clean namespace, it is recommended to map `char`s to states, and denote said states using `enum`s. Notice how the `char_map` declaration uses subscripted `[]` notation to initialize the array. Doing so allows us to initialize individual elements in an array without regard to order.

```c
    const char char_map[128] = {
        ['0'] = ZERO_CHAR,
        ['1'] = ONE_CHAR,
    };
```

On a tangential example, declaring

```c
const int arr[8] = {
    [3] = 100;
};
```

is equivalent to

```c
const int arr[8] = {
    0, 0, 100, 0, 0, 0, 0, 0
};
```

as initializing an array zeroes the other elements. Doing

```c
const int arr[8];
```

initializes none of the values and results in garbage. Looking back at the original snippet of code, it is equivalent to

```c
    const char char_map[128] = {
        [48] = ZERO_CHAR,
        [49] = ONE_CHAR,
    };
```

as `'0'` = `48` and `'1'` = `49`. Note an interesting quirk with C's initialization of arrays. Earlier it was said that when you initialize a C array, the rest of the elements are zero'd. In the particular case of a `char` array, by declaring an array of 128 `char`s (i.e., covering the entire range of ASCII characters), we can pick the characters to support as valid inputs (thanks to the readability of `enums`) and then leave the rest of them as invalids. By declaring our `enum` with `INVALID` as the first label, we default our `char_map` to `INVALID` (as `enum`s in C always start at 0). This means in our declaration, we tell stately that `'0'` (`48`) and `'1'` (`49`) are the only legal values: the rest are illegal (`0`/`'\0'`) and denoting them can be delegated to the `INVALID` label.

Now we attach `char_map` to the `map()` template function:

```c
    int map_chr(const void *chr) {
        return char_map[(int)*(const char *)chr];
    }
```

It is pretty mundane as we are just returning the state associated with the `char` in the `char_map`, but when you start working with `struct`s as machine inputs, the `map()` function starts becoming more useful.

Now we look at the actual initialization of the FSA:

```c
    struct state_machine machine = {
       
        // Start state
        .curr_state = ACCEPTING,

        // Input mapper
        .map = map_chr,

        // States
        .state_table = {

            // Reject state transitions
            [TRAP] = {
                [INVALID]   = TRAP,
                [ZERO_CHAR] = TRAP,
                [ONE_CHAR]  = TRAP,
            },

            // Accept state transitions
            [ACCEPTING] = {
                [INVALID]   = TRAP,
                [ZERO_CHAR] = TRAP,
                [ONE_CHAR]  = ACCEPTING,
            }

        }

    };
```

We initialize the current state like so,

```c
    .curr_state = ACCEPTING,
```

designate the template `map()` function like so,

```c
    .map = map_chr,
```

and then initialize the state table using the same initialization syntax as earlier.

```c
    // States
    .state_table = {

        // Trap state transitions
        [TRAP] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = TRAP,
        },

        // Accept state transitions
        [ACCEPTING] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = ACCEPTING,
        }

    }
```

Reading the rows in the `state_table` in order, we see that when the FSA is in a rejecting state, all three possible inputs (`'0'`, `'1'`, and invalid input) all result in rejection. This is typical for more-formal DFAs (a lot of informal DFAs tend to not show the trap state and it is just implied that an invalid input immediately kills the machine).

Note another quirk with C initialization of arrays. If you recall the `0` assumption trick mentioned earlier in the section about `char_map`, the same trick can be used here. By denoting the trap (rejecting) state as the first state in the state `enum` declaration, we can assume that as long as there is something initialized in the `state_table`, all undeclared transitions default to the trap state (i.e. instant rejection). In reality, this declaration is unneeded:

```c
    // Trap state transitions
    [TRAP] = {
        [INVALID]   = TRAP,
        [ZERO_CHAR] = TRAP,
        [ONE_CHAR]  = TRAP,
    },
```

as because `enum`s start counting from `0` and our `TRAP` (rejecting) state is the first listed in the `state` `enum`, those lines actually are equivalent to:

```c
    // Trap state transitions
    [0] = {
        [INVALID]   = 0,
        [ZERO_CHAR] = 0,
        [ONE_CHAR]  = 0,
    },
```

in fact, if we look at the declaration of the `state_table` as a whole:

```c
    // States
    .state_table = {

        // Trap state transitions
        [TRAP] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = TRAP,
        },

        // Accept state transitions
        [ACCEPTING] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = ACCEPTING,
        }

    }
```

looking through the `enum`s, we actually get:

```c
    .state_table = {
        
        [0] = {
            [0]  = 0,
            [48] = 0,
            [49] = 0,
        },

        [1] = {
            [0]  = 0,
            [48] = 0,
            [49] = 1,
        }

    }
```

which reduces to:

```c
    .state_table = {
        { [0]  = 0, [48] = 0, [49] = 0 },
        { [0]  = 0, [48] = 0, [49] = 1 }
    }
```

which reduces to:

```c
    .state_table = {
        [1] = {0, 0, 1}
    }
```

which reduces to:

```c
    .state_table = {
        [1] = {[2] = 1}
    }
```

or

```c
    .state_table = {
        [ACCEPTING] = { [ONE_CHAR] = ACCEPTING }
    }
```

as everything else is mapped to the `TRAP` state, which we assumed to be the C default initializer value of 0. This one-line declaration is equivalent to the gigantic

```c
    // States
    .state_table = {

        // Trap state transitions
        [TRAP] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = TRAP,
        },

        // Accept state transitions
        [ACCEPTING] = {
            [INVALID]   = TRAP,
            [ZERO_CHAR] = TRAP,
            [ONE_CHAR]  = ACCEPTING,
        }

    }
```

shown at first, but it is sometimes worth listing every transition for the sake of understanding and clarity. That being said, in some of the code samples in the `examples/` folder, I do not include the trap state. The first subscript (row) is not encoded in the array, but is actually used with the second subscript (column) as a displacement to get to the next state. So, by relying on C's default initialization quirk, we can default the most common transition in DFAs (implied rejection) to `0`. For more in-depth examples of this initialization quirk, look at `valid_number.c` and `date_validator.c`.

In actually using the machine, for this particular example one could perform:

```c
for (int i = 0; input_string[i] != '\0'; i++) {
    GET_NEXT_STATE(machine, &input_string[i]);
}

puts(GET_STATE(machine) == ACCEPTING ? "Input accepted" : "Input rejected");
```

## Testing

In creating my example FSAs I create self-checking test harnesses that (usually) rely on test cases of the form:

```c
struct test_case {
    char input[16];
    int expected_result;
};
```

and then with an array of these `test_case`s, testing the stately machines using a test loop of the form:

```c
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
```

For a more in-depth gander at this, look at `date_validator.c` (and if you scroll to lines 634-698 you will get a taste of how robust the `TRAP` state / `INVALID` input initialization quirk is).

## Random Examples

_An example of creating a `map()` function that uses a `struct` to derive a state_:

```c
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
```

_Relying on a more explicit way of declaring `INVALID` input (and not relying on initializaiton quirks)_:

```c
enum input { INVALID, DIGIT, SCIENTIFIC_E, PLUS_MINUS, PERIOD };

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
```

## More Reading

[Modern-day regex vs DFA regex](https://swtch.com/~rsc/regexp/regexp1.html)
