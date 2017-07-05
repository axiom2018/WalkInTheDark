/// Player.h
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Point.h"
#include "Flashlight.h"

void PlayerInit();
void PlayerGUI();
char GetPlayer();
Point GetPlayerPosition();
void ResetPlayerPositionToDefault();
void AdjustHealth(int x);
int GetPlayerHealth();
Flashlight *GetFlashLight();
void AddItemToInventory(void *pItem, int itemType);
int CheckBatteryPower();
char UpdatePlayer();
void PlayerCleanMemory();

#endif // PLAYER_H_INCLUDED
