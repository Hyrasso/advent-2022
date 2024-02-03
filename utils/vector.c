#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "vector.h"

vector *vec_create(size_t initial_capacity) {
    vector* vec = malloc(sizeof(vector));
    if (vec == NULL)
        return NULL;

    return vec_init(vec, initial_capacity);
}

vector* vec_init(vector* vec, size_t initial_capacity) {
    vec->array = (int*)calloc(initial_capacity, sizeof(int));
    if (vec->array == NULL) {
        return NULL;
    }
    vec->capacity = initial_capacity;
    return vec;
}

int vec_resize(vector* vec, size_t new_capacity) {
    fprintf(stderr, "Resizing from %zu to %zu\n", vec->capacity, new_capacity);
    int* res = realloc(vec->array, sizeof(*(vec->array)) * new_capacity);
    if (res == NULL)
        return -1;
    vec->array = res;
    vec->capacity = new_capacity;
    // shrink array, dumped end elements
    if (new_capacity < vec->size)
        vec->size = new_capacity;
    return new_capacity;
}

void vec_append(vector* vec, int value) {
    if (vec->size == vec->capacity)
        assert(vec_resize(vec, vec->capacity * 2) >= 0);

    vec->array[vec->size] = value;
    vec->size++;
}

void vec_free(vector vec) {
    free(vec.array);
}

void vec_debug(vector* vec) {
    fprintf(stderr, "Vec with capacity: %zu size: %zu\n", vec->capacity, vec->size);
    for (int i=0;i < vec->size;i++)
        fprintf(stderr, "%d\t", vec->array[i]);
    fprintf(stderr, "\n");
}

// TODO: better sort
void vec_bubble_sort(vector* vec) {
    int tmp;
    for (int i=0;i < vec->size - 1;i++) {
        for (int j=i;j < vec->size - 1;j++) {
            if (vec->array[j] > vec->array[j+1]) {
                tmp = vec->array[j+1];
                vec->array[j+1] = vec->array[j];
                vec->array[j] = tmp;
            }
        }
    }
}