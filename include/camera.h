#pragma once
#include "types.h"
#include "2639_defs.h"

#include "tiny_gltf.h"

#include "Vector.h"

extern Vector sCameraLook;
extern Vector sCameraSpot;
extern Vector sCameraRPY;

extern Vector sCameraLook_Target;
extern Vector sCameraSpot_Target;
extern Vector sCameraRPY_Target;

enum CameraModes {
    CAMERA_STATIC = 0,
    CAMERA_FREEMOVE,
    CAMERA_PLAYER,
    CAMERA_CUTSCENE,
    CAMERA_OBJECTMOVE,
};

extern u32 gCameraMode;

#ifdef __cplusplus
extern "C" {
#endif

void VectorCopy(Vector * dest, Vector * src);
void VectorFullApproach(Vector *dest, f32 *src, f32 multiplier);
void FloatApproach(f32 *dest, f32 *src, f32 multiplier);
extern void CameraUpdate();

#ifdef __cplusplus
};

class Camera2639 {
    private:
        Vector look;
        Vector lookTarget;
        Vector spot;
        Vector spotTarget;
        Vector RPY;
        Vector RPYTarget;

        void updateFreeMove();

        void approach();
    public:
        Camera2639();
        Camera2639(tinygltf::Camera &c);
        Camera2639(f32 x, f32 y, f32 z) : look(x, y, z), lookTarget(x, y, z) {};
        void update();
};

#endif
