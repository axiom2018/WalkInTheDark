/// Player.h
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include "Point.h"
#include "Flashlight.h"

/// Player management interface.

void InitPlayer();
char GetPlayer();
Point GetPlayerPosition();
int GetPlayerHealth();
Flashlight *GetFlashLight();
void PlayerGUI();
void ResetPlayerPositionToDefault();
void AdjustHealth(int x);
void AddItemToInventory(void *pItem, int itemType);
char UpdatePlayer();
void AddHealth(int amount);
int CheckBatteryPower();
void PlayerCleanMemory();

#endif // PLAYER_H_INCLUDED
