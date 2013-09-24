/*
 * scv - Simple C Vector
 *
 * scv.c
 *
 * Copyright 2003-2013 Joergen Ibsen
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

#define SCV_AT(p, i) ((void *) ((char *) (p)->data + (i) * (p)->objsize))

static int scv_i_grow(struct scv_vector *p, size_t capacity)
{
	void *newdata;
	size_t newcapacity;

	assert(p != NULL);
	assert(p->data != NULL);
	assert(capacity > 0);

	if (capacity <= p->capacity) {
		return 1;
	}

	/* growth factor 1.5 */
	newcapacity = p->capacity + p->capacity / 2 + 1;

	if (capacity > newcapacity) {
		newcapacity = capacity;
	}

	newdata = realloc(p->data, newcapacity * p->objsize);

	if (newdata == NULL) {
		return 0;
	}

	p->data = newdata;
	p->capacity = newcapacity;

	return 1;
}

int scv_init(struct scv_vector *p, size_t objsize, size_t capacity)
{
	assert(p != NULL);
	assert(objsize > 0);

	/* minimum capacity is 64 bytes or 1 element */
	if (capacity * objsize < 64) {
		capacity = (64 + (objsize - 1)) / objsize;
	}

	p->data = malloc(capacity * objsize);

	if (p->data == NULL) {
		return 0;
	}

	p->objsize = objsize;
	p->size = 0;
	p->capacity = capacity;

	return 1;
}

void scv_free(struct scv_vector *p)
{
	assert(p != NULL);

	if (p->data != NULL) {
		free(p->data);
		p->data = NULL;
	}

	p->objsize = 0;
	p->size = 0;
	p->capacity = 0;
}

size_t scv_size(struct scv_vector *p)
{
	assert(p != NULL);

	return p->size;
}

int scv_empty(struct scv_vector *p)
{
	assert(p != NULL);

	return p->size == 0;
}

size_t scv_capacity(struct scv_vector *p)
{
	assert(p != NULL);

	return p->capacity;
}

int scv_shrink_to_fit(struct scv_vector *p)
{
	void *newdata;
	size_t newcapacity;

	assert(p != NULL);
	assert(p->data != NULL);

	if (p->capacity == p->size) {
		return 1;
	}

	newcapacity = p->size;

	/* minimum capacity is 64 bytes or 1 element */
	if (newcapacity * p->objsize < 64) {
		newcapacity = (64 + (p->objsize - 1)) / p->objsize;
	}

	newdata = realloc(p->data, newcapacity * p->objsize);

	if (newdata == NULL) {
		return 0;
	}

	p->data = newdata;
	p->capacity = newcapacity;

	return 1;
}

int scv_reserve(struct scv_vector *p, size_t capacity)
{
	void *newdata;

	assert(p != NULL);
	assert(p->data != NULL);

	if (capacity <= p->capacity) {
		return 1;
	}

	newdata = realloc(p->data, capacity * p->objsize);

	if (newdata == NULL) {
		return 0;
	}

	p->data = newdata;
	p->capacity = capacity;

	return 1;
}

int scv_resize(struct scv_vector *p, size_t size)
{
	assert(p != NULL);
	assert(p->data != NULL);

	if (size > p->capacity) {
		if (!scv_i_grow(p, size)) {
			return 0;
		}
	}

	p->size = size;

	return 1;
}

void *scv_at(struct scv_vector *p, size_t i)
{
	assert(p != NULL);
	assert(p->data != NULL);
	assert(i < p->size);

	return SCV_AT(p, i);
}

int scv_insert(struct scv_vector *p, const void *data, size_t nobj, size_t i)
{
	assert(p != NULL);
	assert(p->data != NULL);
	assert(i <= p->size);

	if (p->size + nobj > p->capacity) {
		if (!scv_i_grow(p, p->size + nobj)) {
			return 0;
		}
	}

	if (i < p->size) {
		memmove(SCV_AT(p, i + nobj), SCV_AT(p, i), (p->size - i) * p->objsize);
	}

	if (data != NULL) {
		memcpy(SCV_AT(p, i), data, nobj * p->objsize);
	}

	p->size += nobj;

	return 1;
}

int scv_erase(struct scv_vector *p, size_t i, size_t j)
{
	assert(p != NULL);
	assert(p->data != NULL);
	assert(i < j);
	assert(i < p->size);
	assert(j <= p->size);

	if (j == p->size) {
		return scv_resize(p, i);
	}

	memmove(SCV_AT(p, i), SCV_AT(p, j), (p->size - j) * p->objsize);

	p->size -= j - i;

	return 1;
}

int scv_push_back(struct scv_vector *p, const void *data)
{
	assert(p != NULL);
	assert(p->data != NULL);

	if (p->size + 1 > p->capacity) {
		if (!scv_i_grow(p, p->size + 1)) {
			return 0;
		}
	}

	if (data != NULL) {
		memcpy(SCV_AT(p, p->size), data, p->objsize);
	}

	p->size += 1;

	return 1;
}

int scv_pop_back(struct scv_vector *p)
{
	assert(p != NULL);
	assert(p->data != NULL);

	if (p->size == 0) {
		return 0;
	}

	p->size -= 1;

	return 1;
}
