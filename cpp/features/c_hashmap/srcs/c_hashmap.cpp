#include "c_hashmap/c_hashmap.h"
#include <stdlib.h>
#include <string.h>

#define HASHMAP_NUM_SLOTS 128

static uint32_t compute_hash(uint8_t* key, size_t key_size) {
    uint32_t hash = 0;
    for (size_t i = 0; i < key_size; i++) {
         hash = (hash << 8) + key[i];
    }

    return hash % HASHMAP_NUM_SLOTS;
}

static int compare_keys(uint8_t* k1, size_t k1_size, uint8_t* k2, size_t k2_size) {
    if (k1_size != k2_size) {
        return 0;
    }

    return (memcmp(k1, k2, k1_size) == 0);
}

Entry* create_entry(uint8_t* key, size_t key_size, int val) {
    Entry* entry = (Entry*)malloc(sizeof(*entry));

    entry->key = (uint8_t*)malloc(key_size * sizeof(uint8_t));
    memcpy(entry->key, key, key_size);
    entry->key_size = key_size;

    entry->val = val;
    entry->next = NULL;

    return entry;
}

void delete_entry(Entry* entry) {
    if (!entry) {
        return;
    }

    if (entry->key) {
        free(entry->key);
    }
    free(entry);
}

HashMap* create_hashmap() {
    HashMap* hashmap = (HashMap*)malloc(sizeof(*hashmap));
    hashmap->entry_table = (Entry**)calloc(HASHMAP_NUM_SLOTS, sizeof(Entry*));
    return hashmap;
}

void delete_hashmap(HashMap* hashmap) {
    if (!hashmap) {
        return;
    }

    if (!hashmap->entry_table) {
        goto free_hashmap;
    }

    for (uint32_t i = 0; i < HASHMAP_NUM_SLOTS; i++) {
        Entry* curr_entry = hashmap->entry_table[i];
        while (curr_entry) {
            Entry* entry_to_free = curr_entry;
            curr_entry = curr_entry->next;
            free(entry_to_free);
        }
    }
    free(hashmap->entry_table);
free_hashmap:
    free(hashmap);
}


int hashmap_insert(HashMap* hashmap, uint8_t* key, size_t key_size, int val) {
    if (!hashmap || !key || key_size == 0) {
        return -1;
    }

    int slot = compute_hash(key, key_size);

    if (!(hashmap->entry_table[slot])) {
        Entry* new_entry = create_entry(key, key_size, val);
        hashmap->entry_table[slot] = new_entry;
        return 0;
    }

    Entry* curr_entry = hashmap->entry_table[slot];
    if (compare_keys(key, key_size, curr_entry->key, curr_entry->key_size)) {
        curr_entry->val = val;
        return 0;
    }

    while (curr_entry->next) {
        if (compare_keys(key, key_size, curr_entry->key, curr_entry->key_size)) {
            curr_entry->val = val;
            return 0;
        }
        curr_entry = curr_entry->next;
    }

    Entry* new_entry = create_entry(key, key_size, val);
    curr_entry->next = new_entry;
    return 0;
}

int hashmap_get(HashMap* hashmap, uint8_t* key, size_t key_size, int* value) {
    if (!hashmap || !key || key_size == 0) {
        return -1;
    }

    int slot = compute_hash(key, key_size);

    Entry* curr_entry = hashmap->entry_table[slot];
    while (curr_entry) {
        if (compare_keys(key, key_size, curr_entry->key, curr_entry->key_size)) {
            *value = curr_entry->val;
            return 0;
        }
        curr_entry = curr_entry->next;
    }

    return -1;
}
