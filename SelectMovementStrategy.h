/// SelectMovementStrategy.h
#ifndef SELECTMOVEMENTSTRATEGY_H_INCLUDED
#define SELECTMOVEMENTSTRATEGY_H_INCLUDED
#include "EnemyMoveStrategy.h"
#include "EnemyMoveData.h"

/// C style Strategy pattern.

typedef void (*SelectMovementStrategy)(EMData *pData, Move *pMoveStrategy);

#endif // SELECTMOVEMENTSTRATEGY_H_INCLUDED
