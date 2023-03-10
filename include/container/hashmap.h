#pragma once

#include <stdint.h>

#define LHASHMAP_DEFAULT_CAPACITY 4

typedef struct lise_hashmap_bucket
{
	const char* key;
	const void* value;

	struct lise_hashmap_bucket* next;
} lise_hashmap_bucket;

typedef struct lise_hashmap
{
	uint64_t base_bucket_capacity;
	uint64_t base_bucket_count;

	lise_hashmap_bucket** buckets;
} lise_hashmap;

lise_hashmap _lise_hashmap_create(uint64_t initial_capacity);

#define _LHASHMAP_CREATE_EMPTY() _lise_hashmap_create(LHASHMAP_DEFAULT_CAPACITY)
#define _LHASHMAP_CREATE_ONE(x) _lise_hansmap_create(x)
#define _LHASHMAP_CREATE_HELPER(_1, NAME, ...) _LHASHMAP_CREATE_##NAME
#define lise_hashmap_create(...) _LHASHMAP_CREATE_HELPER(__VA_OPT__(__VA__ARGS__,) ONE, EMPTY)(__VA_ARGS__)

void lise_hashmap_free(lise_hashmap* hashmap);

const void* lise_hashmap_get(lise_hashmap* hashmap, const char* key);

void lise_hashmap_set(lise_hashmap* hashmap, const char* key, const void* value);
