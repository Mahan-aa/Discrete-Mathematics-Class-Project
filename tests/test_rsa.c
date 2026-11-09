#include "rsa.h"
#include "prime_gen.h"
#include "test_util.h"

int main(void) {
    for (int bits = RSA_MIN_PRIME_BITS; bits <= RSA_MAX_PRIME_BITS; bits += 4)
        for (int k = 0; k < 20; k++) {
            rsa_pub pub; rsa_priv priv;
            CHECK(rsa_keygen(bits, &pub, &priv));
            CHECK(pub.n == priv.n);
            for (int i = 0; i < 50; i++) {
                u64 r; CHECK(rng_u64(&r));
                u64 m = r % pub.n;
                CHECK(rsa_decrypt(rsa_encrypt(m, &pub), &priv) == m);
            }
        }
    rsa_pub pub; rsa_priv priv;
    CHECK(!rsa_keygen(8, &pub, &priv));
    CHECK(!rsa_keygen(40, &pub, &priv));
    puts("rsa OK");
    return 0;
}
