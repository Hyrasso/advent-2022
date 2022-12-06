#include <stdio.h>
#include <stdlib.h>

#define NSTACK 9
#define N 128

#define PART 1

void parse(FILE* file, char stacks[][N]) {
    int col = 0;
    size_t row = 0;
    while(1) {
        char c = fgetc(file);
        if (c == '\n') {
            col = -1;
            row += 1;
        }
        if (c >= 'A' && c <= 'Z') {
            // add to the top of the stack
            int stack_idx = (col - 1) / 4;
            // printf("Found %c, putting it in stack %d\n", c, stack_idx);
            for (size_t j=1;j <= N;j++) {
                if (stacks[stack_idx][N - j] == 0) {
                    stacks[stack_idx][N - j] = c;
                    break;
                }
            }
        }
        if (c == '1') {
            // read until line return
            char c;
            while ((c = fgetc(file)) != '\n') {}
            // next line return
            fgetc(file);
            break;
        }
            
        col++;
    }

    // reverse stacks
    // printf("Reversing stacks\n");
    for (size_t stack_idx=0; stack_idx < NSTACK;stack_idx++) {
        size_t stack_top = 0;
        for (size_t i=0; i < N;i++) {
            if (stacks[stack_idx][i] != 0) {
                // printf("Reversing %c at %zu to %zu\n", stacks[stack_idx * N + i], i, stack_top);
                stacks[stack_idx][stack_top] = stacks[stack_idx][i];
                stacks[stack_idx][i] = 0;
                stack_top++;
            }

        }
    }
}

void print_stacks(char stacks[][N]) {
    for (size_t i=0;i<NSTACK;i++) {
        printf("%zu ", i + 1);
        for (size_t j=0;j<N;j++) {
            if (stacks[i][j] == 0) {
                // printf("_ ");
            } else {
                printf("%c ", stacks[i][j]);
            }
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day5.txt", "r");
    if (file == NULL)
        perror("Error opening file");

    char stacks[NSTACK][N] = {0};

    printf("Parsing\n");
    parse(file, stacks);
    print_stacks(stacks);
    printf("\n");

    int count, from, to;

    while(fscanf(file, "move %d from %d to %d\n", &count, &from, &to) == 3) {
        // printf("count: %d from: %d to: %d\n", count, from, to);
        // start at 0
        from--;
        to--;
        size_t from_stack_top = 0;
        for (;stacks[from][from_stack_top] != 0;from_stack_top++);
        size_t to_stack_top = 0;
        for (;stacks[to][to_stack_top] != 0;to_stack_top++);
        // printf("ST %zu %zu\n", from_stack_top, to_stack_top);
        // switch for part 2
        #if PART == 1
        for (int i = 0;i<count;i++) {
            stacks[to][to_stack_top + i] = stacks[from][from_stack_top - i - 1];
            stacks[from][from_stack_top - i - 1] = 0;
        }
        #else
        for (int i = 0;i<count;i++) {
            stacks[to][to_stack_top + i] = stacks[from][from_stack_top + i - count];
            stacks[from][from_stack_top + i - count] = 0;
        }
        #endif

        // print_stacks(stacks);
        // printf("\n");
    }

    // read top of the stacks
    for (size_t stack_idx=0;stack_idx < NSTACK;stack_idx++) {
        size_t stack_top = 0;
        for (;stacks[stack_idx][stack_top] != 0;stack_top++);
        printf("%c", stacks[stack_idx][stack_top - 1]);
    }
    printf("\n");

    return 0;
}