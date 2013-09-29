/*
 * scv - Simple C Vector
 *
 * scv.h
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

#ifndef SCV_H_INCLUDED
#define SCV_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct scv_vector {
	void *data;
	size_t objsize;
	size_t size;
	size_t capacity;
};

/**
 * Create new `scv_vector`.
 *
 * `capacity` is in number of elements.
 *
 * @param objsize size of each element in bytes.
 * @param capacity initial capacity in number of elements.
 * @return pointer to `scv_vector`, `NULL` on error.
 */
struct scv_vector *scv_new(size_t objsize, size_t capacity);

/**
 * Delete `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 */
void scv_delete(struct scv_vector *p);

/**
 * Return size of each element in `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @return size of each element in bytes.
 */
size_t scv_objsize(const struct scv_vector *p);

/**
 * Return size of `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @return size in number of elements.
 */
size_t scv_size(const struct scv_vector *p);

/**
 * Check if `scv_vector` is empty.
 *
 * @param p pointer to `scv_vector`.
 * @return non-zero if empty.
 */
int scv_empty(const struct scv_vector *p);

/**
 * Return capacity of `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @return capacity in number of elements.
 */
size_t scv_capacity(const struct scv_vector *p);

/**
 * Reserve space in `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @param capacity requested capacity.
 * @return non-zero on success, zero on error.
 */
int scv_reserve(struct scv_vector *p, size_t capacity);

/**
 * Trim capacity of `scv_vector` to the number of elements used.
 *
 * @param p pointer to `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_shrink_to_fit(struct scv_vector *p);

/**
 * Resize number of elements in `scv_vector`.
 *
 * Any new elements are uninitialized.
 *
 * @param p pointer to `scv_vector`.
 * @param size new size.
 * @return non-zero on success, zero on error.
 */
int scv_resize(struct scv_vector *p, size_t size);

/**
 * Copy elements from `src` to `dst`.
 *
 * @param dst pointer to destination `scv_vector`.
 * @param src pointer to source `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_copy(struct scv_vector *dst, const struct scv_vector *src);

/**
 * Swap elements between `scv1` and `scv2`.
 *
 * @param scv1 pointer to `scv_vector`.
 * @param scv2 pointer to `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_swap(struct scv_vector *scv1, struct scv_vector *scv2);

/**
 * Return a pointer to element number `i`.
 *
 * @param p pointer to `scv_vector`.
 * @param i index.
 * @return pointer to element `i`.
 */
void *scv_at(struct scv_vector *p, size_t i);

/*
 * Insert `nobj` elements before element number `i` of `scv_vector`.
 *
 * If `data` is `NULL`, inserted elements are not initialized.
 *
 * `i` can be `p->size`, in which case elements are added at the end.
 *
 * @param p pointer to `scv_vector`.
 * @param i index.
 * @param data pointer to data to copy into new elements.
 * @param nobj number of elements to insert.
 * @return non-zero on success, zero on error.
 */
int scv_insert(struct scv_vector *p, size_t i, const void *data, size_t nobj);

/*
 * Remove elements from `i` up to, but not including, `j` from `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @param i start index.
 * @param i end index.
 * @return non-zero on success, zero on error.
 */
int scv_erase(struct scv_vector *p, size_t i, size_t j);

/*
 * Insert element at end of `scv_vector`.
 *
 * If `data` is `NULL`, the inserted element is not initialized.
 *
 * @param p pointer to `scv_vector`.
 * @param data pointer to data to copy into new element.
 * @return non-zero on success, zero on error.
 */
int scv_push_back(struct scv_vector *p, const void *data);

/*
 * Remove last element of `scv_vector`.
 *
 * @param p pointer to `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_pop_back(struct scv_vector *p);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCV_H_INCLUDED */
