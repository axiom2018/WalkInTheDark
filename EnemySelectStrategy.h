/// EnemySelectStrategy.h
#ifndef ENEMYSELECTSTRATEGY_H_INCLUDED
#define ENEMYSELECTSTRATEGY_H_INCLUDED
#include "EnemyMoveStrategy.h"
#include "EnemyMoveData.h"

/// Before a movement strategy for an enemy is carried out, must decide which strategy to use based on several factors.

void SelectStrategy(EMData *pData, Move *pMoveStrategy);

#endif // ENEMYSELECTSTRATEGY_H_INCLUDED
