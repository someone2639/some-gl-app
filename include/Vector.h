#pragma once

#include <libdragon.h>
#include <initializer_list>
#include "types.h"
#include "Quat.h"

class Vector {
    public:
        union {f32 x; f32 roll;};
        union {f32 y; f32 pitch;};
        union {f32 z; f32 yaw;};

        Vector();
        Vector(f32, f32, f32);
        Vector(Quat &q);

        f32 operator [](int);
        void operator =(std::initializer_list<float> that);
};

