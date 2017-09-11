/// HealthPack.h
#ifndef HEALTHPACK_H_INCLUDED
#define HEALTHPACK_H_INCLUDED
#include "Items.h"

/// Game item.

typedef struct
{
    Items m_item;
    int m_healthAmount;
} HealthPack;

#endif // HEALTHPACK_H_INCLUDED
