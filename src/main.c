#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(void) {
    queue_t q;
    if (queue_init(&q) != 0) {
        fprintf(stderr, "Failed to init queue\n");
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        int *value = malloc(sizeof(int));
        *value = i;
        queue_push(&q, value);
    }

    while (queue_size(&q) > 0) {
        int *value = queue_pop(&q);
        if (value) {
            printf("Got %d\n", *value);
            free(value);
        }
    }

    queue_destroy(&q);
    return 0;
}
