#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include <string.h>


typedef struct dkv_t
{
    char* key;
    size_t value;
} dkv_t;


size_t hash_key(const void* pe_key) {
    char* key = ((dkv_t*)pe_key)->key;
    size_t h = 2166136261UL;
    for(; *key; key++)
    {
        h ^= *key;
        h *= 16777619;
    }
    // if (h == 0) {
    //     return 1;
    // }
    return h;
}

int key_cmp(const void* a, const void* b) {
    dkv_t ea = *(dkv_t*)a;
    dkv_t eb = *(dkv_t*)b;
    return strcmp(ea.key, eb.key);
}

void print_elem(void* dkv) {
    dkv_t* elem = dkv;
    fprintf(stderr, "%s: %zu", elem->key, elem->value);
}

int main(int argc, char* argv[]) {
    dict* d = dict_create(&hash_key, &key_cmp);
    dkv_t entry = {.key="A", .value=1};
    assert((dkv_t*)dict_insert(d, &entry) == NULL);
    dkv_t entryb = {.key="B", .value=2};
    assert(((dkv_t*)dict_insert(d, &entryb)) == NULL);
    dkv_t entryc = {.key="C", .value=3};
    assert(((dkv_t*)dict_insert(d, &entryc)) == NULL);
    dkv_t entryd = {.key="D", .value=4};
    assert(((dkv_t*)dict_insert(d, &entryd)) == NULL);
    // reinsert first entry
    dkv_t *res = dict_insert(d, &entry);
    assert(res->key == entry.key);
    dict_print(d, print_elem);

    dkv_t** new_entries = calloc(16, sizeof(dkv_t*));
    for (int i = 0;i < 16;i++) {
        char* s = calloc(2, sizeof(char));
        s[0] = i + 'a';
        new_entries[i] = calloc(1, sizeof(dkv_t));
        new_entries[i]->key = s;
        new_entries[i]->value = i + 16;
        dict_insert(d, new_entries[i]);
    }

    dkv_t query = {0};
    query.key = "A";
    assert(((dkv_t*)dict_get(d, &query))->value == entry.value);
    query.key = "B";
    assert(((dkv_t*)dict_get(d, &query))->value == entryb.value);
    query.key = "C";
    assert(((dkv_t*)dict_get(d, &query))->value == entryc.value);
    query.key = "D";
    assert(((dkv_t*)dict_get(d, &query))->value == entryd.value);
    query.key = "Oo";
    assert(((dkv_t*)dict_get(d, &query)) == NULL);

    dict_print(d, print_elem);
    
    printf("All check passed\n");
    return 0;
}