#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"

typedef struct {
    char c;
    int n;
} char_count;

static int char_count_cmp(const void *a_, const void *b_) {
    char_count *a = (char_count *)a_, *b = (char_count *)b_;

    int cmp1 = b->n - a->n;

    if (cmp1 == 0) {
        return a->c - b->c;
    } else {
        return cmp1;
    }
}

void d4p1() {
    char *input = file_read_full("inputfiles/day4/input");

    void *reg = regex_compile("(.+)-(\\d+)\\[(\\w{5})\\]");
    size_t captures[2 * 4];
    regex_iter it = regex_iter_create(reg, input);

    int id_sum = 0;

    while (regex_next_match(&it, captures)) {
        char *name = input + captures[2];
        int count[26];
        memset(count, 0, 26 * sizeof(int));

        for (int i = 0; i < captures[3]; i++) {
            int j = name[i] - 'a';

            if (j >= 0 && j < 26) {
                count[j]++;
            }
        }

        char_count sorted[26];

        for (int i = 0; i < 26; i++) {
            sorted[i] = (char_count){.c = 'a' + i, .n = count[i]};
        }

        qsort(sorted, 26, sizeof(char_count), char_count_cmp);

        char *checksum = input + captures[6];
        bool isvalid = true;
        for (int i = 0; i < 5; i++) {
            isvalid &= checksum[i] == sorted[i].c;
        }

        if (isvalid) {
            id_sum += atoi(input + captures[4]);
        }
    }

    printf("Sum: %i\n", id_sum);

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}

void d4p2() {
    char *input = file_read_full("inputfiles/day4/input");

    void *reg = regex_compile("(.+)-(\\d+)\\[(\\w{5})\\]");
    size_t captures[2 * 4];
    regex_iter it = regex_iter_create(reg, input);

    while (regex_next_match(&it, captures)) {
        char *name = input + captures[2];
        int count[26];
        memset(count, 0, 26 * sizeof(int));

        for (int i = 0; i < captures[3]; i++) {
            int j = name[i] - 'a';

            if (j >= 0 && j < 26) {
                count[j]++;
            }
        }

        char_count sorted[26];

        for (int i = 0; i < 26; i++) {
            sorted[i] = (char_count){.c = 'a' + i, .n = count[i]};
        }

        qsort(sorted, 26, sizeof(char_count), char_count_cmp);

        char *checksum = input + captures[6];
        bool isvalid = true;
        for (int i = 0; i < 5; i++) {
            isvalid &= checksum[i] == sorted[i].c;
        }

        if (isvalid) {
            int id = atoi(input + captures[4]);
            char *north = "north";
            bool is_north = true;
            for (int i = 0; i < 5; i++) {
                char c = ((name[i] - 'a') + id) % 26 + 'a';
                is_north &= c == north[i];
            }
            if (is_north) {
                printf("id: %i\n", id);
                goto end;
            }
        }
    }

    end:

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}
