#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../AoC_C_utils/src/file_util.h"

void d9p1() {
    char *input = file_read_full("inputfiles/day9/input");

    size_t counter = 0;

    for (char *c = input; *c != '\n' && *c; c++) {
        if (*c == '(') {
            c++;
            size_t substr_len = atoi(c);
            while (isdigit(*c)) c++;
            c++;
            size_t reps = atoi(c);
            while (isdigit(*c)) c++;
            c += substr_len;
            counter += substr_len * reps;
        } else {
            counter++;
        }
    }

    printf("Length: %zu\n", counter);

    free(input);
}

static size_t multipass_length(char *str, size_t len) {
    size_t counter = 0;

    for (size_t i = 0; i < len && str[i] != '\n' && str[i]; i++) {
        if (str[i] == '(') {
            i++;
            size_t substr_len = atoi(str + i);
            while (isdigit(str[i])) i++;
            i++;
            size_t reps = atoi(str + i);
            while (isdigit(str[i])) i++;
            i++;
            counter += reps * multipass_length(str + i, substr_len);
            i += substr_len - 1;
        } else {
            counter++;
        }
    }

    return counter;
}

void d9p2() {
    char *input = file_read_full("inputfiles/day9/input");

    printf("%zu\n", multipass_length(input, -1));

    free(input);
}
