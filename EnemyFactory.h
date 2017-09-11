/// EnemyFactory.h
#ifndef ENEMYFACTORY_H_INCLUDED
#define ENEMYFACTORY_H_INCLUDED
#include "Point.h"
#include <stdio.h>
#include <stdlib.h>

/// Created an unorthodox factory pattern to pump out enemies.

void InitEnemyFactory();
void UpdateFactoryData(Point mainCoordinates[], Point flPoints[]);
void * CreateEnemy(int type, int levelAssignment);
void EnemyFactoryCleanMemory();

#endif // MONSTERFACTORY_H_INCLUDED
