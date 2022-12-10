#include <stdlib.h>
#include <stdio.h>

#define N 12671
#define PART_N 10

typedef struct
{
    int x;
    int y;
} point;

typedef struct
{
    size_t capacity;
    size_t len;

    point* values;
} set;

// not sure what im doing here
size_t point_hash(point p) {
    size_t hash = (p.x * 265443679) ^ (p.y * 2654435761);
    return hash;
}

set* set_create() {
    set* new_set = calloc(1, sizeof(set));
    if (new_set == NULL) {
        return NULL;
    }

    new_set->values = calloc(N, sizeof(new_set->values));
    if (new_set->values == NULL) {
        free(new_set);
        return NULL;
    }
    new_set->capacity = N;
    new_set->len = 0;

    return new_set;
}

void set_add(set* s, point value) {
    if (s->capacity == s->len) {
        fprintf(stderr, "set is already full, not doing anything\n");
    }

    // never added because used as empty space
    if (value.x == 0 && value.y == 0) {
        return;
    }

    int h = point_hash(value);
    size_t idx = h % s->capacity;

    while (1)
    {
        // free slot
        if (s->values[idx].x == 0 && s->values[idx].y == 0) {
            fprintf(stderr, "Adding %d:%d at %zu\n", value.x, value.y, idx);
            s->values[idx] = value;
            s->len += 1;
            break;
        }
        // already exists
        if (s->values[idx].x == value.x && s->values[idx].y == value.y) {
            break;
        }
        idx = (idx + 1) % s->capacity;
    }
    fprintf(stderr, "%ld collisions happened\n", (long)idx - (h % s->capacity));
}

int clip(int val) {
    if (val > 1) {
        return 1;
    }
    if (val < -1) {
        return -1;
    }
    return val;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day9.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char dir;
    int steps;

    set* pos = set_create();

    point rope[PART_N] = {0};
    int dx, dy;

    while (fscanf(file, "%c %d\n", &dir, &steps) == 2)
    {
        switch (dir)
        {
        case 'R':
            dx = 1;
            dy = 0;
            break;
        case 'L':
            dx = -1;
            dy = 0;
            break;
        case 'U':
            dx = 0;
            dy = 1;
            break;
        case 'D':
            dx = 0;
            dy = -1;
            break;
        default:
            printf("Unknown dir %c\n", dir);
            return 1;
            break;
        }

        for (int i=0;i<steps;i++) {
            rope[0].x += dx;
            rope[0].y += dy;

            for (int rix=1;rix < PART_N;rix++) {
                if (abs(rope[rix-1].x - rope[rix].x) > 1 || abs(rope[rix-1].y - rope[rix].y) > 1) {
                    rope[rix].x += clip(rope[rix-1].x - rope[rix].x);
                    rope[rix].y += clip(rope[rix-1].y - rope[rix].y);
                }
            }
            // add tail pos to set
            set_add(pos, rope[PART_N-1]);
        }
    }
    printf("Tail pos %d %d\n", rope[PART_N - 1].x, rope[PART_N - 1].y);
    printf("Following head pos %d %d\n", rope[1].x, rope[1].y);
    printf("Head pos %d %d\n", rope[0].x, rope[0].y);
    printf("Visited points %zu\n", pos->len + 1);
    
}