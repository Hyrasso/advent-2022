
#include <stdio.h>
#include <stdlib.h>

#include "utils/dict.h"

#define N 64

typedef struct
{
    int x;
    int y;
    int count;
} point;

size_t point_hash(const void* tep) {
    point p = *(point*)tep;
    size_t hash = (p.x * 265443679) ^ (p.y * 2654435761);
    return hash;
}

int point_cmp(const void* tepa, const void* tepb) {
    point pa = *(point*)tepa;
    point pb = *(point*)tepb;

    return (pa.x == pb.x) && (pa.y == pb.y);
}

void point_print(void* tep) {
    point* p = tep;
    fprintf(stderr, "%d, %d", p->x, p->y);
}

dict* step(dict* elves, dict* target_pos, size_t round, int passn) {
    dict* nelves = NULL;
    if (passn == 1) {
        nelves = dict_create(point_hash, point_cmp);
    }
    if (passn == 0) {
        // empty target_pos
        for (size_t idx=0;idx < target_pos->capacity;idx++) {
            free(target_pos->entries[idx]);
            target_pos->entries[idx] = NULL;
        }
    }

    point* query = malloc(sizeof(point));
    // for each elf
    for (size_t idx=0;idx < elves->capacity;idx++) {
        if (elves->entries[idx] != NULL) {
            // move elf
            point e = *(point*)elves->entries[idx];
            point* t = NULL;
            // check surroundings
            // do it twice, 1st time announce move, 2nd time move if the spot is free
            int nncount=0;
            for (size_t dir=0;dir < 4;dir++) {
                dir = (dir + round) % 4;
                int ncount = 0;
                switch (dir)
                {
                case 0:
                    // check north
                    query->y = e.y - 1;
                    query->x = e.x;
                    ncount += (dict_get(elves, query) != NULL);
                    query->x = e.x - 1;
                    ncount += (dict_get(elves, query) != NULL);
                    query->x = e.x + 1;
                    ncount += (dict_get(elves, query) != NULL);
                    // if no neighbor increase count
                    nncount += ncount == 0;
                    // if noone north, move there
                    if (ncount == 0 && t == NULL) {
                        t = malloc(sizeof(point));
                        t->x = e.x;
                        t->y = e.y - 1;
                        t->count = 1;
                    }
                    break;
                case 1:
                    // check south
                    query->y = e.y + 1;
                    query->x = e.x;
                    ncount += (dict_get(elves, query) != NULL);
                    query->x = e.x - 1;
                    ncount += (dict_get(elves, query) != NULL);
                    query->x = e.x + 1;
                    ncount += (dict_get(elves, query) != NULL);
                    // if no neighbor increase count
                    nncount += ncount == 0;
                    // if noone north, move there
                    if (ncount == 0 && t == NULL) {
                        t = malloc(sizeof(point));
                        t->x = e.x;
                        t->y = e.y - 1;
                        t->count = 1;
                    }
                    break;
                case 2:
                    // check west
                    query->x = e.x - 1;
                    query->y = e.y;
                    ncount += (dict_get(elves, query) != NULL);
                    query->y = e.y - 1;
                    ncount += (dict_get(elves, query) != NULL);
                    query->y = e.y + 1;
                    ncount += (dict_get(elves, query) != NULL);
                    // if no neighbor increase count
                    nncount += ncount == 0;
                    // if noone north, move there
                    if (ncount == 0 && t == NULL) {
                        t = malloc(sizeof(point));
                        t->x = e.x;
                        t->y = e.y - 1;
                        t->count = 1;
                    }
                    break;
                case 3:
                    // check west
                    query->x = e.x + 1;
                    query->y = e.y;
                    ncount += (dict_get(elves, query) != NULL);
                    query->y = e.y - 1;
                    ncount += (dict_get(elves, query) != NULL);
                    query->y = e.y + 1;
                    ncount += (dict_get(elves, query) != NULL);
                    // if no neighbor increase count
                    nncount += ncount == 0;
                    // if noone north, move there
                    if (ncount == 0 && t == NULL) {
                        t = malloc(sizeof(point));
                        t->x = e.x;
                        t->y = e.y - 1;
                        t->count = 1;
                    }
                    break;
                }
            }
            // first pass, just announce
            if (passn == 0) {
                // if no neighbors try to move
                // if first just insert, else increase count
                if (nncount == 4) {
                    t = malloc(sizeof(point));
                    t->x = e.x;
                    t->y = e.y;
                    t->count = 1;
                }
                if (t == NULL) {
                    printf("Uninitialized t"); // backtrace to show call tree
                    exit(-1);
                }
                point *prev = dict_get(target_pos, t);
                if (prev == NULL) {
                    dict_insert(target_pos, t);
                } else {
                    prev->count++;
                    free(t);
                }
            } else {
                // second pass actually move
                // if count at target pos is 1 move, otherwise dont
                if (((point*)dict_get(target_pos, t))->count == 1) {
                    dict_insert(nelves, t); // those should never have collision
                } else {
                    dict_insert(nelves, elves->entries[idx]); // those should never have collision
                }
            }
        }
    }

    return nelves;
}
int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day23-sample.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    dict* elves = dict_create(point_hash, point_cmp);

    char buffer[N];
    int x = 0;
    int y = 0;
    while (fgets(buffer, N, file) != NULL)
    {
        char* p = buffer;
        fprintf(stderr, "%s", buffer);
        x = 0;
        while (*p != '\n' && *p != '\0') {
            if (*p == '#') {
                point* p = malloc(sizeof(point));
                p->x = x;
                p->y = y;
                dict_insert(elves, p);
            }
            x++;
            p++;
        }
        y++;
    }

    dict_print(elves, point_print);


    dict* target_pos = dict_create(point_hash, point_cmp);

    for (size_t round=0;round < 10;round++) {
        step(elves, target_pos, round, 0);
        dict* tmp = step(elves, target_pos, round, 1);
        // swap, put new elves in elves and old ones in target pos (which gets cleaned up on first pass)
        target_pos = elves;
        elves = tmp;
    }
    

    return 0;
}