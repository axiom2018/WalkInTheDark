/// SelectMovementStrategyAssistance.c
#include "SelectMovementStrategyAssistance.h"
#include "Definitions.h"
#include "DarkMovement.h"
#include "LightMovement.h"
#include "Boolean.h"
#include <stdio.h>

/// SMSA decides the strategy of movement for all enemies before movement.
void SelectMovementStrategyAssistance(EMData *pData, Move *pMoveStrategy)
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
    switch(i == SIZE_OF_FL_POINTS ? TRUE : FALSE)
    {
    case TRUE:
        *pMoveStrategy = MovementDark; /// i has cycled through the array without stopping at a matching position with the enemy coordinates. Set to dark.
        break;
    case FALSE:
        /// Do nothing.
        break;
    default:
        printf("Error! File: SelectMovementStrategyAssistance.c. SelectMovementStrategyAssistance().\n");
        break;
    }
}
