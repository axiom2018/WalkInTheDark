/// EnemyManagement.c
#include "EnemyManagement.h"
#include "Definitions.h"
#include "EnemyMoveData.h"
#include "Point.h"
#include "LevelManagement.h"
#include "ItemManagement.h"
#include "Player.h"
#include "DrawEnemy.h"
#include "EnemyProcedure.h"
#include "Door.h"
#include "Switch.h"
#include "Werewolf.h"
#include "Banshee.h"
#include "Witch.h"
#include "UpdateEnemy.h"
#include "ArrayOperations.h"
#include "DrawEnemy.h"
#include "World.h"
#include "EnemyFactory.h"
#include "FlashlightManagement.h"
#include <stdio.h>
#include <stdlib.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    EMData *pMessageData;
    Point m_factoryPoints[COORDINATES_TO_SEND];
    void *m_pEnemies[MAX_ENEMIES];
    int m_enemyTypes[MAX_ENEMIES];
} EnemyManager;

/// Static manager pointer for re-use.
static EnemyManager *s_pEnemyManager;

/** Enemy generation system is simple. Based on the level that's the type of enemy will be assigned to that level. If it's
    level one, specifically a werewolf will be generated. Level two, witches. Etc.

    data.x = How MANY enemies we shall be generating.
    data.y = The TYPE of enemies to be generated. */
static Point AmountOfEnemiesToGenerate()
{
    /// Step 1. Declare point variable.
    Point data;

    /// Step 2. Based on the current level, decide on which enemies to generate and the quantity.
    switch(GetCurrentLevel())
    {
    case 0:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 2); /// Get 2 enemies.
        data.y = 0;
        break;
    case 1:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 2); /// Get 2 enemies.
        data.y = 1;
        break;
    case 2:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 3); /// Get 3 enemies.
        data.y = 1;
        break;
    case 3:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 2);
        data.y = 2;
        break;
    case 4:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 3);
        data.y = 2;
        break;
    case 5:
        data.x = MAX_ENEMIES - (MAX_ENEMIES - 4);
        data.y = 2;
        break;
    default:
        printf("Error! File: EnemyManagement.c. Function: AmountOfEnemiesToGenerate().\n");
        break;
    }

    /// Step 3. Return the point.
    return data;
}

/// Get proper index to update enemy factory data.
static void LocateIndex(int *m)
{
    switch(GetCurrentLevel())
    {
    case 0: /// 0 - 3
        *m = EM_LEVEL_ONE_INDEX;
        break;
    case 1: /// 4 - 7
        *m = EM_LEVEL_TWO_INDEX;
        break;
    case 2: /// 8 - 11
        *m = EM_LEVEL_THREE_INDEX;
        break;
    case 3: /// 12 - 15
        *m = EM_LEVEL_FOUR_INDEX;
        break;
    case 4: /// 16 - 19
        *m = EM_LEVEL_FIVE_INDEX;
        break;
    case 5: /// 20 - 23
        *m = EM_LEVEL_SIX_INDEX;
        break;
    default:
        printf("Error! File: EnemyManagement.c. Function: LocateIndex().\n");
        break;
    }
}

/// Finish drawing the enemy.
static int ResolveDrawingEnemy(int type, int x, int y)
{
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;

    switch(s_pEnemyManager->m_enemyTypes[type])
    {
        case 0:
            pWerewolf = s_pEnemyManager->m_pEnemies[type];
            if(DrawWerewolf(pWerewolf, x, y))
                return TRUE;
            break;
        case 1:
            pWitch = s_pEnemyManager->m_pEnemies[type];
            if(DrawWitch(pWitch, x, y))
                return TRUE;
            break;
        case 2:
            pBanshee = s_pEnemyManager->m_pEnemies[type];
            if(DrawBanshee(pBanshee, x, y))
                return TRUE;
            break;
        default:
            printf("Error! File: World.c. Function: DrawEnemies().\n");
            break;
    }

    return FALSE;
}

/** Updates current enemies to the enemy array inside of the pMessageData variable. The reason this must happen here and not inside UpdateEnemy.c
    where the other major updates are occurring is because UpdateEnemy.h/.c does not have access to the s_pEnemyManager->m_pEnemies array. Adding
    the array to the argument list for the function "ResolveEnemyUpdate()" will make it look wonky. Making a new data type from a structure, just
    for this small tedious task does not seem necessary as well. Therefore this operation is done here. */
