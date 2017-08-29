/// EnemyManagement.h
#ifndef ENEMYMANAGEMENT_H_INCLUDED
#define ENEMYMANAGEMENT_H_INCLUDED
#include "EnemyProcedure.h"

/// Interface for managing enemies.

void InitEnemyManagement();
void UpdateEnemyFactoryData();
void UpdateEnemyStrategy(EnemyUpdateProcedure updateType);
void GenerateEnemies();
int DrawEnemies(int x, int y);
void EnemyManagementCleanMemory();

#endif // ENEMYMANAGEMENT_H_INCLUDED
