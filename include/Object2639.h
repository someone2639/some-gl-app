#pragma once
#include <GL/gl.h>
#include <GL/glu.h>

#include <initializer_list>
#include <string>
#include <vector>
#include <tuple>

#include "N64Graphics.h"

#include "Vector.h"

#include <tiny_gltf.h>

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
        u32 _initialized;
        GLuint _texture[1];
        sprite_t *_sprite;
        GLuint _displaylist;

        void initializeInternalParams();

    public:
        f32 _D;
        std::vector<std::tuple<float, float, float>> _DP;
        std::vector<std::tuple<float, float, float>> _DI;
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
        Object2639(std::string glb);
        // Object2639(std::string glb, std::string image);

        void render();
        void update();

        static void RegisterModel(std::string);
};

void UpdateObjects();
