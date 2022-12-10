#include <stdlib.h>
#include <stdio.h>

int step(int* i, int x) {
    // draw pixel
    if (abs(((*i-1) % 40) - x) < 2) {
        putc('#', stdout);
    } else {
        putc('.', stdout);
    }
    if (((*i-1) % 40) == 39) {
        putc('\n', stdout);
    }
    // part 1 score
    int steps[6] = {20, 60, 100, 140, 180, 220};
    int step_val = 0;
    for (size_t k=0;k<6;k++) {
        if (*i == steps[k]) {
            step_val = x * (*i);
            fprintf(stderr, "Increasing score %d\n", step_val);
        }
    }
    *i = *i+1;
    return step_val;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day10.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int x = 1;
    int score = 0;
    int val;
    int count = 1;
    char line[16];
    while (fgets(line, 16, file) != NULL)
    {
        fprintf(stderr, "L %d X=%d : %s", count, x, line);
        if (line[0] == 'n') {
            score += step(&count, x);
            continue;
        }
        if (sscanf(line+5, "%d", &val) != 1)
            return -1;
        score += step(&count, x);
        score += step(&count, x);
        x += val;
    }
    fprintf(stderr, "\n");
    printf("Score: %d\n", score);
    return 0;
    
}