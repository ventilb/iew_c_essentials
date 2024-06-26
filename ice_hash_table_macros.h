/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef IEW_C_ESSENTIALS_ICE_HASH_TABLE_MACROS_H
#define IEW_C_ESSENTIALS_ICE_HASH_TABLE_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "icemalloc.h"
#include "col_error.h"
#include "icelogging.h"

/**
 * Macros to define typed hash tables with linear probing.
 *
 * The base implementation is based on the project in [3]. The
 * article [2] explains the details.
 *
 * [1]: https://www.geeksforgeeks.org/open-addressing-collision-handling-technique-in-hashing/
 * [2]: https://github.com/benhoyt/ht/blob/master/ht.c
 * [3]: https://benhoyt.com/writings/hash-table-in-c/
 */

#define HASHTABLE_INITIAL_CAPACITY 16
#define HASHTABLE_INVALID_KEY 0

#define makeHashTableApi(name, keyType, valueType) \
typedef uint64_t (*PFN_get_hash64_##name)(keyType key);                                             \
typedef bool (*PFN_key_comparator_##name)(keyType k1, keyType k2);                                  \
typedef col_error_t (*PFN_ht_##name##_each)(keyType key, valueType val, void * pUserData);          \
typedef struct ht_##name##_T * ht_##name;                                                           \
ht_##name ht_##name##_new();                                                                        \
ht_##name ht_##name##_free(ht_##name ht);                                                           \
valueType ht_##name##_get(ht_##name ht, const keyType key);                                         \
valueType ht_##name##_erase(ht_##name ht, keyType key);                                             \
valueType ht_##name##_put(ht_##name ht, const keyType key, valueType value);                        \
col_error_t ht_##name##_each(ht_##name ht, PFN_ht_##name##_each cb, void * pUserData);              \
size_t ht_##name##_len(ht_##name ht);

#define makeHashTableImpl(name, keyType, valueType, nullKey, nullValue, fnHashCode, fnKeyComparator) \
typedef struct hash_table_entry_##name##_T {                                                         \
    uint64_t hash;                                                                                   \
    keyType key;                                                                                     \
    valueType value;                                                                                 \
} hash_table_entry_##name;                                                                           \
                                                                                                     \
struct ht_##name##_T {                                                                               \
    hash_table_entry_##name *entries;                                                                \
    uint64_t capacity;                                                                               \
    uint64_t length;                                                                                 \
};                                                                                                   \
ht_##name ht_##name##_new() {                                                          \
    ht_##name ht = (ht_##name) ice_malloc_ptr_aligned(sizeof(struct ht_##name##_T));                 \
    if (ht == NULL) {                           \
        return NULL;                            \
    } \
    ht->capacity = HASHTABLE_INITIAL_CAPACITY; \
    ht->length = 0; \
    ht->entries = (hash_table_entry_##name *) ice_zmalloc_cache_aligned((ht->capacity * sizeof(struct hash_table_entry_##name##_T))); \
    if (ht->entries == NULL) { \
        ice_aligned_free(ht); \
        return NULL; \
    } \
    return ht; \
} \
ht_##name ht_##name##_free(ht_##name ht) { \
    ice_aligned_free(ht->entries); \
    ice_aligned_free(ht); \
    return NULL; \
} \
valueType ht_##name##_get(ht_##name ht, const keyType key) {                                         \
    uint64_t hash = fnHashCode(key);                                                                 \
    IVK_ASSERT(hash != 0, "hash code must not be 0");                                                \
    uint64_t capacity = ht->capacity;                                                                \
    uint64_t index = (hash & (capacity - 1));                                                        \
    IVK_ASSERT(index < capacity, "index must be less than capacity");                                \
                                                                                                     \
    hash_table_entry_##name * entries = ht->entries;                                                 \
    while (entries[index].hash != HASHTABLE_INVALID_KEY) {                                           \
        if (fnKeyComparator(entries[index].key, key)) {                                              \
            return entries[index].value;                                                             \
        }                                                                                            \
        index = (index + 1) & (capacity - 1);                                                        \
        IVK_ASSERT(index < capacity, "index must be less than capacity");                            \
    }                                                                                                \
    return nullValue;                                                                                \
}                                                                                                    \
static inline valueType ht_##name##_set_entry(                                                       \
    hash_table_entry_##name * entries,                                                               \
    const uint64_t capacity,                                                                         \
    const uint64_t hash,                                                                             \
    keyType key,                                                                                     \
    valueType value,                                                                                 \
    uint64_t * pLength) {                                                                            \
                                                                                                     \
    uint64_t index = (hash & (capacity - 1));                                                        \
    uint64_t startIndex = index;                                                                     \
    while (entries[index].hash != HASHTABLE_INVALID_KEY) {                                           \
        if (fnKeyComparator(entries[index].key, key)) {                                              \
            ltrace0("[ht_set_entry] - found hash entry");                                            \
            valueType oldValue = entries[index].value;                                               \
                                                                                                     \
            entries[index].key = key;                                                                \
            entries[index].value = value;                                                            \
            return oldValue;                                                                         \
        }                                                                                            \
        index = (index + 1) & (capacity - 1);                                                        \
        ltrace("[ht_set_entry] - index=%ld, capacity=%ld", index, capacity);                         \
        IVK_ASSERT(index < capacity, "index must be less than capacity");                            \
        IVK_ASSERT(index != startIndex, "invalid index");                                            \
    }                                                                                                \
    /* Insert of new entry */                                                                        \
    if (pLength != NULL) {                                                                           \
        ltrace("[ht_set_entry] - current length = %ld", (*pLength));                                 \
        /* Having () here is important */                                                            \
        (*pLength) ++;                                                                               \
    }                                                                                                \
                                                                                                     \
    entries[index].hash = hash;                                                                      \
    entries[index].key = key;                                                                        \
    entries[index].value = value;                                                                    \
                                                                                                     \
    return nullValue;                                                                                \
}                                                                                                    \
                                                                                                     \
