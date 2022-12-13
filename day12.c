#include <stdio.h>
#include <stdlib.h>

#define N 256

// #define DEBUG_PRINT

typedef struct path
{
    int length;
    int capacity;
    int* pos;
    struct path* next;
} path;

path* path_create(size_t capacity) {
    path* p = malloc(sizeof(path));
    if (p == NULL) {
        return NULL;
    }

    p->pos = calloc(capacity, sizeof(*(p->pos)));
    if (p == NULL) {
        free(p);
        return NULL;
    }
    p->capacity = capacity;
    p->length = 0;
    p->next = NULL;

    return p;
}

void path_extend(path* p, int next_pos) {
    // check for loop in the path
    for (size_t i=0;i < p->length;i++) {
        if (p->pos[i] == next_pos) {
            return;
        }
    }
    // create a copy of the current path
    size_t new_capacity = (p->capacity > p->length + 1) ? p->capacity : p->capacity + 8;
    path* new_path = path_create(new_capacity);
    for (size_t i=0;i < p->length;i++) {
        new_path->pos[i] = p->pos[i];
    }
    new_path->pos[p->length] = next_pos;
    new_path->length = p->length + 1;

    path* current_p = p;
    while (current_p->next != NULL && current_p->next->length <= new_path->length)
    {
        current_p = current_p->next;
    }
    new_path->next = current_p->next;
    current_p->next = new_path;
}

void path_free(path* p) {
    free(p->pos);
    free(p);
}

path* find_shortest(int start, int end, char** map, int map_width, int map_height) {
    // intialise paths
    path* paths;
    paths = path_create(8);
    paths->pos[0] = start;
    paths->length = 1;

    int* visited = calloc(map_width * map_height, sizeof(int));
    if (visited == NULL) {
        exit(-1);
    }

    while (1) {
        int current_pos = paths->pos[paths->length - 1]; 
        if (current_pos == end) {
            break;
        }

        // check possible next steps for current shortest
        int x = current_pos % map_width;
        int y = current_pos / map_width;
        if ((x + 1 < map_width) && (map[y][x] + 1 >= map[y][x + 1])) {

            if (!visited[y * map_width + x + 1])
                path_extend(paths, y * map_width + x + 1);
            visited[y * map_width + x + 1] = 1;
        }
        if ((y + 1 < map_height) && (map[y][x] + 1 >= map[y + 1][x])) {
            if (!visited[(y + 1) * map_width + x])
                path_extend(paths, (y + 1) * map_width + x);
            visited[(y + 1) * map_width + x] = 1;

        }
        if ((x - 1 >= 0) && (map[y][x] + 1 >= map[y][x - 1])) {
            if (!visited[y * map_width + x - 1])
                path_extend(paths, y * map_width + x - 1);
            visited[y * map_width + x - 1] = 1;
        }
        if ((y - 1 >= 0) && (map[y][x] + 1 >= map[y - 1][x])) {
            if (!visited[(y - 1) * map_width + x])
                path_extend(paths, (y - 1) * map_width + x);
            visited[(y - 1) * map_width + x] = 1;
        }
        path* to_del = paths;
        paths = paths->next;
        path_free(to_del);
        // could not reach end
        if (paths == NULL) {
            return paths;
        }
        // paths count
        #ifdef DEBUG_PRINT
        size_t p_count = 1;
        path* p = paths;
        for(path* p=paths;p!=NULL;p=p->next,p_count++);
        fprintf(stderr, "Considering %zu paths\n", p_count);
        // print best path
        fprintf(stderr, "Min length %d ", paths->length);
        for (size_t i=0;i<paths->length;i++) {
            int x = paths->pos[i] % map_width;
            int y = paths->pos[i] / map_width;
            fprintf(stderr, "%c:%d,%d ", map[y][x], x, y);
        }
        fprintf(stderr, "\n");
        #endif
    }
    free(visited);
    return paths;
}

