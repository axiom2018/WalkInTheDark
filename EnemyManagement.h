/// EnemyManagement.h
#ifndef ENEMYMANAGEMENT_H_INCLUDED
#define ENEMYMANAGEMENT_H_INCLUDED
#include "EnemyProcedure.h"

void InitEnemyManagement();
void UpdateEnemyFactoryData();
void GenerateEnemies();
void UpdateEnemies(EnemyUpdateProcedure updateType);
int DrawEnemies(int x, int y);
void EnemyManagementCleanMemory();

#endif // ENEMYMANAGEMENT_H_INCLUDED