static inline void ht_##name##_fix_up(ht_##name ht, uint64_t index) {                                \
    uint64_t next = (index + 1) & (ht->capacity -1);                                                 \
    while(ht->entries[next].hash != HASHTABLE_INVALID_KEY) {                                         \
        uint64_t hash = ht->entries[next].hash;                                                      \
        keyType key = ht->entries[next].key;                                                         \
        valueType value = ht->entries[next].value;                                                   \
        ht->entries[next].hash = HASHTABLE_INVALID_KEY;                                              \
        ht->entries[next].key = nullKey;                                                             \
        ht->entries[next].value = nullValue;                                                         \
        ht_##name##_set_entry(ht->entries, ht->capacity, hash, key, value, NULL);                    \
        next = (next + 1) & (ht->capacity -1);                                                       \
        IVK_ASSERT(next < ht->capacity, "next must be less than capacity");                          \
    }                                                                                                \
}                                                                                                    \
                                                                                                     \
valueType ht_##name##_erase(ht_##name ht, keyType key) {                                             \
    const uint64_t capacity = ht->capacity;                                                      \
    const uint64_t hash = fnHashCode(key);                                                       \
    IVK_ASSERT(hash != 0, "hash code must not be 0");                                            \
                                                                                                 \
    uint64_t index = (hash & (capacity - 1));                                                    \
    while (ht->entries[index].hash != HASHTABLE_INVALID_KEY) {                                   \
        if (fnKeyComparator(ht->entries[index].key, key)) {                                      \
            ltrace0("[ht_##name##_erase] - found hash entry");                                   \
            valueType oldValue = ht->entries[index].value;                                       \
                                                                                                 \
            ht->entries[index].hash = HASHTABLE_INVALID_KEY;                                     \
            ht->entries[index].key = nullKey;                                                    \
            ht->entries[index].value = nullValue;                                                \
            ht->length--;                                                                        \
                                                                                                 \
            ht_##name##_fix_up(ht, index);                                                       \
                                                                                                 \
            ltrace0("[ht_##name##_erase] - entry removed");                                      \
            return oldValue;                                                                     \
        }                                                                                        \
        index = (index + 1) & (capacity - 1);                                                    \
        ltrace("[ht_##name##_erase] - index=%ld, capacity=%ld", index, capacity);                \
        IVK_ASSERT(index < capacity, "index must be less than capacity");                        \
    }                                                                                            \
    return nullValue;                                                                            \
}                                                                                                \
                                                                                                 \
