#include <string.h>
#include "codec.h"
#include "test_util.h"

int main(void) {
    CHECK(codec_block_bytes((1ULL << 32) + 1) == 4);   /* 33-bit n -> 32 bits -> 4 bytes */
    CHECK(codec_block_bytes(1ULL << 31) == 3);          /* 32-bit n -> 31 bits -> 3 bytes */
    CHECK(codec_block_bytes(UINT64_MAX) == 7);
    CHECK(codec_num_blocks(10, 3) == 4);

    const char *msg = "Hello, RSA!";
    size_t len = strlen(msg);
    for (size_t k = 1; k <= 7; k++) {
        size_t nb = codec_num_blocks(len, k);
        u64 blocks[32]; unsigned char out[64] = {0};
        codec_pack((const unsigned char *)msg, len, k, blocks);
        codec_unpack(blocks, nb, k, len, out);
        CHECK(memcmp(out, msg, len) == 0);
        for (size_t b = 0; b < nb; b++) CHECK(blocks[b] < (1ULL << (8 * k)));
    }
    puts("codec OK");
    return 0;
}
