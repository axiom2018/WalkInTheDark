/// LevelInfo.h
#ifndef LEVELINFO_H_INCLUDED
#define LEVELINFO_H_INCLUDED
#include "Point.h"

/// Primary used in EnemyFactory.c for saving important enemy info for the sake of only updating the correct enemies.

typedef struct
{
    Point m_pos;
    int m_assignedLevel;
} LevelInfo;

#endif // LEVELINFO_H_INCLUDED
