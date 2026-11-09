/* main.c - command line interface for the RSA toy implementation.
 *
 *   rsa keygen <prime_bits> <pub_file> <priv_file>
 *   rsa encrypt <pub_file>  [message]     (reads stdin if no message given)
 *   rsa decrypt <priv_file> [cipher_file] (reads stdin if no file given)
 *   rsa demo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rsa.h"
#include "codec.h"

static int usage(void) {
    fputs("usage:\n"
          "  rsa keygen <prime_bits 16..32> <pub_file> <priv_file>\n"
          "  rsa encrypt <pub_file> [message]\n"
          "  rsa decrypt <priv_file> [cipher_file]\n"
          "  rsa demo\n", stderr);
    return 2;
}

static unsigned char *read_all(FILE *f, size_t *len) {
    size_t cap = 256, n = 0;
    unsigned char *buf = malloc(cap);
    if (!buf) return NULL;
    size_t got;
    while ((got = fread(buf + n, 1, cap - n, f)) > 0) {
        n += got;
        if (n == cap) {
            unsigned char *t = realloc(buf, cap *= 2);
            if (!t) { free(buf); return NULL; }
            buf = t;
        }
    }
    *len = n;
    return buf;
}

static int write_key(const char *path, const char *tag, u64 n, u64 x) {
    FILE *f = fopen(path, "w");
    if (!f) { perror(path); return 0; }
    fprintf(f, "%s %llu %llu\n", tag, (unsigned long long)n, (unsigned long long)x);
    fclose(f);
    return 1;
}

static int read_key(const char *path, const char *tag, u64 *n, u64 *x) {
    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return 0; }
    char t[32]; unsigned long long a, b;
    int ok = fscanf(f, "%31s %llu %llu", t, &a, &b) == 3 && strcmp(t, tag) == 0;
    fclose(f);
    if (!ok) { fprintf(stderr, "%s: not a valid %s key file\n", path, tag); return 0; }
    *n = a; *x = b;
    return 1;
}

/* Output format: "<byte_length> <c1> <c2> ... <cN>\n" */
static int do_encrypt(const rsa_pub *pub, const unsigned char *msg, size_t len, FILE *out) {
    size_t k = codec_block_bytes(pub->n), nb = codec_num_blocks(len, k);
    u64 *blocks = malloc((nb ? nb : 1) * sizeof *blocks);
    if (!blocks) return 0;
    codec_pack(msg, len, k, blocks);
    fprintf(out, "%zu", len);
    for (size_t i = 0; i < nb; i++)
        fprintf(out, " %llu", (unsigned long long)rsa_encrypt(blocks[i], pub));
    fputc('\n', out);
    free(blocks);
    return 1;
}

static int do_decrypt(const rsa_priv *priv, FILE *in, FILE *out) {
    size_t k = codec_block_bytes(priv->n), len;
    if (fscanf(in, "%zu", &len) != 1) { fputs("bad ciphertext header\n", stderr); return 0; }
    size_t nb = codec_num_blocks(len, k);
    u64 *blocks = malloc((nb ? nb : 1) * sizeof *blocks);
    unsigned char *plain = malloc(len + 1);
    if (!blocks || !plain) return 0;
    for (size_t i = 0; i < nb; i++) {
        unsigned long long c;
        if (fscanf(in, "%llu", &c) != 1) { fputs("truncated ciphertext\n", stderr); return 0; }
        blocks[i] = rsa_decrypt(c, priv);
    }
    codec_unpack(blocks, nb, k, len, plain);
    fwrite(plain, 1, len, out);
    free(blocks); free(plain);
    return 1;
}

static int cmd_demo(void) {
    rsa_pub pub; rsa_priv priv;
    if (!rsa_keygen(32, &pub, &priv)) return 1;
    printf("n = %llu\ne = %llu\nd = %llu\n", (unsigned long long)pub.n,
           (unsigned long long)pub.e, (unsigned long long)priv.d);
    const char *msg = "Hello, RSA!";
    printf("message   : %s\nciphertext: ", msg);
    do_encrypt(&pub, (const unsigned char *)msg, strlen(msg), stdout);
    FILE *tmp = tmpfile();
    do_encrypt(&pub, (const unsigned char *)msg, strlen(msg), tmp);
    rewind(tmp);
    printf("decrypted : ");
    do_decrypt(&priv, tmp, stdout);
    putchar('\n');
    fclose(tmp);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) return usage();
    if (!strcmp(argv[1], "demo")) return cmd_demo();

    if (!strcmp(argv[1], "keygen") && argc == 5) {
        rsa_pub pub; rsa_priv priv;
        int bits = atoi(argv[2]);
        if (!rsa_keygen(bits, &pub, &priv)) { fputs("keygen failed (bits must be 16..32)\n", stderr); return 1; }
        return write_key(argv[3], "RSA-PUBLIC", pub.n, pub.e) &&
               write_key(argv[4], "RSA-PRIVATE", priv.n, priv.d) ? 0 : 1;
    }
    if (!strcmp(argv[1], "encrypt") && (argc == 3 || argc == 4)) {
        rsa_pub pub;
        if (!read_key(argv[2], "RSA-PUBLIC", &pub.n, &pub.e)) return 1;
        size_t len; unsigned char *msg;
        if (argc == 4) { msg = (unsigned char *)argv[3]; len = strlen(argv[3]); }
        else if (!(msg = read_all(stdin, &len))) return 1;
        return do_encrypt(&pub, msg, len, stdout) ? 0 : 1;
    }
    if (!strcmp(argv[1], "decrypt") && (argc == 3 || argc == 4)) {
        rsa_priv priv;
        if (!read_key(argv[2], "RSA-PRIVATE", &priv.n, &priv.d)) return 1;
        FILE *in = argc == 4 ? fopen(argv[3], "r") : stdin;
        if (!in) { perror(argv[3]); return 1; }
        int ok = do_decrypt(&priv, in, stdout);
        putchar('\n');
        return ok ? 0 : 1;
    }
    return usage();
}