static inline col_error_t ht_##name##_expand(ht_##name ht) {                                     \
    const uint64_t cur_capacity = ht->capacity;                                                  \
    const uint64_t new_capacity = cur_capacity * 2;                                              \
    IVK_ASSERT(((new_capacity & (new_capacity - 1)) == 0), "capacity must be power of 2");       \
    if (new_capacity < cur_capacity) {                                                           \
        IVK_ASSERT(0, "overflow");                                                                \
        return COL_ERR_OVERFLOW;                                                                 \
    }                                                                                            \
    hash_table_entry_##name *new_entries =                                                       \
        (hash_table_entry_##name *) ice_zmalloc_cache_aligned(                                   \
            (new_capacity * sizeof(struct hash_table_entry_##name##_T)));                        \
    if (new_entries == NULL) {                                                                   \
        return COL_ERR_BAD_ALLOC;                                                                \
    }                                                                                            \
    for (uint64_t i = 0; i < cur_capacity; i++) {                                                \
        hash_table_entry_##name entry = ht->entries[i];                                          \
        if (entry.hash != HASHTABLE_INVALID_KEY) {                                               \
            ht_##name##_set_entry(new_entries,                                                   \
                                  new_capacity,                                                  \
                                  entry.hash,                                                    \
                                  entry.key,                                                     \
                                  entry.value,                                                   \
                                  NULL);                                                         \
        }                                                                                        \
    }                                                                                            \
    ice_aligned_free(ht->entries);                                                               \
    ht->entries = new_entries;                                                                   \
    ht->capacity = new_capacity;                                                                 \
    return COL_OK;                                                                               \
}                                                                                                \
valueType ht_##name##_put(ht_##name ht, const keyType key, valueType value) {                    \
    if (value == nullValue) {                                                                    \
        ltrace0("[ht_put] - value is NULL, removing key");                                       \
        return ht_##name##_erase(ht, key);                                                       \
    }                                                                                            \
    if (ht->length >= ht->capacity / 2) {                                                        \
        ltrace0("[ht_put] - expand hash table");                                                 \
        if (ht_##name##_expand(ht) != COL_OK) {                                                  \
            return nullValue;                                                                    \
        }                                                                                        \
    }                                                                                            \
    const uint64_t hash = fnHashCode(key);                                                       \
    IVK_ASSERT(hash != 0, "hash code must not be 0");                                            \
    ltrace("[ht_put] - insert key %d and value: hashCode=%ld, length=%ld", key, hash, ht->length);   \
    return ht_##name##_set_entry(ht->entries, ht->capacity, hash, key, value, &ht->length);          \
}                                                                                                    \
col_error_t ht_##name##_each(ht_##name ht, PFN_ht_##name##_each cb, void * pUserData) {              \
    col_error_t err = COL_OK;                                                                        \
    for (uint64_t i = 0; i < ht->capacity; i++) {                                                    \
        hash_table_entry_##name entry = ht->entries[i];                                              \
        if (entry.hash != HASHTABLE_INVALID_KEY) {                                                   \
            err = cb(entry.key, entry.value, pUserData);                                             \
            if (err != COL_OK) {                                                                     \
                return err;                                                                          \
            }                                                                                        \
        }                                                                                            \
    }                                                                                                \
    return COL_OK;                                                                                   \
}                                                                                                    \
size_t ht_##name##_len(ht_##name ht) {                                                               \
    return (size_t) ht->length;                                                                      \
}

#ifdef __cplusplus
}
#endif

#endif //IEW_C_ESSENTIALS_ICE_HASH_TABLE_MACROS_H
