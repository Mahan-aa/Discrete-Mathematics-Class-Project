/* codec.h - turn bytes into RSA-sized numeric blocks and back.
 *
 * RSA can only encrypt a number m < n. We therefore cut the message into
 * blocks of k bytes, read each block as a big-endian integer, and pick k so
 * that every block is guaranteed to be smaller than n. */
#ifndef CODEC_H
#define CODEC_H
#include <stddef.h>
#include "modmath.h"

/* Largest k such that any k-byte block is < n:  k = (bitlength(n) - 1) / 8. */
size_t codec_block_bytes(u64 n);

/* Number of blocks needed for len bytes (ceil(len / k)). */
size_t codec_num_blocks(size_t len, size_t k);

/* Packs data[0..len) into blocks[]; the last block may be shorter. */
void codec_pack(const unsigned char *data, size_t len, size_t k, u64 *blocks);

/* Inverse of codec_pack: writes exactly len bytes to out[]. */
void codec_unpack(const u64 *blocks, size_t nblocks, size_t k, size_t len, unsigned char *out);

#endif
