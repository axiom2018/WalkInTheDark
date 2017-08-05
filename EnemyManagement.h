/// EnemyManagement.h
#ifndef ENEMYMANAGEMENT_H_INCLUDED
#define ENEMYMANAGEMENT_H_INCLUDED
#include "EnemyProcedure.h"

void InitEnemyManagement();
void GenerateEnemies();
void UpdateEnemies(EnemyUpdateProcedure updateType);

#endif // ENEMYMANAGEMENT_H_INCLUDED
