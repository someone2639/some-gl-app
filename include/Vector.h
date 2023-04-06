#pragma once

#include <libdragon.h>
#include <initializer_list>
#include "types.h"

class Vector {
    public:
        union {f32 x; f32 roll;};
        union {f32 y; f32 pitch;};
        union {f32 z; f32 yaw;};

        Vector();
        Vector(f32, f32, f32);

        void operator =(Vector &that);
        void operator =(std::initializer_list<float> that);
};