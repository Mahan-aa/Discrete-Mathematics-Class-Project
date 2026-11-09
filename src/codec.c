#include "codec.h"

size_t codec_block_bytes(u64 n) {
    if (n < 2) return 0;
    int bitlen = 64 - __builtin_clzll(n);
    return (size_t)(bitlen - 1) / 8;
}

size_t codec_num_blocks(size_t len, size_t k) {
    return k ? (len + k - 1) / k : 0;
}

void codec_pack(const unsigned char *data, size_t len, size_t k, u64 *blocks) {
    size_t nb = codec_num_blocks(len, k);
    for (size_t b = 0; b < nb; b++) {
        u64 v = 0;
        for (size_t i = 0; i < k; i++) {
            size_t pos = b * k + i;
            v = (v << 8) | (pos < len ? data[pos] : 0);  /* zero-pad the tail */
        }
        blocks[b] = v;
    }
}

void codec_unpack(const u64 *blocks, size_t nblocks, size_t k, size_t len, unsigned char *out) {
    for (size_t b = 0; b < nblocks; b++)
        for (size_t i = 0; i < k; i++) {
            size_t pos = b * k + i;
            if (pos < len) out[pos] = (unsigned char)(blocks[b] >> (8 * (k - 1 - i)));
        }
}
