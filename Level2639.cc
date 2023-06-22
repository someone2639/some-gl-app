#include <libdragon.h>

#include <vector>
#include <string>

#include <tiny_gltf.h>
using namespace tinygltf;

#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

#include "Level2639.h"

bool ImageNull(Image *im, const int a1, std::string *a2,
               std::string *a3, int a4, int a5,  const unsigned char *a6,
               int a7, void *user_pointer)
{
    return true;
}

Level2639::Level2639(std::string gltf) {
    std::string err, warn;
    Model model;

    // we do texturing locally, and also
    // tinygltf does not explain this function prototype at all,
    // so we will not trust it with image loading.
    this->_loader.SetImageLoader(ImageNull, nullptr);

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
    this->cam.update();

    for (Object2639 &i : this->objects) {
        // i.texturePath = "rom:/grass.sprite";
        i.update();
    }
}
