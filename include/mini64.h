#pragma once

#include <sys/types.h>

size_t
mini64_encoded_length(size_t len)
#ifdef __GNUC__
    __attribute__((const))
#endif
    ;

size_t
mini64_max_decoded_length(size_t len)
#ifdef __GNUC__
    __attribute__((const))
#endif
    ;

void
mini64_encode(const unsigned char *data, size_t len, char *dst);

int
mini64_decode(const char *string, size_t len, unsigned char *data, size_t *data_len);