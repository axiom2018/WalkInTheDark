/// Door.h
#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED
#include "Point.h"

/// Player must get the Door to advance the game in every level.

typedef struct
{
    Point m_coord;
    char m_symbol;
} Door;

#endif // DOOR_H_INCLUDED
