#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE* file;
    file = fopen("input/day2.txt", "r");
    char ap, bp;

    int score_1 = 0;
    int score_2 = 0;

    while (fscanf(file, "%c %c\n", &ap, &bp) > 0)
    {
        // printf("%c %c\n", ap, bp);
        ap = ap - 'A';
        bp = bp - 'X';
        score_1 += bp + 1;
        // printf("%d %d\n", ap, bp);
        if ((ap + 1) % 3 == bp) {
            score_1 += 6;
        } else if ((bp + 1) % 3 == ap) {
            score_1 += 0;
        } else {
            score_1 += 3;
        }
        // printf("Score: %d\n", score_1);
        // printf("Score 2: %d\n", score_2);
        score_2 += bp * 3;
        int rem = (ap + (bp - 1)) % 3;
        // modulo and remainder do not behave the same
        score_2 += ((rem < 0)?rem+3:rem) + 1;
    }
    printf("Score: %d\n", score_1);
    printf("Score 2: %d\n", score_2);
    // 15702
    // 15453 too low
    
    return 0;
}