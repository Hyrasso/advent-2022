#include <stdlib.h>
#include <stdio.h>

#define N 128

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day13-sample.txt", "r");
    if (file == NULL) {
        return -1;
    }

    char line1[N];
    char line2[N];

    int score = 0;

    while (1)
    {
        fgets(line1, N, file);
        fgets(line2, N, file);

        fprintf(stderr, "%s", line1);
        fprintf(stderr, "%s", line2);

        size_t i1 = 0;
        size_t i2 = 0;
        for(;line1[i1] == '[';i1++);
        for(;line2[i2] == '[';i2++);

        
        //empty line
        if (fgets(line1, N, file) == NULL) {
            break;
        }
    }
    

    return 0;
}