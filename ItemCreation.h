/// ItemCreation.h
#ifndef ITEMCREATION_H_INCLUDED
#define ITEMCREATION_H_INCLUDED
#include "Flashlight.h"
#include "HealthPack.h"
#include "Battery.h"
#include "Point.h"

/// Delegate creating items to ItemCreation.c using this interface.

void CreateFlashlight(Flashlight *pFlashlight, Point p);
void CreateBattery(Battery *pBattery, Point p);
void CreateHealthPack(HealthPack *pHealthPack, Point p);

#endif // ITEMCREATION_H_INCLUDED
