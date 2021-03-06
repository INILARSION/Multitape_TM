#pragma once
#include <stdio.h>

int get_file_line_count(FILE *file_ptr);

/*
 * This structure encodes a standard tape as a string with its size
 */
struct tape {
    int length;
    int *tape_arr;
};

struct tapes {
    // list of all tape structs
    struct tape **tapes;
    int tape_count;
};

/*
 * Struct contains one delta/transition of the program
 */
struct deltas {
    // index of state name in state_names in program struct
    int state;
    // index of subsequent state name in state_names in program struct
    int subsequent_state;
    // List of index of read alphabet element in alphabet in program struct
    int *read_symbols;
    // List of index of write alphabet element in alphabet in program struct
    int *write_symbols;
    // List of move direction of heads. Can be '<', '>' and '-'.
    char *movements;
};

/*
 * Helper struct which contains a list and size of list of deltas-pointers of the same state name.
 */
struct sorted_deltas {
    // list of deltas-pointers with the same state
    struct deltas **deltas_same_state;
    // number of deltas of the same state name, size of deltas_same_state
    int same_state_count;
};

/*
 * Struct contains all information of the TM-Program
 */
struct program {
    int head_count;
    // Array of all state names.
    char **state_names;
    // Number of different states.
    int state_count;
    int start_state;
    int accept_state;
    int reject_state;
    // Array of all elements of the alphabet. Elements are strings.
    char **alphabet;
    // Number of different alphabet elements
    int alphabet_size;
    // Array of all deltas/transitions of the Program
    struct deltas **deltas;
    //Number of all deltas/transitions
    int deltas_count;
    // list of sorted_deltas structs.
    // Size is deltas_counts
    // Index of list corresponds to the state name in the state_names list, ie. state number 1 has index 1
    struct sorted_deltas *state_delta_mapping;
    // This flag saves if this simulation should run Verbose.
    // 0 = False, 1 = True
    int is_verbose;
};