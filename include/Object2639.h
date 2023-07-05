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
using namespace tinygltf;

#define O_TEXCOUNT 32

class Object2639 {
    private:
        // internal params
        u32 _initialized;

        // TODO: more texture control
        GLuint _texture[O_TEXCOUNT];
        rdpq_texparms_t _texParams[O_TEXCOUNT];
        bool hasTexture[O_TEXCOUNT];
        u32 _texIndex;
        sprite_t *_sprite;
        GLuint _displaylist;

        void initializeInternalParams();

        // helper functions for dl loading
        void loadVertexPositions(Model &model, Primitive &prim);
        void loadNormals(Model &model, Primitive &prim);
        void loadColors(Model &model, Primitive &prim);
        void loadUVs(Model &model, Primitive &prim);
        void loadJoints(Model &model, Primitive &prim);
        void drawTris(Model &model, Primitive &prim);

    public:
        std::string name;

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
        Object2639(std::string directory, Model &m, Scene &s);
        // Object2639(std::string glb, std::string image);

        // void processGLTF(std::vector<Mesh> meshes);

        void render();
        void update();
};
