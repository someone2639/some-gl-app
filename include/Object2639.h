#pragma once

#include "N64Graphics.h"

enum Material {
    MATERIAL_TEXTURE = 0,
    MATERIAL_4B_TEXTURE,
    MATERIAL_8B_TEXTURE,
    MATERIAL_8B_TEXTURE_REPEAT,
    MATERIAL_NONE,
    MATERIAL_COLOR,
};

typedef struct Object2639 {
    Vector move;
    Vector rotate;
    Vector scale;

    // modelList[0] has the only transform in this chain
    //  also sentinel terminated lists are cringe
    u32 segmentCount;
    gtGfx *modelList;

    enum Material matType;
    // general purpose, might just be a texture pointer and params
    union{
        u32 matParamWord;
        // struct {
        //     u8 fmt;
        //     u8 siz;
        //     u8 wd;
        //     u8 ht;
        // } matParamTexProps;
    };
    volatile void *matPtr;

    void (*init)(struct Object2639 *o);
    void (*loop)(struct Object2639 *o);
} Object2639;
