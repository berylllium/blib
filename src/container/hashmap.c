#include "container/hashmap.h"

#include <stdlib.h>
#include <string.h>

#define HASH_SEED 0x12345678
#define HASHMAP_CAPACITY_GROWTH_FACTOR 2

static uint32_t hash(const char* str)
{
	uint32_t h = HASH_SEED;

	while (*str++) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

static lise_hashmap_bucket* search_bucket(lise_hashmap* hashmap, const char* key);

static inline void hashmap_resize(lise_hashmap* hashmap, uint64_t new_capacity);

lise_hashmap _lise_hashmap_create(uint64_t initial_capacity)
{
	lise_hashmap hashmap = {};
	hashmap.base_bucket_capacity = initial_capacity;

	// Allocate buckets.
	hashmap.buckets = calloc(hashmap.base_bucket_capacity, sizeof(lise_hashmap_bucket));

	return hashmap;
}

void lise_hashmap_free(lise_hashmap* hashmap)
{
	// Free buckets.
	for (uint64_t i = 0; i < hashmap->base_bucket_capacity; i++)
	{
		lise_hashmap_bucket* current_bucket = hashmap->buckets[i];
		lise_hashmap_bucket* previous_bucket = NULL;

		while (current_bucket)
		{
			previous_bucket = current_bucket;
			current_bucket = current_bucket->next;

			free(previous_bucket);
		}
	}
}

const void* lise_hashmap_get(lise_hashmap* hashmap, const char* key)
{
	if (!hashmap || !key) return NULL;

	lise_hashmap_bucket* found_bucket = search_bucket(hashmap, key);

	if (found_bucket)
	{
		return found_bucket->value;
	}

	return NULL;
}

void lise_hashmap_set(lise_hashmap* hashmap, const char* key, const void* value)
{
	if (!value || !key || !hashmap) return;

	// Check if the key already exists. If so, overwrite the old value.
	lise_hashmap_bucket* found_bucket = search_bucket(hashmap, key);

	if (found_bucket)
	{
		found_bucket->value = value;
		return;
	}
	
	// The key does not already exist. Create a new bucket.
	uint32_t h = hash(key);
	uint64_t idx = h % hashmap->base_bucket_capacity;

	lise_hashmap_bucket* new_bucket = calloc(1, sizeof(lise_hashmap_bucket));
	new_bucket->key = key;
	new_bucket->value = value;

	// Check if there already is a bucket at the calculated index.
	if (hashmap->buckets[idx])
	{
		// There is a bucket at the calculated index. Append new bucket at the head of the linked list.
		new_bucket->next = hashmap->buckets[idx];

		hashmap->buckets[idx] = new_bucket;
	}
	else
	{
		// There is no bucket at the calculated position. Add bucket to the base buckets.
		hashmap->buckets[idx] = new_bucket;
		hashmap->base_bucket_count++;

		// Check if the capacity has been reached. If so, resize the hashmap.
		if (hashmap->base_bucket_count == hashmap->base_bucket_capacity)
		{
			hashmap_resize(hashmap, hashmap->base_bucket_capacity * HASHMAP_CAPACITY_GROWTH_FACTOR);
		}
	}
}

// Static helper functions

static lise_hashmap_bucket* search_bucket(lise_hashmap* hashmap, const char* key)
{
	// Compute the index
	uint32_t h = hash(key);
	uint64_t idx = h % hashmap->base_bucket_capacity;

	// Get the initial bucket
	lise_hashmap_bucket* bucket = hashmap->buckets[idx];

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

static inline void hashmap_resize(lise_hashmap* hashmap, uint64_t new_capacity)
{
	lise_hashmap new_hashmap = {};

	new_hashmap.base_bucket_capacity = new_capacity;
	new_hashmap.base_bucket_count = hashmap->base_bucket_count;
	new_hashmap.buckets = calloc(new_hashmap.base_bucket_capacity, sizeof(lise_hashmap_bucket));

	// Traverse through the old hashmap and rehash existing buckets.
	for (uint64_t i = 0; i < hashmap->base_bucket_capacity; i++)
	{
		lise_hashmap_bucket* bucket = hashmap->buckets[i];

		while(bucket)
		{
			lise_hashmap_set(&new_hashmap, bucket->key, bucket->value);

			bucket = bucket->next;
		}
	}

	lise_hashmap_free(hashmap);

	*hashmap = new_hashmap;
}
