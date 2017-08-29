/// InventoryControls.c
#include "InventoryControls.h"
#include "InventoryManagement.h"
#include "Definitions.h"
#include <stdio.h>
#include <stdlib.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    /// char m_symbol;
    char m_selection;
    char m_marker;
    int m_index;
} InventoryControls;

/// Static InventoryControls pointer for re-use.
static InventoryControls *s_pInventoryControls;

/// When player decides to use an item, proceed to use it.
static void SelectItem()
{
    /// Step 1. Get item type.
    int type = GetItemType(s_pInventoryControls->m_index);

    /// Step 2. Process the type received.
    switch(type)
    {
    case 0:
        UseItem(0, s_pInventoryControls->m_index);
        break;
    case 1:
        UseItem(1, s_pInventoryControls->m_index);
        break;
    case 2:
        UseItem(2, s_pInventoryControls->m_index);
        break;
    default:
        printf("Error! File: InventoryControls.c. Function: EquipOrUseSelectedItem().\n");
        break;
    }

    s_pInventoryControls->m_index = 0;
}

/// Move upwards while player navigates inventory.
static void Ascend()
{
    /// Step 1. In the function Ascend() we safely know the search for the previous non-null index will be going "upward" or a decreasing linear search.
    int locateNonNullIndex = s_pInventoryControls->m_index - 1;

    while(locateNonNullIndex >= 0)
    {
        if(CheckInventoryPosition(locateNonNullIndex))
        {
            --locateNonNullIndex;
        }

        else
        {
            /// Step 3. If the index in inventory is not NULL, we successfully found a position, assign index to s_pPlayer->m_index and break from loop.
            s_pInventoryControls->m_index = locateNonNullIndex;
            break;
        }
    }
}

/// Move downwards while player navigates inventory.
static void Descend()
{
    /// Step 1. In the function Descend() we safely know the search for the next non-null index will be going "downward" or an increasing linear search.
    int locateNonNullIndex = s_pInventoryControls->m_index + 1;

    while(locateNonNullIndex < MAX_INVENTORY_SPACE)
    {
        if(CheckInventoryPosition(locateNonNullIndex))
        {
            ++locateNonNullIndex;
        }

        else
        {
            /// Step 3. If the index in inventory is not NULL, we successfully found a position, assign index to s_pPlayer->m_index and break from loop.
            s_pInventoryControls->m_index = locateNonNullIndex;
            break;
        }
    }
}

/// Apply the motion selected by the IndicatorMotion function.
static void ApplyMotion(char key)
{
    switch(key)
    {
    case 's':
        Descend();
        break;
    case 'w':
        Ascend();
        break;
    default:
        break;
    }
}

/// Clean memory.
void InventoryControlsCleanMemory()
{
    free(s_pInventoryControls);
    s_pInventoryControls = 0;
}

/// Function helps move the indicator up one or down one when player navigates inventory.
void IndicatorMotion(char *input)
{
    /// Step 1. Player cannot select anything else if they only have 1 item in their inventory.
    if(GetItemCount() == 1)
        return;

    /// Step 2. If Step 1 did not return, process the input.
    switch(*input)
    {
    case 'w':
        ApplyMotion(*input);
        break;
    case 's':
        ApplyMotion(*input);
        break;
    case 'u':
        SelectItem();
        break;
    default:
        break;
    }
}

/// When the inventory is open, give the player a guide on which controls to use.
void InventoryKeyInstructions()
{
    printf("Use the -w/s- keys to navigate up and down.\n");
    printf("Press -u- to use an item.\n");
    printf("Press -q- to return to game.\n\n");
}

/// Return a char to indicate if current index is what player selected, otherwise, return m_selection.
char ReturnIndicator(int currentIndex)
{
    if(s_pInventoryControls->m_index == currentIndex)
    {
        return s_pInventoryControls->m_selection;
    }

    return s_pInventoryControls->m_marker;
}

/// Initialization.
void InitInventoryControls()
{
    /// Step 1. Set up and initialize structure.
    s_pInventoryControls = malloc(sizeof(InventoryControls));

    /// Step 2. Initialize variables.
    s_pInventoryControls->m_selection = 'S';
    s_pInventoryControls->m_marker = ' ';
    s_pInventoryControls->m_index = 0;
}
