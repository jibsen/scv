/*
 * scv - Simple C Vector
 *
 * test_scv.c
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
#include <stdio.h>

#include "greatest.h"

#include "scv.h"

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

static int check_int_vector(struct scv_vector *v)
{
	size_t i;
	int j;
	int *p;

	if (scv_objsize(v) != sizeof(int)) {
		return 0;
	}

	if (scv_empty(v)) {
		return 0;
	}

	p = scv_data(v);

	for (i = 0, j = 0; i < scv_size(v); ++i, ++j) {
		if (p[i] != j) {
			return 0;
		}
	}

	return 1;
}

/* scv_new */

TEST new_objsize_zero(void)
{
	struct scv_vector *v;

	v = scv_new(0, 1);

	ASSERT(v == NULL);

	PASS();
}

TEST new_capacity_zero(void)
{
	struct scv_vector *v;

	v = scv_new(1, 0);

	ASSERT(v != NULL);

	scv_delete(v);

	PASS();
}

TEST new_objsize_max(void)
{
	struct scv_vector *v;

	v = scv_new((size_t) -1, 0);

	ASSERT(v == NULL);

	PASS();
}

TEST new_capacity_max(void)
{
	struct scv_vector *v;

	v = scv_new(1, (size_t) -1);

	ASSERT(v == NULL);

	PASS();
}

/* scv_delete */

TEST delete_null(void)
{
	scv_delete(NULL);

	PASS();
}

/* scv_at */

TEST at_inside(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	p = scv_at(v, 1);

	ASSERT(p != NULL && p == (int *) v->data + 1);

	scv_delete(v);

	PASS();
}

TEST at_outside(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	p = scv_at(v, 6);

	ASSERT(p == NULL);

	scv_delete(v);

	PASS();
}

TEST at_empty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	p = scv_at(v, 0);

	ASSERT(p == NULL);

	scv_delete(v);

	PASS();
}

/* scv_front */

TEST front_nonempty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	p = scv_front(v);

	ASSERT(p != NULL && p == (int *) v->data);

	scv_delete(v);

	PASS();
}

TEST front_empty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	p = scv_front(v);

	ASSERT(p == NULL);

	scv_delete(v);

	PASS();
}

/* scv_back */

TEST back_nonempty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	p = scv_back(v);

	ASSERT(p != NULL && p == (int *) v->data + 4);

	scv_delete(v);

	PASS();
}

TEST back_single(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 1);

	p = scv_back(v);

	ASSERT(p != NULL && p == (int *) v->data);

	scv_delete(v);

	PASS();
}

TEST back_empty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	p = scv_back(v);

	ASSERT(p == NULL);

	scv_delete(v);

	PASS();
}

/* scv_data */

TEST data_nonempty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	p = scv_data(v);

	ASSERT(p != NULL && p == (int *) v->data);

	scv_delete(v);

	PASS();
}

TEST data_empty(void)
{
	struct scv_vector *v;
	int *p;

	v = scv_new(sizeof(int), 25);

	p = scv_data(v);

	ASSERT(p == NULL);

	scv_delete(v);

	PASS();
}

/* scv_empty */

TEST empty_empty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	res = scv_empty(v);

	ASSERT(res != 0);

	scv_delete(v);

	PASS();
}

TEST empty_nonempty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_resize(v, 1);

	res = scv_empty(v);

	ASSERT(res == 0);

	scv_delete(v);

	PASS();
}

/* scv_size */

TEST size_empty(void)
{
	struct scv_vector *v;

	v = scv_new(3, 25);

	ASSERT(v != NULL && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST size_nonempty(void)
{
	struct scv_vector *v;

	v = scv_new(3, 25);

	scv_resize(v, 10);

	ASSERT(v != NULL && scv_size(v) == 10);

	scv_delete(v);

	PASS();
}

/* scv_objsize */

TEST objsize(void)
{
	struct scv_vector *v;

	v = scv_new(3, 25);

	ASSERT(v != NULL && scv_objsize(v) == 3);

	scv_delete(v);

	PASS();
}

/* scv_reserve */

TEST reserve_zero(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	res = scv_reserve(v, 0);

	ASSERT(res == SCV_OK && scv_capacity(v) == 100);

	scv_delete(v);

	PASS();
}

TEST reserve_below_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 120);

	scv_resize(v, 100);

	res = scv_reserve(v, 80);

	ASSERT(res == SCV_OK && scv_size(v) == 100 && scv_capacity(v) == 120);

	scv_delete(v);

	PASS();
}

