#include <stdio.h>
#include <stdlib.h>

#include "utils/vector.h"

#define N 32

int main(int argc, char* argv[]) {
    FILE *file;
    file = fopen("input/day1.txt", "r");
    char line[N];
    int cal;
    int acc = 0;
    int lc = 0;

    vector *vec = vec_create(8);
    if (vec == NULL)
        return -1;

    vec_debug(vec);

    while(1) {
        lc++;
        if (fgets(line, N, file) == NULL) {
            break;
        }
        // printf("%s", line);
        if (line[0] == '\n') {
            vec_append(vec, acc);
            // vec_debug(vec);
            acc = 0;
        } else {
            cal = strtol(line, NULL, 10);
            // printf("cal %d\n", cal);
            acc += cal;
        }
    };
    vec_bubble_sort(vec);
    vec_debug(vec);

    printf("Max cal: %d, sum top 3 %d\n", vec->array[vec->size-1], vec->array[vec->size-1] + vec->array[vec->size-2] + vec->array[vec->size-3]);

    // 32766 too low
    // 92764, 1445292102 too high
    return 0;
}