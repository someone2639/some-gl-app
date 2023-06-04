#pragma once
#include <GL/gl.h>
#include <GL/glu.h>

#include "2639_defs.h"

#include "Vector.h"

#include <vector>


class Surface {
    public:
        Vector v0;
        Vector v1;
        Vector v2;

        Vector normal;


        // probably just going to use sm64 format for this
        Surface(u8 *binary);
};

extern std::vector<Surface> surfacePool;
