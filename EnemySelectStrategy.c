/// EnemySelectStrategy.c
#include "EnemySelectStrategy.h"
#include "Definitions.h"
#include "DarkMovement.h"
#include "LightMovement.h"
#include "Boolean.h"
#include "Switch.h"
#include <stdio.h>

void SelectStrategy(EMData *pData, Move *pMoveStrategy)
{
    /// Step 1. Go through points and determine if the enemy's position matches. If so, assign light movement.
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(pData->m_flashlightPoints[i].x > ERROR_INDICATOR &&
           pData->m_flashlightPoints[i].y > ERROR_INDICATOR)
        {
            if(pData->m_flashlightPoints[i].y == pData->m_pEnemyPos->x
                && pData->m_flashlightPoints[i].x == pData->m_pEnemyPos->y)
            {
                *pMoveStrategy = MovementLight;
                break;
            }
        }
    }

    /// Step 2. Did we reach the end without assigning light movement? If so, assign dark movement.
    if(UseSwitch(i, SIZE_OF_FL_POINTS))
        *pMoveStrategy = MovementDark;
}
