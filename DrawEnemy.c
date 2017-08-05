/// DrawEnemy.c
#include "DrawEnemy.h"
#include "Point.h"
#include "Boolean.h"
#include "LevelManagement.h"

static int DisplayEnemies(int x, int y, Point *pEnemyPos)
{
    if(y == pEnemyPos->x &&
       x == pEnemyPos->y)
    {
        return TRUE;
    }

    return FALSE;
}

static int CheckEnemyCurrentLevel(int enemyCurrentLevel)
{
    /// I can use the switch on this later as well. USE THE SWITCH!
    if(enemyCurrentLevel == GetCurrentLevel())
    {
        return TRUE;
    }

    return FALSE;
}



void DrawWerewolf(Werewolf *pWerewolf, int x, int y)
{
    /// Step 1. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
    if(!CheckEnemyCurrentLevel(pWerewolf->m_info.m_assignedLevel))
        return;

    /// Step 2. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pWerewolf->m_info.m_Pos)
       && pWerewolf->m_info.m_strategyCheck(&pWerewolf->m_info.m_moveStrategy))
    {
        printf("%c", pWerewolf->m_info.m_symbol);
    }
}

void DrawBanshee(Banshee *pBanshee, int x, int y)
{
    /// Step 1. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
    if(!CheckEnemyCurrentLevel(pBanshee->m_info.m_assignedLevel))
        return;

    /// Step 2. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pBanshee->m_info.m_Pos)
       && pBanshee->m_info.m_strategyCheck(&pBanshee->m_info.m_moveStrategy))
    {
        printf("%c", pBanshee->m_info.m_symbol);
    }
}

void DrawWitch(Witch *pWitch, int x, int y)
{
    /// Step 1. Don't bother if the enemy in the array isn't assigned to the level we're currently on.
    if(!CheckEnemyCurrentLevel(pWitch->m_info.m_assignedLevel))
        return;

    /// Step 2. Check if the coordinates are correct, and assign proper strategy.
    if(DisplayEnemies(x, y, &pWitch->m_info.m_Pos)
       && pWitch->m_info.m_strategyCheck(&pWitch->m_info.m_moveStrategy))
    {
        printf("%c", pWitch->m_info.m_symbol);
    }
}