path* find_shortest_2(int start, char** map, int map_width, int map_height) {
    // intialise paths
    path* paths;
    paths = path_create(8);
    paths->pos[0] = start;
    paths->length = 1;

    int* visited = calloc(map_width * map_height, sizeof(int));
    if (visited == NULL) {
        exit(-1);
    }

    while (1) {
        int current_pos = paths->pos[paths->length - 1]; 
        if (map[current_pos / map_width][current_pos % map_width] == 'a') {
            break;
        }

        // check possible next steps for current shortest
        int x = current_pos % map_width;
        int y = current_pos / map_width;
        if ((x + 1 < map_width) && (map[y][x] - 1 <= map[y][x + 1])) {
            if (!visited[y * map_width + x + 1])
                path_extend(paths, y * map_width + x + 1);
            visited[y * map_width + x + 1] = 1;
        }
        if ((y + 1 < map_height) && (map[y][x] - 1 <= map[y + 1][x])) {
            if (!visited[(y + 1) * map_width + x])
                path_extend(paths, (y + 1) * map_width + x);
            visited[(y + 1) * map_width + x] = 1;
        }
        if ((x - 1 >= 0) && (map[y][x] - 1 <= map[y][x - 1])) {
            if (!visited[y * map_width + x - 1])
                path_extend(paths, y * map_width + x - 1);
            visited[y * map_width + x - 1] = 1;
        }
        if ((y - 1 >= 0) && (map[y][x] - 1 <= map[y - 1][x])) {
            if (!visited[(y - 1) * map_width + x])
                path_extend(paths, (y - 1) * map_width + x);
            visited[(y - 1) * map_width + x] = 1;
        }
        path* to_del = paths;
        paths = paths->next;
        path_free(to_del);
        // could not reach end
        if (paths == NULL) {
            return paths;
        }
        #ifdef DEBUG_PRINT
        // paths count
        size_t p_count = 1;
        path* p = paths;
        for(path* p=paths;p!=NULL;p=p->next,p_count++);
        fprintf(stderr, "Considering %zu paths\n", p_count);
        // print best path
        fprintf(stderr, "Min length %d ", paths->length);
        for (size_t i=0;i<paths->length;i++) {
            int x = paths->pos[i] % map_width;
            int y = paths->pos[i] / map_width;
            fprintf(stderr, "%c:%d,%d ", map[y][x], x, y);
        }
        fprintf(stderr, "\n");
        #endif
    }
    free(visited);
    return paths;
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("input/day12.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char buffer[N];
    char** map = malloc(sizeof(char*));
    int w = 0;
    int h = 1;

    int start = -1;
    int end = -1;

    while (fgets(buffer, N, file) != NULL)
    {
        if (w == 0) {
            for (w=0;buffer[w] != '\n';w++);
        }
        map = realloc(map, h * (sizeof(char*)));
        map[h-1] = calloc(w, sizeof(char));
        for (size_t i=0;i < w;i++) {
            map[h-1][i] = buffer[i];
            if (buffer[i] == 'S') {
                start = (h-1) * w + i;
                map[h-1][i] = 'a';
            }
            if (buffer[i] == 'E') {
                end = (h-1) * w + i;
                map[h-1][i] = 'z';
            }
        }
        h++;
    }
    h--;
    printf("%d %d\n", start, end);
    for (size_t j=0;j<h;j++) {
        for (size_t i=0;i<w;i++) {
            putc(map[j][i], stderr);
        }
        putc('\n', stderr);
    }
    
    // find shortest path
    path* shortest = find_shortest(start, end, map, w, h);
    printf("Shortest: %d\n", shortest->length - 1);
    while (shortest != NULL)
    {
        path* to_del = shortest;
        shortest = shortest->next;
        path_free(to_del);
    }
    

    // find best shortest path
    path* best_shortest = find_shortest_2(end, map, w, h);
    printf("Best: %d\n", best_shortest->length - 1);
    while (best_shortest != NULL)
    {
        path* to_del = best_shortest;
        best_shortest = best_shortest->next;
        path_free(to_del);
    }

    for (size_t i=0;i<h;i++) {
        free(map[i]);
    }
    free(map);

    free(file);
    return 0;
}