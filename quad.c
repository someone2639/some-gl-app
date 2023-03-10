#include "2639_defs.h"
#include "Object2639.h"
float TestVtx[8][3] = {
    {-1, 0, -1},
    {-1, 0, 1} ,
    {1, 0, -1} ,
    {1, 0, 1}  ,

    {0, -1, -1},
    {0, -1, 1} ,
    {0, 1, -1} ,
    {0, 1, 1}  ,
};
gtTriN TestTris[2] = {
    {0, 1, 2}, {1, 2, 3},
};
gtTriN TestTris_[2] = {
    {0, 1, 2}, {1, 2, 3},
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
    {
        NULL,
        (gtState *)&TestState,
        TestVtx + 4,
        TestTris_,
    },
};

Object2639 Test_Obj = {
    {0, 325, 1000},
    {90, 180, 0},
    {32.0, 32.0, 32.0},


    2, // gfxlist count
    (gtGfx*)&Test_GfxList, // gfxlist name

    // MATERIAL_TEXTURE,
    // {G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32},
    0,
    {0},
    NULL,

    NULL,
    NULL,
};



