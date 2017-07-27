/// World.h
#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED
#include "Point.h"
#include "Boolean.h"
#include "Door.h"
#include <windows.h>

void WorldInit();
Door * GetDoor();
void AdjustTextColor(int value);
void UpdateGame();
void WorldCleanMemory();

#endif // WORLD_H_INCLUDED
