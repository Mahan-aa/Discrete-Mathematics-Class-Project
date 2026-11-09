/* modmath.h - modular arithmetic primitives on 64-bit integers.
 *
 * Every function here is safe against overflow for ANY 64-bit modulus
 * because intermediate products are computed in 128 bits. */
#ifndef MODMATH_H
#define MODMATH_H
#include <stdint.h>

typedef uint64_t u64;
/* __int128 is a GCC/Clang extension; __extension__ silences -Wpedantic. */
__extension__ typedef unsigned __int128 u128;
__extension__ typedef __int128 i128;

/* (a * b) mod m, without overflow. m must be > 0. */
u64 mulmod(u64 a, u64 b, u64 m);

/* (base ^ exp) mod m via square-and-multiply, O(log exp). m must be > 0. */
u64 powmod(u64 base, u64 exp, u64 m);

#endif
