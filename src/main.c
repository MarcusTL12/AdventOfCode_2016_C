#include <stdio.h>
#include <stdlib.h>

#include "days.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Enter day and part as command line arguments!\n");
    } else {
        int day = atoi(argv[1]) - 1;
        int part = atoi(argv[2]) - 1;

        int index = 2 * day + part;

        days[index]();
    }
}
