#include <libdragon.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <tiny_gltf.h>
using namespace tinygltf;

#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

#include "Level2639.h"
#include "LevelStruct.h"

bool ImageNull(Image *im, const int a1, std::string *a2,
               std::string *a3, int a4, int a5,  const unsigned char *a6,
               int a7, void *user_pointer)
{
    return true;
}

Level2639::Level2639(std::string path) {
    std::string err, warn;
    Model model;
    this->directory = path;

    std::string gltf;
    std::fstream lvl;
    lvl.open(path + std::string("title.lvl"));

    std::string music("rom:/music/");

    std::string token;

    while (lvl >> token) {
        if (token == "gltf") {
            lvl >> gltf;
        }
        if (token == "bgm") {
            lvl >> token;
            music += token;
            music += std::string("64");
        }
    }

    wav64_open(&this->bgm, music.c_str());
    wav64_set_loop(&this->bgm, true);
    wav64_play(&this->bgm, 0);

    // we do texturing locally, and also
    // tinygltf does not explain this function prototype at all,
    // so we will not trust it with image loading.
    this->_loader.SetImageLoader(ImageNull, nullptr);

    bool ret = this->_loader.LoadASCIIFromFile(&model, &err, &warn, this->directory + gltf);
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
        this->geometryObjects.emplace_back(Object2639(this->directory, model, s));
    }
}

void Level2639::update() {
    this->cam.update();

    for (Object2639 &i : this->geometryObjects) {
        i.render();
    }
}
