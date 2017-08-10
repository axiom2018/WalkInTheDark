/// MonsterFactory.h
#ifndef ENEMYFACTORY_H_INCLUDED
#define ENEMYFACTORY_H_INCLUDED
#include "Point.h"
#include <stdio.h>
#include <stdlib.h>

/// C style Factory pattern".

void InitEnemyFactory();
void *EnemyCreate(int type, int levelAssignment);
void EnemyUpdateData(Point mainCoordinates[], Point flPoints[]);
void EnemyFactoryCleanMemory();

#endif // MONSTERFACTORY_H_INCLUDED
