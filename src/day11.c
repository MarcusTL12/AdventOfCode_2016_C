#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/deque.h"
#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"
#include "../packages/hashmap.c/hashmap.h"

#define LINES 4

typedef struct {
    char *name;
    size_t name_len, chip_floor, gen_floor;
} substance;

typedef struct {
    size_t *chips, *gens, index;
} iter_data;

static int s_cmp(const void *a_, const void *b_, void *udata) {
    substance *a = (substance *)a_, *b = (substance *)b_;

    int len_cmp = a->name_len - b->name_len;

    if (len_cmp) {
        return 1;
    } else {
        return memcmp(a->name, b->name, a->name_len);
    }
}

static uint64_t s_hash(const void *item_, uint64_t seed0, uint64_t seed1) {
    substance *item = (substance *)item;
    return hashmap_sip(item->name, item->name_len, seed0, seed1);
}

static bool s_iter(const void *item_, void *udata) {
    substance *item = (substance *)item_;
    iter_data *it_dat = (iter_data *)udata;

    it_dat->chips[it_dat->index] = item->chip_floor;
    it_dat->gens[it_dat->index] = item->gen_floor;

    it_dat->index++;
    return true;
}

static void parse_input(char *filename, size_t **chips, size_t **gens,
                        size_t *amt_substances) {
    char *input = file_read_full(filename);

    char *lines[LINES];
    size_t line_lengths[LINES], cur_line = 0;

    memset(line_lengths, 0, LINES * sizeof(size_t));

    for (char *c = input; *c; c++) {
        if (*c != '\n') {
            if (!line_lengths[cur_line]) lines[cur_line] = c;
            line_lengths[cur_line]++;
        } else {
            cur_line++;
        }
    }

    void *reg = regex_compile("a (\\w+)(?:-compatible)? (\\w+)");
    regex_iter it = regex_iter_create(reg, NULL);
    size_t captures[6];

    struct hashmap *name_map =
        hashmap_new(sizeof(substance), 0, 0, 0, s_hash, s_cmp, NULL);

    for (size_t line_nr = 0; line_nr < LINES; line_nr++) {
        char *cur_str = lines[line_nr];
        regex_iter_update(&it, cur_str, line_lengths[line_nr]);

        while (regex_next_match(&it, captures)) {
            substance buf;
            buf.name = cur_str + captures[2];
            buf.name_len = captures[3];
            buf.chip_floor = 0;
            buf.gen_floor = 0;

            substance *sub = hashmap_get(name_map, &buf);

            bool exists = sub != NULL;

            if (!exists) sub = &buf;

            if (cur_str[captures[4]] == 'm') {
                sub->chip_floor = line_nr;
            } else if (cur_str[captures[4]] == 'g') {
                sub->gen_floor = line_nr;
            }

            if (!exists) hashmap_set(name_map, sub);
        }
    }

    *amt_substances += hashmap_count(name_map);

    *chips = malloc(*amt_substances * sizeof(size_t));
    *gens = malloc(*amt_substances * sizeof(size_t));

    iter_data it_dat;
    it_dat.chips = *chips;
    it_dat.gens = *gens;
    it_dat.index = 0;

    hashmap_scan(name_map, s_iter, &it_dat);

    hashmap_free(name_map);
    regex_iter_free(it);
    regex_free(reg);
    free(input);
}

void d11p1() {
    size_t *chips, *gens, amt_substances = 0;

    parse_input("inputfiles/day11/ex1", &chips, &gens, &amt_substances);

    for (size_t i = 0; i < amt_substances; i++) {
        printf("Subs %zu: Chip on floor %zu, Gen on floor %zu\n", i, chips[i],
               gens[i]);
    }

    free(chips);
    free(gens);
}

void d11p2() {}
