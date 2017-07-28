/// ItemManagement.c
#include "Definitions.h"
#include "Flashlight.h"
#include "HealthPack.h"
#include "Battery.h"
#include "Point.h"
#include "ArrayOperations.h"
#include "LevelManagement.h"
#include "Player.h"
#include "LevelManagement.h"
#include "Boolean.h"
#include "World.h"
#include "EnemyMoveData.h"
#include "Door.h"
#include <stdlib.h>
#include <stdio.h>

/// Defined struct that holds all item related data needed.
typedef struct
{
    void *m_pLevelItems[MAX_ITEMS];
    int m_levelItemTypes[MAX_ITEMS];
    Point m_generatedItemPoints[MAX_ITEMS];
    int m_itemGeneration;
    int m_itemGenerationAssistance;
} ItemManager;

/// Static manager pointer for re-use.
static ItemManager *s_pItemManager;

/// __________ Private static functions __________
static int CheckItemLevel(int *pItemLevel)
{
    /// Return True if item's assigned level matches current level. Return False if otherwise.
    switch(*pItemLevel == GetCurrentLevel() ? TRUE : FALSE)
    {
    case 1:
        return TRUE;
    case 0:
        return FALSE;
    default:
        printf("Error! File: ItemManagement.c. Function: CheckItemLevel(int *pItemLevel)\n");
        break;
    }
    return ERROR_INDICATOR;
}

/// Depending on the range of the players flashlight, they will get a high or low steps required before the battery needs a recharge.
static void AssignStepsBeforeRecharge(Flashlight *pFL)
{
    switch(pFL->m_range)
    {
    case 2:
        pFL->m_stepsBeforeRecharge = rand() % (200 + 1 - 130) + 130;
        break;
    case 3:
        pFL->m_stepsBeforeRecharge = rand() % (200 + 1 - 130) + 130;
        break;
    case 4:
        pFL->m_stepsBeforeRecharge = rand() % (210 + 1 - 150) + 150;
        break;
    case 5:
        pFL->m_stepsBeforeRecharge = rand() % (190 + 1 - 130) + 130;
        break;
    case 6:
        pFL->m_stepsBeforeRecharge = rand() % (180 + 1 - 120) + 120;
        break;
    default:
        printf("Default case in GivePlayerFlashlight!\n");
        break;
    }
}

/// Sometimes items might get assigned the same position, change that.
static void AssignItemNewPosition(Point *pPrevPos)
{
    pPrevPos->x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
    pPrevPos->y = rand() % ((ROWS - 1) + 1 - 0) + 0;
}

/// Check if items position conflicts with any other already established position.
static int FindDuplicate(Point *pPoint, Point pointToCheck)
{
    /// Step 1. Check the pointer with the normal point variable. If there is a match give the pointer new values.
    if(pPoint->x == pointToCheck.x
       && pPoint->y == pointToCheck.y)
    {
        AssignItemNewPosition(pPoint);
        return TRUE;
    }

    /// Step 2. Check item with door.
    else if(pPoint->x == GetDoor()->m_coord.x
            && pPoint->y == GetDoor()->m_coord.y)
    {
        AssignItemNewPosition(pPoint);
        return TRUE;
    }

    /// Step 3. Check item with player.
    else if(pPoint->x == GetPlayerPosition().x
            && pPoint->y == GetPlayerPosition().y)
    {
        AssignItemNewPosition(pPoint);
        return TRUE;
    }

    /// (Optional Step). Return false if no duplicate was found.
    return FALSE;
}
/// __________ Private static functions __________


/// Clean memory to avoid leaks.
void ItemManagementCleanMemory()
{
    int a;
    for(a = 0; a < MAX_ITEMS; ++a)
    {
        if(s_pItemManager->m_pLevelItems[a] != NULL)
        {
            free(s_pItemManager->m_pLevelItems[a]);
            s_pItemManager->m_pLevelItems[a] = 0;
        }
    }

    free(s_pItemManager);
    s_pItemManager = 0;
}

