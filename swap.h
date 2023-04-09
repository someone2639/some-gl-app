#pragma once
#include <libdragon.h>
#include "types.h"
#include "2639_defs.h"
// I HATE x86!!! I HATE x86!!!
extern void f32_swap_endianness(float *buf, size_t count);
extern void u32_swap_endianness(u32 *buf, size_t count);
extern void u16_swap_endianness(u16 *buf, size_t count);
