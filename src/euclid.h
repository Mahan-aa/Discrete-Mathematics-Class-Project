/* euclid.h - gcd, Extended Euclidean Algorithm and modular inverse. */
#ifndef EUCLID_H
#define EUCLID_H
#include "modmath.h"

u64 gcd_u64(u64 a, u64 b);

/* Extended Euclid: returns g = gcd(a, b) and sets *x, *y so that
 *     a*x + b*y = g      (Bezout's identity).
 * Signed 128-bit is used because the coefficients can be negative. */
i128 ext_gcd(i128 a, i128 b, i128 *x, i128 *y);

/* Modular inverse: finds inv in [1, m) with (a * inv) % m == 1.
 * Returns 1 on success, 0 if gcd(a, m) != 1 (no inverse exists). */
int modinv(u64 a, u64 m, u64 *inv);

#endif
