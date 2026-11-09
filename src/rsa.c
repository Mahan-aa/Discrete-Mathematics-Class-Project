#include "rsa.h"
#include "euclid.h"
#include "prime_gen.h"

int rsa_keygen(int prime_bits, rsa_pub *pub, rsa_priv *priv) {
    if (prime_bits < RSA_MIN_PRIME_BITS || prime_bits > RSA_MAX_PRIME_BITS) return 0;
    for (;;) {
        u64 p, q;
        if (!random_prime(prime_bits, &p) || !random_prime(prime_bits, &q)) return 0;
        if (p == q) continue;                      /* need two DISTINCT primes */

        u64 n   = p * q;                           /* < 2^64 by RSA_MAX_PRIME_BITS */
        u64 phi = (p - 1) * (q - 1);               /* Euler's totient of n         */
        u64 e   = RSA_PUBLIC_EXPONENT, d;

        if (gcd_u64(e, phi) != 1) continue;        /* e must be invertible mod phi */
        if (!modinv(e, phi, &d)) continue;         /* d = e^-1 mod phi (ext. Euclid) */

        pub->n = n;  pub->e = e;
        priv->n = n; priv->d = d;
        return 1;
    }
}

u64 rsa_encrypt(u64 m, const rsa_pub *pub)  { return powmod(m, pub->e, pub->n); }
u64 rsa_decrypt(u64 c, const rsa_priv *priv){ return powmod(c, priv->d, priv->n); }
