#pragma once

#include <stdint.h>

#define BHASHMAP_DEFAULT_CAPACITY 4

typedef struct blib_hashmap_bucket
{
	const char* key;
	const void* value;

	struct blib_hashmap_bucket* next;
} blib_hashmap_bucket;

typedef struct blib_hashmap
{
	uint64_t base_bucket_capacity;
	uint64_t base_bucket_count;
	uint64_t total_bucket_count;

	blib_hashmap_bucket** buckets;
} blib_hashmap;

blib_hashmap _blib_hashmap_create(uint64_t initial_capacity);

#define _BHASHMAP_CREATE_EMPTY() _blib_hashmap_create(BHASHMAP_DEFAULT_CAPACITY)
#define _BHASHMAP_CREATE_ONE(x) _blib_hashmap_create(x)
#define _BHASHMAP_CREATE_HELPER(_1, NAME, ...) _BHASHMAP_CREATE_##NAME
#define blib_hashmap_create(...) _BHASHMAP_CREATE_HELPER(__VA_OPT__(__VA__ARGS__,)ONE, EMPTY)(__VA_ARGS__)

void blib_hashmap_free(blib_hashmap* hashmap);

const void* blib_hashmap_get(blib_hashmap* hashmap, const char* key);

void blib_hashmap_set(blib_hashmap* hashmap, const char* key, const void* value);
