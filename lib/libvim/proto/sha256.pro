/* sha256.c */
void sha256_start(context_sha256_T *ctx);
void sha256_update(context_sha256_T *ctx, char_u *input, UINT32_T length);
void sha256_finish(context_sha256_T *ctx, char_u digest[32]);
/* vim: set ft=c : */
