/// UpdateEnemy.c
#include "UpdateEnemy.h"
#include "Switch.h"
#include "ItemManagement.h"
#include "World.h"
#include "Player.h"
#include "LevelManagement.h"
#include "StrategyCheck.h"

static void UpdateEnemyData(EMData *pData)
{
    /// Step 1. Reset variables to 0.
    pData->m_maxEnemies = 0;
    pData->m_maxItems = 0;

    /// Step 2. Enemies must be aware of items too. ItemManager.h included for that purpose.
    UpdateEnemyItemData(pData);

    /// Step 3. Gather other relevant data.
    pData->m_pDoorPos = &GetDoor()->m_coord;

    pData->m_playerPos = GetPlayerPosition();

    pData->m_playerHealth = GetPlayerHealth();

    pData->m_saveCurrentLevel = GetCurrentLevel();
}

static void ResolveBansheeUpdate(EMData *pData, Banshee *pBanshee)
{
    /// Step 1. Enemy position might be changed so it must be given.
    pData->m_pEnemyPos = &pBanshee->m_info.m_Pos;
    /// Step 2. Pass the memory address of the enemies movement delay variable.
    pData->m_pEnemyMovementDelay = &pBanshee->m_info.m_movementDelay;
    /// Step 3. In case of collision with player we must have the enemy's damage rate.
    pData->m_enemyDamageRate = pBanshee->m_info.m_damage;
    /// Step 4. Also pass the default movement delay amount.
    pData->m_enemyDefaultMovementDelay = pBanshee->m_info.m_defaultMovementDelay;
    /// Step 5. Update remaining info.
    UpdateEnemyData(pData);

    /// Step 6. Depending on where the enemy is, a certain type of strategy will be selected.
    if(updateType == UpdateStrategy)
        pBanshee->m_info.m_selectMovementStrategy(pData, &pBanshee->m_info.m_moveStrategy);
    else if(updateType == UpdateMovement)
        pBanshee->m_info.m_moveStrategy(pData);
}

static void ResolveWitchUpdate(EMData *pData, Witch *pWitch)
{
    /// Step 1. Enemy position might be changed so it must be given.
    pData->m_pEnemyPos = &pWitch->m_info.m_Pos;
    /// Step 2. Pass the memory address of the enemies movement delay variable.
    pData->m_pEnemyMovementDelay = &pWitch->m_info.m_movementDelay;
    /// Step 3. In case of collision with player we must have the enemy's damage rate.
    pData->m_enemyDamageRate = pWitch->m_info.m_damage;
    /// Step 4. Also pass the default movement delay amount.
    pData->m_enemyDefaultMovementDelay = pWitch->m_info.m_defaultMovementDelay
    /// Step 5. Update remaining info.
    UpdateEnemyData(pData);

    /// Step 6. Depending on where the enemy is, a certain type of strategy will be selected.
    if(updateType == UpdateStrategy)
        pWitch->m_info.m_selectMovementStrategy(pData, &pWitch->m_info.m_moveStrategy);
    else if(updateType == UpdateMovement)
        pWitch->m_info.m_moveStrategy(pData);
}

static void ResolveWerewolfUpdate(EMData *pData, Werewolf *pWerewolf)
{
    /// Step 1. Enemy position might be changed so it must be given.
    pData->m_pEnemyPos = &pWerewolf->m_info.m_Pos;
    /// Step 2. Pass the memory address of the enemies movement delay variable.
    pData->m_pEnemyMovementDelay = &pWerewolf->m_info.m_movementDelay;
    /// Step 3. In case of collision with player we must have the enemy's damage rate.
    pData->m_enemyDamageRate = pWerewolf->m_info.m_damage;
    /// Step 4. Also pass the default movement delay amount.
    pData->m_enemyDefaultMovementDelay = pWerewolf->m_info.m_defaultMovementDelay;
    /// Step 5. Update remaining info.
    UpdateEnemyData(pData);

    /// Step 6. Depending on where the enemy is, a certain type of strategy will be selected.
    if(updateType == UpdateStrategy)
        pWerewolf->m_info.m_selectMovementStrategy(pData, &pWerewolf->m_info.m_moveStrategy);
    else if(updateType == UpdateMovement)
        pWerewolf->m_info.m_moveStrategy(pData);
}

void ResolveEnemyUpdate(EMData *pData, EnemyUpdateProcedure updateType)
{
    /// Step 1. Set up necessary variables.
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;
    int i;

    /// Step 2. Update each enemy.
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(pData->m_pEnemyArr[i] != NULL)
        {
            switch(pData->m_enemyTypes[i])
            {
            case 0:
                pWerewolf = pData->m_pEnemyArr[i];

                if(!UseSwitch(pWerewolf->m_info.m_assignedLevel, GetCurrentLevel()))
                    continue;

                ResolveWerewolfUpdate(pData, pWerewolf);
                break;
            case 1:
                pWitch = pData->m_pEnemyArr[i];

                if(!UseSwitch(pWitch->m_info.m_assignedLevel, GetCurrentLevel()))
                    continue;

                ResolveWitchUpdate(pData, pWitch);
                break;
            case 2:
                pBanshee = pData->m_pEnemyArr[i];

                if(!UseSwitch(pBanshee->m_info.m_assignedLevel, GetCurrentLevel()))
                    continue;

                ResolveBansheeUpdate(pData, pBanshee);
                break;
            default:
                printf("Error! File: World.c. Function: UpdateEnemies().\n");
                break;
            }
        }
    }
}
