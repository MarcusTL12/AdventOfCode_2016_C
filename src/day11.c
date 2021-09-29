#include <stdio.h>

#include "../AoC_C_utils/src/deque.h"

void d11p1() {
    deque_t q = deque_create(sizeof(int));

    int list[15] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};

    for (int i = 0; i < 8; i++) deque_push_back(&q, &list[i]);

    int buf;

    for (int i = 0; i < 4; i++) {
        deque_pop_front(&q, &buf);
        deque_push_back(&q, &buf);
    }

    

    for (int i = 8; i < 15; i++) deque_push_back(&q, &list[i]);

    while (deque_pop_front(&q, &buf)) {
        printf("%i\n", buf);
    }

    deque_free(&q);
}

void d11p2() {}
