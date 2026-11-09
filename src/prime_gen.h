/* prime_gen.h - secure randomness and random prime generation. */
#ifndef PRIME_GEN_H
#define PRIME_GEN_H
#include "modmath.h"

/* Fills *out with 64 random bits from the OS CSPRNG (/dev/urandom).
 * Returns 1 on success, 0 on failure. */
int rng_u64(u64 *out);

/* Generates a random prime with exactly `bits` bits (2 <= bits <= 63).
 * For bits >= 3 the top TWO bits are forced to 1, so that the product of two
 * such primes always has the full 2*bits-1 .. 2*bits bit length.
 * Returns 0 on RNG failure or invalid `bits`. */
int random_prime(int bits, u64 *out);

#endif
