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

        Vector look;
        Vector lookTarget;
        Vector spot;
        Vector spotTarget;
        Vector RPY;
        Vector RPYTarget;

        u32 projection;
        f32 near;
        f32 far;

        // if perspective:
        f32 aspect;
        // if ortho:
        f32 xmag;
        f32 ymag;

        void updateFreeMove();
        void approach();
    public:
        Camera2639();
        Camera2639(tinygltf::Camera &c);
        Camera2639(f32 x, f32 y, f32 z) : spot(x, y, z), spotTarget(x, y, z) {};
        void update();
};