int CheckGeneratedItemsForDuplicatePositions()
{
    int i, t;
    for(i = 0; i < ITEMS_PER_LEVEL; ++i)
    {
        Point *pPoint = &s_pItemManager->m_generatedItemPoints[i];

        for(t = 0; t < ITEMS_PER_LEVEL; ++t)
        {
            if(i != t)
            {
                Point pointToCompare = s_pItemManager->m_generatedItemPoints[t];

                if(FindDuplicate(pPoint, pointToCompare))
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

Point GetItemPoint(int index)
{
    return s_pItemManager->m_generatedItemPoints[index];
}

void GivePlayerItem(int itemType)
{
    Flashlight *pFlashlight;
    Battery *pBattery;
    HealthPack *pHealthPack;
    void *pItem;

    switch(itemType)
    {
    case 0:
        pFlashlight = malloc(sizeof(Flashlight));
        pFlashlight->m_item.m_name = "Flashlight";
        pFlashlight->m_item.m_symbol = 'F';
        pFlashlight->m_range = rand() % (6 + 1 - 2) + 2;
        AssignStepsBeforeRecharge(pFlashlight);
        pItem = pFlashlight;
        break;
    case 1:
        pBattery = malloc(sizeof(Battery));
        pBattery->m_item.m_name = "Battery";
        pBattery->m_item.m_symbol = 'B';
        pBattery->m_rechargeAmount = BATTERY_UPGRADE;
        pItem = pBattery;
        break;
    case 2:
        pHealthPack = malloc(sizeof(HealthPack));
        pHealthPack->m_item.m_name = "HealthPack";
        pHealthPack->m_item.m_symbol = 'H';
        pHealthPack->m_healthAmount = HEALTH_UPGRADE;
        pItem = pHealthPack;
        break;
    default:
        printf("Error! File: ItemManagement.c. Function: GivePlayerItem(int itemType).\n");
        break;
    }

    AddItemToInventory(pItem, itemType);
}

static void AllItemPositions()
{
    int i;
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        if(s_pItemManager->m_generatedItemPoints[i].y != ERROR_INDICATOR &&
           s_pItemManager->m_generatedItemPoints[i].x != ERROR_INDICATOR)
        {
            printf("X: %d , Y: %d\n", s_pItemManager->m_generatedItemPoints[i].x, s_pItemManager->m_generatedItemPoints[i].y);
        }
    }
}

static int

void ItemCollisionDetection()
{
    int i;
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        /// Check for error indicator to see if value is meaningful.
        /// Check the items level.
        if(s_pItemManager->m_generatedItemPoints[i].y != ERROR_INDICATOR
           && s_pItemManager->m_generatedItemPoints[i].x != ERROR_INDICATOR
           && s_pItemManager->)
        {

        }

        if(GetPlayerPosition().x == s_pItemManager->m_generatedItemPoints[i].y &&
           GetPlayerPosition().y == s_pItemManager->m_generatedItemPoints[i].x)
        {
            printf("\nPlayer X: %d", GetPlayerPosition().y);
            printf("\nPlayer Y: %d", GetPlayerPosition().x);
            printf("\nItem X: %d", s_pItemManager->m_generatedItemPoints[i].x);
            printf("\nItem Y: %d", s_pItemManager->m_generatedItemPoints[i].y);

            /// Issue with item collision detection here. Player clearly isn't on an item.
            /// But trigger happens.
            /// List all item coordinates to see if it's an item on a different level?

            /// Triggered 21 18 on level 3, maybe it's not being updated properly.
            AllItemPositions();

            break;
        }
    }

    switch(i == MAX_ITEMS ? TRUE : FALSE)
    {
    case 1:
        return;
        break;
    case 0:
        break;
    default:
        printf("Error! File: ItemManagement.c. Function: ItemCollisionDetection().\n");
        break;
    }

    int itemType = s_pItemManager->m_levelItemTypes[i];

    s_pItemManager->m_pLevelItems[i] = NULL;
    s_pItemManager->m_levelItemTypes[i] = ERROR_INDICATOR;
    s_pItemManager->m_generatedItemPoints[i].x = ERROR_INDICATOR;
    s_pItemManager->m_generatedItemPoints[i].y = ERROR_INDICATOR;

    switch(itemType)
    {
    case 0:
        GivePlayerItem(0);
        break;
    case 1:
        GivePlayerItem(1);
        break;
    case 2:
        GivePlayerItem(2);
        break;
    default:
        printf("Error! File: ItemManagement.c. Function: CollisionDetection().\n");
        break;
    }
}

void UpdateEnemyItemData(EMData *pData)
{
    int i;
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        if(s_pItemManager->m_pLevelItems[i] != NULL)
        {
            pData->m_pItemsArr[i] = s_pItemManager->m_pLevelItems[i];
            pData->m_itemTypes[i] = s_pItemManager->m_levelItemTypes[i];
            pData->m_maxItems += 1;
        }
    }
}

int UpdateGeneratedItems(int x, int y)
{
    Flashlight *pFlashlight;
    Battery *pBattery;
    HealthPack *pHealthPack;

    /// Step 1. Go through the item coordinates for matching position.
    int i;
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        if(x == s_pItemManager->m_generatedItemPoints[i].y
           && y == s_pItemManager->m_generatedItemPoints[i].x)
        {
            break;
        }
    }

    /// Step 2. If i is equal to MAX_ITEMS, no matching item position was found so exit this function.
    switch(i == MAX_ITEMS ? TRUE : FALSE)
    {
    case 1:
        return 0;
        break;
    case 0:
        break;
    default:
        printf("Error! File: ItemManagement.c. Function: UpdateGeneratedItems(int x, int y) Part 1.\n");
        break;
    }

    /// Step 3. Save type of item we found.
    int itemType = s_pItemManager->m_levelItemTypes[i];

    /// Step 4. Set up other necessary variables.
    int assignedLevel;
    int color;
    char symbol;

    /// Step 5. Get information of matching item.
    switch(itemType)
    {
    case 0:
        pFlashlight = s_pItemManager->m_pLevelItems[i];

        assignedLevel = pFlashlight->m_item.m_assignedLevel;
        color = pFlashlight->m_item.m_color;
        symbol = pFlashlight->m_item.m_symbol;
        break;
    case 1:
        pBattery = s_pItemManager->m_pLevelItems[i];

        assignedLevel = pBattery->m_item.m_assignedLevel;
        color = pBattery->m_item.m_color;
        symbol = pBattery->m_item.m_symbol;
        break;
    case 2:
        pHealthPack = s_pItemManager->m_pLevelItems[i];

        assignedLevel = pHealthPack->m_item.m_assignedLevel;
        color = pHealthPack->m_item.m_color;
        symbol = pHealthPack->m_item.m_symbol;
        break;
    default:
        printf("Error! File: ItemManagement.c. Function: UpdateGeneratedItems(int x, int y) Part 2.\n");
        break;
    }

    /// Step 6. Possibly the most important step, check if the item is even assigned to the current level.
    if(!CheckItemLevel(&assignedLevel))
    {
        return FALSE;
    }

    /// Step 7. If step 6 does not fail then  apply text coloring and print out item symbol.
    AdjustTextColor(color);
    printf("%c", symbol);
    AdjustTextColor(7); /// Default color.

    return TRUE;
}

