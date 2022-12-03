#include <stdio.h>
#include <assert.h>

#define N 64

int get_score(char c) {
    if (c < 'a') {
        return c - 'A' + 27;
    }
    return c - 'a' + 1;
}

int main(int argc, char* argv[]) {
    FILE* file;
    file = fopen("input/day3.txt", "r");
    if (file == NULL)
        perror("Could not open file");
    
    int len, len1, len2;
    char line[N] = {0};
    char line1[N] = {0};
    char line2[N] = {0};
    int score = 0;
    int priority = 0;
    int line_priority = 0;
    int line_number = 0;

    while(1) {
        // Endof file, probably
        if (fgets(line, N, file) == NULL)
            break;
        
        for (len=0;len < N;len++) {
            if (line[len] == '\n')
                break;
        }
        assert(len < N-1);

        int comp_len = len / 2;
        // printf("%d %d %s", len, comp_len, line);
        int line_score = 0;
        for (int i=0;i < comp_len;i++) {
            for (int j=0;j < comp_len;j++) {
                if (line[i] == line[j + comp_len]) {
                    line_score = get_score(line[i]);
                    // printf("%c matching at %d %d, score: %d\n", line[i], i, j, line_score);
                    break;
                }
            }
            if (line_score)
                break;
        }
        score += line_score;

        switch (line_number % 3) {
        case 0:
            len1 = len;
            for (int i=0;i < len;i++)
                line1[i] = line[i];
            break;
        case 1:
            len2 = len;
            for (int i=0;i < len;i++)
                line2[i] = line[i];
            break;
        case 2:
            line_priority = 0;
            for (int i=0;i < len;i++) {
                for (int j=0;j < len1;j++) {
                    for (int k=0;k < len2;k++) {
                        if ((line[i] == line1[j]) && (line[i] == line2[k])) {
                            line_priority = get_score(line[i]);
                            break;
                        }
                    }
                    if (line_priority)
                        break;
                }
                if (line_priority)
                    break;
            }
            priority += line_priority;
            break;
        }
        line_number++;
    }
    printf("Score: %d\n", score);
    printf("Priority sum: %d\n", priority);

    fclose(file);
    return 0;
}