/// Flashlight.h
#ifndef FLASHLIGHT_H_INCLUDED
#define FLASHLIGHT_H_INCLUDED
#include "Items.h"

/// Item.

typedef struct
{
    Items m_item;
    int m_stepsBeforeRecharge;
    int m_range;
} Flashlight;

#endif // FLASHLIGHT_H_INCLUDED