static void CreateFlashlight(Flashlight *pFlashlight, int index, Point p)
{
    pFlashlight->m_item.m_name = "Flashlight";
    pFlashlight->m_item.m_color = 5;
    pFlashlight->m_item.m_symbol = 'F';
    pFlashlight->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Assigned level: %d\n", pFlashlight->m_item.m_assignedLevel);

    pFlashlight->m_item.m_pos.x = p.x;
    pFlashlight->m_item.m_pos.y = p.y;

    pFlashlight->m_range = rand() % (6 + 1 - 3) + 3;
    AssignStepsBeforeRecharge(pFlashlight);

    s_pItemManager->m_pLevelItems[index] = pFlashlight;
}

static void CreateBattery(Battery *pBattery, int index, Point p)
{
    pBattery->m_item.m_name = "Battery";
    pBattery->m_item.m_color = 3;
    pBattery->m_item.m_symbol = 'B';
    pBattery->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Assigned level: %d\n", pBattery->m_item.m_assignedLevel);

    pBattery->m_item.m_pos.x = p.x;
    pBattery->m_item.m_pos.y = p.y;

    pBattery->m_rechargeAmount = BATTERY_UPGRADE;
    s_pItemManager->m_pLevelItems[index] = pBattery;
}

static void CreateHealthPack(HealthPack *pHealthPack, int index, Point p)
{
    pHealthPack->m_item.m_name = "HealthPack";
    pHealthPack->m_item.m_color = 12;
    pHealthPack->m_item.m_symbol = 'H';
    pHealthPack->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Assigned level: %d\n", pHealthPack->m_item.m_assignedLevel);

    pHealthPack->m_item.m_pos.x = p.x;
    pHealthPack->m_item.m_pos.y = p.y;

    pHealthPack->m_healthAmount = HEALTH_UPGRADE;
    s_pItemManager->m_pLevelItems[index] = pHealthPack;
}

