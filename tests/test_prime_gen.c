#include "prime_gen.h"
#include "miller_rabin.h"
#include "test_util.h"

int main(void) {
    for (int bits = 8; bits <= 32; bits += 4)
        for (int i = 0; i < 50; i++) {
            u64 p;
            CHECK(random_prime(bits, &p));
            CHECK(is_prime(p));
            CHECK(p >> (bits - 1) == 1);          /* exactly `bits` bits */
        }
    u64 p;
    CHECK(!random_prime(1, &p));
    CHECK(!random_prime(64, &p));
    puts("prime_gen OK");
    return 0;
}
