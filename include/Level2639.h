#pragma once

#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

#include <vector>
#include <string>
#include <map>

#include <tiny_gltf.h>
using namespace tinygltf;


#include "2639_defs.h"
#include "Object2639.h"
#include "Level2639.h"
#include "camera.h"
#include "collision.h"


class Level2639 {
    private:
        TinyGLTF _loader;
    public:
        Level2639(std::string gltf);
        // todo: std::vector<Light2639> lights;
        Camera2639 cam;
        std::vector<Object2639> objects;

        void update();
};

