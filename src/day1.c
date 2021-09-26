#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"
#include "../packages/hashmap.c/hashmap.h"

void d1p1() {
    char *input = file_read_full("inputfiles/day1/input");

    void *reg = regex_compile("(?:R|L)(\\d+)");
    size_t captures[4];
    regex_iter it = regex_iter_create(reg, input);

    int x = 0, y = 0;
    int dx = 0, dy = 1;

    while (regex_next_match(&it, captures)) {
        char RL = input[captures[0]];
        int num = atoi(input + captures[2]);

        int tmp = dx;
        dx = -dy;
        dy = tmp;

        if (RL == 'R') {
            dx = -dx;
            dy = -dy;
        }

        x += dx * num;
        y += dy * num;
    }

    printf("answer: %i\n", abs(x) + abs(y));

    regex_iter_free(it);
    regex_free(reg);
    free(input);
}

static int p_cmp(const void *a, const void *b, void *udata) {
    return memcmp(a, b, 2 * sizeof(int));
}

static uint64_t p_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    return hashmap_sip(item, 2 * sizeof(int), seed0, seed1);
}

void d1p2() {
    char *input = file_read_full("inputfiles/day1/input");

    void *reg = regex_compile("(?:R|L)(\\d+)");
    size_t captures[4];
    regex_iter it = regex_iter_create(reg, input);

    int xy[2] = {0, 0};
    int dx = 0, dy = 1;

    struct hashmap *map =
        hashmap_new(2 * sizeof(int), 0, 0, 0, p_hash, p_cmp, NULL);

    hashmap_set(map, xy);

    while (regex_next_match(&it, captures)) {
        char RL = input[captures[0]];
        int num = atoi(input + captures[2]);

        int tmp = dx;
        dx = -dy;
        dy = tmp;

        if (RL == 'R') {
            dx = -dx;
            dy = -dy;
        }

        for (int i = 0; i < num; i++) {
            xy[0] += dx;
            xy[1] += dy;

            if (hashmap_get(map, xy)) {
                goto end_loop;
            }
            hashmap_set(map, xy);
        }
    }

end_loop:

    printf("answer: %i\n", abs(xy[0]) + abs(xy[1]));

    hashmap_free(map);
    regex_iter_free(it);
    regex_free(reg);
    free(input);
}
