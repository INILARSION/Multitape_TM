#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tape_parser.h"
#include "helper.h"

/*
 * Get the amount of fields on the tape.
 */
int get_tape_length(const char *line, size_t line_length) {
    int tape_length = 1;
    for (int i = 0; i < line_length; ++i) {
        if (line[i] == '|')
            ++tape_length;
    }
    return tape_length;
}

/*
 * Get substring from the line, delimiter is '|'. Destination char* can be uninitialized.
 */
int get_state_substr_from_line(char *line, char **dest) {
    int tmp_str_size = 0;
    while (line[tmp_str_size] != '|' && line[tmp_str_size] != '\n' && line[tmp_str_size] != '\0')
        ++tmp_str_size;

    *dest = calloc(tmp_str_size + 1, sizeof(char));
    strncpy(*dest, line, tmp_str_size);
    return tmp_str_size;
}

/*
 * Get index of matching state name of first substring in the line.
 */
int search_matching_alphabet_element(struct program *program, char *line, int *alphabet_element) {
    char *tmp_str;
    int tmp_str_size = get_state_substr_from_line(line, &tmp_str);
    int found_match = -1;

    for (int j = 0; j < program->alphabet_size; ++j) {
        if(strcmp(tmp_str, program->alphabet[j]) == 0) {
            *alphabet_element = j;
            found_match = 0;
            break;
        }
    }
    if (found_match == -1) {
        printf("Tape contains element which is not contained in the alphabet!");
        exit(-1);
    }
    return tmp_str_size;
}

/*
 * This function parses a file containing one tape.
 */
struct tapes *parse_tape(char *tape_file_path, struct program *program){
    FILE *file_ptr = fopen(tape_file_path, "r");
    size_t line_length;
    char *line = NULL;
    size_t buffer_size = 0;

    // check that file is found
    if(file_ptr == NULL) {
        printf("File %s not found!", tape_file_path);
        exit(1);
    }

    int line_count = get_file_line_count(file_ptr);

    struct tapes *tapes = malloc(sizeof(struct tapes));
    tapes->tape_count = line_count;
    tapes->tapes = malloc(line_count * sizeof(struct tape*));

    struct tape *tape;
    char *tmp_line;

    for (int i = 0; i < line_count; ++i) {
        tape = malloc(sizeof(struct tape));

        line_length = getline(&line, &buffer_size, file_ptr);
        tmp_line = strdup(line);

        if (line_length == -1)
            exit(-1);

        // exclude the newline
        if (line[line_length - 1] == '\n')
            --line_length;

        if (line_length == 0)
            tape->length = 0;

        tape->length = get_tape_length(tmp_line, line_length);
        tape->tape_arr = malloc(tape->length * sizeof(int));

        int alphabet_element_length;
        for (int j = 0; j < tape->length; ++j) {
            alphabet_element_length = search_matching_alphabet_element(program, line, &tape->tape_arr[j]);
            line += alphabet_element_length + 1;
        }
        tapes->tapes[i] = tape;
    }

    return tapes;
}
