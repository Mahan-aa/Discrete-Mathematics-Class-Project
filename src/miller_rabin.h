/* miller_rabin.h - Miller-Rabin primality test. */
#ifndef MILLER_RABIN_H
#define MILLER_RABIN_H
#include "modmath.h"

/* Returns 1 if n is prime, 0 if composite.
 *
 * Uses the first 12 primes (2..37) as witnesses. For n < 3.317e24 this set is
 * proven to give a DETERMINISTIC answer, which covers every 64-bit integer. */
int is_prime(u64 n);

#endif
