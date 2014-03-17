/*
 * scv - Simple C Vector
 *
 * scv.h
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

#ifndef SCV_H_INCLUDED
#define SCV_H_INCLUDED

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Structure representing a `scv_vector`.
 *
 * @see scv_new
 */
struct scv_vector {
	void *data;      /**< Pointer to the underlying memory. */
	size_t objsize;  /**< Size of each element in bytes. */
	size_t size;     /**< Used size in number of elements. */
	size_t capacity; /**< Capacity in number of elements. */
};

/**
 * Status codes returned by some functions.
 */
typedef enum {
	SCV_OK = 0,      /**< Success. */
	SCV_ERROR = -1,  /**< Generic error. */
	SCV_ENOMEM = -2, /**< Out of memory. */
	SCV_ERANGE = -3, /**< Overflow or out of range. */
	SCV_EINVAL = -4  /**< Invalid argument. */
} scv_error_code;

/**
 * Creates a new `scv_vector`.
 *
 * `capacity` is in number of elements.
 *
 * @param objsize size of each element in bytes
 * @param capacity initial capacity in number of elements
 * @return pointer to `scv_vector`, `NULL` on error
 */
struct scv_vector *scv_new(size_t objsize, size_t capacity);

/**
 * Destroys `v`, freeing the associated memory.
 *
 * @param v pointer to `scv_vector`.
 */
void scv_delete(struct scv_vector *v);

/**
 * Returns a pointer to element number `i` of `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i index
 * @return pointer to element `i`, `NULL` on error
 */
void *scv_at(struct scv_vector *v, size_t i);

/**
 * Returns a pointer to the first element of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return pointer to first element, `NULL` on error
 */
void *scv_front(struct scv_vector *v);

/**
 * Returns a pointer to the last element of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return pointer to last element, `NULL` on error
 */
void *scv_back(struct scv_vector *v);

/**
 * Returns a pointer to the elements of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return pointer to elements, `NULL` if empty
 */
void *scv_data(struct scv_vector *v);

/**
 * Checks if `v` is empty.
 *
 * @param v pointer to `scv_vector`
 * @return non-zero if empty
 */
int scv_empty(const struct scv_vector *v);

/**
 * Returns the size of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return size in number of elements
 */
size_t scv_size(const struct scv_vector *v);

/**
 * Returns the size of each element in `v`.
 *
 * @param v pointer to `scv_vector`
 * @return size of each element in bytes
 */
size_t scv_objsize(const struct scv_vector *v);

/**
 * Reserves space in `v`.
 *
 * @param v pointer to `scv_vector`
 * @param capacity requested capacity
 * @return zero on success, error code on error
 */
int scv_reserve(struct scv_vector *v, size_t capacity);

/**
 * Returns the capacity of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return capacity in number of elements
 */
size_t scv_capacity(const struct scv_vector *v);

/**
 * Trims the capacity of `v` to the number of elements used.
 *
 * @param v pointer to `scv_vector`
 * @return zero on success, error code on error
 */
int scv_shrink_to_fit(struct scv_vector *v);

/**
 * Removes all elements from `v`.
 *
 * @param v pointer to `scv_vector`
 * @return zero on success, error code on error
 */
int scv_clear(struct scv_vector *v);

/**
 * Replaces the contents of `v` with `nobj` elements from `data`.
 *
 * If `data` is `NULL`, any assigned elements are not initialized.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`
 * @param data pointer to data to copy into assigned elements
 * @param nobj number of elements to assign
 * @return zero on success, error code on error
 */
int scv_assign(struct scv_vector *v, const void *data, size_t nobj);

/**
 * Replaces elements from `i` up to, but not including, `j` in `v`, with
 * `nobj` elements from `data`.
 *
 * If `data` is `NULL`, any inserted elements are not initialized.
 *
 * `i` can be `scv_size(v)`, in which case elements are added at the end.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i start index
 * @param j end index
 * @param data pointer to data to copy into new elements
 * @param nobj number of elements to insert
 * @return zero on success, error code on error
 */
int scv_replace(struct scv_vector *v, size_t i, size_t j, const void *data, size_t nobj);

/**
 * Inserts `nobj` elements from `data` before element number `i` of `v`.
 *
 * If `data` is `NULL`, inserted elements are not initialized.
 *
 * `i` can be `scv_size(v)`, in which case elements are added at the end.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i index
 * @param data pointer to data to copy into new elements
 * @param nobj number of elements to insert
 * @return zero on success, error code on error
 */
int scv_insert(struct scv_vector *v, size_t i, const void *data, size_t nobj);

/**
 * Removes elements from `i` up to, but not including, `j` from `v`.
 *
 * @param v pointer to `scv_vector`
 * @param i start index
 * @param j end index
 * @return zero on success, error code on error
 */
int scv_erase(struct scv_vector *v, size_t i, size_t j);

/**
 * Inserts a single element from `data` at the end of `v`.
 *
 * If `data` is `NULL`, the inserted element is not initialized.
 *
 * `data` must not point inside `v`.
 *
 * @param v pointer to `scv_vector`
 * @param data pointer to data to copy into new element
 * @return zero on success, error code on error
 */
int scv_push_back(struct scv_vector *v, const void *data);

/**
 * Removes the last element of `v`.
 *
 * @param v pointer to `scv_vector`
 * @return zero on success, error code on error
 */
int scv_pop_back(struct scv_vector *v);

/**
 * Resizes the number of elements in `v`.
 *
 * Any new elements are uninitialized.
 *
 * @param v pointer to `scv_vector`
 * @param size new size
 * @return zero on success, error code on error
 */
int scv_resize(struct scv_vector *v, size_t size);

/**
 * Copies elements from `src` to `dst`.
 *
 * @param dst pointer to destination `scv_vector`
 * @param src pointer to source `scv_vector`
 * @return zero on success, error code on error
 */
int scv_copy(struct scv_vector *dst, const struct scv_vector *src);

/**
 * Swaps elements between `scv1` and `scv2`.
 *
 * @param scv1 pointer to `scv_vector`
 * @param scv2 pointer to `scv_vector`
 * @return zero on success, error code on error
 */
int scv_swap(struct scv_vector *scv1, struct scv_vector *scv2);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCV_H_INCLUDED */
