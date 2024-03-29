#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "2639_defs.h"
#include "camera.h"
#include "Vector.h"

#define M_PI 3.1415926535897932384626433832795
#define M_PIF 3.1415926535897932384626433832795f

#define MV_SPD 0.6f
#define ROT_SPD (MV_SPD* 6)
#define ANG_CLMP 180.0f
#define clampA(x) if ((x) > ANG_CLMP) {(x) = ANG_CLMP;}
#define clampAN(x) if ((x) < -ANG_CLMP) {(x) = -ANG_CLMP;}
#define clampZ(x) if ((x) < 0) {(x) = 0;}

void VectorSubtract (Vector &dest, Vector &v1, Vector &v2) {
    dest.x = v1.x - v2.x;
    dest.y = v1.y - v2.y;
    dest.z = v1.z - v2.z;
}

f32 VectorNormalize (Vector *dest, Vector *src) {
    f32 vecMag = sqrtf((src->x * src->x) + (src->y * src->y) + (src->z * src->z));
    if (vecMag == 0.0f) {
        return 0.0f;
    }
    dest->x *= 1.0f / vecMag;
    dest->y *= 1.0f / vecMag;
    dest->z *= 1.0f / vecMag;
    return vecMag;
}

void VectorApproach(Vector &dest, Vector &src, f32 multiplier) {
    dest.x = dest.x + (src.x - dest.x) * multiplier;
    dest.y = dest.y + (src.y - dest.y) * multiplier;
    dest.z = dest.z + (src.z - dest.z) * multiplier;
}

void VectorAngleApproach(Vector &dest, Vector &src, f32 speed) {
    Vector subResult;
    Vector approachResult;

    VectorSubtract(subResult, dest, src);
    VectorApproach(approachResult, subResult, speed);
    VectorSubtract(dest, dest, approachResult);
}

void VectorExtend(Vector &dest, Vector &src, f32 dist, f32 pitch, f32 yaw) {
    dest.x = src.x + dist * cosf(pitch) * sinf(yaw);
    dest.y = src.y + dist * sinf(pitch);
    dest.z = src.z + dist * cosf(pitch) * cosf(yaw);
}

void Camera2639::approach(void) {
    VectorApproach(this->spot, this->spotTarget, 0.2f);
    VectorApproach(this->look, this->lookTarget, 0.2f);
    VectorApproach(this->RPY, this->RPYTarget, 0.2f);
}

void CameraPosApply(Vector &v, f32 dist, f32 yaw, f32 pitch) {
    VectorExtend(v, v, dist, 0,
        (M_PI / 180.0f) * yaw
    );
}

void Camera2639::updateFreeMove() {
    if (abs(ContReadHeld(0, x)) >= 14) {
        if (ContReadHeld(0, x) > 0) {
            CameraPosApply(this->spotTarget, -MV_SPD, this->RPY.yaw + 90, this->RPY.pitch);
        } else {
            CameraPosApply(this->spotTarget,  MV_SPD, this->RPY.yaw + 90, this->RPY.pitch);
        }
    }
    if (abs(ContReadHeld(0, y)) >= 14) {
        if (ContReadHeld(0, y) > 0) {
            CameraPosApply(this->spotTarget,  MV_SPD, this->RPY.yaw, this->RPY.pitch);
        } else {
            CameraPosApply(this->spotTarget, -MV_SPD, this->RPY.yaw, this->RPY.pitch);
        }
    }

    // #define CAM_YAW (45)
    if (ContReadHeld(0, C_right)) {
        this->RPYTarget.yaw -= ROT_SPD;
    }
    if (ContReadHeld(0, C_left)) {
        this->RPYTarget.yaw += ROT_SPD;
    }
    if (ContReadHeld(0, C_up)) {
        this->RPYTarget.pitch -= ROT_SPD;
    }
    if (ContReadHeld(0, C_down)) {
        this->RPYTarget.pitch += ROT_SPD;
    }

    if (ContReadHeld(0, A)) {
        this->spotTarget.y += MV_SPD;
    }
    if (ContReadHeld(0, B)) {
        this->spotTarget.y -= MV_SPD;
    }

    clampA(this->RPYTarget.yaw);
    clampAN(this->RPYTarget.yaw);
    clampA(this->RPYTarget.pitch);
    clampAN(this->RPYTarget.pitch);

    clampA(this->RPY.yaw);
    clampAN(this->RPY.yaw);
    clampA(this->RPY.pitch);
    clampAN(this->RPY.pitch);

    VectorExtend(this->lookTarget, this->spot, 500.0f, this->RPY.pitch * (M_PI / 180.0f),
        (M_PI / 180.0f) * (this->RPY.yaw)
    );
    approach();
}

void Camera2639::update() {
    switch (this->mode) {
        case CAMERA_STATIC: break;
        case CAMERA_FREEMOVE: this->updateFreeMove(); break;
        case CAMERA_CUTSCENE: break;

        // something else is setting the lookat matrix so we dont care
        case CAMERA_OBJECTMOVE: break;
    }
    gluLookAt(
        this->spot.x, this->spot.y, this->spot.z,
        this->look.x, this->look.y, this->look.z,
        0,1,0
    );

// TODO: modulate this with gltf camera somehow
    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 5.0f;
    float far_plane = 300.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect_ratio, near_plane, far_plane);
}

Camera2639::Camera2639(tinygltf::Node &node, tinygltf::Camera &cam) : Camera2639() {
    // Node contains pos/rotation, Camera contains Perspective/Ortho data
    if (node.translation.size() == 3) {
        this->spot.x = this->spotTarget.x = node.translation[0];
        this->spot.y = this->spotTarget.y = node.translation[1];
        this->spot.z = this->spotTarget.z = node.translation[2];
    }

    if (node.rotation.size() == 4) {
        Quat q(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);

        this->RPY = Vector(q);
        this->RPYTarget = Vector(q);
    }

    this->mode = CAMERA_STATIC;
}
