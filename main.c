#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "program_parser.h"
#include "tape_parser.h"
#include "simulator.h"

int main(int argc, char** argv) {
    char *program_file_path;
    char *tape_file_path;
    int is_verbose;

    if (argc == 3) {
        is_verbose = 0;
        program_file_path = argv[1];
        tape_file_path = argv[2];
    } else if (argc == 4) {
        if (strcmp(argv[1], "-v") != 0) {
            printf("Usage: %s [optional flag: -v] [Program file] [Tape file]", argv[0]);
            return -1;
        }
        is_verbose = 1;
        program_file_path = argv[2];
        tape_file_path = argv[3];
    } else {
        printf("Usage: %s [optional flag: -v] [Program file]  [Tape file]", argv[0]);
        return -1;
    }

    struct program *program = parse_program(program_file_path, tape_file_path);
    program->is_verbose = is_verbose;

    struct tapes *tapes = parse_tape(tape_file_path, program);

    simulate(tapes, program);

    return 0;
}
