/// EnemyMoveStrategy.h
#ifndef ENEMYMOVESTRATEGY_H_INCLUDED
#define ENEMYMOVESTRATEGY_H_INCLUDED
#include "Point.h"
#include "EnemyMoveData.h"

/// C style strategy pattern to determine enemy movement. See DarkMovement.h/.c and LightMovement.h/.c.

typedef void (*Move)(EMData *pData);

#endif // MONSTERMOVESTRATEGY_C_INCLUDED
