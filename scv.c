/*
 * scv - Simple C Vector
 *
 * scv.c
 *
 * Copyright 2003-2014 Joergen Ibsen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "scv.h"

/**
 * Return a pointer to element number `i` of `v`, without error checking.
 *
 * @param v pointer to `scv_vector`
 * @param i index
 * @return pointer to element `i`
 */
#define SCV_AT(v, i) ((void *) ((char *) (v)->data + (i) * (v)->objsize))

/**
 * Minimum allocation size in bytes.
 */
#define SCV_MIN_ALLOC (64u)

/**
 * Grow the capacity of `v` to at least `capacity`.
 *
 * If more space is needed, grow `v` to `capacity`, but at least by a factor
 * of 1.5.
 *
 * @param v pointer to `scv_vector`
 * @param capacity requested capacity
 * @return zero on success, error code on error
 */
static int scv_i_grow(struct scv_vector *v, size_t capacity)
{
	void *newdata;
	size_t newcapacity;

	assert(v != NULL);
	assert(v->data != NULL);

	if (capacity <= v->capacity) {
		return SCV_OK;
	}

	assert(v->objsize > 0);

	if (capacity >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	/* Growth factor 2 for small vectors, 1.5 for larger */
	if (v->capacity < 4096 / v->objsize) {
		newcapacity = v->capacity + v->capacity + 1;
	}
	else {
		newcapacity = v->capacity + v->capacity / 2 + 1;
	}

	if (capacity > newcapacity || newcapacity >= (size_t) -1 / v->objsize) {
		newcapacity = capacity;
	}

	newdata = realloc(v->data, newcapacity * v->objsize);

	if (newdata == NULL) {
		return SCV_ENOMEM;
	}

	v->data = newdata;
	v->capacity = newcapacity;

	return SCV_OK;
}

struct scv_vector *scv_new(size_t objsize, size_t capacity)
{
	struct scv_vector *v;

	if (capacity == 0) {
		capacity = 1;
	}

	if (objsize == 0 || capacity >= (size_t) -1 / objsize) {
		return NULL;
	}

	v = (struct scv_vector *) malloc(sizeof *v);

	if (v == NULL) {
		return NULL;
	}

	/* Minimum capacity is SCV_MIN_ALLOC bytes or 1 element */
	if (capacity * objsize < SCV_MIN_ALLOC) {
		capacity = (SCV_MIN_ALLOC + (objsize - 1)) / objsize;
	}

	v->data = malloc(capacity * objsize);

	if (v->data == NULL) {
		free(v);
		return NULL;
	}

	v->objsize = objsize;
	v->size = 0;
	v->capacity = capacity;

	return v;
}

void scv_delete(struct scv_vector *v)
{
	if (v == NULL) {
		return;
	}

	if (v->data != NULL) {
		free(v->data);
		v->data = NULL;
	}

	v->objsize = 0;
	v->size = 0;
	v->capacity = 0;

	free(v);
}

void *scv_at(struct scv_vector *v, size_t i)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (i >= v->size) {
		return NULL;
	}

	return SCV_AT(v, i);
}

void *scv_front(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size == 0) {
		return NULL;
	}

	return v->data;
}

void *scv_back(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size == 0) {
		return NULL;
	}

	return SCV_AT(v, v->size - 1);
}

void *scv_data(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size == 0) {
		return NULL;
	}

	return v->data;
}

int scv_empty(const struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	return v->size == 0;
}

size_t scv_size(const struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	return v->size;
}

size_t scv_objsize(const struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	return v->objsize;
}

