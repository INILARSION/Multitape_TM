#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "helper.h"

void print_tapes(struct tapes *tapes, struct program *program, char *prefix, int *head_positions) {
    for (int i = 0; i < program->head_count; ++i) {
        int head_position = head_positions[i];

        int head_offset = strlen(prefix) + 2;
        //print tape
        printf("%s: ", prefix);
        for (int j = 0; j < tapes->tapes[i]->length; ++j) {
            // calculate string offset of head position
            if (j < head_position) {
                head_offset += strlen(program->alphabet[tapes->tapes[i]->tape_arr[j]]) + 1;
            }
            printf("%s", program->alphabet[tapes->tapes[i]->tape_arr[j]]);
            // print delimiter between tape symbols
            if (i < tapes->tapes[i]->length - 1)
                printf("|");
        }
        printf("\n");

        //print head position
        for (int j = 0; j < head_offset; ++j) {
            printf(" ");
        }
        printf("^\n\n");
        int test = 12;
    }

}

void print_delta(struct program *program, struct deltas *delta) {
    printf("Delta: %s,", program->state_names[delta->state]);

    for (int i = 0; i < program->head_count; ++i) {
        printf("%s", program->alphabet[delta->read_symbols[i]]);
        if (i < program->head_count - 1)
            printf("|");
    }

    printf(",%s,", program->state_names[delta->subsequent_state]);

    for (int i = 0; i < program->head_count; ++i) {
        printf("%s", program->alphabet[delta->write_symbols[i]]);
        if (i < program->head_count - 1)
            printf("|");
    }

    printf(",");

    for (int i = 0; i < program->head_count; ++i) {
        printf("%c", delta->movements[i]);
        if (i < program->head_count - 1)
            printf("|");
    }

    printf("\n");
}


void simulate(struct tapes *tapes, struct program *program) {
    //struct configuration *configuration = malloc(sizeof(struct configuration));
    int *head_positions = calloc(program->head_count, sizeof(int));
    int current_state = program->start_state;
    struct sorted_deltas *deltas_of_state;
    struct deltas *delta = NULL;
    int wrong_read_symbol = 0;

    while (1) {
        deltas_of_state = &program->state_delta_mapping[current_state];

        // check read symbols and find delta to apply
        for (int i = 0; i < deltas_of_state->same_state_count; ++i) {
            for (int j = 0; j < program->head_count; ++j) {
                if (tapes->tapes[j]->tape_arr[head_positions[j]] != deltas_of_state->deltas_same_state[i]->read_symbols[j]) {
                    wrong_read_symbol = 1;
                    break;
                }
            }
            if (wrong_read_symbol) {
                wrong_read_symbol = 0;
            } else {
                wrong_read_symbol = 0;
                delta = deltas_of_state->deltas_same_state[i];
                break;
            }
        }
        // No delta found
        if (delta == NULL) {
            printf("Nicht Akzeptiert!\n");
            break;
        }

        // apply delta
        for (int i = 0; i < program->head_count; ++i) {
            // change symbol on tape for write symbol
            tapes->tapes[i]->tape_arr[head_positions[i]] = delta->write_symbols[i];


            switch (delta->movements[i]) {
                // move head right. Expand the tape if head is now out of bounds.
                case '>':
                    ++head_positions[i];
                    // if head index out of bounds of tape make a bigger tape, copy old tape, add empty symbol to last field of tape
                    if (head_positions[i] == tapes->tapes[i]->length){
                        int *new_tape = malloc((tapes->tapes[i]->length + 1) * sizeof(int));
                        // copy existing tape starting index 0. Last index will be the empty element.
                        memcpy(new_tape, tapes->tapes[i]->tape_arr, tapes->tapes[i]->length * sizeof(int));
                        new_tape[head_positions[i]] = 0;
                        free(tapes->tapes[i]->tape_arr);
                        ++tapes->tapes[i]->length;
                        tapes->tapes[i]->tape_arr = new_tape;
                    }
                    break;
                    // move head left. Expand the tape if head is now out of bounds.
                case '<':
                    --head_positions[i];
                    // if head index out of bounds of tape make a bigger tape, copy old tape, add empty symbol to first field of tape
                    if (head_positions[i] == -1){
                        head_positions[i] = 0;
                        int *new_tape = malloc((tapes->tapes[i]->length + 1) * sizeof(int));
                        // copy existing tape starting index 1. Index 0 will be the empty element.
                        memcpy(new_tape + 1, tapes->tapes[i]->tape_arr, tapes->tapes[i]->length * sizeof(int));
                        new_tape[0] = 0;
                        head_positions[i] = 0;
                        free(tapes->tapes[i]->tape_arr);
                        ++tapes->tapes[i]->length;
                        tapes->tapes[i]->tape_arr = new_tape;
                    }
                    break;
            }

        }

        if (program->is_verbose) {
            print_delta(program, delta);
            print_tapes(tapes, program, "Tape", head_positions);
        }

        current_state = delta->subsequent_state;

        if (current_state == program->accept_state) {
            printf("Accepted!\n");
            print_delta(program, delta);
            print_tapes(tapes, program, "Final tape", head_positions);
            break;
        } else if (current_state == program->reject_state) {
            printf("Nicht Akzeptiert!\n");
            break;
        }
        delta = NULL;
    }

}