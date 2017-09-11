/// EnemyCreation.c
#ifndef ENEMYCREATION_C_INCLUDED
#define ENEMYCREATION_C_INCLUDED
#include "EnemyCreation.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "DarkMovement.h"
#include <stdlib.h>

static Banshee * CreateBanshee(int levelAssignment, Point pos)
{
    /// Step 1. Set up banshee variable.
    Banshee *pBanshee = malloc(sizeof(Banshee));

    /// Step 2. Apply all details.
    pBanshee->m_info.m_assignedLevel = levelAssignment;
    pBanshee->m_info.m_damage = rand() % (BANSHEE_DAMAGE_HIGH + 1 - BANSHEE_DAMAGE_LOW) + BANSHEE_DAMAGE_LOW;
    pBanshee->m_info.m_Pos.x = pos.x;
    pBanshee->m_info.m_Pos.y = pos.y;
    pBanshee->m_info.m_symbol = BANSHEE_SYMBOL;
    pBanshee->m_info.m_moveStrategy = MovementDark;
    pBanshee->m_info.m_defaultMovementDelay = BANSHEE_MOVEMENT_DELAY;
    pBanshee->m_info.m_movementDelay = BANSHEE_MOVEMENT_DELAY;

    /// Step 3. Return the banshee.
    return pBanshee;
}

static Witch * CreateWitch(int levelAssignment, Point pos)
{
    /// Step 1. Set up witch variable.
    Witch *pWitch = malloc(sizeof(Witch));

    /// Step 2. Apply all details.
    pWitch->m_info.m_assignedLevel = levelAssignment;
    pWitch->m_info.m_damage = rand() % (WITCH_DAMAGE_HIGH + 1 - WITCH_DAMAGE_LOW) + WITCH_DAMAGE_LOW;
    pWitch->m_info.m_Pos.x = pos.x;
    pWitch->m_info.m_Pos.y = pos.y;
    pWitch->m_info.m_symbol = WITCH_SYMBOL;
    pWitch->m_info.m_moveStrategy = MovementDark;
    pWitch->m_info.m_defaultMovementDelay = WITCH_MOVEMENT_DELAY;
    pWitch->m_info.m_movementDelay = WITCH_MOVEMENT_DELAY;

    /// Step 3. Return the witch.
    return pWitch;
}

static Werewolf * CreateWerewolf(int levelAssignment, Point pos)
{
    /// Step 1. Set up werewolf variable.
    Werewolf *pWerewolf = malloc(sizeof(Werewolf));

    /// Step 2. Apply all details.
    pWerewolf->m_info.m_assignedLevel = levelAssignment;
    pWerewolf->m_info.m_damage = rand() % (WEREWOLF_DAMAGE_HIGH + 1 - WEREWOLF_DAMAGE_LOW) + WEREWOLF_DAMAGE_LOW;
    pWerewolf->m_info.m_Pos.x = pos.x;
    pWerewolf->m_info.m_Pos.y = pos.y;
    pWerewolf->m_info.m_symbol = WEREWOLF_SYMBOL;
    pWerewolf->m_info.m_moveStrategy = MovementDark;
    pWerewolf->m_info.m_defaultMovementDelay = WEREWOLF_MOVEMENT_DELAY;
    pWerewolf->m_info.m_movementDelay = WEREWOLF_MOVEMENT_DELAY;

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