TEST reserve_equals_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 120);

	scv_resize(v, 100);

	res = scv_reserve(v, 100);

	ASSERT(res == SCV_OK && scv_size(v) == 100 && scv_capacity(v) == 120);

	scv_delete(v);

	PASS();
}

TEST reserve_above_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 120);

	scv_resize(v, 100);

	res = scv_reserve(v, 110);

	ASSERT(res == SCV_OK && scv_size(v) == 100 && scv_capacity(v) == 120);

	scv_delete(v);

	PASS();
}

TEST reserve_above_capacity(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 120);

	scv_resize(v, 100);

	res = scv_reserve(v, 140);

	ASSERT(res == SCV_OK && scv_size(v) == 100 && scv_capacity(v) == 140);

	scv_delete(v);

	PASS();
}

TEST reserve_capacity_max(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	res = scv_reserve(v, (size_t) -1);

	ASSERT(res != SCV_OK);

	scv_delete(v);

	PASS();
}

/* scv_capacity */

TEST capacity(void)
{
	struct scv_vector *v;

	v = scv_new(3, 25);

	ASSERT(v != NULL && scv_capacity(v) >= 25);

	scv_delete(v);

	PASS();
}

/* scv_shrink_to_fit */

TEST shrink_to_fit_empty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	res = scv_shrink_to_fit(v);

	ASSERT(res == SCV_OK && scv_capacity(v) > 0);

	scv_delete(v);

	PASS();
}

TEST shrink_to_fit_size_equals_capacity(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_resize(v, 100);

	res = scv_shrink_to_fit(v);

	ASSERT(res == SCV_OK && scv_size(v) == scv_capacity(v));

	scv_delete(v);

	PASS();
}

TEST shrink_to_fit_size_below_capacity(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_resize(v, 80);

	res = scv_shrink_to_fit(v);

	ASSERT(res == SCV_OK && scv_size(v) == scv_capacity(v));

	scv_delete(v);

	PASS();
}

/* scv_clear */

TEST clear_nonempty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	scv_insert(v, 0, NULL, 5);

	res = scv_clear(v);

	ASSERT(res == SCV_OK && scv_size(v) == 0 && scv_capacity(v) == 25);

	scv_delete(v);

	PASS();
}

TEST clear_empty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_clear(v);

	ASSERT(res == SCV_OK && scv_size(v) == 0 && scv_capacity(v) == 25);

	scv_delete(v);

	PASS();
}

/* scv_assign */

