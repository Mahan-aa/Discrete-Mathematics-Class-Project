# RSA in C

Implementation of the Text Book RSA (64) in C11 :

- **Large prime generation** The deterministic **Miller–Rabin** primality test.
- **Public/private key computation** The Extended Euclidean Algorithm, (EEA) .
- **Encryption and decryption of numeric text** (bytes packed into integer blocks) .

> note : The cuurent state of this repo is the implementation of textbook RSA with a modulus of at most 64 bits.
It can be factored in milliseconds.


## Cmake :

```bash
make            # builds build/rsa
make test       # builds and runs all unit tests
./build/rsa demo  # a little example
```

runs on gcc or clang (preferably)  (`__int128`) and a Unix for (`/dev/urandom`).

```bash
./build/rsa keygen 32 public.key private.key     # two 32-bit primes -> ~64-bit modulus
./build/rsa encrypt public.key "Hello, RSA!" > msg.enc
./build/rsa decrypt private.key msg.enc
```

format: `<byte_length> <c1> <c2> ... <cN>` (decimal numbers, one per block).

## Structure

### 1. Prime gen (`miller_rabin.c`, `prime_gen.c`)
Write `n - 1 = d · 2^s` with `d` odd. For a base `a`, `n` passes if `a^d ≡ 1` or `a^(d·2^r) ≡ -1 (mod n)` for some `0 ≤ r < s`. Using the first 12 primes (2…37) as bases is **proven deterministic for all n < 3.3·10²⁴**, so it is exact for every 64-bit value. used `/dev/urandom` with the top two bits and the lowest bit forced to 1 (full length, odd).

### 2. Keys (`euclid.c`, `rsa.c`)
1. Pick two distinct primes `p`, `q`; `n = p·q`; `φ(n) = (p-1)(q-1)`.
2. Public exponent `e = 65537`.
3. Private exponent `d = e⁻¹ mod φ`, came from **Extended Euclidean Algorithm**, returns `x, y` with `e·x + φ·y = gcd(e, φ) = 1`, so `x` is the inverse (normalised into `[1, φ)`).

### 3. Encrypt / decrypt (`rsa.c`, `codec.c`)
`c = m^e mod n` and `m = c^d mod n`, computed with square-and-multiply (`powmod`) using 128-bit intermediates so no overflow.
Text is cut into blocks of `k = ⌊(bitlen(n) − 1) / 8⌋` bytes, each read as a big-endian integer — with `< n`. The original length is stored in the ciphertext header so zero-padding of the last block is removed on decryption.

## Layout:

| File | why |
|---|---|
| `src/modmath.[ch]` | `mulmod`, `powmod` (128-bit safe) |
| `src/euclid.[ch]` | `gcd`, `ext_gcd`, `modinv` |
| `src/miller_rabin.[ch]` | `is_prime` |
| `src/prime_gen.[ch]` | CSPRNG + `random_prime(bits)` |
| `src/rsa.[ch]` | `rsa_keygen`, `rsa_encrypt`, `rsa_decrypt` |
| `src/codec.[ch]` | bytes ↔ numeric blocks |
| `src/main.c` | driver |
| `tests/` | one unit-test program per module |

## limits / next step :

- [ ] **Arbitrary-precision integers** (own bigint, or GMP) to reach real key sizes (2048+ bits). (change `mulmod`/`powmod`/`ext_gcd` types )
- [ ] **Padding** (OAEP) — textbook RSA is deterministic ; blocks `0` and `1` encrypt to themselves.
- [ ] **CRT decryption** using `p` and `q` for ~4× speed-up.
- [ ] Store `p`, `q` in the private key; add key-file validation.