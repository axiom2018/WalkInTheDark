/// HealthPack.h
#ifndef HEALTHPACK_H_INCLUDED
#define HEALTHPACK_H_INCLUDED
#include "Items.h"

/// Item.

typedef struct
{
    Items m_item;
    int m_healthAmount;
} HealthPack;

#endif // HEALTHPACK_H_INCLUDED
