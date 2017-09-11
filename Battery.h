/// Battery.h
#ifndef BATTERY_H_INCLUDED
#define BATTERY_H_INCLUDED
#include "Items.h"

/// Item.

typedef struct
{
    Items m_item;
    int m_rechargeAmount;
} Battery;

#endif // BATTERY_H_INCLUDED
