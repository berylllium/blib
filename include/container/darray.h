#pragma once

#include <stdint.h>

#define BLIB_DARRAY_DEFAULT_CAPACITY 1
#define BLIB_DARRAY_RESIZE_FACTOR 2

typedef struct blib_darray
{
	uint64_t size;
	uint64_t capacity;
	uint64_t stride;

	void* data;
} blib_darray;

blib_darray _blib_darray_create(uint64_t default_capacity, uint64_t stride);

#define blib_darray_create(type) \
	_blib_darray_create(BLIB_DARRAY_DEFAULT_CAPACITY, sizeof(type))

void* blib_darray_get(blib_darray* darray, uint64_t index);

void blib_darray_push_back(blib_darray* darray, void* element);

void blib_darray_resize(blib_darray* darray, uint64_t new_capacity);

void blib_darray_free(blib_darray* darray);
