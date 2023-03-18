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

blib_darray _blib_darray_create(uint64_t stride, uint64_t default_capacity);

#define _BDARRAY_CREATE_ONE(x) _blib_darray_create(sizeof(x), BLIB_DARRAY_DEFAULT_CAPACITY)
#define _BDARRAY_CREATE_TWO(x, y) _blib_darray_create(sizeof(x), y)
#define _BDARRAY_CREATE_HELPER(_1, _2, NAME, ...) _BDARRAY_CREATE_##NAME
#define blib_darray_create(...) _BDARRAY_CREATE_HELPER(__VA_OPT__(__VA_ARGS__,) TWO, ONE, EMPTY)(__VA_ARGS__)

void* blib_darray_get(blib_darray* darray, uint64_t index);

void blib_darray_push_back(blib_darray* darray, void* element);

void blib_darray_resize(blib_darray* darray, uint64_t new_capacity);

void blib_darray_free(blib_darray* darray);
