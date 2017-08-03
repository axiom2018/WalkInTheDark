/// StrategyCheck.c
#include "Boolean.h"
#include "LightMovement.h"
#include "EnemyMoveStrategy.h"

/// Enemies are only allowed to be printed if they are currently working with LightMovement.
int StrategyCheck(Move *pMoveStrategy)
{
    if(*pMoveStrategy == MovementLight)
    {
        return TRUE;
    }

    return FALSE;
}
