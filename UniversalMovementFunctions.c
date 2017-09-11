/// UniversalMovementFunctions.c
#include "UniversalMovementFunctions.h"
#include "Definitions.h"
#include "Boolean.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "Flashlight.h"
#include "Battery.h"
#include "HealthPack.h"
#include <windows.h>
#include <stdio.h>

/// Set up typedef for functions.
typedef int (*Tests)(EMData *pData, Point *pDesiredEnemyPos);
/// Array for functions.
Tests CoordinateTests[MAX_TESTS];
/// Initialization boolean.
static int s_initCoordTestsArr = FALSE;

/// Return true if indeed out of bounds or return false if not.
static int CheckEnemyBounds(EMData *pData, Point *pDesiredEnemyPos)
{
    /// Step 1. Check West.
    if(pDesiredEnemyPos->x < 0)
        return TRUE;

    /// Step 2. Check East.
    else if(pDesiredEnemyPos->x >= COLUMNS)
        return TRUE;

    /// Step 3. Check North.
    else if(pDesiredEnemyPos->y < 0)
        return TRUE;

    /// Step 4. Check South.
    else if(pDesiredEnemyPos->y >= ROWS)
        return TRUE;

    return FALSE;
}

static int CheckForOtherEnemies(EMData *pData, Point *pDesiredEnemyPos)
{
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;
    Point enemyPos;
    int saveMonsterAssignedLevel;

    /// Step 1. Begin by looping through the enemies.
    int i;
    for(i = 0; i < pData->m_maxEnemies; ++i)
    {
        if(pData->m_pEnemyArr[i] != NULL)
        {
            /// Step 2. Get the proper type of enemy.
            switch(pData->m_enemyTypes[i])
            {
            case 0:
                pWerewolf = pData->m_pEnemyArr[i];
                saveMonsterAssignedLevel = pWerewolf->m_info.m_assignedLevel;
                enemyPos = pWerewolf->m_info.m_Pos;
                break;
            case 1:
                pWitch = pData->m_pEnemyArr[i];
                saveMonsterAssignedLevel = pWitch->m_info.m_assignedLevel;
                enemyPos = pWitch->m_info.m_Pos;
                break;
            case 2:
                pBanshee = pData->m_pEnemyArr[i];
                saveMonsterAssignedLevel = pBanshee->m_info.m_assignedLevel;
                enemyPos = pBanshee->m_info.m_Pos;
                break;
            default:
                printf("Error! File: UniversalMovementFunctions.c Function: CheckForOtherEnemies(MMData *pData).\n");
                break;
            }
        }

        /// Step 3. Check if the enemy is assigned to the current level, if not, continue.
        if(saveMonsterAssignedLevel == pData->m_saveCurrentLevel)
        {
            /// Step 4. Compare the position where an enemy wants to go with other enemy positions.
            if(enemyPos.x == pDesiredEnemyPos->x
                && enemyPos.y == pDesiredEnemyPos->y)
            {
                /// The position we want our current enemy to move too is occupied by another enemy, do not continue.
                return TRUE;
            }
        }
    }
    return FALSE;
}

static int CheckForItemPositions(EMData *pData, Point *pDesiredEnemyPos)
{
    Flashlight *pFlashlight;
    Battery *pBattery;
    HealthPack *pHealthPack;
    int saveItemsAssignedLevel;
    Point itemPoint;

    /// Step 1. Begin the loop through all items.
    int i;
    for(i = 0; i < pData->m_maxItems; ++i)
    {
        /// Step 2. Must check if NULL.
        if(pData->m_pItemsArr[i] != NULL)
        {
            /// Step 3. If not NULL, check what type of item.
            switch(pData->m_itemTypes[i])
            {
            case 0:
                pFlashlight = pData->m_pItemsArr[i];
                saveItemsAssignedLevel = pFlashlight->m_item.m_assignedLevel;
                itemPoint = pFlashlight->m_item.m_pos;
                break;
            case 1:
                pBattery = pData->m_pItemsArr[i];
                saveItemsAssignedLevel = pBattery->m_item.m_assignedLevel;
                itemPoint = pBattery->m_item.m_pos;
                break;
            case 2:
                pHealthPack = pData->m_pItemsArr[i];
                saveItemsAssignedLevel = pHealthPack->m_item.m_assignedLevel;
                itemPoint = pHealthPack->m_item.m_pos;
                break;
            default:
                printf("Type: %d\n", pData->m_itemTypes[i]);
                printf("Error! File: UniversalMovementFunctions.c Function: CheckForItemPositions().\n");
                break;
            }

            /// Step 4. Check if the item is assigned to the current level, if not, continue.
            if(saveItemsAssignedLevel == pData->m_saveCurrentLevel)
            {
                /// Step 5. Check to see if the position we desire, is taken by an item.
                if(itemPoint.x == pDesiredEnemyPos->x
                   && itemPoint.y == pDesiredEnemyPos->y)
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static int CheckForDoor(EMData *pData, Point *pDesiredEnemyPos)
{
    if(pData->m_pDoorPos->x == pDesiredEnemyPos->x
       && pData->m_pDoorPos->y == pDesiredEnemyPos->y)
    {
        return TRUE;
    }

    return FALSE;
}

static void InitializeCoordinateTestsArray()
{
    /// Step 1. Assign each index of the array individually.
    CoordinateTests[0] = CheckEnemyBounds;
    CoordinateTests[1] = CheckForOtherEnemies;
    CoordinateTests[2] = CheckForItemPositions;
    CoordinateTests[3] = CheckForDoor;

    /// Step 2. Be sure to set s_initTestsArr to true, so when another enemy has to go through these tests, no need to reinitialize.
    s_initCoordTestsArr = TRUE;
}

int PlayerCollision(Point playerPos, Point enemyPos)
{
    /// Obviously if this is true, enemy should do damage to player. Therefore I should put Player Health in pData.
    if(playerPos.y == enemyPos.x
       && playerPos.x == enemyPos.y)
    {
        return TRUE;
    }

    return FALSE;
}

/// Enemies can move only if their delay has reached 0. This determines speed.
int RunMovementDelayTest(EMData *pData)
{
    switch(*pData->m_pEnemyMovementDelay == 0 ? TRUE : FALSE)
    {
    case TRUE:
        *pData->m_pEnemyMovementDelay = pData->m_enemyDefaultMovementDelay;
        return TRUE;
    case FALSE:
        *pData->m_pEnemyMovementDelay -= 1;
        break;
    default:
        break;
    }

    return FALSE;
}

/// Begin proper tests for position.
int RunPositionTests(EMData *pData, Point *pDesiredEnemyPos)
{
    /// Step 1. Initialize the array of function pointers and assign each index to a different test.
    if(!s_initCoordTestsArr)
    {
        InitializeCoordinateTestsArray();
    }

    /// Step 2. Run through tests.
    int i;
    for(i = 0; i < MAX_TESTS; ++i)
    {
        if(CoordinateTests[i](pData, pDesiredEnemyPos))
        {
            /// If ANY of the tests return the desired position for the enemy cannot be given.
            return TRUE;
        }
    }

    return FALSE;
}
