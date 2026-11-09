#include <stdio.h>
#include "prime_gen.h"
#include "miller_rabin.h"

int rng_u64(u64 *out) {
    FILE *f = fopen("/dev/urandom", "rb");
    if (!f) return 0;
    size_t got = fread(out, sizeof *out, 1, f);
    fclose(f);
    return got == 1;
}

int random_prime(int bits, u64 *out) {
    if (bits < 2 || bits > 63) return 0;
    u64 mask = (1ULL << bits) - 1;
    for (;;) {
        u64 x;
        if (!rng_u64(&x)) return 0;
        x &= mask;
        x |= 1ULL << (bits - 1);                /* exact bit length */
        if (bits >= 3) x |= 1ULL << (bits - 2); /* top two bits set */
        x |= 1;                                 /* odd              */
        if (is_prime(x)) { *out = x; return 1; }
    }
}
