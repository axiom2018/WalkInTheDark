/// EnemyFactory.c
#include "EnemyFactory.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "Point.h"
#include "Boolean.h"
#include "LevelInfo.h"
#include "DarkMovement.h"
#include "Definitions.h"
#include "Switch.h"
#include "EnemyCreation.h"
#include "ArrayOperations.h"

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    int m_initEnemyFactoryTesting;
    Point m_mainCoordinates[COORDINATES_TO_SEND];
    Point m_flPoints[SIZE_OF_FL_POINTS];
    LevelInfo m_enemySpecifics[MAX_ENEMIES];
    int m_levelAssignment;
} EnemyFactory;

/// Static EnemyFactory pointer for re-use.
static EnemyFactory *s_pEnemyFactory;
/// Enemy tests function pointer.
typedef int (*EnemyFactoryTests)(Point *pPos);
/// Defined array for enemy tests.
EnemyFactoryTests EFTests[MAX_ENEMY_FACTORY_TESTS];

/// Check if initialized before use.
static int IsEnemyFactoryInitialized()
{
    if(UseSwitch(s_pEnemyFactory->m_initEnemyFactoryTesting, TRUE))
        return TRUE;

    return FALSE;
}

/// Referenced in EnemyManagement.c.
void UpdateFactoryData(Point mainCoordinates[], Point flPoints[])
{
    /// Step 1. Assign values to mainCoordinates.
    int r;
    for(r = 0; r < COORDINATES_TO_SEND; ++r)
    {
        s_pEnemyFactory->m_mainCoordinates[r] = mainCoordinates[r];
    }

    /// Step 2. Assign values to flPoints.
    for(r = 0; r < SIZE_OF_FL_POINTS; ++r)
    {
        if(flPoints[r].x > ERROR_INDICATOR
           && flPoints[r].y > ERROR_INDICATOR)
        {
            s_pEnemyFactory->m_flPoints[r].x = flPoints[r].x;
            s_pEnemyFactory->m_flPoints[r].y = flPoints[r].y;
        }
    }
}

/// Grab empty index of the array.
static int GetEnemySpecificsPos()
{
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(UseSwitch(s_pEnemyFactory->m_enemySpecifics[i].m_assignedLevel, ERROR_INDICATOR))
            return i;
    }

    return ERROR_INDICATOR;
}

/// In case position enemy desires is taken, toss out a different one.
static void AdjustDesiredPosition(Point *pPosToAdjust)
{
    pPosToAdjust->x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
    pPosToAdjust->y = rand() % ((ROWS - 1) + 1 - 0) + 0;
}

/// Check for player position, door, etc.
static int MainCoordinateTest(Point *pPos)
{
    int i;
    for(i = 0; i < COORDINATES_TO_SEND; ++i)
    {
        if(s_pEnemyFactory->m_mainCoordinates[i].x == pPos->x
           && s_pEnemyFactory->m_mainCoordinates[i].y == pPos->y)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/// The enemy movement will adapt depending on if the enemy's position matches with a flashlight coordinate or not.
static int FlashlightPointTest(Point *pPos)
{
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_pEnemyFactory->m_flPoints[i].y == pPos->x
           && s_pEnemyFactory->m_flPoints[i].x == pPos->y)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/// Check against other enemies.
static int EnemyCoordinateTest(Point *pPos)
{
    /// Step 1. Check for at least 1 other enemy. If not return False. No need to continue because we have nothing to compare too.
    if(UseSwitch(s_pEnemyFactory->m_enemySpecifics[0].m_assignedLevel, ERROR_INDICATOR))
        return FALSE;

    /// Step 2. Begin checking for duplicates.
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pEnemyFactory->m_enemySpecifics[i].m_assignedLevel == s_pEnemyFactory->m_levelAssignment
           && s_pEnemyFactory->m_enemySpecifics[i].m_pos.x == pPos->x
           && s_pEnemyFactory->m_enemySpecifics[i].m_pos.y == pPos->y)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static void InitializeEnemyFactoryTestsArray()
{
    /// Step 1. Ready each of the tests. Each one helps decide on the enemy position.
    EFTests[0] = MainCoordinateTest;
    EFTests[1] = FlashlightPointTest;
    EFTests[2] = EnemyCoordinateTest;

    /// Step 2. Set initialization to true.
    s_pEnemyFactory->m_initEnemyFactoryTesting = TRUE;
}

void *CreateEnemy(int type, int levelAssignment)
{
    /// Step 1. Defensive programming. Be sure the tests are initialized.
    if(!IsEnemyFactoryInitialized())
    {
        InitializeEnemyFactoryTestsArray();
    }

    /// Step 2. Set up a position for the enemy.
    Point pos;
    pos.x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
    pos.y = rand() % ((ROWS - 1) + 1 - 0) + 0;

    /// Step 3. Begin testing desired position.
    int i;
    for(i = 0; i < MAX_ENEMY_FACTORY_TESTS; ++i)
    {
        if(EFTests[i](&pos))
        {
            /// (Optional Step) If any of the tests return true, that means we need to change the position and re-run the tests by setting i to -1.
            AdjustDesiredPosition(&pos);
            i = ERROR_INDICATOR;
        }
    }

    /// Step 4. Request enemy from EnemyCreation.c.
    void *requestedEnemy = RequestToCreateEnemy(type, levelAssignment, pos);

    /// Step 5. Save enemy details for future duplicate checks.
    int vacancy = GetEnemySpecificsPos();
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_assignedLevel = levelAssignment;
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_pos.x = pos.x;
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_pos.y = pos.y;

    /// Step 6. Return the newly created enemy.
    return requestedEnemy;
}

void InitEnemyFactory()
{
    /// Step 1. Allocate space.
    s_pEnemyFactory = malloc(sizeof(EnemyFactory));

    /// Step 2. Set up testing functions.
    InitializeEnemyFactoryTestsArray();

    /// Step 3. Initialize enemy specifics.
    SetLevelInfoArray(s_pEnemyFactory->m_enemySpecifics, MAX_ENEMIES);
}

void EnemyFactoryCleanMemory()
{
    /// Free memory.
    free(s_pEnemyFactory);
    s_pEnemyFactory = 0;
}
