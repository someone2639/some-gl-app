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
#include "glb_impl.h"
#include "Object2639.h"
#include "Level2639.h"
#include "camera.h"
#include "swap.h"


class Level2639 {
    public:
        Camera2639 cam;
        std::vector<Object2639> objects;
};

