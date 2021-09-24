#include <stdio.h>
#include "helper.h"

/*
 * Get the line count of a file.
 */
int get_file_line_count(FILE *file_ptr) {
    char *line = NULL;
    size_t buffer_size = 0;
    int line_count = 0;
    //count lines and reset to beginning of file
    while (getline(&line, &buffer_size, file_ptr) != -1) {
        ++line_count;
    }
    fseek(file_ptr, 0, 0);
    return line_count;
}