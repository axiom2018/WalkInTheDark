/// Player.c
#include "Player.h"
#include "World.h"
#include "Point.h"
#include "Battery.h"
#include "Flashlight.h"
#include "HealthPack.h"
#include "Definitions.h"
#include "Boolean.h"
#include "ArrayOperations.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct
{
    char m_symbol;
    char m_selection;
    char m_marker;
    int m_index;
    int m_itemCount;
    int m_health;
    Point m_playerPosition;
    Flashlight *m_pEquippedFlashlight;
    void *m_pInventory[MAX_INVENTORY_SPACE];
    int m_itemTypes[MAX_INVENTORY_SPACE];
} Player;

static Player *s_pPlayer;

/**
                                        THE FOLLOWING STATIC FUNCTIONS ARE C STYLE "PRIVATE" AND
                                        DECLARED BEFORE THE PRIMARY LIST OF FUNCTIONS IN PLAYER.H

                                        Make several generic array functions for use tomorrow.
*/

static void DecreaseStepsBeforeRecharge()
{
    if(s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge <= 0)
    {
        s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge = 0;
    }
    else
    {
        s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge -= 1;
    }
}

static void AddHealth(int amount)
{
    int addedHealth = s_pPlayer->m_health + amount;
    if(addedHealth > 100)
    {
        s_pPlayer->m_health = 100;
    }

    else
    {
        s_pPlayer->m_health += amount;
    }
}

static int LocateEquippedFlashlightInInventory()
{
    /// Step 1. Loop through inventory focusing just on flashlights.
    int indexOfEquippedFlashlight;
    int i;
    for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
    {
        if(s_pPlayer->m_pInventory[i] != NULL
           && s_pPlayer->m_itemTypes[i] == 0)
        {
            if(s_pPlayer->m_pInventory[i] == s_pPlayer->m_pEquippedFlashlight)
            {
                indexOfEquippedFlashlight = i;
                break;
            }
        }
    }
    return indexOfEquippedFlashlight;
}

static void EquipFlashlight()
{
    if(s_pPlayer->m_pEquippedFlashlight == s_pPlayer->m_pInventory[s_pPlayer->m_index])
    {
        printf("This flashlight is already equipped!\n");
        return;
    }

    else
    {
        /// Step 1. Find the index location of the equipped flashlight.
        int flIndex = LocateEquippedFlashlightInInventory();

        /// Step 2. Save the equipped flashlight in a temp variable.
        Flashlight *pTemp = s_pPlayer->m_pInventory[flIndex];

        /// Step 3. Assign flashlight we desired in m_pEquippedFlashlight position.
        s_pPlayer->m_pEquippedFlashlight = s_pPlayer->m_pInventory[s_pPlayer->m_index];

        /// Step 4. Make SURE both flashlights are inside of the inventory in their proper positions.
        s_pPlayer->m_pInventory[s_pPlayer->m_index] = pTemp;
        s_pPlayer->m_pInventory[flIndex] = s_pPlayer->m_pEquippedFlashlight;
    }
}

static void UseBattery()
{
    /// Step 1. Take the battery out of the inventory.
    Battery *pBattery = s_pPlayer->m_pInventory[s_pPlayer->m_index];

    /// Step 2. Apply the battery charge to the players equipped flashlight.
    s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge += pBattery->m_rechargeAmount;

    /// Step3. Remove battery from inventory and item types array.
    s_pPlayer->m_pInventory[s_pPlayer->m_index] = NULL;
    s_pPlayer->m_itemTypes[s_pPlayer->m_index] = ERROR_INDICATOR;

    /// Step 4. After deleting an old inventory item reset the index to the top of the inventory.
    s_pPlayer->m_index = 0;
}

static void UseHealthpack()
{
    /// Step 1. Take the health pack out of the inventory.
    HealthPack *pHealthpack = s_pPlayer->m_pInventory[s_pPlayer->m_index];

    /// Step 2. Add the value from the health pack to the players health.
    AddHealth(pHealthpack->m_healthAmount);

    /// Step 3. Remove health pack from inventory and item types array.
    s_pPlayer->m_pInventory[s_pPlayer->m_index] = NULL;
    s_pPlayer->m_itemTypes[s_pPlayer->m_index] = ERROR_INDICATOR;

    /// Step 4. After deleting an old inventory item reset the index to the top of the inventory.
    s_pPlayer->m_index = 0;
}

static void SelectItem()
{
    switch(s_pPlayer->m_itemTypes[s_pPlayer->m_index])
    {
    case 0:
        EquipFlashlight();
        break;
    case 1:
        UseBattery();
        break;
    case 2:
        UseHealthpack();
        break;
    default:
        printf("Error! File: Player.c. Function: EquipOrUseSelectedItem().\n");
        break;
    }
}

