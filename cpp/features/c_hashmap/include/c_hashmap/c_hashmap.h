
#pragma once


#include <stdio.h>
#include <stdint.h>

typedef struct Entry {
    struct Entry* next;
    uint8_t* key;
    size_t key_size;
    int val;
} Entry;

Entry* create_entry(uint8_t* key, size_t key_size, int val);
void delete_entry(Entry* entry);

typedef struct HashMap {
    Entry** entry_table;
} HashMap;

HashMap* create_hashmap();
void delete_hasmap(HashMap* hashmap);

int hashmap_insert(HashMap* hashmap, uint8_t* key, size_t key_size, int val);
int hashmap_get(HashMap* hashmap, uint8_t* key, size_t key_size, int* value);

