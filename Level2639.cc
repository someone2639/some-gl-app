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

bool ImageNull(Image *im, const int a1, std::string *a2,
               std::string *a3, int a4, int a5,  const unsigned char *a6,
               int a7, void *user_pointer)
{
    return true;
}

Level2639::Level2639(std::string gltf) {
    this->_loader.SetImageLoader(ImageNull, nullptr);
    std::string err, warn;
    Model model;

    bool ret = this->_loader.LoadASCIIFromFile(&model, &err, &warn, gltf);
    assertf(err.empty(), err.c_str());
    assertf(warn.empty(), warn.c_str());
    assert(ret);

// make camera
    if (model.cameras.size() > 0) {
        this->cam = Camera2639(model.cameras[0]);
    } else {
        this->cam = Camera2639(0, 0, 0);
    }

// make an object per scene
    for (Scene &s : model.scenes) {
        this->objects.emplace_back(Object2639(model, s));
    }
}

void Level2639::update() {
    for (Object2639 &i : this->objects) {
        // i.texturePath = "rom:/grass.sprite";
        i.update();
    }
}
