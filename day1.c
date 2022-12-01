#include <stdio.h>
#include <stdlib.h>

#define N 32

int main(int argc, char* argv[]) {
    FILE *file;
    file = fopen("input/day1.txt", "r");
    char line[N];
    int cal;
    int acc = 0;
    int lc = 0;
    int max_cal1 = 0;
    int max_cal2 = 0;
    int max_cal3 = 0;

    while(1) {
        lc++;
        if (fgets(line, N, file) == NULL) {
            break;
        }
        // printf("%s", line);
        if (line[0] == '\n') {
            // if (acc > max_cal)
            //     printf("%d - current, max: %d, %d\n", lc, acc, max_cal);
            if (acc > max_cal1) {
                max_cal3 = max_cal2;
                max_cal2 = max_cal1;
                max_cal1 = acc;
            } else if (acc > max_cal2) {
                max_cal3 = max_cal2;
                max_cal2 = acc;
            } else if (acc > max_cal3) {
                max_cal3 = acc;
            }
            acc = 0;
        } else {
            cal = strtol(line, NULL, 10);
            // printf("cal %d\n", cal);
            acc += cal;
        }
    };
    printf("Max cal: %d %d %d = %d\n", max_cal1, max_cal2, max_cal3, max_cal1 + max_cal2 + max_cal3);

    // 32766 too low
    // 92764, 1445292102 too high
    return 0;
}