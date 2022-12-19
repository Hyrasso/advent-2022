#include <stdlib.h>
#include <stdio.h>

#define N 12000
#define PILE_CAPACITY (1 << 14)

typedef struct 
{
    char occupied[4];
    int height;
} rock;


void rock_init(rock* r, size_t i) {
    
    for (size_t k=0;k<4;k++) {
        r->occupied[k] = 0;
    }

    switch (i % 5)
    {
    case 0:
        r->height = 1;
        r->occupied[0] = 30; // 0b11110;
        break;
    case 1:
        r->height = 3;
        r->occupied[2] = 8;  // 0b01000;
        r->occupied[1] = 28; // 0b11100;
        r->occupied[0] = 8;  // 0b01000;
        break;
    case 2:
        r->height = 3;
        r->occupied[2] = 4;  // 0b00100;
        r->occupied[1] = 4;  // 0b00100;
        r->occupied[0] = 28; // 0b11100;
        break;
    case 3:
        r->height = 4;
        r->occupied[3] = 16; // 0b10000;
        r->occupied[2] = 16; // 0b10000;
        r->occupied[1] = 16; // 0b10000;
        r->occupied[0] = 16; // 0b10000;
        break;
    case 4:
        r->height = 2;
        r->occupied[1] = 24; // 0b11000;
        r->occupied[0] = 24; // 0b11000;
        break;
    
    }
}

void dprint_pile(char* p, size_t l) {
    for (int i=l;i >= 0;i--) {
        if (p[i] == 0) {
            continue;
        }
        for (int k=6;k>= 0;k--) {
            fprintf(stderr, (p[i] & (1 << k)) ? "#" : " ");
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "=======\n");
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day17-sample.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char jets[N];

    fgets(jets, N, file);
    size_t jets_len;
    for (jets_len=0;jets[jets_len];jets_len++);
    size_t jet_idx = 0;
    fprintf(stderr, "%ld %s\n", jets_len, jets);

    char* pile = calloc(PILE_CAPACITY, sizeof(char));
    if (pile == NULL) {
        return -1;
    }
    size_t pile_capacity = PILE_CAPACITY;
    rock r = {0};


    for (size_t count = 0;count < 2022;count++) {
        rock_init(&r, count);
        // fprintf(stderr, "R %d.%d.%d.%d\n", r.occupied[0], r.occupied[1], r.occupied[2], r.occupied[3]);

        size_t pile_top = pile_capacity;
        // getting current pile top
        for (pile_top = pile_capacity;pile[pile_top] == 0 && pile_top > 0;pile_top--);

        // dprint_pile(pile, pile_capacity);
        // fprintf(stderr, "Current top %ld\n", pile_top);

        // pile = realloc(pile, sizeof(char) * (pile_top + 8));
        // if (pile == NULL) {
        //     return -1;
        // }
        // pile_capacity = pile_top + 8;
        // fprintf(stderr, "pile capacity %ld\n", pile_capacity);

        size_t ridx = pile_top + ((pile[pile_top]) ? 4 : 3);
        // fprintf(stderr, "Rock start falling at %ld\n", ridx);

        while (1)
        {
            if (jets[jet_idx] == '>') {
                // can shift right
                int can_shift = 1;
                for (size_t j=0;j<r.height;j++) {
                    if (r.occupied[j] & 1) {
                        can_shift = 0;
                    }
                    if (pile[ridx + j] & (r.occupied[j] >> 1)) {
                        can_shift = 0;
                    }
                }
                // push right
                // fprintf(stderr, "Rshift %d\n", can_shift);
                if (can_shift) {
                    for (size_t j=0;j<r.height;j++) {
                        r.occupied[j] >>= 1;
                    }
                }
            }
            if (jets[jet_idx] == '<') {
                // can shift left
                int can_shift = 1;
                for (size_t j=0;j<r.height;j++) {
                    if (r.occupied[j] & (1 << 6)) {
                        can_shift = 0;
                    }
                    if (pile[ridx + j] & (r.occupied[j] << 1)) {
                        can_shift = 0;
                    }
                }
                // push left
                // fprintf(stderr, "Lshift %d\n", can_shift);
                if (can_shift) {
                    for (size_t j=0;j<r.height;j++) {
                        r.occupied[j] <<= 1;
                    }
                }
            }
            jet_idx = (jet_idx + 1) % jets_len;

            // fall down
            int can_fall = ridx > 0;
            for (size_t j=0;j<r.height;j++) {
                // one step down blocked
                if (pile[ridx - 1 + j] & r.occupied[j]) {
                    can_fall = 0;
                }
            }

            if (!can_fall) {
                for (size_t j=0;j<r.height;j++) {
                    // add piece
                    pile[ridx + j] |= r.occupied[j];
                }
                break;
            }
            ridx--;
        }
    }
    // dprint_pile(pile, pile_capacity);
    size_t pile_top = pile_capacity;
    // getting current pile top
    for (pile_top = pile_capacity;pile[pile_top] == 0 && pile_top > 0;pile_top--);

    printf("Pile top: %zu\n", pile_top + 1);
    // 4850 too high

    return 0;
}