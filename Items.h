/// Items.h
#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED
#include "Point.h"

/// Each item with have an Items variable.
typedef struct
{
    char *m_name;
    char m_symbol; /// Flashlight is F, Health Pack is H, and a Battery is B.
    Point m_pos;
    int m_color;
    int m_assignedLevel;
} Items;
#endif // ITEMS_H_INCLUDED
