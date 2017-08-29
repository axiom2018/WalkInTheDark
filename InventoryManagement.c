/// InventoryManagement.c
#include "InventoryManagement.h"
#include "Definitions.h"
#include "ArrayOperations.h"
#include "Switch.h"
#include "Flashlight.h"
#include "Boolean.h"
#include "InventoryControls.h"
#include "HealthPack.h"
#include "Battery.h"
#include "Flashlight.h"
#include "Player.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    Flashlight *m_pEquippedFlashlight;
    void *m_pInventory[MAX_INVENTORY_SPACE];
    int m_itemTypes[MAX_INVENTORY_SPACE];
    int m_itemCount;
} InventoryManager;

/// Static InventoryManager pointer for re-use.
static InventoryManager *s_pInventoryManager;

/// Find index of currently equipped flashlight.
static int LocateEquippedFlashlightInInventory()
{
    int i;
    for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
    {
        if(s_pInventoryManager->m_pInventory[i] != NULL)
        {
            if(s_pInventoryManager->m_pInventory[i] == s_pInventoryManager->m_pEquippedFlashlight)
                return i;
        }
    }

    return i;
}

/// Use the health pack the player wishes.
static void UseHealthpack(int index)
{
    /// Step 1. Take the health pack out of the inventory.
    HealthPack *pHealthpack = s_pInventoryManager->m_pInventory[index];

    /// Step 2. Add the value from the health pack to the players health.
    AddHealth(pHealthpack->m_healthAmount);

    /// Step 3. Remove health pack from inventory and item types array.
    s_pInventoryManager->m_pInventory[index] = NULL;
    s_pInventoryManager->m_itemTypes[index] = ERROR_INDICATOR;
}

/// Use the battery the player wishes.
static void UseBattery(int index)
{
    /// Step 1. Take the battery out of the inventory.
    Battery *pBattery = s_pInventoryManager->m_pInventory[index];

    /// Step 2. Apply the battery charge to the players equipped flashlight.
    s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge += pBattery->m_rechargeAmount;

    /// Step3. Remove battery from inventory and item types array.
    s_pInventoryManager->m_pInventory[index] = NULL;
    s_pInventoryManager->m_itemTypes[index] = ERROR_INDICATOR;
}


/// Select the flashlight player wishes.
static void UseFlashlight(int index)
{
    if(s_pInventoryManager->m_pEquippedFlashlight == s_pInventoryManager->m_pInventory[index])
    {
        printf("This flashlight is already equipped!\n");
        return;
    }

    else
    {
        /// Step 1. Find the index location of the equipped flashlight.
        int flIndex = LocateEquippedFlashlightInInventory();

        /// Step 2. Save the equipped flashlight in a temp variable.
        Flashlight *pTemp = s_pInventoryManager->m_pInventory[flIndex];

        /// Step 3. Assign flashlight we desired in m_pEquippedFlashlight position.
        s_pInventoryManager->m_pEquippedFlashlight = s_pInventoryManager->m_pInventory[index];

        /// Step 4. Make SURE both flashlights are inside of the inventory in their proper positions.
        s_pInventoryManager->m_pInventory[index] = pTemp;
        s_pInventoryManager->m_pInventory[flIndex] = s_pInventoryManager->m_pEquippedFlashlight;
    }
}

/// Health pack info.
static void HealthpackInfo(int index)
{
    HealthPack *pHealthPack = (HealthPack*)s_pInventoryManager->m_pInventory[index];
    printf("[%c] Name: %s Health amount: %d\n", ReturnIndicator(index),
           pHealthPack->m_item.m_name, pHealthPack->m_healthAmount);
}

/// Battery info.
static void BatteryInfo(int index)
{
    Battery *pBattery = (Battery*)s_pInventoryManager->m_pInventory[index];
    printf("[%c] Name: %s Recharge amount: %d\n", ReturnIndicator(index),
           pBattery->m_item.m_name, pBattery->m_rechargeAmount);
}

/// Flashlight Info.
static void FlashlightInto(int index)
{
    Flashlight *pFlashlight = (Flashlight*)s_pInventoryManager->m_pInventory[index];
    printf("[%c] Name: %s Charge: %d, Range: %d\n", ReturnIndicator(index),
           pFlashlight->m_item.m_name, pFlashlight->m_stepsBeforeRecharge, pFlashlight->m_range);
}

/// Add HealthPack to inventory.
static void AddHealthPack(void *pItem, int index, int type)
{
    /// Step 1. Set void pointer to type HealthPack.
    HealthPack *pHP = (HealthPack*)pItem;

    /// Step 2. Place item in item array.
    s_pInventoryManager->m_pInventory[index] = pHP;

    /// Step 3. Place type in type array.
    s_pInventoryManager->m_itemTypes[index] = type;
}

