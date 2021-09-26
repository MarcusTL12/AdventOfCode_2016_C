#include <stdio.h>
#include <stdlib.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"

static bool valid_triangle(int sides[3]) {
    int sum = sides[0] + sides[1] + sides[2];

    return ((2 * sides[0]) < sum) && ((2 * sides[1]) < sum) &&
           ((2 * sides[2]) < sum);
}

void d3p1() {
    char *input = file_read_full("inputfiles/day3/input");

    void *reg = regex_compile("(\\d+) +(\\d+) +(\\d+)");
    size_t captures[2 * 4];
    regex_iter it = regex_iter_create(reg, input);

    int count = 0;

    while (regex_next_match(&it, captures)) {
        int nums[3];

        for (int i = 0; i < 3; i++) {
            nums[i] = atoi(input + captures[2 * (i + 1)]);
        }

        count += valid_triangle(nums);
    }

    printf("Count: %i\n", count);

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}

void d3p2() {
    char *input = file_read_full("inputfiles/day3/input");

    void *reg = regex_compile("(\\d+) +(\\d+) +(\\d+)");
    size_t captures[2 * 3 * 4];
    regex_iter it = regex_iter_create(reg, input);

    int count = 0;

    while (regex_next_match(&it, captures)) {
        regex_next_match(&it, captures + 2 * 4);
        regex_next_match(&it, captures + 4 * 4);

        for (int i = 0; i < 3; i++) {
            int nums[3];

            for (int j = 0; j < 3; j++) {
                nums[j] = atoi(input + captures[2 * 4 * j + 2 * (i + 1)]);
            }

            count += valid_triangle(nums);
        }
    }

    printf("Count: %i\n", count);

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}