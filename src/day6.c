#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"

static void find_msg(bool (cmp)(int a, int b)) {
    char *input = file_read_full("inputfiles/day6/input");

    void *reg = regex_compile("\\w+");
    size_t captures[2];
    regex_iter it = regex_iter_create(reg, input);

    int msg_len = 0;
    for (; input[msg_len] != '\n'; msg_len++)
        ;

    int *counters = malloc(26 * msg_len * sizeof(int));
    memset(counters, 0, 26 * msg_len * sizeof(int));

    while (regex_next_match(&it, captures)) {
        char *line = input + *captures;

        for (int i = 0; i < msg_len; i++) {
            int letter_ind = line[i] - 'a';
            counters[letter_ind + i * 26]++;
        }
    }

    char *msg = malloc(msg_len);

    for (int i = 0; i < msg_len; i++) {
        int minmax_letter_ind = 0;
        int minmax_letter_count = counters[i * 26];

        for (int j = 1; j < 26; j++) {
            int c = counters[i * 26 + j];
            if (cmp(c, minmax_letter_count)) {
                minmax_letter_ind = j;
                minmax_letter_count = c;
            }
        }

        msg[i] = 'a' + minmax_letter_ind;
    }

    printf("Message: %.*s\n", msg_len, msg);

    free(msg);
    free(counters);

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}

static bool less(int a, int b) {
    return a < b;
}

static bool greater(int a, int b) {
    return a > b;
}

void d6p1() {
    find_msg(greater);
}

void d6p2() {
    find_msg(less);
}
