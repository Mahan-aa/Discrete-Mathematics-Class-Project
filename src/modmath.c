#include "modmath.h"

u64 mulmod(u64 a, u64 b, u64 m) {
    return (u64)(((u128)a * b) % m);
}

u64 powmod(u64 base, u64 exp, u64 m) {
    u64 result = 1 % m;          /* 1 % m handles m == 1 correctly */
    base %= m;
    while (exp) {
        if (exp & 1) result = mulmod(result, base, m);
        base = mulmod(base, base, m);
        exp >>= 1;
    }
    return result;
}
