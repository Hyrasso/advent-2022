#include <stdio.h>
#include <stdlib.h>

#define N 3000

typedef struct {
    int x;
    int y;
    int z;
} point;


int cube_surface(point* points, size_t npoint, size_t idx) {
    int surface = 6;
    for (size_t j=0;j<npoint;j++) {
        if ((points[idx].x + 1 == points[j].x) && (points[idx].y == points[j].y) && (points[idx].z == points[j].z)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
        if ((points[idx].x - 1 == points[j].x) && (points[idx].y == points[j].y) && (points[idx].z == points[j].z)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
        if ((points[idx].y + 1 == points[j].y) && (points[idx].x == points[j].x) && (points[idx].z == points[j].z)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
        if ((points[idx].y - 1 == points[j].y) && (points[idx].x == points[j].x) && (points[idx].z == points[j].z)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
        if ((points[idx].z + 1 == points[j].z) && (points[idx].y == points[j].y) && (points[idx].x == points[j].x)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
        if ((points[idx].z - 1 == points[j].z) && (points[idx].y == points[j].y) && (points[idx].x == points[j].x)) {
            surface--;
            fprintf(stderr, "%d,%d,%d is neigbor\n", points[j].x, points[j].y, points[j].z);
        }
    }

    return surface;
}

int coolable_surface(point point, char cooling_cubes[25][25][25]) {
    int surface = 0;

    surface += cooling_cubes[point.x + 1 + 1][point.y + 1][point.z + 1] & 1;
    surface += cooling_cubes[point.x + 1][point.y + 1 + 1][point.z + 1] & 1;
    surface += cooling_cubes[point.x + 1][point.y + 1][point.z + 1 + 1] & 1;

    surface += cooling_cubes[point.x + 1][point.y + 1][point.z + 1 - 1] & 1;
    surface += cooling_cubes[point.x + 1][point.y + 1 - 1][point.z + 1] & 1;
    surface += cooling_cubes[point.x + 1 - 1][point.y + 1][point.z + 1] & 1;

    return surface;
}

void dfs(point* points, size_t npoint, char visited[25][25][25]) {

    point* to_visit = calloc(15360, sizeof(point));
    to_visit[0] = (point){-1, -1, -1};
    size_t to_visit_len = 1;
    size_t to_visit_capacity = 15360;

    // set occupied cubes to 2
    for (size_t j=0;j<npoint;j++) {
        visited[points[j].x + 1][points[j].y + 1][points[j].z + 1] = 2;
    }

    while (to_visit_len > 0)
    {
        if (to_visit_capacity <= to_visit_len + 6) {
            to_visit = realloc(to_visit, sizeof(point) * to_visit_capacity << 1);
            if (to_visit == NULL) {
                exit(-1);
            }
            to_visit_capacity <<= 1;
            fprintf(stderr, "Resizing visit stack to %ld\n", to_visit_capacity);
            fprintf(stderr, "Available memory %p %p\n", to_visit, to_visit + to_visit_capacity);
        }
        point current = to_visit[--to_visit_len];
        if (to_visit_len % 10 == 0) {
            fprintf(stderr, "Current stack size %ld\n", to_visit_len);
        }
        // fprintf(stderr, "New cube to visit %d:%d:%d\n", current.x, current.y, current.z);
        if (visited[1 + current.x][1 + current.y][1 + current.z] == 2) {
            exit(-2);
        }
        visited[1 + current.x][1 + current.y][1 + current.z] = 1;

        if ((current.x + 1 < 24 ) && (0 == visited[1 + current.x + 1][1 + current.y][1 + current.z])) {
            to_visit[to_visit_len++] = (point){current.x + 1, current.y, current.z};
        }
        if ((current.y + 1 < 24 ) && (0 == visited[1 + current.x][1 + current.y + 1][1 + current.z])) {
            to_visit[to_visit_len++] = (point){current.x, current.y + 1, current.z};
        }
        if ((current.z + 1 < 24 ) && (0 == visited[1 + current.x][1 + current.y][1 + current.z + 1])) {
            to_visit[to_visit_len++] = (point){current.x, current.y, current.z + 1};
        }
        if ((current.x - 1 >= -1) && (0 == visited[1 + current.x - 1][1 + current.y][1 + current.z])) {
            to_visit[to_visit_len++] = (point){current.x - 1, current.y, current.z};
        }
        if ((current.y - 1 >= -1) && (0 == visited[1 + current.x][1 + current.y - 1][1 + current.z])) {
            to_visit[to_visit_len++] = (point){current.x, current.y - 1, current.z};
        }
        if ((current.z - 1 >= -1) && (0 == visited[1 + current.x][1 + current.y][1 + current.z - 1])) {
            to_visit[to_visit_len++] = (point){current.x, current.y, current.z - 1};
        }

    }

    return visited;
}


int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day18.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    point points[N] = {0};
    size_t npoint = 0;

    while (fscanf(file, "%d,%d,%d\n", &(points[npoint].x), &(points[npoint].y), &(points[npoint].z)) == 3)
    {
        npoint++;
    }

    // find all outside empty cube
    char cooling_points[25][25][25] = {0};
    dfs(points, npoint, cooling_points);

    // iterate on lava an check if neighbor is in air
    int surface = 0;
    int total_coolable_surface = 0;
    for (size_t i=0;i<npoint;i++) {
        fprintf(stderr, "%d,%d,%d\n", points[i].x, points[i].y, points[i].z);
        int s = cube_surface(points, npoint, i);
        fprintf(stderr, "Cube surface: %d\n", s);
        int cs = coolable_surface(points[i], cooling_points);
        fprintf(stderr, "Coolable surface: %d\n", cs);

        surface += s;
        total_coolable_surface += cs;
    }

    printf("Surface: %d\n", surface);

    printf("Total coolable surface: %d\n", total_coolable_surface);

    return 0;
}