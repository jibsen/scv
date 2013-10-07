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
 * Delete `v`.
 *
 * @param v pointer to `scv_vector`.
 */
void scv_delete(struct scv_vector *v);

/**
 * Return size of each element in `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return size of each element in bytes.
 */
size_t scv_objsize(const struct scv_vector *v);

/**
 * Return size of `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return size in number of elements.
 */
size_t scv_size(const struct scv_vector *v);

/**
 * Check if `v` is empty.
 *
 * @param v pointer to `scv_vector`.
 * @return non-zero if empty.
 */
int scv_empty(const struct scv_vector *v);

/**
 * Return capacity of `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return capacity in number of elements.
 */
size_t scv_capacity(const struct scv_vector *v);

/**
 * Reserve space in `v`.
 *
 * @param v pointer to `scv_vector`.
 * @param capacity requested capacity.
 * @return non-zero on success, zero on error.
 */
int scv_reserve(struct scv_vector *v, size_t capacity);

/**
 * Trim capacity of `v` to the number of elements used.
 *
 * @param v pointer to `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_shrink_to_fit(struct scv_vector *v);

/**
 * Resize number of elements in `v`.
 *
 * Any new elements are uninitialized.
 *
 * @param v pointer to `scv_vector`.
 * @param size new size.
 * @return non-zero on success, zero on error.
 */
int scv_resize(struct scv_vector *v, size_t size);

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
 * @param v pointer to `scv_vector`.
 * @param i index.
 * @return pointer to element `i`.
 */
void *scv_at(struct scv_vector *v, size_t i);

/**
 * Return a pointer to the first element of `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return pointer to first element.
 */
void *scv_front(struct scv_vector *v);

/**
 * Return a pointer to the last element of `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return pointer to last element.
 */
void *scv_back(struct scv_vector *v);

/**
 * Replace elements from `i` up to, but not including, `j` in `v`.
 *
 * If `data` is `NULL`, any inserted elements are not initialized.
 *
 * `j` can be `p->size`, in which case elements are added at the end.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`.
 * @param i start index.
 * @param j end index.
 * @param data pointer to data to copy into new elements.
 * @param nobj number of elements to insert.
 * @return non-zero on success, zero on error.
 */
int scv_replace(struct scv_vector *v, size_t i, size_t j, const void *data, size_t nobj);

/**
 * Insert `nobj` elements before element number `i` of `v`.
 *
 * If `data` is `NULL`, inserted elements are not initialized.
 *
 * `i` can be `p->size`, in which case elements are added at the end.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`.
 * @param i index.
 * @param data pointer to data to copy into new elements.
 * @param nobj number of elements to insert.
 * @return non-zero on success, zero on error.
 */
int scv_insert(struct scv_vector *v, size_t i, const void *data, size_t nobj);

/**
 * Remove elements from `i` up to, but not including, `j` from `v`.
 *
 * @param v pointer to `scv_vector`.
 * @param i start index.
 * @param j end index.
 * @return non-zero on success, zero on error.
 */
int scv_erase(struct scv_vector *v, size_t i, size_t j);

/**
 * Insert element at end of `v`.
 *
 * If `data` is `NULL`, the inserted element is not initialized.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`.
 * @param data pointer to data to copy into new element.
 * @return non-zero on success, zero on error.
 */
int scv_push_back(struct scv_vector *v, const void *data);

/**
 * Remove last element of `v`.
 *
 * @param v pointer to `scv_vector`.
 * @return non-zero on success, zero on error.
 */
int scv_pop_back(struct scv_vector *v);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCV_H_INCLUDED */
