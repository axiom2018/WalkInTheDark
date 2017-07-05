/// EnemyFactory.c
#include "EnemyFactory.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "Point.h"
#include "Boolean.h"
#include "MediatorPattern.h"
#include "LevelInfo.h"
#include "DarkMovement.h"
#include "SelectMovementStrategyAssistance.h"
#include "StrategyCheck.h"
#include "Definitions.h"

/// This is a "poor mans" factory pattern I created. Not as good as the C++ version which obviously is capable of OOP but this one gets the job done.
typedef struct
{
    int m_initEnemyFactoryTesting;
    Point m_mainCoordinates[COORDINATES_TO_SEND];
    Point m_flPoints[SIZE_OF_FL_POINTS];
    LevelInfo m_enemySpecifics[MAX_ENEMIES];
    int m_levelAssignment;
    void (*EFReceiveData)(Point mainCoordinates[], Point flPoints[]);
    void *(*EFCreateEnemy)(int type, int levelAssignment);
} EnemyFactory;

static EnemyFactory *s_pEnemyFactory;
typedef int (*EnemyFactoryTests)(Point *pPos);
EnemyFactoryTests EFTests[MAX_ENEMY_FACTORY_TESTS];

static int IsEnemyFactoryInitialized()
{
    if(s_pEnemyFactory->m_initEnemyFactoryTesting)
    {
        return TRUE;
    }
    return FALSE;
}

static void EnemyFactoryReceiveData(Point mainCoordinates[], Point flPoints[])
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

static int GetEnemySpecificsPos()
{
    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pEnemyFactory->m_enemySpecifics[i].m_assignedLevel == ERROR_INDICATOR)
        {
            return i;
        }
    }
    return ERROR_INDICATOR;
}

static void AdjustDesiredPosition(Point *pPosToAdjust)
{
    pPosToAdjust->x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
    pPosToAdjust->y = rand() % ((ROWS - 1) + 1 - 0) + 0;
}

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

static int EnemyCoordinateTest(Point *pPos)
{
    /// Step 1. Check for at least 1 other enemy. If we do not have one, return False. No need to continue because we have nothing to compare too.
    switch(s_pEnemyFactory->m_enemySpecifics[0].m_assignedLevel == ERROR_INDICATOR ? TRUE : FALSE)
    {
    case 1:
        return FALSE;
        break;
    case 0:
        break;
    default:
        printf("Error! File: EnemyFactory.c. Function: EnemyCoordinateTest(Point *pPos).\n");
        break;
    }

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
    EFTests[0] = MainCoordinateTest;
    EFTests[1] = FlashlightPointTest;
    EFTests[2] = EnemyCoordinateTest;

    s_pEnemyFactory->m_initEnemyFactoryTesting = TRUE;
}

static void *CreateEnemy(int type, int levelAssignment)
{
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;
    void *pEnemy;

    /// Step 1. Defensive programming. Arrays were already initialized in EnemyFactoryInit().
    if(!IsEnemyFactoryInitialized())
    {
        InitializeEnemyFactoryTestsArray();
    }

    /// Step 2. Set up a position for the enemy.
    Point pos;
    pos.x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
    pos.y = rand() % ((ROWS - 1) + 1 - 0) + 0;

    /// Step 3. Save the assigned level for future use in the last checking function.
    s_pEnemyFactory->m_levelAssignment = levelAssignment;

    /// Step 4. Begin testing desired position.
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

    /// Step 5. Begin creating enemy.
    switch(type)
    {
    case 0:
        pWerewolf = malloc(sizeof(Werewolf));
        pWerewolf->m_info.m_assignedLevel = levelAssignment;
        pWerewolf->m_info.m_damage = rand() % (15 + 1 - 10) + 10;
        pWerewolf->m_info.m_Pos.x = pos.x;
        pWerewolf->m_info.m_Pos.y = pos.y;
        pWerewolf->m_info.m_symbol = '@';
        pWerewolf->m_info.m_moveStrategy = MovementDark;
        pWerewolf->m_info.m_selectMovementStrategy = SelectMovementStrategyAssistance;
        pWerewolf->m_info.m_strategyCheck = StrategyCheck;
        pWerewolf->m_info.m_defaultMovementDelay = 3;
        pWerewolf->m_info.m_movementDelay = 3;
        pEnemy = pWerewolf;
        break;
    case 1:
        pWitch = malloc(sizeof(Witch));
        pWitch->m_info.m_assignedLevel = levelAssignment;
        pWitch->m_info.m_damage = rand() % (25 + 1 - 20) + 20;
        pWitch->m_info.m_Pos.x = pos.x;
        pWitch->m_info.m_Pos.y = pos.y;
        pWitch->m_info.m_symbol = 237;
        pWitch->m_info.m_moveStrategy = MovementDark;
        pWitch->m_info.m_selectMovementStrategy = SelectMovementStrategyAssistance;
        pWitch->m_info.m_strategyCheck = StrategyCheck;
        pWitch->m_info.m_defaultMovementDelay = 2;
        pWitch->m_info.m_movementDelay = 2;
        pEnemy = pWitch;
        break;
    case 2:
        pBanshee = malloc(sizeof(Banshee));
        pBanshee->m_info.m_assignedLevel = levelAssignment;
        pBanshee->m_info.m_damage = rand() % (45 + 1 - 40) + 40;
        pBanshee->m_info.m_Pos.x = pos.x;
        pBanshee->m_info.m_Pos.y = pos.y;
        pBanshee->m_info.m_symbol = 234;
        pBanshee->m_info.m_moveStrategy = MovementDark;
        pBanshee->m_info.m_selectMovementStrategy = SelectMovementStrategyAssistance;
        pBanshee->m_info.m_strategyCheck = StrategyCheck;
        pBanshee->m_info.m_defaultMovementDelay = 1;
        pBanshee->m_info.m_movementDelay = 1;
        pEnemy = pBanshee;
        break;
    default:
        printf("Error! File: EnemyFactory.c Function: *CreateEnemy(int type, int levelAssignment).\n");
        break;
    }

    /// Step 6. Save enemy details for future duplicate checks.
    int vacancy = GetEnemySpecificsPos();
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_assignedLevel = levelAssignment;
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_pos.x = pos.x;
    s_pEnemyFactory->m_enemySpecifics[vacancy].m_pos.y = pos.y;

    /// Step 7. Return the newly created enemy.
    return pEnemy;
}

void EnemyFactoryInit()
{
    s_pEnemyFactory = malloc(sizeof(EnemyFactory));

    s_pEnemyFactory->EFReceiveData = EnemyFactoryReceiveData;

    s_pEnemyFactory->EFCreateEnemy = CreateEnemy;

    InitializeEnemyFactoryTestsArray();

    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        s_pEnemyFactory->m_enemySpecifics[i].m_assignedLevel = ERROR_INDICATOR;
        s_pEnemyFactory->m_enemySpecifics[i].m_pos.x = ERROR_INDICATOR;
        s_pEnemyFactory->m_enemySpecifics[i].m_pos.y = ERROR_INDICATOR;
    }

    Register(s_pEnemyFactory, 0);
}

void *EnemyCreate(int type, int levelAssignment)
{
    void *pEnemy = s_pEnemyFactory->EFCreateEnemy(type, levelAssignment);
    return pEnemy;
}

void EnemyReceiveData(Point mainCoordinates[], Point flPoints[])
{
    s_pEnemyFactory->EFReceiveData(mainCoordinates, flPoints);
}

void EnemyFactoryCleanMemory()
{
    free(s_pEnemyFactory);
    s_pEnemyFactory = 0;
}
