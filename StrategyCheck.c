/// StrategyCheck.c
#include "Boolean.h"
#include "LightMovement.h"
#include "EnemyMoveStrategy.h"

int StrategyCheck(Move *pMoveStrategy)
{
    if(*pMoveStrategy == MovementLight)
        return TRUE;

    return FALSE;
}
