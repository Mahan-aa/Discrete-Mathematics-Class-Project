#include <stddef.h>
#include "miller_rabin.h"

static const u64 WITNESSES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
#define NW (sizeof WITNESSES / sizeof WITNESSES[0])

/* Returns 1 if 'a' PROVES n composite. Write n-1 = d * 2^s with d odd, then
 * n is "probably prime to base a" if a^d = 1 or a^(d*2^r) = -1 for some r<s. */
static int is_witness(u64 a, u64 n, u64 d, int s) {
    u64 x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return 0;
    for (int r = 1; r < s; r++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return 0;
    }
    return 1;
}

int is_prime(u64 n) {
    if (n < 2) return 0;
    for (size_t i = 0; i < NW; i++) {
        if (n == WITNESSES[i]) return 1;
        if (n % WITNESSES[i] == 0) return 0;   /* cheap trial division */
    }
    u64 d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (size_t i = 0; i < NW; i++)
        if (is_witness(WITNESSES[i], n, d, s)) return 0;
    return 1;
}
