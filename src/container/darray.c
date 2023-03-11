#include "container/darray.h"

#include <stdlib.h>
#include <string.h>

blib_darray _blib_darray_create(uint64_t default_capacity, uint64_t stride)
{
	blib_darray darray = {};

	darray.capacity = default_capacity;
	darray.stride = stride;

	darray.data = calloc(1, darray.capacity * darray.stride);

	return darray;
}

void* blib_darray_get(blib_darray* darray, uint64_t index)
{
	return darray->data + darray->stride * index;
}

void blib_darray_push_back(blib_darray* darray, void* element)
{
	if (darray->size + 1 > darray->capacity)
	{
		blib_darray_resize(darray, darray->capacity * BLIB_DARRAY_RESIZE_FACTOR);
	}

	memcpy(darray->data + darray->size * darray->stride, element, darray->stride);

	darray->size++;
}

void blib_darray_resize(blib_darray* darray, uint64_t new_capacity)
{
	darray->capacity = new_capacity;

	void* temp = realloc(darray->data, darray->stride * new_capacity);

	darray->data = temp;
}

void blib_darray_free(blib_darray* darray)
{
	free(darray->data);

	darray->data = NULL;
	darray->size = darray->capacity = darray->stride = 0;
}