static void Ascend()
{
    /// Step 1. In the function Ascend() we safely know the search for the previous non-null index will be going "upward" or a decreasing linear search.
    int locateNonNullIndex = s_pPlayer->m_index - 1;

    while(locateNonNullIndex >= 0)
    {
        /// Step 2. If Step 2 says our desired position is fine, then proceed to check that position in the inventory for NULL.
        if(s_pPlayer->m_pInventory[locateNonNullIndex] == NULL)
        {
            /// (Optional Step) If this space is NULL, we don't want it. Let's try the next index. Decrement locateNonNullIndex.
            --locateNonNullIndex;
        }

        else
        {
            /// Step 3. If the index in inventory is not NULL, we successfully found a position, assign index to s_pPlayer->m_index and break from loop.
            s_pPlayer->m_index = locateNonNullIndex;
            break;
        }
    }
}

static void Descend()
{
    /// Step 1. In the function Descend() we safely know the search for the next non-null index will be going "downward" or an increasing linear search.
    int locateNonNullIndex = s_pPlayer->m_index + 1;

    while(locateNonNullIndex < MAX_INVENTORY_SPACE)
    {
        /// Step 2. If Step 2 says our desired position is fine, then proceed to check that position in the inventory for NULL.
        if(s_pPlayer->m_pInventory[locateNonNullIndex] == NULL)
        {
            /// (Optional Step) If this space is NULL, we don't want it. Let's try the next index. Increment locateNonNullIndex.
            ++locateNonNullIndex;
        }

        else
        {
            /// Step 3. If the index in inventory is not NULL, we successfully found a position, assign index to s_pPlayer->m_index and break from loop.
            s_pPlayer->m_index = locateNonNullIndex;
            break;
        }
    }
}

static void ApplyMotion(char key)
{
    if(key == 's')
    {
        Descend();
    }

    else if(key == 'w')
    {
        Ascend();
    }
}

static void IndicatorMotion(char *input)
{
    if(s_pPlayer->m_itemCount == 1) /// Player cannot select anything else because they only have 1 item in their inventory.
    {
        return;
    }

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
        /// Do nothing.
        break;
    }
}

static char ReturnIndicator(int currentIndex)
{
    /// If index 0 is the current index we're checking in the inventory (currentIndex), and m_index is 0, we must return the selection variable.
    if(s_pPlayer->m_index == currentIndex)
    {
        return s_pPlayer->m_selection;
    }
    return s_pPlayer->m_marker;
}

static void DisplayTips()
{
    while(TRUE)
    {
        system("cls");
        printf("__________SURVIVAL TIPS__________\n");
        printf("\n(a) Short range flashlights are better because it gives enemies less chance to find you.\n\n");
        printf("(b) Once you found the door, it's better to go to the next level than wasting battery searching for more items.\n\n");
        printf("(c) You can't get rid of flashlights in your inventory. Save room for health packs and batteries!\n\n");
        printf("Press -q- to return to game.\n\n");

        char input = _getch();
        if(input == 'q')
        {
            break;
        }
    }
}

static void DisplayInstructions()
{
    while(TRUE)
    {
        system("cls");
        printf("__________INSTRUCTIONS__________\n");
        printf("\nNavigate the area with the w/a/s/d keys.\n\n");
        printf("Items are indicated by H - health pack, B - Battery, F - Flashlight.\n\n");
        printf("You CANNOT fight monsters. If they spot you, run!\n\n");
        printf("Find the door %c to go to next level.\n\n", GetDoor());
        printf("Press -q- to return to game.\n\n");

        char input = _getch();
        if(input == 'q')
        {
            break;
        }
    }
}

