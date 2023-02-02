#include <math.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chacha.h"

void hex_dump(char *prefix, unsigned char *str, int num)
{
    fprintf(stderr, "%s", prefix);
    for(int i=0; i<num; i++)
        fprintf(stderr, "%02x", str[i]);
    fprintf(stderr, "\n");
}

/*
#if defined(__linux__) || defined(__CYGWIN__)
#include <endian.h>

#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define htole32(x) OSSwapHostToLittleInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#elif defined(__OpenBSD__)
#include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
#include <sys/endian.h>
#define le32toh(x) letoh32(x)
#define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)
#include <sys/param.h>
#include <winsock2.h>
*/
#if BYTE_ORDER == LITTLE_ENDIAN
#define htole32(x) (x)
#define le32toh(x) (x)

#define htole64(x) (x)
#define le64toh(x) (x)

#elif BYTE_ORDER == BIG_ENDIAN
#define htole32(x) __builtin_bswap32(x)
#define le32toh(x) __builtin_bswap32(x)

#define htole64(x) __builtin_bswap64(x)
#define le64toh(x) __builtin_bswap64(x)

#else
#error byte order not supported
#endif /* endif byteorder */

//#else
//#error platform not supported
//#endif /* endif platform */

static const uint8_t testkey[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
    0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

//const uint8_t one[8] = {1, 0, 0, 0, 0, 0, 0, 0}; /* NB little-endian */

int chacha_test(void)
{
    struct chacha_ctx ctx;

    uint64_t chacha_iv = 0;
    uint64_t chacha_iv_le = htole64(chacha_iv);

    uint64_t ccc = 0;
    uint64_t counter = ccc;
    uint64_t counter_le = htole64(counter);

    chacha_keysetup(&ctx, testkey, 256);

    uint8_t plaintext[20] = {0};
    uint8_t mactext[16] = {0};
    uint8_t cryptotext[20] = {0};

    counter = ccc;
    counter_le = htole64(counter);


    chacha_ivsetup(&ctx, (uint8_t*)&chacha_iv_le, (uint8_t*)&counter_le);
    chacha_encrypt_bytes(&ctx, plaintext, mactext, 16);
    hex_dump("", mactext, 4);

    plaintext[16] = mactext[0];
    plaintext[17] = mactext[1];
    plaintext[18] = mactext[2];
    plaintext[19] = mactext[3];

    counter++;
    counter_le = htole64(counter);

    chacha_ivsetup(&ctx, (uint8_t*)&chacha_iv_le, (uint8_t*)&counter_le);
    chacha_encrypt_bytes(&ctx, plaintext, cryptotext, 20);
    hex_dump("", cryptotext, 20);




    chacha_ivsetup(&ctx, (uint8_t*)&chacha_iv_le, (uint8_t*)&counter_le);
    chacha_encrypt_bytes(&ctx, cryptotext, plaintext, 20);
    hex_dump("", plaintext, 20);

    counter--;
    counter_le = htole64(counter);

    chacha_ivsetup(&ctx, (uint8_t*)&chacha_iv_le, (uint8_t*)&counter_le);
    chacha_encrypt_bytes(&ctx, plaintext, cryptotext, 20);
    hex_dump("", cryptotext, 20);


    return 0;
}

#ifdef CHACHA_TEST_MAIN
int main(void) {
    chacha_test();
    return 0;
}
#endif
