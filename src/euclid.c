#include "euclid.h"

u64 gcd_u64(u64 a, u64 b) {
    while (b) { u64 t = a % b; a = b; b = t; }
    return a;
}

/* Iterative version (no recursion depth concerns). Invariant at every step:
 *     old_r = a*old_x + b*old_y     and     r = a*cur_x + b*cur_y
 * When r reaches 0, old_r is the gcd and (old_x, old_y) are the coefficients. */
i128 ext_gcd(i128 a, i128 b, i128 *x, i128 *y) {
    i128 old_r = a, r = b;
    i128 old_x = 1, cur_x = 0;
    i128 old_y = 0, cur_y = 1;
    while (r != 0) {
        i128 q = old_r / r, t;
        t = old_r - q * r;     old_r = r;     r = t;
        t = old_x - q * cur_x; old_x = cur_x; cur_x = t;
        t = old_y - q * cur_y; old_y = cur_y; cur_y = t;
    }
    *x = old_x; *y = old_y;
    return old_r;
}

int modinv(u64 a, u64 m, u64 *inv) {
    if (m < 2) return 0;
    i128 x, y;
    i128 g = ext_gcd((i128)(a % m), (i128)m, &x, &y);
    if (g != 1) return 0;
    x %= (i128)m;
    if (x < 0) x += (i128)m;     /* normalise negative coefficient */
    *inv = (u64)x;
    return 1;
}