static void DisplayInventory()
{
    while(TRUE)
    {
        system("cls");
        s_pPlayer->m_itemCount = 0;
        printf("__________INVENTORY__________\n");
        int i;
        for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
        {
            if(s_pPlayer->m_pInventory[i] != NULL)
            {
                if(s_pPlayer->m_itemTypes[i] == 0)
                {
                    Flashlight *pFlashlight = (Flashlight*)s_pPlayer->m_pInventory[i];
                    printf("[%c] Name: %s Charge: %d, Range: %d\n", ReturnIndicator(i), pFlashlight->m_item.m_name, pFlashlight->m_stepsBeforeRecharge, pFlashlight->m_range);
                }

                else if(s_pPlayer->m_itemTypes[i] == 1)
                {
                    Battery *pBattery = (Battery*)s_pPlayer->m_pInventory[i];
                    printf("[%c] Name: %s Recharge amount: %d\n", ReturnIndicator(i), pBattery->m_item.m_name, pBattery->m_rechargeAmount);
                }

                else if(s_pPlayer->m_itemTypes[i] == 2)
                {
                    HealthPack *pHealthPack = (HealthPack*)s_pPlayer->m_pInventory[i];
                    printf("[%c] Name: %s Health amount: %d\n", ReturnIndicator(i), pHealthPack->m_item.m_name, pHealthPack->m_healthAmount);
                }
                s_pPlayer->m_itemCount += 1;
                printf("\n");
            }
        }

        printf("Use the -w/s- keys to navigate up and down.\n");
        printf("Press -u- to use an item.\n");
        printf("Press -q- to return to game.\n\n");

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

static void CheckBounds(char *input)
{
    int rows = ROWS - 1;
    int columns = COLUMNS - 1;

    switch(*input)
    {
    case 'a':
        if(s_pPlayer->m_playerPosition.y - 1 < 0)
        {
            return;
        }
        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.y -= 1;
        break;
    case 'd':
        if(s_pPlayer->m_playerPosition.y + 1 > columns)
        {
            return;
        }
        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.y += 1;
        break;
    case 'w':
        if(s_pPlayer->m_playerPosition.x - 1 < 0)
        {
            return;
        }
        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.x -= 1;
        break;
    case 's':
        if(s_pPlayer->m_playerPosition.x + 1 > rows)
        {
            return;
        }
        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.x += 1;
        break;
    case 'o':
        DisplayInventory();
        break;
    case 'i':
        DisplayInstructions();
        break;
    case 't':
        DisplayTips();
        break;
    default:
        /// No need for error message here.
        break;
    }
}

/**
                                          THE ABOVE STATIC FUNCTIONS ARE C STYLE "PRIVATE" AND
                                        DECLARED BEFORE THE PRIMARY LIST OF FUNCTIONS IN PLAYER.H
*/

void PlayerInit()
{
    int rows = ROWS - 1;
    int columns = COLUMNS - 1;

    s_pPlayer = malloc(sizeof(Player));
    s_pPlayer->m_symbol = '$';
    s_pPlayer->m_health = 100;
    s_pPlayer->m_playerPosition.x = rows;
    s_pPlayer->m_playerPosition.y = columns;
    s_pPlayer->m_pEquippedFlashlight = malloc(sizeof(Flashlight));
    s_pPlayer->m_pEquippedFlashlight = NULL;
    s_pPlayer->m_selection = 'S';
    s_pPlayer->m_marker = ' ';
    s_pPlayer->m_index = 0;
    s_pPlayer->m_itemCount = 0;

    int i;
    for(i = 0; i < MAX_INVENTORY_SPACE; ++i)
    {
        s_pPlayer->m_pInventory[i] = NULL;
        s_pPlayer->m_itemTypes[i] = ERROR_INDICATOR;
    }
}

void PlayerGUI()
{
    printf("\nHealth: %d - Steps before battery dies: %d Max inventory space: %d\n", s_pPlayer->m_health, s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge, MAX_INVENTORY_SPACE);
    printf("Escape before monsters destroy you!\n");
    printf("[o]Open Inventory [i]Instructions [t]Tips [q]Quit");
}

char GetPlayer()
{
    return s_pPlayer->m_symbol;
}

Point GetPlayerPosition()
{
    return s_pPlayer->m_playerPosition;
}

void ResetPlayerPositionToDefault()
{
    s_pPlayer->m_playerPosition.x = ROWS - 1;
    s_pPlayer->m_playerPosition.y = COLUMNS - 1;
}

void AdjustHealth(int x)
{
    s_pPlayer->m_health = x;
}

int GetPlayerHealth()
{
    return s_pPlayer->m_health;
}

Flashlight *GetFlashLight()
{
    return s_pPlayer->m_pEquippedFlashlight;
};

void AddItemToInventory(void *pItem, int itemType)
{
    int index = GetPointerArrayPos(s_pPlayer->m_pInventory, MAX_INVENTORY_SPACE);

    if(index == ERROR_INDICATOR)
    {
        return;
    }

    int itemTypesIndex = GetIntArrayPos(s_pPlayer->m_itemTypes, MAX_INVENTORY_SPACE);

    if(itemType == 0)
    {
        Flashlight *pFL = (Flashlight*)pItem;

        s_pPlayer->m_pInventory[index] = pFL;
        s_pPlayer->m_itemTypes[itemTypesIndex] = itemType;

        if(s_pPlayer->m_pEquippedFlashlight == NULL)
        {
            s_pPlayer->m_pEquippedFlashlight = (Flashlight*)s_pPlayer->m_pInventory[index];
            s_pPlayer->m_pEquippedFlashlight->m_range = 3;
        }
    }

    else if(itemType == 1)
    {
        Battery *pBat = (Battery*)pItem;

        s_pPlayer->m_pInventory[index] = pBat;
        s_pPlayer->m_itemTypes[itemTypesIndex] = itemType;
    }

    else if(itemType == 2)
    {
        HealthPack *pHP = (HealthPack*)pItem;

        s_pPlayer->m_pInventory[index] = pHP;
        s_pPlayer->m_itemTypes[itemTypesIndex] = itemType;
    }
}

int CheckBatteryPower()
{
    if(s_pPlayer->m_pEquippedFlashlight->m_stepsBeforeRecharge > 0)
    {
        return TRUE;
    }
    return FALSE;
}

char UpdatePlayer()
{
    char input = _getch();
    CheckBounds(&input);
    return input;
}

void PlayerCleanMemory()
{
    free(s_pPlayer->m_pEquippedFlashlight);
    s_pPlayer->m_pEquippedFlashlight = 0;
    free(s_pPlayer);
    s_pPlayer = 0;
}