/// Add Battery to inventory.
static void AddBattery(void *pItem, int index, int type)
{
    /// Step 1. Set void pointer to type Battery.
    Battery *pBattery = (Battery*)pItem;

    /// Step 2. Place item in item array.
    s_pInventoryManager->m_pInventory[index] = pBattery;

    /// Step 3. Place type in type array.
    s_pInventoryManager->m_itemTypes[index] = type;
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
    if(s_pInventoryManager->m_pEquippedFlashlight == NULL)
    {
        s_pInventoryManager->m_pEquippedFlashlight = (Flashlight*)s_pInventoryManager->m_pInventory[index];
        s_pInventoryManager->m_pEquippedFlashlight->m_range = 3;
        /// Remove this 300 boost later! It's for testing purposes only.
        s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge = 300;
    }
}

/// Free up allocated memory.
void InventoryManagementCleanMemory()
{
    int i;
    for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
    {
        if(s_pInventoryManager->m_pInventory[i] != NULL)
        {
            free(s_pInventoryManager->m_pInventory[i]);
            s_pInventoryManager->m_pInventory[i] = 0;
        }
    }

    InventoryControlsCleanMemory();

    free(s_pInventoryManager);
    s_pInventoryManager = 0;
}

/// Referenced in Player.c
void DecreaseStepsBeforeRecharge()
{
    if(s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge <= 0)
    {
        s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge = 0;
    }

    else
    {
        s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge -= 1;
    }
}

/// Referenced in InventoryControls.c
int GetItemCount()
{
    return s_pInventoryManager->m_itemCount;
}

/// Referenced in FlashlightManagement.c
Flashlight * ReturnFlashLight()
{
    return s_pInventoryManager->m_pEquippedFlashlight;
};

/// Referenced in InventoryControls.c
int GetItemType(int index)
{
    return s_pInventoryManager->m_itemTypes[index];
}

/// Referenced in InventoryControls.c
void UseItem(int type, int index)
{
    switch(type)
    {
    case 0:
        UseFlashlight(index);
        break;
    case 1:
        UseBattery(index);
        break;
    case 2:
        UseHealthpack(index);
        break;
    default:
        break;
    }
}

/// Referenced in InventoryControls.c
int CheckInventoryPosition(int index)
{
    if(s_pInventoryManager->m_pInventory[index] == NULL)
        return TRUE;

    return FALSE;
}

/// Display inventory.
void ShowInventory()
{
    while(TRUE)
    {
        system("cls");
        s_pInventoryManager->m_itemCount = 0;
        printf("__________INVENTORY__________\n");
        int i;

        for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
        {
            /// Pass inventory array.
            if(s_pInventoryManager->m_pInventory[i] != NULL)
            {
                switch(s_pInventoryManager->m_itemTypes[i])
                {
                case 0:
                    FlashlightInto(i);
                    break;
                case 1:
                    BatteryInfo(i);
                    break;
                case 2:
                    HealthpackInfo(i);
                    break;
                default:
                    break;
                }

                s_pInventoryManager->m_itemCount += 1;
                printf("\n");
            }
        }

        InventoryKeyInstructions();

        char input = _getch();

        if(input == 'q')
        {
            break;
        }

        else
        {
            IndicatorMotion(&input);
        }
    }
}

/// Flashlight algorithm in FlashlightManagement.c will not run if the flashlight battery is not charged.
int CheckBattery()
{
    if(s_pInventoryManager->m_pEquippedFlashlight->m_stepsBeforeRecharge > 0)
        return TRUE;

    return FALSE;
}

/// Flashlight management.c needs the flashlight range to properly perform the algorithm.
int GetFlashlightRange()
{
    return s_pInventoryManager->m_pEquippedFlashlight->m_range;
}

/// Add item to player inventory.
void AddToInventory(void *pItem, int itemType)
{
    /// Step 1. Get empty index.
    int index = GetPointerArrayPos(s_pInventoryManager->m_pInventory, MAX_INVENTORY_SPACE);

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

/// Initialize management.
void InitInventoryManagement()
{
    /// Step 1. Set up and initialize structure.
    s_pInventoryManager = malloc(sizeof(InventoryManager));

    /// Step 2. Allocate space for flashlight and set to NULL.
    s_pInventoryManager->m_pEquippedFlashlight = malloc(sizeof(Flashlight)); /// Segmentation fault.
    s_pInventoryManager->m_pEquippedFlashlight = NULL;

    /// Step 3. Set arrays to default value.
    int i;
    for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
    {
        s_pInventoryManager->m_pInventory[i] = NULL;
        s_pInventoryManager->m_itemTypes[i] = ERROR_INDICATOR;
    }

    /// Step 4. Set to 0.
    s_pInventoryManager->m_itemCount = 0;

    /// Step 5. The controls will be initialized and handled here.
    InitInventoryControls();
}
