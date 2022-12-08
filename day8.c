#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H 99
#define W 99

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day8.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char forest[H][W];

    for (size_t i=0;i < H;i++) {
        if (fread(forest[i], 1, W, file) != W) {
            fprintf(stderr, "Could not read row %zu\n", i);
            return 1;
        }
        // line return
        fgetc(file);
    }

    int total_visible = 0;

    int best_scenic_score = 0;
    for (size_t y=0;y < H;y++) {
        for (size_t x=0;x < H;x++) {

            int visible = 0;
            // fprintf(stderr, "\n");
            // fprintf(stderr, "Checking tree at %zu %zu\n", x, y);
            int scenic_score = 1;
            for (int dx=-1;dx<2;dx+=2) {
                int flag = 1;
                int dir_scenic_score=0;
                for (size_t lx=x+dx;lx >= 0 && lx < W;lx+=dx) {
                    dir_scenic_score++;
                    if (forest[y][lx] >= forest[y][x]) {
                        // fprintf(stderr, "view blocked at %zu %zu\n", lx, y);
                        flag = 0;
                        break;
                    }
                }
                scenic_score *= dir_scenic_score;
                if (!visible && flag) {
                    visible = 1;
                    // fprintf(stderr, "Tree visible at %zu %zu\n", x, y);
                    total_visible += 1;
                }
            }
            for (int dy=-1;dy<2;dy+=2) {

                int flag = 1;
                int dir_scenic_score=0;
                for (size_t ly=y+dy;ly >= 0 && ly < H;ly+=dy) {
                    dir_scenic_score++;
                    if (forest[ly][x] >= forest[y][x]) {
                        // fprintf(stderr, "view blocked at %zu %zu\n", x, ly);
                        flag = 0;
                        break;
                    }
                }
                scenic_score *= dir_scenic_score;
                if (!visible && flag) {
                    visible = 1;
                    total_visible += 1;
                    // fprintf(stderr, "Tree visible at %zu %zu\n", x, y);
                }
            }
            // max
            if (scenic_score > best_scenic_score) {
                // fprintf(stderr, "best scenic score so far %d at %zu %zu", scenic_score, x, y);
                best_scenic_score = scenic_score;
            }
        }
    }

    printf("Total visible %d\n", total_visible);
    printf("Best scenic score %d\n", best_scenic_score);
    // 124 too low
    
    
    return 0;
}