#pragma once
#include "types.h"
#include "2639_defs.h"

#include "tiny_gltf.h"

#include "Vector.h"

enum CameraModes {
    CAMERA_STATIC = 0,
    CAMERA_FREEMOVE,
    CAMERA_PLAYER,
    CAMERA_CUTSCENE,
    CAMERA_OBJECTMOVE,
};

class Camera2639 {
    private:
        u32 mode;

        Vector spot;
        Vector spotTarget;
        Vector look;
        Vector lookTarget;
        Vector RPY;
        Vector RPYTarget;

        u32 projection;
        f32 znear;
        f32 zfar;

        f32 fovy;
        // if perspective:
        f32 aspectRatio;
        // if ortho:
        f32 xmag;
        f32 ymag;

        void updateFreeMove();
        void approach();
    public:
        Camera2639() : spot(0, 0, 0),
                       spotTarget(0, 0, 0),
                       RPY(0, 0, 0),
                       RPYTarget(0, 0, 0),
                       znear(0.5f),
                       zfar(50.0f),
                       fovy(45.0f),
                       aspectRatio((float)display_get_width() / (float)display_get_height())
                           {this->mode = CAMERA_FREEMOVE;};
        Camera2639(tinygltf::Node &node, tinygltf::Camera &cam);
        Camera2639(f32 x, f32 y, f32 z) : spot(x, y, z), spotTarget(x, y, z) {
            this->mode = CAMERA_FREEMOVE;
        };
        void update();
};
