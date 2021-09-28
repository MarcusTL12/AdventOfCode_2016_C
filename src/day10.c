#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

#define int_t int

typedef struct {
    bool vals_set, val1_bot, val2_bot, val1_hi, val2_hi;
    int_t val1, val2;
} bot_t;

// Bits in bools:
// 0: is bot
// 1: 0 => lo, 1 => hi
typedef struct {
    uint8_t bools;
    int_t val;
} out_t;

static void get_amt_outs_and_bots(char *input, size_t *outs, size_t *bots) {
    void *reg = regex_compile("(\\w+) (\\d+)");
    regex_iter it = regex_iter_create(reg, input);
    size_t captures[6];

    *outs = 0;
    *bots = 0;

    while (regex_next_match(&it, captures)) {
        char t = input[captures[2]];
        size_t id = atoi(input + captures[4]);
        if (t == 'o') {
            *outs = max(*outs, id);
        } else if (t == 'b') {
            *bots = max(*bots, id);
        }
    }

    (*outs)++;
    (*bots)++;

    regex_iter_free(it);
    regex_free(reg);
}

static void compute_bot(bot_t *bots, size_t index) {
    bot_t *b = &bots[index];

    if (b->val1_bot) {
        compute_bot(bots, b->val1);
        bot_t *b2 = &bots[b->val1];
        if (b->val1_hi) {
            b->val1 = b2->val2;
        } else {
            b->val1 = b2->val1;
        }
    }

    b->val1_bot = false;

    if (b->val2_bot) {
        compute_bot(bots, b->val2);
        bot_t *b2 = &bots[b->val2];
        if (b->val2_hi) {
            b->val2 = b2->val2;
        } else {
            b->val2 = b2->val1;
        }
    }

    b->val2_bot = false;

    if (b->val1 > b->val2) {
        int_t tmp = b->val1;
        b->val1 = b->val2;
        b->val2 = tmp;
    }
}

static int_t get_out_val(bot_t *bots, out_t *outs, size_t index) {
    out_t *o = &outs[index];
    if (o->bools & 1) {
        compute_bot(bots, o->val);
        bot_t *b = &bots[o->val];
        if (o->bools & 0b10) {
            o->val = b->val2;
        } else {
            o->val = b->val1;
        }
        o->bools = 0;
    }
    return o->val;
}

static void print_bots(bot_t *bots, size_t amt_bots) {
    for (size_t i = 0; i < amt_bots; i++) {
        bot_t *b = &bots[i];
        printf("Bot %zu: ", i);
        if (b->val1_bot) {
            printf("val1");
            if (b->val1_hi) {
                printf(" high ");
            } else {
                printf(" low ");
            }
            printf("from bot %d, ", b->val1);
        } else {
            printf("val1 = %d, ", b->val1);
        }
        if (b->val2_bot) {
            printf("val2");
            if (b->val2_hi) {
                printf(" high ");
            } else {
                printf(" low ");
            }
            printf("from bot %d\n", b->val2);
        } else {
            printf("val2 = %d\n", b->val2);
        }
    }
}

static void do_stuff(void(stuff)(out_t *outs, size_t amt_outs, bot_t *bots,
                                 size_t amt_bots)) {
    char *input = file_read_full("inputfiles/day10/input");

    size_t amt_outs, amt_bots;
    get_amt_outs_and_bots(input, &amt_outs, &amt_bots);

    void *val_reg = regex_compile("value (\\d+) goes to bot (\\d+)");
    void *bot_reg = regex_compile(
        "bot (\\d+) gives low to (\\w+) (\\d+) and high to (\\w+) (\\d+)");

    regex_iter val_it = regex_iter_create(val_reg, input);
    regex_iter bot_it = regex_iter_create(bot_reg, input);

    size_t captures[2 * 6];

    out_t *outs = malloc(amt_outs * sizeof(out_t));
    memset(outs, 0, amt_outs * sizeof(out_t));

    bot_t *bots = malloc(amt_bots * sizeof(bot_t));
    memset(bots, 0, amt_bots * sizeof(bot_t));

    while (regex_next_match(&val_it, captures)) {
        int_t val = atoi(input + captures[2]);
        bot_t *cur_bot = &bots[atoi(input + captures[4])];

        if (cur_bot->vals_set) {
            cur_bot->val2 = val;
        } else {
            cur_bot->val1 = val;
            cur_bot->vals_set = true;
        }
    }

    while (regex_next_match(&bot_it, captures)) {
        size_t in_bot = atoi(input + captures[2]);
        char lo_type = input[captures[4]];
        size_t lo_num = atoi(input + captures[6]);
        char hi_type = input[captures[8]];
        size_t hi_num = atoi(input + captures[10]);

        if (lo_type == 'o') {
            out_t *o = &outs[lo_num];

            o->val = in_bot;
            o->bools = 0b1;
        } else if (lo_type == 'b') {
            bot_t *b = &bots[lo_num];

            if (b->vals_set) {
                b->val2 = in_bot;
                b->val2_bot = true;
            } else {
                b->val1 = in_bot;
                b->vals_set = true;
                b->val1_bot = true;
            }
        } else {
            printf("Shit went wrong!\n");
        }

        if (hi_type == 'o') {
            out_t *o = &outs[hi_num];

            o->val = in_bot;
            o->bools = 0b11;
        } else if (hi_type == 'b') {
            bot_t *b = &bots[hi_num];

            if (b->vals_set) {
                b->val2 = in_bot;
                b->val2_bot = true;
                b->val2_hi = true;
            } else {
                b->val1 = in_bot;
                b->vals_set = true;
                b->val1_bot = true;
                b->val1_hi = true;
            }
        } else {
            printf("Shit went wrong!\n");
        }
    }

    stuff(outs, amt_outs, bots, amt_bots);

    free(bots);
    free(outs);
    regex_iter_free(bot_it);
    regex_iter_free(val_it);
    regex_free(bot_reg);
    regex_free(val_reg);
    free(input);
}

#define LO 17
#define HI 61

static void check_responsible(out_t *outs, size_t amt_outs, bot_t *bots,
                              size_t amt_bots) {
    for (size_t i = 0; i < amt_bots; i++) {
        compute_bot(bots, i);
        bot_t *b = &bots[i];
        if (b->val1 == LO && b->val2 == HI) {
            printf("Bot number: %zu\n", i);
            break;
        }
    }
}

static void get_outputs(out_t *outs, size_t amt_outs, bot_t *bots,
                        size_t amt_bots) {
    int_t ans = get_out_val(bots, outs, 0) * get_out_val(bots, outs, 1) *
                get_out_val(bots, outs, 2);
    printf("Product: %d\n", ans);
}

void d10p1() { do_stuff(check_responsible); }

void d10p2() { do_stuff(get_outputs); }
