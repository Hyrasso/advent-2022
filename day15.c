#include <stdio.h>
#include <stdlib.h>

#define ROWL (1 << 22)

#define ROWY 10
#define W 20

#define ROWY 2000000
#define W 4000000

typedef struct {
    int x;
    int y;
    int d;
} sensor;

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day15.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int sx, sy, bx, by;

    int* row = calloc(ROWL * 2, sizeof(int));

    // can look for x and y independtly?
    // for each i:
    // abs(tx - six) + abs(ty - siy) > abs(bix - six) + abs(biy - siy)
    // can be solved by checking each x,y

    sensor* sensors = calloc(32, sizeof(sensor));
    int n_sensor = 0;

    while (fscanf(file, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sx, &sy, &bx, &by) == 4)
    {
        int d = abs(sx - bx) + abs(sy - by);

        fprintf(stderr, "Sensor %d:%d beacon %d:%d\n", sx, sy, bx, by);
        // sensors and beacons do not count as empty spaces
        sensors[n_sensor++] = (sensor){sx, sy, d};
        if (sy == ROWY) {
            row[ROWL + sx] = 3;
        }
        if (by == ROWY) {
            row[ROWL + bx] = 7;
        }

        d -= abs(sy - ROWY);
        for (int dx=0;dx <= d;dx++) {
            row[ROWL + sx + dx] |= 1;
            row[ROWL + sx - dx] |= 1;
        }
    }

    // for (size_t i=0;i < ROWL * 2;i++) {
    //     if (i == ROWL) {
    //         fprintf(stderr, "|");
    //     } else {
    //         fprintf(stderr, " ");
    //     }
    // }
    // fprintf(stderr, "\n");

    int empty = 0;
    for (size_t i=0;i < ROWL * 2;i++) {
        empty += (row[i] == 1);
        // fprintf(stderr, "%d", row[i]);
    }
    // fprintf(stderr, "\n");

    printf("Empty count %d\n", empty);

    int flag = 0;
    int tx, ty;
    for (tx=0;tx <= W;tx++) {
        // fprintf(stderr, "%d\n", tx);
        for (ty=0;ty <= W;ty++) {

            flag = 0;
            for (size_t i=0;i < n_sensor;i++) {
                int d = abs(tx - sensors[i].x) + abs(ty - sensors[i].y);
                if (d <= sensors[i].d) {
                    // can skip a lot by increasing y directly to the end of the empty space for given tx
                    int dy = sensors[i].d + sensors[i].y - ty - abs(tx - sensors[i].x);
                    // fprintf(stderr, "t (%d:%d) s (%d:%d) dist %d, sdist %d, skipping by %d\n", tx, ty, sensors[i].x, sensors[i].y, d, sensors[i].d, dy);
                    ty += dy;
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                break;
            }
        }
        if (!flag) {
            break;
        }
    }
    
    // 796961409 too low (because of overflow when multiplying int by 4000000)
    printf("Spot %d:%d\n", tx, ty);
    return 1;
}