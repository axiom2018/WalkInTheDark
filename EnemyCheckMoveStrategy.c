/// EnemyCheckMoveStrategy.c
#include "EnemyCheckMoveStrategy.h"
#include "Boolean.h"
#include "LightMovement.h"

/// Enemies are only displayed in the light. If this returns true, enemy will be displayed. Referenced in DrawEnemy.c.
int StrategyCheck(Move *pMoveStrategy)
{
    if(*pMoveStrategy == MovementLight)
    {
        return TRUE;
    }

    return FALSE;
}
