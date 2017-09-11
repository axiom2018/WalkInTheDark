/// EnemyMoveData.h
#ifndef ENEMYMOVEDATA_H_INCLUDED
#define ENEMYMOVEDATA_H_INCLUDED
#include "Definitions.h"
#include "Point.h"

/** EnemyMoveData.h exists to avoid a more undesirable argument list for updating the enemies movement in World.c
Original arguments: void *pMonsterArr[], int monsterArrSize, Point flashlightPoints[], int flPointArrSize, Point *pPlayerPos. Obviously this
does not look so tidy but thanks to the structure below (that also looks like a real data type outside of the file) the code looks a bit cleaner.
Making the code much more readable. */

typedef struct
{
    void *m_pEnemyArr[MAX_ENEMIES];
    int m_enemyTypes[MAX_ENEMIES];
    int m_maxEnemies;
    void *m_pItemsArr[MAX_ITEMS];
    int m_itemTypes[MAX_ITEMS];
    int m_maxItems;
    Point m_flashlightPoints[SIZE_OF_FL_POINTS];
    Point m_playerPos;
    Point *m_pEnemyPos;
    Point *m_pDoorPos;
    int m_saveCurrentLevel;
    int m_playerHealth;
    int m_enemyDamageRate;
    int m_enemyDefaultMovementDelay;
    int *m_pEnemyMovementDelay;
} EMData;

#endif // MONSTERMOVEDATA_H_INCLUDED