void GenerateLevelItems()
{
    static int i;
    int itemType;
    Point pos;

    Flashlight *pFlashlight;
    Battery *pBattery;
    HealthPack *pHealthPack;

    if(i == MAX_ITEMS && s_pItemManager->m_itemGeneration == MAX_ITEMS)
    {
        return; /// Defensive programming. We cannot continue generating past MAX_ITEMS. We however can update previously generated items.
    }

    for(i = s_pItemManager->m_itemGenerationAssistance; i < s_pItemManager->m_itemGeneration; ++i)
    {
        int index = GetPointerArrayPos(s_pItemManager->m_pLevelItems, MAX_ITEMS);

        if(index == ERROR_INDICATOR)
        {
            return;
        }

        int levelItemTypesIndex = GetIntArrayPos(s_pItemManager->m_levelItemTypes, MAX_ITEMS);

        itemType = rand() % (2 + 1 - 0) + 0;

        pos.x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
        pos.y = rand() % ((ROWS - 1) + 1 - 0) + 0;

        switch(itemType)
        {
        case 0:
            pFlashlight = malloc(sizeof(Flashlight));
            CreateFlashlight(pFlashlight, index, pos);
            break;
        case 1:
            pBattery = malloc(sizeof(Battery));
            CreateBattery(pBattery, index, pos);
            break;
        case 2:
            pHealthPack = malloc(sizeof(HealthPack));
            CreateHealthPack(pHealthPack, index, pos);
            break;
        default:
            printf("Error! File: ItemManagement.c. Function: CreateItem(int typeOfItem)\n");
            break;
        }
        s_pItemManager->m_generatedItemPoints[index] = pos;
        s_pItemManager->m_levelItemTypes[levelItemTypesIndex] = itemType;
    }

    s_pItemManager->m_itemGeneration += ITEMS_PER_LEVEL;
    s_pItemManager->m_itemGenerationAssistance = i;

    AllItemPositions();

    system("cls");
}

void InitItemManagement()
{
    s_pItemManager = malloc(sizeof(ItemManager));

    s_pItemManager->m_itemGeneration = ITEMS_PER_LEVEL;
    s_pItemManager->m_itemGenerationAssistance = 0;

    int i;
    for(i = 0; i < MAX_ITEMS; ++i)
    {
        s_pItemManager->m_pLevelItems[i] = NULL;
        s_pItemManager->m_levelItemTypes[i] = ERROR_INDICATOR;
        s_pItemManager->m_generatedItemPoints[i].x = ERROR_INDICATOR;
        s_pItemManager->m_generatedItemPoints[i].y = ERROR_INDICATOR;
    }
}
