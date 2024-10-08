#include <libdragon.h>
#include <initializer_list>


#include "Vector.h"
#include "Quat.h"

Vector::Vector() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector::Vector(f32 x, f32 y, f32 z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

f32 Vector::operator [](int idx) {
    switch (idx % 3) {
        case 0: return this->x; break;
        case 1: return this->y; break;
        case 2: return this->z; break;
    }

    return -1.0f; // impossible?
}

void Vector::operator =(std::initializer_list<float> that) {
    float *vec = (float *) that.begin();

    this->x = vec[0];
    this->y = vec[1];
    this->z = vec[2];
}

// todo: verify
Vector::Vector(Quat& q) {
    this->x = 2.0f * (q.b * q.d - q.a * q.c);
    this->y = 2.0f * (q.c * q.d + q.a * q.b);
    this->z = q.a * q.a - q.b * q.b - q.c * q.c + q.d * q.d;
}
