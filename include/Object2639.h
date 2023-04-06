#pragma once
#include <GL/gl.h>
#include <GL/glu.h>

#include <initializer_list>

#include "N64Graphics.h"

#include "Vector.h"

enum Material {
    MATERIAL_TEXTURE = 0,
    MATERIAL_4B_TEXTURE,
    MATERIAL_8B_TEXTURE,
    MATERIAL_8B_TEXTURE_REPEAT,
    MATERIAL_NONE,
    MATERIAL_COLOR,
};

class Object2639 {
    private:
        // internal params
        GLuint _displaylist;
        GLuint _texture[1];
        sprite_t *_sprite;

    public:
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

        Object2639();
        Object2639(
            std::initializer_list<float> move,
            std::initializer_list<float> rotate,
            std::initializer_list<float> scale,
            u32 segmentCount,
            gtGfx *modelList
        );

        void load();
        void render();
        void renderList();
};

