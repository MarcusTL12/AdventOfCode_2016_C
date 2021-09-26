#include <stdio.h>
#include <stdlib.h>

#include "../AoC_C_utils/src/file_util.h"

static char p1_coord2key(int x, int y) { return (y * 3 + x) + '1'; }

static char p2_coord2key(int x, int y) {
    static const char keypad[25] = "  1   234 56789 ABC   D  ";

    return keypad[x + y * 5];
}

static void find_code(char(coord2key)(int, int), int size) {
    char *input = file_read_full("inputfiles/day2/input");

    char code[10] = "";
    int i = 0;

    char *c = input;
    for (int x = 0, y = 2; *c; c++) {
        int old_x = x, old_y = y;
        switch (*c) {
            case 'U':
                y--;
                break;
            case 'L':
                x--;
                break;
            case 'R':
                x++;
                break;
            case 'D':
                y++;
                break;
            case '\n':
                code[i] = coord2key(x, y);
                i++;
                break;
            default:
                printf("Invalid character in input file!\n");
                return;
                break;
        }
        if (x < 0 || x > size || y < 0 || y > size || coord2key(x, y) == ' ') {
            x = old_x;
            y = old_y;
        }
    }

    printf("Code: %s\n", code);

    free(input);
}

void d2p1() {
    find_code(p1_coord2key, 2);
}

void d2p2() {
    find_code(p2_coord2key, 4);
}
