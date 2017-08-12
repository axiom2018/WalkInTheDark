/// InventoryManagement.c
#include "InventoryManagement.h"
#include "Definitions.h"
#include "ArrayOperations.h"
#include "Switch.h"
#include "Flashlight.h"
#include <stdio.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    Flashlight *m_pEquippedFlashlight;
    void *m_pInventory[MAX_INVENTORY_SPACE];
    int m_itemTypes[MAX_INVENTORY_SPACE];
} InventoryManager;

/// Static InventoryManager pointer for re-use.
static InventoryManager *s_pInventoryManager;

/// Add HealthPack to inventory.
static void AddHealthPack(void *pItem, int index, int type)
{
    /// Step 1. Set void pointer to type HealthPack.
    HealthPack *pHP = (HealthPack*)pItem;

    /// Step 2. Place item in item array.
    s_pPlayer->m_pInventory[index] = pHP;

    /// Step 3. Place type in type array.
    s_pPlayer->m_itemTypes[itemTypesIndex] = itemType;
}

/// Add Battery to inventory.
static void AddBattery(void *pItem, int index, int type)
{
    /// Step 1. Set void pointer to type Battery.
    Battery *pBattery = (Battery*)pItem;

    /// Step 2. Place item in item array.
    s_pPlayer->m_pInventory[index] = pBat;

    /// Step 3. Place type in type array.
    s_pPlayer->m_itemTypes[index] = itemType;
}

/// Add Flashlight to inventory.
static void AddFlashlight(void *pItem, int index, int type)
{
    /// Step 1. Set void pointer to type Flashlight.
    Flashlight *pFlashlight = (Flashlight*)pItem;

    /// Step 2. Place item in item array.
    s_pInventoryManager->m_pInventory[index] = pFlashlight;

    /// Step 3. Place type in type array.
    s_pInventoryManager->m_itemTypes[index] = type;

    /// Step 4. Check if equipped flashlight is NULL, if so, assign this flashlight to it.
    if(s_pPlayer->m_pEquippedFlashlight == NULL)
    {
        s_pPlayer->m_pEquippedFlashlight = (Flashlight*)s_pPlayer->m_pInventory[index];
        s_pPlayer->m_pEquippedFlashlight->m_range = 3;
        /// Remove this 300 boost later! It's for testing purposes only.
        s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge = 300;
    }
}

/// Add item to player inventory.
void AddItemToInventory(void *pItem, int itemType)
{
    /// Step 1. Get empty index.
    int index = GetPointerArrayPos(s_pPlayer->m_pInventory, MAX_INVENTORY_SPACE);

    /// Step 2. Check if index is valid. Otherwise return.
    if(UseSwitch(index, ERROR_INDICATOR))
        return;

    /// Step 3. Place item in players inventory.
    switch(itemType)
    {
    case 0:
        AddFlashlight(pItem, index, itemType);
        break;
    case 1:
        AddBattery(pItem, index, itemType);
        break;
    case 2:
        AddHealthPack(pItem, index, itemType);
        break;
    default:
        printf("Error! InventoryManagement.c!\n");
        break;
    }
}

