/// FlashlightManagement.h
#ifndef FLASHLIGHTMANAGEMENT_H_INCLUDED
#define FLASHLIGHTMANAGEMENT_H_INCLUDED
#include "Point.h"

/// Delegate the responsibility of the Flashlight functionality to this interface.

void InitFlashlightManagement();
int DisplayFlashlight(int x, int y);
Point * GetFlashLightPoints();
void GatherFlashlightPoints();
void ClearFlashlightPoints();

#endif // FLASHLIGHTMANAGEMENT_H_INCLUDED
