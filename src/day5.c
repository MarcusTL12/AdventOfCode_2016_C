#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/md5_util.h"

// const char *input = "ffykfhsq";
const char *input = "abc";

void d5p1() {
    char md5_buf[32];
    char text_buf[32];
    memset(text_buf, 0, 32);

    size_t inp_len = strlen(input);
    memcpy(text_buf, input, inp_len);

    char *num_buf = text_buf + inp_len;

    char pass_buf[8];

    for (int i = 1, j = 0; j < 8; i++) {
        int num_len = sprintf(num_buf, "%i", i);
        md5(md5_buf, text_buf, inp_len + num_len);
        if (!memcmp("00000", md5_buf, 5)) {
            pass_buf[j] = md5_buf[5];
            printf("%c\n", md5_buf[5]);
            j++;
        }
    }

    printf("%.*s\n", 8, pass_buf);
}

void d5p2() {
    char md5_buf[32];
    char text_buf[32];
    memset(text_buf, 0, 32);

    size_t inp_len = strlen(input);
    memcpy(text_buf, input, inp_len);

    char *num_buf = text_buf + inp_len;

    char pass_buf[8];
    memset(pass_buf, ' ', 8);

    for (int i = 1, j = 0; j < 8; i++) {
        int num_len = sprintf(num_buf, "%i", i);
        md5(md5_buf, text_buf, inp_len + num_len);
        if (!memcmp("00000", md5_buf, 5)) {
            int ind = md5_buf[5] - '0';
            if (pass_buf[ind] == ' ') {
                pass_buf[ind] = md5_buf[6];
                j++;
                printf("%.*s\n", 8, pass_buf);
            }
        }
    }

    // printf("%.*s\n", 8, pass_buf);
}
