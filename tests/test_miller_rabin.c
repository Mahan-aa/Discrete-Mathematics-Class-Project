#include "miller_rabin.h"
#include "test_util.h"

static int naive(u64 n) {
    if (n < 2) return 0;
    for (u64 i = 2; i * i <= n; i++) if (n % i == 0) return 0;
    return 1;
}

int main(void) {
    for (u64 n = 0; n < 20000; n++) CHECK(is_prime(n) == naive(n));
    CHECK(is_prime(2147483647ULL));                 /* 2^31 - 1, Mersenne */
    CHECK(is_prime((1ULL << 61) - 1));              /* 2^61 - 1, Mersenne */
    CHECK(is_prime(18446744073709551557ULL));       /* largest 64-bit prime */
    /* Carmichael numbers fool the Fermat test but not Miller-Rabin */
    CHECK(!is_prime(561)); CHECK(!is_prime(41041)); CHECK(!is_prime(825265));
    CHECK(!is_prime(341550071728321ULL));           /* strong pseudoprime to many bases */
    CHECK(!is_prime(4294967291ULL * 4294967279ULL));/* product of two primes */
    puts("miller_rabin OK");
    return 0;
}
