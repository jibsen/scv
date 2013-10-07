
Simple C Vector
===============

Copyright 2003-2013 Joergen Ibsen

<http://www.ibsensoftware.com/>


About
-----

Simple C Vector (scv) is a basic C implementation of a [dynamic array][dyna].

I wrote it for a project years ago, and decided to dust it off and make it
available under the Apache License.

scv uses `malloc()` for memory allocation, which works reasonably well in
many cases, but if needed, you can improve performance by using a custom
allocator.

[dyna]: http://en.wikipedia.org/wiki/Dynamic_array
