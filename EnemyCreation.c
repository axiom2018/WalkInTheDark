/// EnemyCreation.c
#ifndef ENEMYCREATION_C_INCLUDED
#define ENEMYCREATION_C_INCLUDED
#include "EnemyCreation.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "SelectMovementStrategyAssistance.h"
#include "StrategyCheck.h"
#include "DarkMovement.h"
#include <stdlib.h>

static Banshee * CreateBanshee(int levelAssignment, Point pos)
{
    /// Step 1. Set up banshee variable.
    Banshee *pBanshee = malloc(sizeof(Banshee));

    /// Step 2. Apply all details.
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

    /// Step 3. Return the banshee.
    return pBanshee;
}

static Witch * CreateWitch(int levelAssignment, Point pos)
{
    /// Step 1. Set up witch variable.
    Witch *pWitch = malloc(sizeof(Witch));

    /// Step 2. Apply all details.
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

    /// Step 3. Return the witch.
    return pWitch;
}

static Werewolf * CreateWerewolf(int levelAssignment, Point pos)
{
    /// Step 1. Set up werewolf variable.
    Werewolf *pWerewolf = malloc(sizeof(Werewolf));

    /// Step 2. Apply all details.
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

    /// Step 3. Return the werewolf.
    return pWerewolf;
}

void *RequestToCreateEnemy(int type, int levelAssignment, Point pos)
{
    /// Step 1. Set up variables.
    void *pEnemy;

    /// Step 2. Begin creating enemy.
    switch(type)
    {
    case 0:
        pEnemy = CreateWerewolf(levelAssignment, pos);
        break;
    case 1:
        pEnemy = CreateWitch(levelAssignment, pos);
        break;
    case 2:
        pEnemy = CreateBanshee(levelAssignment, pos);
        break;
    default:
        printf("Error! File: EnemyCreation.c Function: *CreateEnemy(int type, int levelAssignment).\n");
        break;
    }

    /// Step 3. Return enemy in void pointer form.
    return pEnemy;
}

#endif // ENEMYCREATION_C_INCLUDED
