#include "types.h"
#include "swap.h"
// I HATE x86!!! I HATE x86!!!
void f32_swap_endianness(float *buf, size_t count) {
    u8 *bytes = reinterpret_cast<u8 *>(buf);

    for (size_t i = 0; i < count; i++) {
        u8 tmp[4];
        tmp[0] = bytes[(i * 4) + 3];
        tmp[1] = bytes[(i * 4) + 2];
        tmp[2] = bytes[(i * 4) + 1];
        tmp[3] = bytes[(i * 4) + 0];

        bytes[(i * 4) + 0] = tmp[0];
        bytes[(i * 4) + 1] = tmp[1];
        bytes[(i * 4) + 2] = tmp[2];
        bytes[(i * 4) + 3] = tmp[3];
    }
}

void u32_swap_endianness(u32 *buf, size_t count) {
    u8 *bytes = reinterpret_cast<u8 *>(buf);

    for (size_t i = 0; i < count; i++) {
        u8 tmp[4];
        tmp[0] = bytes[(i * 4) + 3];
        tmp[1] = bytes[(i * 4) + 2];
        tmp[2] = bytes[(i * 4) + 1];
        tmp[3] = bytes[(i * 4) + 0];

        bytes[(i * 4) + 0] = tmp[0];
        bytes[(i * 4) + 1] = tmp[1];
        bytes[(i * 4) + 2] = tmp[2];
        bytes[(i * 4) + 3] = tmp[3];
    }
}

void u16_swap_endianness(u16 *buf, size_t count) {
    u8 *bytes = reinterpret_cast<u8 *>(buf);

    for (size_t i = 0; i < count; i++) {
        u8 tmp[2];
        tmp[0] = bytes[(i * 2) + 1];
        tmp[1] = bytes[(i * 2) + 0];

        bytes[(i * 2) + 0] = tmp[0];
        bytes[(i * 2) + 1] = tmp[1];
    }
}
