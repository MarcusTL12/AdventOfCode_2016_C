#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/bitfield.h"
#include "../AoC_C_utils/src/file_util.h"

void d7p1() {
    char *input = file_read_full("inputfiles/day7/input");

    int counter = 0;

    bool in_bracket = false, abba_outside = false, abba_inside = false;

    for (char *p = input; *p; p++) {
        if (p[3] == '\n' || !p[3]) {
            p += 3;
            counter += abba_outside && !abba_inside;
            in_bracket = false;
            abba_inside = false;
            abba_outside = false;
            continue;
        }

        switch (*p) {
            case '[':
                in_bracket = true;
                break;
            case ']':
                in_bracket = false;
                break;
            default:
                if (p[0] == p[3] && p[1] == p[2] && p[0] != p[1]) {
                    if (in_bracket) {
                        abba_inside = true;
                    } else {
                        abba_outside = true;
                    }
                }
                break;
        }
    }

    printf("TLS: %i\n", counter);

    free(input);
}

static bool supports_ssl(char *line, char **next_line) {
    uint64_t outbits[11], inbits[11];

    memset(outbits, 0, 11 * sizeof(uint64_t));
    memset(inbits, 0, 11 * sizeof(uint64_t));

    bool in_bracket = false;

    for (; line[2] != '\n' && line[2]; line++) {
        if (*line == '[') {
            in_bracket = true;
            continue;
        } else if (*line == ']') {
            in_bracket = false;
            continue;
        }

        if (line[1] != '[' && line[1] != ']' && line[0] == line[2] &&
            line[0] != line[1]) {
            if (in_bracket) {
                int index =
                    ((int)(line[0] - 'a')) * 26 + ((int)(line[1] - 'a'));
                bitfield_set(inbits, index, true);
            } else {
                int index =
                    ((int)(line[1] - 'a')) * 26 + ((int)(line[0] - 'a'));
                bitfield_set(outbits, index, true);
            }
        }
    }

    bool ssl = false;

    for (int i = 0; i < 11; i++) {
        ssl |= (outbits[i] & inbits[i]);
    }

    if (!line[2]) {
        *next_line = NULL;
    } else {
        *next_line = line + 3;
    }

    return ssl;
}

void d7p2() {
    char *input = file_read_full("inputfiles/day7/input");

    char *line = input, *next_line = input;

    int counter = 0;

    while (next_line) {
        counter += supports_ssl(line, &next_line);
        line = next_line;
    }

    printf("SSL: %i\n", counter);

    free(input);
}