int scv_reserve(struct scv_vector *v, size_t capacity)
{
	void *newdata;

	assert(v != NULL);
	assert(v->data != NULL);

	if (capacity <= v->capacity) {
		return SCV_OK;
	}

	assert(v->objsize > 0);

	if (capacity >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	newdata = realloc(v->data, capacity * v->objsize);

	if (newdata == NULL) {
		return SCV_ENOMEM;
	}

	v->data = newdata;
	v->capacity = capacity;

	return SCV_OK;
}

size_t scv_capacity(const struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	return v->capacity;
}

int scv_shrink_to_fit(struct scv_vector *v)
{
	void *newdata;
	size_t newcapacity;

	assert(v != NULL);
	assert(v->data != NULL);

	if (v->capacity == v->size) {
		return SCV_OK;
	}

	newcapacity = v->size;

	assert(v->objsize > 0);
	assert(newcapacity < (size_t) -1 / v->objsize);

	/* Minimum capacity is SCV_MIN_ALLOC bytes or 1 element */
	if (newcapacity * v->objsize < SCV_MIN_ALLOC) {
		newcapacity = (SCV_MIN_ALLOC + (v->objsize - 1)) / v->objsize;
	}

	newdata = realloc(v->data, newcapacity * v->objsize);

	if (newdata == NULL) {
		return SCV_ENOMEM;
	}

	v->data = newdata;
	v->capacity = newcapacity;

	return SCV_OK;
}

int scv_clear(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	v->size = 0;

	return SCV_OK;
}

int scv_assign(struct scv_vector *v, const void *data, size_t nobj)
{
	assert(v != NULL);
	assert(v->data != NULL);

	assert(v->objsize > 0);

	if (nobj >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	if (nobj > v->capacity) {
		int res = scv_i_grow(v, nobj);

		if (res != SCV_OK) {
			return res;
		}
	}

	if (data != NULL && nobj > 0) {
		memcpy(v->data, data, nobj * v->objsize);
	}

	v->size = nobj;

	return SCV_OK;
}

int scv_replace(struct scv_vector *v, size_t i, size_t j, const void *data, size_t nobj)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (i > j || j > v->size) {
		return SCV_ERANGE;
	}

	assert(v->objsize > 0);

	if (nobj >= (size_t) -1 / v->objsize - (v->size - (j - i))) {
		return SCV_ERANGE;
	}

	if (v->size - (j - i) + nobj > v->capacity) {
		int res = scv_i_grow(v, v->size - (j - i) + nobj);

		if (res != SCV_OK) {
			return res;
		}
	}

	if (j < v->size && i + nobj != j) {
		memmove(SCV_AT(v, i + nobj), SCV_AT(v, j), (v->size - j) * v->objsize);
	}

	if (data != NULL && nobj > 0) {
		memcpy(SCV_AT(v, i), data, nobj * v->objsize);
	}

	v->size = v->size - (j - i) + nobj;

	return SCV_OK;
}

int scv_insert(struct scv_vector *v, size_t i, const void *data, size_t nobj)
{
	return scv_replace(v, i, i, data, nobj);
}

int scv_erase(struct scv_vector *v, size_t i, size_t j)
{
	return scv_replace(v, i, j, NULL, 0);
}

int scv_push_back(struct scv_vector *v, const void *data)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size + 1 > v->capacity) {
		int res = scv_i_grow(v, v->size + 1);

		if (res != SCV_OK) {
			return res;
		}
	}

	assert(v->objsize > 0);

	if (data != NULL) {
		memcpy(SCV_AT(v, v->size), data, v->objsize);
	}

	v->size += 1;

	return SCV_OK;
}

int scv_pop_back(struct scv_vector *v)
{
	assert(v != NULL);
	assert(v->data != NULL);

	if (v->size == 0) {
		return SCV_ERANGE;
	}

	v->size -= 1;

	return SCV_OK;
}

int scv_resize(struct scv_vector *v, size_t size)
{
	assert(v != NULL);
	assert(v->data != NULL);

	assert(v->objsize > 0);

	if (size >= (size_t) -1 / v->objsize) {
		return SCV_ERANGE;
	}

	if (size > v->capacity) {
		int res = scv_i_grow(v, size);

		if (res != SCV_OK) {
			return res;
		}
	}

	v->size = size;

	return SCV_OK;
}

int scv_copy(struct scv_vector *dst, const struct scv_vector *src)
{
	assert(dst != NULL);
	assert(dst->data != NULL);
	assert(src != NULL);
	assert(src->data != NULL);

	if (dst == src || dst->objsize != src->objsize) {
		return SCV_EINVAL;
	}

	return scv_assign(dst, src->data, src->size);
}

int scv_swap(struct scv_vector *scv1, struct scv_vector *scv2)
{
	struct scv_vector tmp;

	assert(scv1 != NULL);
	assert(scv2 != NULL);

	tmp = *scv1;
	*scv1 = *scv2;
	*scv2 = tmp;

	return SCV_OK;
}
