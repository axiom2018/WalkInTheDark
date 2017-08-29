/// Door.h
#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED
#include "Point.h"

/// The door is the target the player must get to in every level.

typedef struct
{
    Point m_coord;
    char m_symbol;
} Door;

#endif // DOOR_H_INCLUDED
