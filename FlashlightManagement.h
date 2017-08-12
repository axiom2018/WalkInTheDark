/// FlashlightManagement.h
#ifndef FLASHLIGHTMANAGEMENT_H_INCLUDED
#define FLASHLIGHTMANAGEMENT_H_INCLUDED
#include "Point.h"

/// Delegate the responsibility of the Flashlight functionality to this interface.
void PrintFlashlightPoints(); /// Test.
void InitFlashlightManagement();
int DisplayFlashlight(int x, int y);
void ClearFlashlightPoints();
void GatherFlashlightPoints();
Point * GetFlashLightPoints();

#endif // FLASHLIGHTMANAGEMENT_H_INCLUDED
