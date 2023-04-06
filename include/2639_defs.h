#pragma once
#include <libdragon.h>
#include "types.h"

extern struct controller_data gPressedButtons;
extern struct controller_data gHeldButtons;


#define abs(val) ((val) < 0 ? (-(val)) : (val))
#define ContRead(num, field) gPressedButtons.c[num].field
#define ContReadHeld(num, field) gPressedButtons.c[num].field




