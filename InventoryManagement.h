/// InventoryManagement.h
#ifndef INVENTORYMANAGEMENT_H_INCLUDED
#define INVENTORYMANAGEMENT_H_INCLUDED
#include "Flashlight.h"

/// Interface to handle the players inventory system.

void InitInventoryManagement();
void AddToInventory(void *pItem, int itemType);
int GetFlashlightRange();
int CheckBattery();
void ShowInventory();
int CheckInventoryPosition(int index);
void UseItem(int type, int index);
int GetItemType(int index);
Flashlight * ReturnFlashLight();
int GetItemCount();
void DecreaseStepsBeforeRecharge();
void InventoryManagementCleanMemory();

#endif // INVENTORYMANAGEMENT_H_INCLUDED
