#include <libdragon.h>

#include "types.h"
#include "Vector.h"
#include "Quat.h"

Quat::Quat() {
    this->a = this->b = this->c = this->d = 0;
}

Quat::Quat(f32 a, f32 b, f32 c, f32 d) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
}
