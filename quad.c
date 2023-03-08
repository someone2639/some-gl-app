#include "2639_defs.h"
#include "Object2639.h"
Vtx TestVtx[4] = {
    {{{-1, 0, -1}                  ,0, {595, 8},{0x0, 0x81, 0x0, 0xFF}}},
    {{{-1, 0, 1}                   ,0, {502, 8},{0x0, 0x81, 0x0, 0xFF}}},
    {{{1, 0, -1}                   ,0, {490, 8},{0x0, 0x81, 0x0, 0xFF}}},
    {{{1, 0, 1}                    ,0, {301, 9},{0x0, 0x81, 0x0, 0xFF}}},
};
gtTriN TestTris[2] = {
    {0, 1, 2, 0}, {1, 2, 3, 0},
};


gtStateL TestState = {
    {
        .triCount = 2,
        .vtxCount = 4,
    }
};


gtGfx Test_GfxList[] = {
    {
        NULL,
        (gtState *)&TestState,
        TestVtx,
        TestTris,
    },
};

Object2639 Test_Obj = {
    {0, 325, 1000},
    {90, 180, 0},
    {32.0, 32.0, 32.0},


    1, // gfxlist count
    &Test_GfxList, // gfxlist name

    // MATERIAL_TEXTURE,
    // {G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32},
    0,
    {0, 0, 0, 0},
    NULL,

    NULL,
    NULL,
};



