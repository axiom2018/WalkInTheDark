/// UpdateEnemy.h
#ifndef UPDATEENEMY_H_INCLUDED
#define UPDATEENEMY_H_INCLUDED
#include "EnemyMoveData.h"
#include "EnemyProcedure.h"

/** In the old implementation of EnemyManagement.c there were 2 functions who had the responsibility of updating the enemies.
Mainly for the purpose of properly assigning the movement strategy. However, the code took up a significant portion of the
EnemyManagement.c file. Delegating the responsibility to a different file is not a bad idea. The responsibility is a bit shorter
now as well.*/

void ResolveEnemyUpdate(EMData *pData, EnemyUpdateProcedure updateType);

#endif // UPDATEENEMY_H_INCLUDED
