#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define INIT_CAPACITY 16

// #define DEBUG_PRINT

typedef struct 
{
    void** entries;
    size_t capacity;
    size_t (*hash)(const void*);
    int (*key_cmp)(const void*, const void*);
} dict;

void* dict_insert(dict* d, void* key_val);

dict* dict_create(size_t (*hash)(const void*), int (*key_cmp)(const void*, const void*)) {
    if (hash == NULL || key_cmp == NULL) {
        return NULL;
    }
    dict* d = (dict *)malloc(sizeof(dict));
    if (d == NULL)
        return NULL;
    
    d->hash = hash;
    d->key_cmp = key_cmp;

    d->entries = calloc(sizeof(*(d->entries)), INIT_CAPACITY);
    if (d->entries == NULL) {
        free(d);
        return NULL;
    }
    d->capacity = INIT_CAPACITY;

    return d;
}

size_t resize_dict(dict* d, size_t new_size) {
    assert(new_size > d->capacity);
    void** new_entries = calloc(new_size, sizeof(*(d->entries)));
    if (new_entries == NULL)
        return 0;
    void** old_entries = d->entries;
    size_t old_capacity = d->capacity;
    d->entries = new_entries;
    d->capacity = new_size;
    
    for (size_t i=0;i < old_capacity;i++) {
        if (old_entries[i] != NULL) {
            dict_insert(d, old_entries[i]);
        }
    }

    free(old_entries);
    return new_size;
}

void* dict_insert(dict* d, void* key_val) {
    // compute occupation, TODO: extract a function to do this
    size_t occupied = 0;
    for (size_t i=0;i < d->capacity;i++) {
        if (d->entries[i] != NULL) {
            occupied += 1;
        }
    }
    #ifdef DEBUG_PRINT
    fprintf(stderr, "D %zu occupied slots\n", occupied);
    #endif
    if (occupied >= d->capacity * 2/3) {
        #ifdef DEBUG_PRINT
        fprintf(stderr, "D Start resizing to %zu\n", d->capacity << 1);
        #endif
        if (resize_dict(d, d->capacity << 1) == 0)
            perror("Failed to resize entries\n");
        #ifdef DEBUG_PRINT
        fprintf(stderr, "D End resizing to %zu\n", d->capacity);
        #endif
    }

    size_t h = d->hash(key_val);
    size_t idx = h & (d->capacity - 1);
    // todo extract a matching or empty idx function
    while (1)
    {
        // empty slot
        if (d->entries[idx] == NULL) {
            break;
        }
        if (d->key_cmp(d->entries[idx], key_val) == 0) {
            break;
        }
        // probbing
        #ifdef DEBUG_PRINT
        fprintf(stderr, "D collision\n");
        #endif
        idx = (idx + 1) & (d->capacity - 1);
    }
    #ifdef DEBUG_PRINT
    fprintf(stderr, "D inserting at idx: %zu with hash 0x%zx\n", idx, h);
    #endif
    void* old_val = d->entries[idx];
    d->entries[idx] = key_val;
    return old_val;
}

void* dict_get(dict* d, void* key_value) {
    size_t h = d->hash(key_value);
    size_t idx = h & (d->capacity - 1);

    while (1)
    {
        // empty slot
        if (d->entries[idx] == NULL) {
            return NULL;
        }
        if (d->key_cmp(d->entries[idx], key_value) == 0) {
            #ifdef DEBUG_PRINT
            fprintf(stderr, "Found query at idx: %ld\n", idx);
            #endif
            break;
        }
        // prob
        idx = (idx + 1) & (d->capacity - 1);
    }
    return d->entries[idx];
}

void dict_print(dict* d, void (*print_dkv)(void*)) {
    for (size_t i=0;i < d->capacity;i++) {
        void* current = d->entries[i];
        if (current != NULL) {
            fprintf(stderr, "%zu - %p: ", i, current);
            print_dkv(current);
            fprintf(stderr, "\n");
        } else {
            fprintf(stderr, "%zu - null\n", i);
        }
    }
}

#endif