#include <stdio.h>
#include <stdlib.h>

#define N 64
#define BUFFER_SIZE 512

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
    if (s->capacity < s->len * 2) {
        fprintf(stderr, "updating set size %zu, len %zu\n", s->capacity, s->len);
        point* new_val = calloc(s->capacity * 2, sizeof(*(s->values)));
        if (new_val == NULL)
            fprintf(stderr, "set is already full and failed new alloc\n");
        set* new_s = malloc(sizeof(set));
        new_s->values = new_val;
        new_s->len = 0;
        new_s->capacity = s->capacity * 2;
        for (size_t i=0;i < s->capacity;i++) {
            set_add(new_s, s->values[i]);
        }
        // free(s->values);
        // free(s);
        *s = *new_s;
        fprintf(stderr, "updating set size new capacity %zu, len %zu\n", s->capacity, s->len);
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
            // fprintf(stderr, "Adding %d:%d at %zu\n", value.x, value.y, idx);
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
    // fprintf(stderr, "%ld collisions happened\n", (long)idx - (h % s->capacity));
}

int set_contains(set* s, point value) {
    // never added because used as empty space
    if (value.x == 0 && value.y == 0) {
        fprintf(stderr, "W testing dummy value\n");
        return 0;
    }

    int h = point_hash(value);
    size_t idx = h % s->capacity;

    while (1)
    {
        // free slot
        if (s->values[idx].x == 0 && s->values[idx].y == 0) {
            // fprintf(stderr, "Adding %d:%d at %zu\n", value.x, value.y, idx);
            return 0;
        }
        // already exists
        if (s->values[idx].x == value.x && s->values[idx].y == value.y) {
            return 1;
        }
        idx = (idx + 1) % s->capacity;
    }
}


int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day14.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    set* filled_space = set_create();

    char buffer[BUFFER_SIZE];

    int expected_size = 0;
    int maxy = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        // fprintf(stderr, "L %s", buffer);
        char* p = buffer;
        int startx = strtol(p, &p, 10);
        p++; // skip ,
        int starty = strtol(p, &p, 10);
        p += 4;
        // fprintf(stderr, "S %d:%d\n", startx, starty);
        while (1) {
            int endx = strtol(p, &p, 10);
            p++; // skip ,
            int endy = strtol(p, &p, 10);
            // fprintf(stderr, "S %d:%d -> %d:%d\n", startx, starty, endx, endy);

            if (startx != endx) {
                int dx = (startx < endx) ? 1 : -1;
                for (int i=0;i <= abs(startx - endx);i++) {
                    // fprintf(stderr, "%d:%d\n", startx + i*dx, starty);
                    set_add(filled_space, (point){startx + i*dx, starty});
                    expected_size++;
                }
            }
            if (starty != endy) {
                int dy = (starty < endy) ? 1 : -1;
                for (int i=0;i <= abs(starty - endy);i++) {
                    // fprintf(stderr, "%d:%d\n", startx, starty + i*dy);
                    maxy = (maxy < starty + i*dy) ? starty + i*dy : maxy;
                    set_add(filled_space, (point){startx, starty + i*dy});
                    expected_size++;
                }
            }
            expected_size++;
            startx = endx;
            starty = endy;

            if (*p == '\n') {
                break;
            }
            p += 4;
        }
    }
    fprintf(stderr, "Occupied spaces: %zu %d\n", filled_space->len, expected_size);
    fprintf(stderr, "maxy: %d \n", maxy);
    
    int count = 0;
    int run = 1;
    while (run)
    {
        // add new at 500, 0
        point new_p = {500, 0};
        // fprintf(stderr, "Occupied spaces: %zu\n", filled_space->len);

        if (set_contains(filled_space, new_p)) {
            break;
        }

        while (1)
        {
            if (maxy + 1 == new_p.y) {
                set_add(filled_space, new_p);
                break;
            }

            if (set_contains(filled_space, (point){new_p.x, new_p.y + 1})) {
                if (!set_contains(filled_space, (point){new_p.x - 1, new_p.y + 1})) {
                    new_p = (point){new_p.x - 1, new_p.y + 1};
                    continue;
                }
                if (!set_contains(filled_space, (point){new_p.x + 1, new_p.y + 1})) {
                    new_p = (point){new_p.x + 1, new_p.y + 1};
                    continue;
                }
                set_add(filled_space, new_p);
                break;
            }
            if (new_p.y > maxy + 2) {
                fprintf(stderr, "Stopping adding new grains %d:%d\n", new_p.x, new_p.y);
                run = 0;
                break;
            }
            new_p.y++;
        }
        count++;
    }

    printf("Count %d\n", count - 1);
    

    return 0;
}