/// EnemyInfo.h
#ifndef ENEMYINFO_H_INCLUDED
#define ENEMYINFO_H_INCLUDED
#include "Point.h"
#include "EnemyMoveStrategy.h"
#include "SelectMovementStrategy.h"
#include "EnemyStrategyCheck.h"

/// Each enemy will have a generic EnemyInfo variable since enemies will use  the same data.
typedef struct
{
    char m_symbol;
    Point m_Pos;
    int m_damage;
    int m_assignedLevel;
    int m_defaultMovementDelay;
    int m_movementDelay;
    Move m_moveStrategy;
    SelectMovementStrategy m_selectMovementStrategy;
    EnemyStrategyCheck m_strategyCheck;
} EnemyInfo;

#endif // MONSTERINFO_H_INCLUDED
