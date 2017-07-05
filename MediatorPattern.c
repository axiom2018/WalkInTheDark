/// MediatorPattern.c
#include "MediatorPattern.h"
#include "World.h"
#include "EnemyFactory.h"
#include "Boolean.h"
#include "Definitions.h"
#include "ArrayOperations.h"
#include <stdio.h>
#include <windows.h>

typedef struct
{
    void *m_pSubs[SUBSCRIBERS];
    int m_typeOfSub[SUBSCRIBERS];
} Mediate;

static Mediate *s_pMediate;

/// World and EnemyFactory must be registered before Mediator can be used.
static int CheckRegistration()
{
    /// Step 1. If m_pSubs is empty then no one registered. We can't send information to anyone anyhow so return.
    if(s_pMediate->m_pSubs[0] == NULL)
    {
        return FALSE;
    }

    /// Step 2. If index 0 above is NOT empty then check who registered. That way we know who ISN'T registered.
    int resultOfIndexCheck;
    switch(s_pMediate->m_typeOfSub[0])
    {
    case 0:
        resultOfIndexCheck = 0; /// EnemyFactory.c
        break;
    case 1:
        resultOfIndexCheck = 1; /// World.c
        break;
    default:
        printf("Error! File: MediatorPattern.c. Function: CheckRegistration() Part 1.\n");
        break;
    }

    /// Step 3. Check to see if index 1 is NULL. if so we're missing someone we will return false since index 1 is NULL.
    if(s_pMediate->m_pSubs[1] == NULL)
    {
        switch(resultOfIndexCheck)
        {
        case 0:
            printf("World registration missing!\n");
            break;
        case 1:
            printf("EnemyFactory registration missing!\n");
            break;
        default:
            printf("Error! File: MediatorPattern.c. Function: CheckRegistration() Part 2.\n");
            break;
        }
        return FALSE;
    }
    return TRUE;
}

void MediatorInit()
{
    s_pMediate = malloc(sizeof(Mediate));

    int i;
    for(i = 0; i < SUBSCRIBERS; ++i)
    {
        s_pMediate->m_pSubs[i] = NULL;
        s_pMediate->m_typeOfSub[i] = ERROR_INDICATOR;
    }
}

void Register(void *pMember, int memberShipType)
{
    /// World and EnemyFactory must be registered before Mediator can be used. If memberShipType is 0, enemy factory is registering. If 1, the World.

    /// Step 1. Find empty index.
    int indexOfSubs = GetPointerArrayPos(s_pMediate->m_pSubs, SUBSCRIBERS);

    if(indexOfSubs == ERROR_INDICATOR)
    {
        return;
    }

    /// Step 2. Find another empty index.
    int indexOfTypeOfSub = GetIntArrayPos(s_pMediate->m_typeOfSub, SUBSCRIBERS);

    /// Step 3. Assign to proper spaces.
    s_pMediate->m_pSubs[indexOfSubs] = pMember;
    s_pMediate->m_typeOfSub[indexOfTypeOfSub] = memberShipType;
}

void MessageData(Point mainCoordinates[], Point flPoints[])
{
    /// Step 1. Run CheckRegistration(). If it returns TRUE then proceed otherwise return.
    if(!CheckRegistration())
    {
        return;
    }

    /// Step 2. Send data.
    EnemyReceiveData(mainCoordinates, flPoints);
}

void *GetEnemy(int typeOfMonster, int levelToAssign)
{
    void *pEnemy = EnemyCreate(typeOfMonster, levelToAssign);
    return pEnemy;
}

void MediatorCleanMemory()
{
    free(s_pMediate);
    s_pMediate = 0;
}