TEST assign_empty_and_nonempty(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	ASSERT(scv_size(v) == 0);

	res = scv_assign(v, NULL, 0);

	ASSERT(res == SCV_OK && scv_capacity(v) == 25 && scv_size(v) == 0);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	res = scv_assign(v, NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST assign_less(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, -1, -1, -1, -1, -1 };
	const int data2[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_assign(v, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST assign_more(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, -1 };
	const int data2[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_assign(v, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST assign_null_data(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	ASSERT(scv_size(v) == 0);

	res = scv_assign(v, NULL, 50);

	ASSERT(res == SCV_OK && scv_size(v) == 50);

	scv_delete(v);

	PASS();
}

TEST assign_growing_capacity(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, -1 };
	int data2[50];
	int res;
	int i;

	for (i = 0; i < ARRAY_SIZE(data2); ++i) {
		data2[i] = i;
	}

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));
	ASSERT(scv_capacity(v) == 25);

	res = scv_assign(v, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data2));
	ASSERT(scv_capacity(v) >= ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST assign_nobj_max(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, NULL, (size_t) -1);

	ASSERT(res != SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

/* scv_replace */

TEST replace_empty_vector(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_replace(v, 0, 0, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_empty_range_start(void)
{
	struct scv_vector *v;
	const int data1[] = { 2, 3, 4 };
	const int data2[] = { 0, 1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 0, 0, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_empty_range_middle(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, 4 };
	const int data2[] = { 2, 3 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 2, 2, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_empty_range_end(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, 2 };
	const int data2[] = { 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 3, 3, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_start(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, 2, 3, 4 };
	const int data2[] = { 0, 1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 0, 2, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_middle(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, -1, -1, 4 };
	const int data2[] = { 2, 3 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 2, 4, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_end(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, 2, -1, -1 };
	const int data2[] = { 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 3, scv_size(v), data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_start_with_nothing(void)
{
	struct scv_vector *v;
	const int data[] = { -1, -1, 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, 0, 2, NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_middle_with_nothing(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, -1, -1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, 2, 4, NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_end_with_nothing(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4, -1, -1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, scv_size(v) - 2, scv_size(v), NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_with_less(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, -1, -1, -1, 4 };
	const int data2[] = { 2, 3 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 2, 5, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == 5);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_with_more(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, -1, 4 };
	const int data2[] = { 2, 3 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 2, 3, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == 5);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_all_with_less(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, -1, -1, -1, -1, -1 };
	const int data2[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 0, scv_size(v), data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_all_with_more(void)
{
	struct scv_vector *v;
	const int data1[] = { -1, -1, -1 };
	const int data2[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 0, scv_size(v), data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_with_null_data(void)
{
	struct scv_vector *v;
	const int data[] = { -1, -1, -1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, 0, scv_size(v), NULL, 50);

	ASSERT(res == SCV_OK && scv_size(v) == 50);

	scv_delete(v);

	PASS();
}

TEST replace_growing_capacity(void)
{
	struct scv_vector *v;
	const int data1[] = { 45, 46, 47, 48, 49 };
	int data2[45];
	int res;
	int i;

	for (i = 0; i < ARRAY_SIZE(data2); ++i) {
		data2[i] = i;
	}

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_replace(v, 0, 0, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_nobj_max(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_replace(v, 0, 0, NULL, (size_t) -1);

	ASSERT(res != SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST replace_outside_range(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, 2, scv_size(v) + 1, NULL, 0);

	ASSERT(res != SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST replace_negative_range(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_replace(v, 3, 2, NULL, 1);

	ASSERT(res != SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

/* scv_insert */

TEST insert_empty_vector(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_start(void)
{
	struct scv_vector *v;
	const int data1[] = { 2, 3, 4 };
	const int data2[] = { 0, 1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_insert(v, 0, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_middle(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, 4 };
	const int data2[] = { 2, 3 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_insert(v, 2, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_end(void)
{
	struct scv_vector *v;
	const int data1[] = { 0, 1, 2 };
	const int data2[] = { 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_insert(v, scv_size(v), data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_nothing(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_insert(v, 0, NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	res = scv_insert(v, 2, NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	res = scv_insert(v, scv_size(v), NULL, 0);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_null_data(void)
{
	struct scv_vector *v;
	const int data[] = { -1, -1, -1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_insert(v, 0, NULL, 50);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) + 50);

	scv_delete(v);

	PASS();
}

TEST insert_growing_capacity(void)
{
	struct scv_vector *v;
	const int data1[] = { 45, 46, 47, 48, 49 };
	int data2[45];
	int res;
	int i;

	for (i = 0; i < ARRAY_SIZE(data2); ++i) {
		data2[i] = i;
	}

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data1, ARRAY_SIZE(data1));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1));

	res = scv_insert(v, 0, data2, ARRAY_SIZE(data2));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data1) + ARRAY_SIZE(data2));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST insert_nobj_max(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, NULL, (size_t) -1);

	ASSERT(res != SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST insert_outside_range(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_insert(v, 0, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_insert(v, scv_size(v) + 1, NULL, 0);

	ASSERT(res != SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

/* scv_erase */

TEST erase_nothing(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_erase(v, 0, 0);

	ASSERT(res == SCV_OK && scv_size(v) == 0);

	scv_resize(v, 10);

	res = scv_erase(v, 0, 0);

	ASSERT(res == SCV_OK && scv_size(v) == 10);

	scv_delete(v);

	PASS();
}

TEST erase_start(void)
{
	struct scv_vector *v;
	const int data[] = { -1, -1, 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, 0, 2);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST erase_middle(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, -1, -1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, 2, 4);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST erase_end(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4, -1, -1 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, scv_size(v) - 2, scv_size(v));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 2);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST erase_all(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, 0, scv_size(v));

	ASSERT(res == SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST erase_outside_range(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, 2, scv_size(v) + 1);

	ASSERT(res != SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST erase_negative_range(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_erase(v, 3, 2);

	ASSERT(res != SCV_OK && scv_size(v) == ARRAY_SIZE(data));
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

/* scv_push_back */

TEST push_back_empty(void)
{
	struct scv_vector *v;
	const int i = 0;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_push_back(v, &i);

	ASSERT(res == SCV_OK && scv_size(v) == 1);
	ASSERT(check_int_vector(v));

	PASS();
}

TEST push_back(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	const int i = 5;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_push_back(v, &i);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) + 1);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST push_back_growing_capacity(void)
{
	struct scv_vector *v;
	int i;
	int res;

	v = scv_new(sizeof(int), 25);

	for (i = 0; i < 30; ++i) {
		res = scv_push_back(v, &i);
		ASSERT(res == SCV_OK && scv_size(v) == (size_t) i + 1);
		ASSERT(check_int_vector(v));
	}

	scv_delete(v);

	PASS();
}

/* scv_pop_back */

TEST pop_back(void)
{
	struct scv_vector *v;
	const int data[] = { 0, 1, 2, 3, 4 };
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_assign(v, data, ARRAY_SIZE(data));

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data));

	res = scv_pop_back(v);

	ASSERT(res == SCV_OK && scv_size(v) == ARRAY_SIZE(data) - 1);
	ASSERT(check_int_vector(v));

	scv_delete(v);

	PASS();
}

TEST pop_back_empty(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(sizeof(int), 25);

	res = scv_pop_back(v);

	ASSERT(res != SCV_OK && scv_size(v) == 0);

	PASS();
}

/* scv_resize */

TEST resize_zero_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_assign(v, NULL, 50);

	res = scv_resize(v, 0);

	ASSERT(res == SCV_OK && scv_size(v) == 0);

	scv_delete(v);

	PASS();
}

TEST resize_smaller_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_assign(v, NULL, 50);

	res = scv_resize(v, 25);

	ASSERT(res == SCV_OK && scv_size(v) == 25);

	scv_delete(v);

	PASS();
}

TEST resize_equal_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_assign(v, NULL, 50);

	res = scv_resize(v, 50);

	ASSERT(res == SCV_OK && scv_size(v) == 50);

	scv_delete(v);

	PASS();
}

TEST resize_larger_size(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_assign(v, NULL, 50);

	res = scv_resize(v, 75);

	ASSERT(res == SCV_OK && scv_size(v) == 75);

	scv_delete(v);

	PASS();
}

TEST resize_larger_capacity(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	scv_assign(v, NULL, 50);

	res = scv_resize(v, 125);

	ASSERT(res == SCV_OK && scv_size(v) == 125);

	scv_delete(v);

	PASS();
}

TEST resize_size_max(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(1, 100);

	res = scv_resize(v, (size_t) -1);

	ASSERT(res != SCV_OK);

	scv_delete(v);

	PASS();
}

/* scv_copy */

TEST copy_empty_to_empty(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(4, 25);
	v2 = scv_new(4, 50);

	res = scv_copy(v1, v2);

	ASSERT(res == SCV_OK && scv_capacity(v1) == 25 && scv_size(v1) == 0);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

TEST copy_empty_to_nonempty(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(4, 25);
	v2 = scv_new(4, 50);

	scv_insert(v1, 0, NULL, 5);

	res = scv_copy(v1, v2);

	ASSERT(res == SCV_OK && scv_capacity(v1) == 25 && scv_size(v1) == 0);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

TEST copy_nonempty_to_empty(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(4, 25);
	v2 = scv_new(4, 50);

	scv_insert(v2, 0, NULL, 5);

	res = scv_copy(v1, v2);

	ASSERT(res == SCV_OK && scv_capacity(v1) == 25 && scv_size(v1) == 5);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

TEST copy_nonempty_to_nonempty(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(4, 25);
	v2 = scv_new(4, 50);

	scv_insert(v1, 0, NULL, 10);
	scv_insert(v2, 0, NULL, 5);

	res = scv_copy(v1, v2);

	ASSERT(res == SCV_OK && scv_capacity(v1) == 25 && scv_size(v1) == 5);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

TEST copy_objsize_mismatch(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(4, 25);
	v2 = scv_new(2, 50);

	res = scv_copy(v1, v2);

	ASSERT(res != SCV_OK);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

TEST copy_to_itself(void)
{
	struct scv_vector *v;
	int res;

	v = scv_new(4, 25);

	res = scv_copy(v, v);

	ASSERT(res != SCV_OK);

	scv_delete(v);

	PASS();
}

/* scv_swap */

TEST swap(void)
{
	struct scv_vector *v1;
	struct scv_vector *v2;
	int res;

	v1 = scv_new(1, 100);
	v2 = scv_new(4, 25);

	res = scv_swap(v1, v2);

	ASSERT(res == SCV_OK && scv_capacity(v1) == 25 && scv_capacity(v2) == 100);

	scv_delete(v1);
	scv_delete(v2);

	PASS();
}

SUITE(scv)
{
	RUN_TEST(new_objsize_zero);
	RUN_TEST(new_capacity_zero);
	RUN_TEST(new_objsize_max);
	RUN_TEST(new_capacity_max);

	RUN_TEST(delete_null);

	RUN_TEST(at_inside);
	RUN_TEST(at_outside);
	RUN_TEST(at_empty);

	RUN_TEST(front_nonempty);
	RUN_TEST(front_empty);

	RUN_TEST(back_nonempty);
	RUN_TEST(back_single);
	RUN_TEST(back_empty);

	RUN_TEST(data_nonempty);
	RUN_TEST(data_empty);

	RUN_TEST(empty_empty);
	RUN_TEST(empty_nonempty);

	RUN_TEST(size_empty);
	RUN_TEST(size_nonempty);

	RUN_TEST(objsize);

	RUN_TEST(reserve_zero);
	RUN_TEST(reserve_below_size);
	RUN_TEST(reserve_equals_size);
	RUN_TEST(reserve_above_size);
	RUN_TEST(reserve_above_capacity);
	RUN_TEST(reserve_capacity_max);

	RUN_TEST(capacity);

	RUN_TEST(shrink_to_fit_empty);
	RUN_TEST(shrink_to_fit_size_equals_capacity);
	RUN_TEST(shrink_to_fit_size_below_capacity);

	RUN_TEST(clear_nonempty);
	RUN_TEST(clear_empty);

	RUN_TEST(assign_empty_and_nonempty);
	RUN_TEST(assign_less);
	RUN_TEST(assign_more);
	RUN_TEST(assign_null_data);
	RUN_TEST(assign_growing_capacity);
	RUN_TEST(assign_nobj_max);

	RUN_TEST(replace_empty_vector);
	RUN_TEST(replace_empty_range_start);
	RUN_TEST(replace_empty_range_middle);
	RUN_TEST(replace_empty_range_end);
	RUN_TEST(replace_start);
	RUN_TEST(replace_middle);
	RUN_TEST(replace_end);
	RUN_TEST(replace_start_with_nothing);
	RUN_TEST(replace_middle_with_nothing);
	RUN_TEST(replace_end_with_nothing);
	RUN_TEST(replace_with_less);
	RUN_TEST(replace_with_more);
	RUN_TEST(replace_all_with_less);
	RUN_TEST(replace_all_with_more);
	RUN_TEST(replace_with_null_data);
	RUN_TEST(replace_growing_capacity);
	RUN_TEST(replace_nobj_max);
	RUN_TEST(replace_outside_range);
	RUN_TEST(replace_negative_range);

	RUN_TEST(insert_empty_vector);
	RUN_TEST(insert_start);
	RUN_TEST(insert_middle);
	RUN_TEST(insert_end);
	RUN_TEST(insert_nothing);
	RUN_TEST(insert_null_data);
	RUN_TEST(insert_growing_capacity);
	RUN_TEST(insert_nobj_max);
	RUN_TEST(insert_outside_range);

	RUN_TEST(erase_nothing);
	RUN_TEST(erase_start);
	RUN_TEST(erase_middle);
	RUN_TEST(erase_end);
	RUN_TEST(erase_all);
	RUN_TEST(erase_outside_range);
	RUN_TEST(erase_negative_range);

	RUN_TEST(push_back_empty);
	RUN_TEST(push_back);
	RUN_TEST(push_back_growing_capacity);

	RUN_TEST(pop_back);
	RUN_TEST(pop_back_empty);

	RUN_TEST(resize_zero_size);
	RUN_TEST(resize_smaller_size);
	RUN_TEST(resize_equal_size);
	RUN_TEST(resize_larger_size);
	RUN_TEST(resize_larger_capacity);
	RUN_TEST(resize_size_max);

	RUN_TEST(copy_empty_to_empty);
	RUN_TEST(copy_empty_to_nonempty);
	RUN_TEST(copy_nonempty_to_empty);
	RUN_TEST(copy_nonempty_to_nonempty);
	RUN_TEST(copy_objsize_mismatch);
	RUN_TEST(copy_to_itself);

	RUN_TEST(swap);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[])
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(scv);
	GREATEST_MAIN_END();
}
