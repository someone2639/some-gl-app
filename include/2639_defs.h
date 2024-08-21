#pragma once
#include <libdragon.h>
#include "types.h"

extern joypad_inputs_t gPressedButtons;
extern joypad_buttons_t gHeldButtons;


// #define abs(val) ((val) < 0 ? (-(val)) : (val))
#define ContRead(num, field) gHeldButtons.field
#define ContReadHeld(num, field) gPressedButtons.btn.field
#define StickRead(field) (gPressedButtons.stick_ ## field)




