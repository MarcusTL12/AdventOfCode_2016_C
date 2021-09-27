#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/bitfield.h"
#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"

#define WIDTH 50
#define HEIGHT 6

static void display_screen(uint64_t *screen) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            size_t index = x + y * WIDTH;
            printf("%c", bitfield_get(screen, index) ? '#' : '.');
        }
        printf("\n");
    }
}

static void get_screen(uint64_t *screen) {
    char *input = file_read_full("inputfiles/day8/input");

    void *line_reg = regex_compile(".+");
    size_t line_captures[2];
    regex_iter line_it = regex_iter_create(line_reg, input);

    void *rect_reg = regex_compile("rect (\\d+)x(\\d+)");
    void *rot_reg = regex_compile("rotate \\w+ \\w=(\\d+) by (\\d+)");
    size_t ins_captures[2 * 3];

    memset(screen, 0, 5 * sizeof(uint64_t));

    while (regex_next_match(&line_it, line_captures)) {
        char *line = input + line_captures[0];

        if (line[1] == 'e') {
            regex_match(rect_reg, line, ins_captures);

            int a = atoi(line + ins_captures[2]);
            int b = atoi(line + ins_captures[4]);

            for (int y = 0; y < b; y++) {
                for (int x = 0; x < a; x++) {
                    bitfield_set(screen, x + WIDTH * y, true);
                }
            }
        } else if (line[1] == 'o') {
            regex_match(rot_reg, line, ins_captures);

            int a = atoi(line + ins_captures[2]);
            int b = atoi(line + ins_captures[4]);

            if (line[7] == 'r') {
                uint64_t buf = 0;
                int y = a;
                for (int x = 0; x < WIDTH; x++) {
                    bitfield_set(&buf, x, bitfield_get(screen, WIDTH * y + x));
                }
                for (int x = 0; x < WIDTH; x++) {
                    bitfield_set(screen, WIDTH * y + (x + b) % WIDTH,
                                 bitfield_get(&buf, x));
                }
            } else if (line[7] == 'c') {
                uint64_t buf = 0;
                int x = a;
                for (int y = 0; y < HEIGHT; y++) {
                    bitfield_set(&buf, y, bitfield_get(screen, WIDTH * y + x));
                }
                for (int y = 0; y < HEIGHT; y++) {
                    bitfield_set(screen, WIDTH * ((y + b) % HEIGHT) + x,
                                 bitfield_get(&buf, y));
                }
            } else {
                printf("Something went wrong\n");
            }
        } else {
            printf("Something went wrong\n");
        }
    }

    regex_free(rot_reg);
    regex_free(rect_reg);

    regex_iter_free(line_it);
    regex_free(line_reg);
    free(input);
}

void d8p1() {
    uint64_t screen[5];
    get_screen(screen);

    size_t counter = 0;
    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
        counter += bitfield_get(screen, i);
    }

    printf("Pixels: %zu\n", counter);
}

void d8p2() {
    uint64_t screen[5];
    get_screen(screen);
    display_screen(screen);
}
