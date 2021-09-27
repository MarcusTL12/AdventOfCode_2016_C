#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/md5_util.h"

const char *input = "ffykfhsq";
// const char *input = "abc";

void d5p1() {
    char md5_buf[16];
    char hex_buf[6];
    char text_buf[32];
    memset(text_buf, 0, 32);

    size_t inp_len = strlen(input);
    memcpy(text_buf, input, inp_len);

    char *num_buf = text_buf + inp_len;

    char pass_buf[8];

    for (int i = 1, j = 0; j < 8; i++) {
        int num_len = sprintf(num_buf, "%i", i);
        md5(md5_buf, text_buf, inp_len + num_len);
        raw2hex(hex_buf, md5_buf, 3);
        if (!memcmp("00000", hex_buf, 5)) {
            pass_buf[j] = hex_buf[5];
            printf("%c", hex_buf[5]);
            fflush(stdout);
            j++;
        }
    }

    printf("\n");
}

void d5p2() {
    char md5_buf[16];
    char hex_buf[8];
    char text_buf[32];
    memset(text_buf, 0, 32);

    size_t inp_len = strlen(input);
    memcpy(text_buf, input, inp_len);

    char *num_buf = text_buf + inp_len;

    char pass_buf[8];
    memset(pass_buf, '_', 8);
    printf("%.*s", 8, pass_buf);
    fflush(stdout);

    for (int i = 1, j = 0; j < 8; i++) {
        int num_len = sprintf(num_buf, "%i", i);
        md5(md5_buf, text_buf, inp_len + num_len);
        raw2hex(hex_buf, md5_buf, 4);
        if (!memcmp("00000", hex_buf, 5)) {
            int ind = hex_buf[5] - '0';
            if (0 <= ind && ind < 8 && pass_buf[ind] == '_') {
                pass_buf[ind] = hex_buf[6];
                j++;
                printf("\r%.*s", 8, pass_buf);
                fflush(stdout);
            }
        }
    }

    printf("\n");
}
