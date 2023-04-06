#include <libdragon.h>
#include <initializer_list>


#include "Vector.h"

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

void Vector::operator =(Vector &that) {
    this->x = that.x;
    this->y = that.y;
    this->z = that.z;
}

void Vector::operator =(std::initializer_list<float> that) {
    float *vec = (float *) that.begin();

    this->x = vec[0];
    this->y = vec[1];
    this->z = vec[2];
}


