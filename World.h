/// World.h
#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED
#include "Point.h"
#include "Boolean.h"
#include "Door.h"
#include <windows.h>

void WorldInit();
Door * GetDoor();
void UpdateGame();
void AdjustTextColor(int value);
void WorldCleanMemory();

#endif // WORLD_H_INCLUDED
