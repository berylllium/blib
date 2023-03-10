#include "container/hashmap.h"

#include <stdlib.h>
#include <string.h>

#define HASH_SEED 525201411107845655ULL
#define HASHMAP_CAPACITY_GROWTH_FACTOR 2

#ifdef _BTESTING
uint64_t collisions = 0;
#endif

static uint64_t hash(const char* str)
{
	uint64_t h = HASH_SEED;

	while (*str++) {
		h ^= *str;
		h *= 0x5bd1e9955bd1e995;
		h ^= h >> 47;
	}
	return h;
}

static blib_hashmap_bucket* search_bucket(blib_hashmap* hashmap, const char* key);

static inline void hashmap_resize(blib_hashmap* hashmap, uint64_t new_capacity);

blib_hashmap _blib_hashmap_create(uint64_t initial_capacity)
{
	blib_hashmap hashmap = {};
	hashmap.base_bucket_capacity = initial_capacity;

	// Allocate buckets.
	hashmap.buckets = calloc(hashmap.base_bucket_capacity, sizeof(blib_hashmap_bucket));

	return hashmap;
}

void blib_hashmap_free(blib_hashmap* hashmap)
{
	// Free buckets.
	for (uint64_t i = 0; i < hashmap->base_bucket_capacity; i++)
	{
		blib_hashmap_bucket* current_bucket = hashmap->buckets[i];
		blib_hashmap_bucket* previous_bucket = NULL;

		while (current_bucket)
		{
			previous_bucket = current_bucket;
			current_bucket = current_bucket->next;

			free(previous_bucket);
		}
	}

	free(hashmap->buckets);
}

const void* blib_hashmap_get(blib_hashmap* hashmap, const char* key)
{
	if (!hashmap || !key) return NULL;

	blib_hashmap_bucket* found_bucket = search_bucket(hashmap, key);

	if (found_bucket)
	{
		return found_bucket->value;
	}

	return NULL;
}

void blib_hashmap_set(blib_hashmap* hashmap, const char* key, const void* value)
{
	if (!value || !key || !hashmap) return;

	// Check if the key already exists. If so, overwrite the old value.
	blib_hashmap_bucket* found_bucket = search_bucket(hashmap, key);

	if (found_bucket)
	{
		found_bucket->value = value;
		return;
	}
	
	// The key does not already exist. Create a new bucket.
	uint64_t h = hash(key);
	uint64_t idx = h % hashmap->base_bucket_capacity;

	blib_hashmap_bucket* new_bucket = calloc(1, sizeof(blib_hashmap_bucket));
	new_bucket->key = key;
	new_bucket->value = value;

	hashmap->total_bucket_count++;

	// Check if there already is a bucket at the calculated index.
	if (hashmap->buckets[idx])
	{
		// There is a bucket at the calculated index. Append new bucket at the head of the linked list.
		new_bucket->next = hashmap->buckets[idx];

		hashmap->buckets[idx] = new_bucket;
		
#ifdef _BTESTING
		collisions++;
#endif
	}
	else
	{
		// There is no bucket at the calculated position. Add bucket to the base buckets.
		hashmap->buckets[idx] = new_bucket;
		hashmap->base_bucket_count++;
	}

	// Check if the capacity has been reached or the bucket count is to high. If so, resize the hashmap.
	if (hashmap->base_bucket_count == hashmap->base_bucket_capacity ||
		hashmap->total_bucket_count > 1.5 * hashmap->base_bucket_capacity
	)
	{
		hashmap_resize(hashmap, hashmap->base_bucket_capacity * HASHMAP_CAPACITY_GROWTH_FACTOR);
	}
}

// Static helper functions

static blib_hashmap_bucket* search_bucket(blib_hashmap* hashmap, const char* key)
{
	// Compute the index
	uint64_t h = hash(key);
	uint64_t idx = h % hashmap->base_bucket_capacity;

	// Get the initial bucket
	blib_hashmap_bucket* bucket = hashmap->buckets[idx];

	if (!bucket)
	{
		return NULL;
	} 

	// Traverse through the links until the corrosponding key has been found.
	while (bucket)
	{
		// Compare keys. If they match, return the bucket.
		if (strcmp(key, bucket->key) == 0)
		{
			return bucket;
		}

		// If the keys did not match, check the next bucket in the linked list. If there are no buckets left in the
		// linked list, bucket->next will be NULL, and the while loop will stop.
		bucket = bucket->next;
	}

	return NULL;
}

static inline void hashmap_resize(blib_hashmap* hashmap, uint64_t new_capacity)
{
#ifdef _BTESTING
	collisions = 0;
#endif

	blib_hashmap new_hashmap = blib_hashmap_create(new_capacity);

	// Traverse through the old hashmap and rehash existing buckets.
	for (uint64_t i = 0; i < hashmap->base_bucket_capacity; i++)
	{
		blib_hashmap_bucket* bucket = hashmap->buckets[i];

		while(bucket)
		{
			blib_hashmap_set(&new_hashmap, bucket->key, bucket->value);

			bucket = bucket->next;
		}
	}

	blib_hashmap_free(hashmap);

	*hashmap = new_hashmap;
}
