#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mini64.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define ALPHABET       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define INVALID_SEXTET 64

static char
convert_sextet(unsigned char sextet)
{
    return ALPHABET[sextet];
}

static unsigned char
deconvert_sextet(char c)
{
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    else if (c >= 'a' && c <= 'z') {
        return 26 + (c - 'a');
    }
    else if (c >= '0' && c <= '9') {
        return 52 + (c - '0');
    }
    else if (c == '+') {
        return 62;
    }
    else if (c == '/') {
        return 63;
    }
    else {
        return INVALID_SEXTET;
    }
}

size_t
mini64_encoded_length(size_t len)
{
    return ((len + 2) / 3) * 4;
}

size_t
mini64_max_decoded_length(size_t len)
{
    return (len / 4) * 3;
}

void
mini64_encode(const unsigned char *data, size_t len, char *dst)
{
    if (!data || len == 0 || !dst) {
        return;
    }

    for (size_t k = 0; k < len; k += 3) {
        unsigned int group_size = MIN(3, len - k);
        uint32_t group = 0;
        char word[5] = "====";

        memcpy((unsigned char *)&group + 1, data + k, group_size);
        group = ntohl(group);

        for (unsigned int j = 0; j <= group_size; j++) {
            unsigned char sextet = (group >> 18) & 0x3f;

            word[j] = convert_sextet(sextet);
            group <<= 6;
        }

        sprintf(dst, "%s", word);
        dst += 4;
    }
}

int
mini64_decode(const char *string, size_t len, unsigned char *dst, size_t *data_len)
{
    unsigned char *orig_dst = dst;

    if (!string || len == 0 || !dst || !data_len) {
        return -1;
    }

    if (len % 4 != 0) {
        return -1;
    }

    for (size_t k = 0; k < len; k += 4) {
        unsigned int group_size = 3;
        uint32_t group = 0;

        for (unsigned int j = 0; j < 4; j++) {
            char c = string[k + j];
            unsigned char sextet;

            group <<= 6;

            if (c == '=') {
                unsigned char check_mask;

                if (k != len - 4 || j < 2 || (j == 2 && string[k + 3] != '=')) {
                    return -1;
                }
                if (j == 2) {
                    check_mask = 0x0f;
                    group <<= 6;
                }
                else {
                    check_mask = 0x03;
                }

                if (deconvert_sextet(string[k + j - 1]) & check_mask) {
                    return -1;
                }

                group_size = j - 1;
                break;
            }
            sextet = deconvert_sextet(c);
            if (sextet == INVALID_SEXTET) {
                return -1;
            }
            group |= sextet;
        }

        group = htonl(group);
        memcpy(dst, (unsigned char *)&group + 1, group_size);
        dst += group_size;
    }

    *data_len = dst - orig_dst;
    return 0;
};