static void AppointDataToEnemyMoveData()
{
    /// Step 2. Transfer enemy data from Manager to pMessageData.
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pEnemyManager->m_pEnemies[i] != NULL)
        {
            s_pEnemyManager->pMessageData->m_pEnemyArr[i] = s_pEnemyManager->m_pEnemies[i]; /// Assign the enemies damit!
            s_pEnemyManager->pMessageData->m_enemyTypes[i] = s_pEnemyManager->m_enemyTypes[i];
            s_pEnemyManager->pMessageData->m_maxEnemies += 1;
        }
    }

    /// Step 3. Reset the pMessageData's flashlight points to prevent values stacking.
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_pEnemyManager->pMessageData->m_flashlightPoints[i].x != ERROR_INDICATOR &&
           s_pEnemyManager->pMessageData->m_flashlightPoints[i].y != ERROR_INDICATOR)
        {
            s_pEnemyManager->pMessageData->m_flashlightPoints[i].x = ERROR_INDICATOR;
            s_pEnemyManager->pMessageData->m_flashlightPoints[i].y = ERROR_INDICATOR;
        }
    }

    /// Step 4. Assign flashlight values.
    Point *pArr = GetFlashLightPoints();
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(pArr[i].x != ERROR_INDICATOR
           && pArr[i].y != ERROR_INDICATOR)
        {
            s_pEnemyManager->pMessageData->m_flashlightPoints[i].x = pArr[i].x;
            s_pEnemyManager->pMessageData->m_flashlightPoints[i].y = pArr[i].y;
        }
    }
}

void EnemyManagementCleanMemory()
{
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pEnemyManager->m_pEnemies[i] != NULL)
        {
            free(s_pEnemyManager->m_pEnemies[i]);
            s_pEnemyManager->m_pEnemies[i] = 0;
        }
    }

    free(s_pEnemyManager->pMessageData);
    s_pEnemyManager->pMessageData = 0;
}

int DrawEnemies(int x, int y)
{
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pEnemyManager->m_pEnemies[i] != NULL)
        {
            if(ResolveDrawingEnemy(i, x, y))
                return TRUE;
        }
    }

    return FALSE;
}

void GenerateEnemies()
{
    int i;
    /// Step 1. Receive the amount of enemies to be generated based on the current level.
    Point enemiesToGenerate = AmountOfEnemiesToGenerate();

    for(i = 0; i < enemiesToGenerate.x; ++i)
    {
        /// Step 2. Find an empty position for m_pEnemies.
        int index = GetPointerArrayPos(s_pEnemyManager->m_pEnemies, MAX_ENEMIES);
        if(index == ERROR_INDICATOR)
        {
            printf("Enemy generation array full!\n");
            return;
        }

        /// Step 3. Find an empty position for m_enemyTypes.
        int enemyTypesIndex = GetIntArrayPos(s_pEnemyManager->m_enemyTypes, MAX_ENEMIES);

        /// Step 4. Save the enemy and it's type in the following arrays.
        s_pEnemyManager->m_pEnemies[index] = CreateEnemy(enemiesToGenerate.y, GetCurrentLevel());
        s_pEnemyManager->m_enemyTypes[enemyTypesIndex] = enemiesToGenerate.y;
    }
}

/// Updating strategies for enemies keeps them functioning properly.
void UpdateEnemyStrategy(EnemyUpdateProcedure updateType)
{
    /// Step 1. Before we update the enemy, we must update the data it needs to move properly.
    AppointDataToEnemyMoveData();

    /// Step 2. Begin update.
    ResolveEnemyUpdate(s_pEnemyManager->pMessageData, updateType);
}

/// Update vital data for the enemy.
void UpdateEnemyFactoryData()
{
    /// Step 1. Declare variable to be used as bookmark and set it.
    int min;
    LocateIndex(&min);

    /// Step 2. Assign item points.
    int i;
    for(i = 0; i < ITEMS_PER_LEVEL; ++i)
    {
        s_pEnemyManager->m_factoryPoints[i] = GetItemPoint(min);
        ++min;
    }

    /// Step 3. Save player position.
    s_pEnemyManager->m_factoryPoints[4] = GetPlayerPosition();

    /// Step 5. Save door position.
    s_pEnemyManager->m_factoryPoints[5] = GetDoor()->m_coord;

    /// Step 6. Pass data to EnemyFactory.
    UpdateFactoryData(s_pEnemyManager->m_factoryPoints, GetFlashLightPoints());
}

void InitEnemyManagement()
{
    /// Step 1. Allocate memory for s_pEnemyManager pointer.
    s_pEnemyManager = malloc(sizeof(EnemyManager));

    /// Step 2. Allocate memory for pMessageData.
    s_pEnemyManager->pMessageData = malloc(sizeof(EMData));

    /// Step 3. Set enemy data to default values.
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        s_pEnemyManager->m_pEnemies[i] = NULL;
        s_pEnemyManager->m_enemyTypes[i] = ERROR_INDICATOR;
        s_pEnemyManager->pMessageData->m_pEnemyArr[i] = NULL;
    }

    /// Step 4. Assign pMessageData's items array to NULL.
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        s_pEnemyManager->pMessageData->m_pItemsArr[i] = NULL;
    }

    /// Step 5. Set the factory points to ERROR_INDICATOR.
    ResetPointArray(s_pEnemyManager->m_factoryPoints, COORDINATES_TO_SEND);
}
