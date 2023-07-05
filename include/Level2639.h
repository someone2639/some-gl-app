#pragma once
#include <libdragon.h>

#include <vector>
#include <string>

#include <tiny_gltf.h>
using namespace tinygltf;

#include "Object2639.h"
#include "camera.h"
#include "collision.h"


class Level2639 {
    private:
        std::string directory;

        TinyGLTF _loader;
        wav64_t bgm;

    public:

        GLubyte bgcolor[3];

        Level2639(std::string gltf);
        // todo: Collision map
        // todo: std::vector<Light2639> lights;
        Camera2639 cam;
        std::vector<Object2639> objects;

        void update();
};

