#pragma once

#include <libdragon.h>
#include "types.h"
#include "Quat.h"

class Quat {
    public:
        f32 a;
        f32 b;
        f32 c;
        f32 d;

        Quat();
        Quat(f32, f32, f32, f32);
};
