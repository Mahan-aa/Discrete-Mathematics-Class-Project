#include "modmath.h"
#include "test_util.h"

int main(void) {
    CHECK(mulmod(7, 8, 10) == 6);
    /* would overflow 64 bits if done naively */
    CHECK(mulmod(UINT64_MAX - 1, UINT64_MAX - 1, UINT64_MAX) == 1);
    CHECK(powmod(2, 10, 1000) == 24);
    CHECK(powmod(5, 0, 13) == 1);
    CHECK(powmod(5, 3, 1) == 0);
    /* Fermat: a^(p-1) = 1 mod p for prime p = 2^61 - 1 */
    CHECK(powmod(123456789, (1ULL << 61) - 2, (1ULL << 61) - 1) == 1);
    puts("modmath OK");
    return 0;
}
