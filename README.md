
Simple C Vector
===============

Copyright 2003-2013 Joergen Ibsen

<http://www.ibsensoftware.com/>


About
-----

Simple C Vector (scv) is a basic C implementation of a [dynamic array][dyna].

I wrote it for a project years ago, and decided to dust it off and make it
available under the Apache License.

To enable the code to work in general, independent of the type of objects
stored in a particular `scv_vector`, functions for inserting and accessing
elements take and return `void` pointers.

This has the minor drawback that you cannot use a constant expression
directly, but need an actual object that you can pass a pointer to, when
calling functions like `scv_push_back()`.

scv uses `malloc()` for memory allocation, which works reasonably well in
many cases, but if needed, you can improve performance by using a custom
allocator.

[dyna]: http://en.wikipedia.org/wiki/Dynamic_array


Example
-------

Here is a simple example that reads integer coordinates in the form `x,y`
into a `scv_vector` and prints them in lexicographical order:

```C
#include <stdlib.h>
#include <stdio.h>

#include "scv.h"

struct point {
	int x;
	int y;
};

int point_compare(const struct point *lhs, const struct point *rhs)
{
	if (lhs->x == rhs->x) {
		return lhs->y - rhs->y;
	}

	return lhs->x - rhs->x;
}

int main(void)
{
	struct scv_vector *v;
	struct point p;
	size_t i;

	v = scv_new(sizeof p, 10);

	while (scanf("%d,%d", &p.x, &p.y) == 2) {
		scv_push_back(v, &p);
	}

	qsort(scv_front(v), scv_size(v), scv_objsize(v), point_compare);

	for (i = 0; i < scv_size(v); ++i) {
		struct point *pp = scv_at(v, i);

		printf("%d,%d\n", pp->x, pp->y);
	}

	scv_delete(v);

	return 0;
}
```
