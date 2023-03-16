#pragma once
#include <GL/gl.h>
#include <GL/glu.h>

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
    // serviceable params
    Vector move;
    Vector rotate;
    Vector scale;
    void (*init)(struct Object2639 *o);
    void (*loop)(struct Object2639 *o);
    char *texturePath;

    // generated params
    u32 segmentCount;
    gtGfx *modelList;


    // internal params
    GLuint _displaylist;
    GLuint _texture[1];
    sprite_t *_sprite;
} Object2639;



extern void Object2639_Register(Object2639 *o);
extern void Object2639_RenderList(Object2639 *o);

