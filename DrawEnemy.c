/// DrawEnemy.c
#include "DrawEnemy.h"
#include "Point.h"
#include "Boolean.h"
#include "LevelManagement.h"
#include "EnemyCheckMoveStrategy.h"
#include <stdio.h>

static int DisplayEnemies(int x, int y, Point *pEnemyPos)
{
    if(y == pEnemyPos->x &&
       x == pEnemyPos->y)
    {
        return TRUE;
    }

    return FALSE;
}

/// Must check the assigned level of the enemy in order to display them.
static int CheckEnemyCurrentLevel(int enemyCurrentLevel)
{
    if(enemyCurrentLevel == GetCurrentLevel())
    {
        return TRUE;
    }

    return FALSE;
}

int DrawWerewolf(Werewolf *pWerewolf, int x, int y)
{
    /// Step 1. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pWerewolf->m_info.m_Pos)
       && StrategyCheck(&pWerewolf->m_info.m_moveStrategy))
    {
        /// Step 2. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
        if(!CheckEnemyCurrentLevel(pWerewolf->m_info.m_assignedLevel))
            return FALSE;

        /// Step 3. Print enemy.
        printf("%c", pWerewolf->m_info.m_symbol);
        return TRUE;
    }

    return FALSE;
}

int DrawBanshee(Banshee *pBanshee, int x, int y)
{
    /// Step 1. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pBanshee->m_info.m_Pos)
       && StrategyCheck(&pBanshee->m_info.m_moveStrategy))
    {
        /// Step 2. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
        if(!CheckEnemyCurrentLevel(pBanshee->m_info.m_assignedLevel))
            return FALSE;

        /// Step 3. Print enemy.
        printf("%c", pBanshee->m_info.m_symbol);
        return TRUE;
    }

    return FALSE;
}

int DrawWitch(Witch *pWitch, int x, int y)
{
    /// Step 1. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pWitch->m_info.m_Pos)
       && StrategyCheck(&pWitch->m_info.m_moveStrategy))
    {
        /// Step 2. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
        if(!CheckEnemyCurrentLevel(pWitch->m_info.m_assignedLevel))
            return FALSE;

        /// Step 3. Print enemy.
        printf("%c", pWitch->m_info.m_symbol);
        return TRUE;
    }

    return FALSE;
}
