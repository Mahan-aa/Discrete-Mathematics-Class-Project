#include "euclid.h"
#include "test_util.h"

int main(void) {
    CHECK(gcd_u64(48, 18) == 6);
    CHECK(gcd_u64(17, 0) == 17);

    i128 x, y;
    i128 g = ext_gcd(240, 46, &x, &y);
    CHECK(g == 2 && 240 * x + 46 * y == g);

    u64 inv;
    CHECK(modinv(3, 11, &inv) && inv == 4);          /* 3*4 = 12 = 1 mod 11 */
    CHECK(!modinv(6, 9, &inv));                      /* gcd = 3: no inverse */
    CHECK(modinv(65537, 3120, &inv) && mulmod(65537, inv, 3120) == 1);

    for (u64 m = 2; m < 200; m++)                    /* exhaustive small check */
        for (u64 a = 1; a < m; a++) {
            if (gcd_u64(a, m) == 1) CHECK(modinv(a, m, &inv) && (a * inv) % m == 1);
            else CHECK(!modinv(a, m, &inv));
        }
    puts("euclid OK");
    return 0;
}
