/// ItemCreation.h
#ifndef ITEMCREATION_H_INCLUDED
#define ITEMCREATION_H_INCLUDED
#include "Flashlight.h"
#include "HealthPack.h"
#include "Battery.h"
#include "Point.h"

/// Take a bit of work from ItemManager.c and delegate creating items to ItemCreation.c.

void CreateFlashlight(Flashlight *pFlashlight, Point p);
void CreateBattery(Battery *pBattery, Point p);
void CreateHealthPack(HealthPack *pHealthPack, Point p);

#endif // ITEMCREATION_H_INCLUDED
