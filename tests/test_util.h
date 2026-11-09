#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdio.h>
#include <stdlib.h>
#define CHECK(cond) do { if (!(cond)) { \
    fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); exit(1); } } while (0)
#endif
