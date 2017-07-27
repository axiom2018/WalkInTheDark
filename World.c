/// World.c
#include "World.h"
#include "Player.h"
#include "LevelManagement.h"
#include "ArrayOperations.h"
#include "Flashlight.h"
#include "HealthPack.h"
#include "Battery.h"
#include "MediatorPattern.h"
#include "Definitions.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "EnemyMoveData.h"
#include "EnemyProcedure.h"
#include "ItemManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


typedef struct
{
    char m_area[ROWS][COLUMNS];
    int m_gameRunning;

    Point m_flashLightPoints[SIZE_OF_FL_POINTS];
    Door *m_pDoor;

//    void *m_pLevelItems[MAX_ITEMS];
//    int m_levelItemTypes[MAX_ITEMS];
//    Point m_generatedItemPoints[MAX_ITEMS];
    Point m_enemyFactoryMainCoordinates[COORDINATES_TO_SEND];
//    int m_itemGeneration;
//    int m_itemGenerationAssistance;

    void *m_pEnemies[MAX_ENEMIES];
    int m_enemyTypes[MAX_ENEMIES];

} WorldManager;

static WorldManager *s_pWorld;
EMData *pMessageData;

/**
                                        THE FOLLOWING STATIC FUNCTIONS ARE C STYLE "PRIVATE" AND
                                        DECLARED BEFORE THE PRIMARY LIST OF FUNCTIONS IN WORLD.H
*/

/// Using the windows api to change color of text because system() calls are unbelievably slow.
void AdjustTextColor(int value)
{
    WORD colorToAdjust;
    HANDLE hdle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO saveInfo;

    if(GetConsoleScreenBufferInfo(hdle, &saveInfo))
    {
        colorToAdjust = (saveInfo.wAttributes & 0xF0) + (value & 0x0F);
        SetConsoleTextAttribute(hdle, colorToAdjust);
    }
}

static int GetEmptyIndexForFlashlightPoints()
{
    int flPointIndex = GetPointArrayPos(s_pWorld->m_flashLightPoints, SIZE_OF_FL_POINTS);

    if(flPointIndex == ERROR_INDICATOR)
    {
        return ERROR_INDICATOR;
    }
    return flPointIndex;
}

static Point AmountOfEnemiesToGenerate()
{
    /** The enemy generation system will be simple. Based on the level that's the type of enemy will be assigned inside of that level. If it's
    level one, specifically a werewolf will be generated. Level two, witches.*/

    Point numberOfEnemiesToGenerate;
    /** numberOfEnemiesToGenerate.x = How MANY enemies we shall be generating.
    numberOfEnemiesToGenerate.y = The TYPE of enemies to be generated. */

    switch(GetCurrentLevel())
    {
    case 0:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 2); /// Get 1 enemy.
        numberOfEnemiesToGenerate.y = 0;
        break;
    case 1:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 2); /// Get 2 enemies.
        numberOfEnemiesToGenerate.y = 1;
        break;
    case 2:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 3); /// Get 3 enemies.
        numberOfEnemiesToGenerate.y = 1;
        break;
    case 3:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 2);
        numberOfEnemiesToGenerate.y = 2;
        break;
    case 4:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 3);
        numberOfEnemiesToGenerate.y = 2;
        break;
    case 5:
        numberOfEnemiesToGenerate.x = MAX_ENEMIES - (MAX_ENEMIES - 4);
        numberOfEnemiesToGenerate.y = 2;
        break;
    default:
        printf("Error! File: World.c. Function: AmountOfEnemiesToGenerate().\n");
        break;
    }
    return numberOfEnemiesToGenerate;
}

static void UpdateEnemyData()
{
    pMessageData->m_maxMonsters = 0;

    pMessageData->m_maxItems = 0;

    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pWorld->m_pEnemies[i] != NULL)
        {
            pMessageData->m_pMonsterArr[i] = s_pWorld->m_pEnemies[i];
            pMessageData->m_monsterTypes[i] = s_pWorld->m_enemyTypes[i];
            pMessageData->m_maxMonsters += 1;
        }
    }

    //UpdateEnemyItemData(pMessageData);

//    for(i = 0; i < MAX_ITEMS; ++i)
//    {
//        if(s_pWorld->m_pLevelItems[i] != NULL)
//        {
//            pMessageData->m_pItemsArr[i] = s_pWorld->m_pLevelItems[i];
//            pMessageData->m_itemTypes[i] = s_pWorld->m_levelItemTypes[i];
//            pMessageData->m_maxItems += 1;
//        }
//    }

    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(pMessageData->m_flashlightPoints[i].x != ERROR_INDICATOR &&
           pMessageData->m_flashlightPoints[i].y != ERROR_INDICATOR)
        {
            pMessageData->m_flashlightPoints[i].x = ERROR_INDICATOR;
            pMessageData->m_flashlightPoints[i].y = ERROR_INDICATOR;
        }
    }

    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {

        if(s_pWorld->m_flashLightPoints[i].x > ERROR_INDICATOR &&
           s_pWorld->m_flashLightPoints[i].y > ERROR_INDICATOR)
        {
            pMessageData->m_flashlightPoints[i].x = s_pWorld->m_flashLightPoints[i].x;
            pMessageData->m_flashlightPoints[i].y = s_pWorld->m_flashLightPoints[i].y;
        }
    }

    pMessageData->m_pDoorPos = &s_pWorld->m_pDoor->m_coord;

    pMessageData->m_playerPos = GetPlayerPosition();

    pMessageData->m_playerHealth = GetPlayerHealth();

    pMessageData->m_saveCurrentLevel = GetCurrentLevel();
}

static int DisplayEnemies(int x, int y, Point *pEnemyPos)
{
    if(y == pEnemyPos->x &&
       x == pEnemyPos->y)
    {
        return TRUE;
    }
    return FALSE;
}

static int CheckMonstersCurrentLevel(int monsterCurrentLevel)
{
    if(monsterCurrentLevel == GetCurrentLevel())
    {
        return TRUE;
    }
    return FALSE;
}

/// Drawing enemies and updating enemies are two different things. The below function will draw, the function immediately following, will update.
static int DrawEnemies(int x, int y)
{
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;

    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pWorld->m_pEnemies[i] != NULL)
        {
            switch(s_pWorld->m_enemyTypes[i])
            {
            case 0:
                pWerewolf = s_pWorld->m_pEnemies[i];

                if(DisplayEnemies(x, y, &pWerewolf->m_info.m_Pos)
                   && pWerewolf->m_info.m_strategyCheck(&pWerewolf->m_info.m_moveStrategy))
                {
                    /// Don't bother if the enemy in the array isn't assigned to the level we're currently on.
                    if(!CheckMonstersCurrentLevel(pWerewolf->m_info.m_assignedLevel))
                        continue;

                    /// Display enemy.
                    printf("%c", pWerewolf->m_info.m_symbol);
                    return TRUE;
                }
                break;
            case 1:
                pWitch = s_pWorld->m_pEnemies[i];

                if(DisplayEnemies(x, y, &pWitch->m_info.m_Pos)
                   && pWitch->m_info.m_strategyCheck(&pWitch->m_info.m_moveStrategy))
                {
                    if(!CheckMonstersCurrentLevel(pWitch->m_info.m_assignedLevel))
                        continue;

                    printf("%c", pWitch->m_info.m_symbol);
                    return TRUE;
                }
                break;
            case 2:
                pBanshee = s_pWorld->m_pEnemies[i];

                if(DisplayEnemies(x, y, &pBanshee->m_info.m_Pos)
                   && pBanshee->m_info.m_strategyCheck(&pBanshee->m_info.m_moveStrategy))
                {
                    if(!CheckMonstersCurrentLevel(pBanshee->m_info.m_assignedLevel))
                        continue;

                    printf("%c", pBanshee->m_info.m_symbol);
                    return TRUE;
                }
                break;
            default:
                printf("Error! File: World.c. Function: DrawEnemies().\n");
                break;
            }
        }
    }
    return FALSE;
}

/** Regardless of the current level the player is in, I will update all enemies. However, the only enemies that will be visually printed is the are the ones
that are on the same level as the player.*/
static int UpdateEnemies(EnemyUpdateProcedure EUP)
{
    Werewolf *pWerewolf;
    Witch *pWitch;
    Banshee *pBanshee;

    int i;
    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        if(s_pWorld->m_pEnemies[i] != NULL)
        {
            switch(s_pWorld->m_enemyTypes[i])
            {
            case 0:
                pWerewolf = s_pWorld->m_pEnemies[i];

                /// Don't bother if the enemy in the array isn't assigned to the level we're currently on.
                if(!CheckMonstersCurrentLevel(pWerewolf->m_info.m_assignedLevel))
                    continue;

                /// Enemy position might be changed and must be given.
                pMessageData->m_pEnemyPos = &pWerewolf->m_info.m_Pos;
                /// Pass the memory address of the enemies movement delay variable.
                pMessageData->m_pEnemyMovementDelay = &pWerewolf->m_info.m_movementDelay;
                /// In case of collision with player we must have the enemy's damage rate.
                pMessageData->m_enemyDamageRate = pWerewolf->m_info.m_damage;
                /// Also pass the movement delays.
                pMessageData->m_enemyDefaultMovementDelay = pWerewolf->m_info.m_defaultMovementDelay;
                pMessageData->m_pEnemyMovementDelay = &pWerewolf->m_info.m_movementDelay;
                /// Update other info.
                UpdateEnemyData();

                /// Depending on where the enemy is, a certain type of strategy will be selected.
                if(EUP == UpdateStrategy)
                    pWerewolf->m_info.m_selectMovementStrategy(pMessageData, &pWerewolf->m_info.m_moveStrategy);
                else if(EUP == UpdateMovement)
                    pWerewolf->m_info.m_moveStrategy(pMessageData);
                break;
            case 1:
                pWitch = s_pWorld->m_pEnemies[i];

                /// Don't bother if the enemy in the array isn't assigned to the level we're currently on.
                if(!CheckMonstersCurrentLevel(pWitch->m_info.m_assignedLevel))
                    continue;

                pMessageData->m_pEnemyPos = &pWitch->m_info.m_Pos;
                pMessageData->m_pEnemyMovementDelay = &pWitch->m_info.m_movementDelay;
                pMessageData->m_enemyDamageRate = pWitch->m_info.m_damage;
                pMessageData->m_enemyDefaultMovementDelay = pWitch->m_info.m_defaultMovementDelay;
                pMessageData->m_pEnemyMovementDelay = &pWitch->m_info.m_movementDelay;
                UpdateEnemyData();

                if(EUP == UpdateStrategy)
                    pWitch->m_info.m_selectMovementStrategy(pMessageData, &pWitch->m_info.m_moveStrategy);
                else if(EUP == UpdateMovement)
                    pWitch->m_info.m_moveStrategy(pMessageData);
                break;
            case 2:
                pBanshee = s_pWorld->m_pEnemies[i];

                /// Don't bother if the enemy in the array isn't assigned to the level we're currently on.
                if(!CheckMonstersCurrentLevel(pBanshee->m_info.m_assignedLevel))
                    continue;

                pMessageData->m_pEnemyPos = &pBanshee->m_info.m_Pos;
                pMessageData->m_pEnemyMovementDelay = &pBanshee->m_info.m_movementDelay;
                pMessageData->m_enemyDamageRate = pBanshee->m_info.m_damage;
                pMessageData->m_enemyDefaultMovementDelay = pBanshee->m_info.m_defaultMovementDelay;
                pMessageData->m_pEnemyMovementDelay = &pBanshee->m_info.m_movementDelay;
                UpdateEnemyData();

                if(EUP == UpdateStrategy)
                    pBanshee->m_info.m_selectMovementStrategy(pMessageData, &pBanshee->m_info.m_moveStrategy);
                else if(EUP == UpdateMovement)
                    pBanshee->m_info.m_moveStrategy(pMessageData);
                break;
            default:
                printf("Error! File: World.c. Function: UpdateEnemies().\n");
                break;
            }
        }
    }
    return FALSE;
}

///// Depending on the range of the players flashlight, they will get a high or low steps required before the battery needs a recharge.
//static void AssignStepsBeforeRecharge(Flashlight *pFL)
//{
//    switch(pFL->m_range)
//    {
//    case 2:
//        pFL->m_stepsBeforeRecharge = rand() % (200 + 1 - 130) + 130;
//        break;
//    case 3:
//        pFL->m_stepsBeforeRecharge = rand() % (200 + 1 - 130) + 130;
//        break;
//    case 4:
//        pFL->m_stepsBeforeRecharge = rand() % (210 + 1 - 150) + 150;
//        break;
//    case 5:
//        pFL->m_stepsBeforeRecharge = rand() % (190 + 1 - 130) + 130;
//        break;
//    case 6:
//        pFL->m_stepsBeforeRecharge = rand() % (180 + 1 - 120) + 120;
//        break;
//    default:
//        printf("Default case in GivePlayerFlashlight!\n");
//        break;
//    }
//}

/// Every "level" will have a door you must get to in order to advance the game. Case 0 puts the door top left, case 1, top right, case 2, bottom left.
static void GenerateDoor()
{
    s_pWorld->m_pDoor->m_symbol = 178;

    int pos = rand() % (2 + 1 - 0) + 0;

    switch(pos)
    {
    case 0:
        s_pWorld->m_pDoor->m_coord.x = 0;
        s_pWorld->m_pDoor->m_coord.y = 0;
        break;
    case 1:
        s_pWorld->m_pDoor->m_coord.x = COLUMNS - 1;
        s_pWorld->m_pDoor->m_coord.y = 0;
        break;
    case 2:
        s_pWorld->m_pDoor->m_coord.x = 0;
        s_pWorld->m_pDoor->m_coord.y = ROWS - 1;
        break;
    default:
        printf("Error! File: World.c. Function: GenerateDoor().\n");
        break;
    }
}

//static void GenerateLevelItems()
//{
//    static int i;
//    int itemType;
//    Point pos;
//
//    Flashlight *pFlashlight;
//    Battery *pBattery;
//    HealthPack *pHealthPack;
//
//    if(i == MAX_ITEMS && s_pWorld->m_itemGeneration == MAX_ITEMS)
//    {
//        return; /// Defensive programming. We cannot continue generating past MAX_ITEMS. We however can update previously generated items.
//    }
//
//    for(i = s_pWorld->m_itemGenerationAssistance; i < s_pWorld->m_itemGeneration; ++i)
//    {
//        int index = GetPointerArrayPos(s_pWorld->m_pLevelItems, MAX_ITEMS);
//
//        if(index == ERROR_INDICATOR)
//        {
//            return;
//        }
//
//        int levelItemTypesIndex = GetIntArrayPos(s_pWorld->m_levelItemTypes, MAX_ITEMS);
//
//        itemType = rand() % (2 + 1 - 0) + 0;
//
//        pos.x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
//        pos.y = rand() % ((ROWS - 1) + 1 - 0) + 0;
//
//        switch(itemType)
//        {
//        case 0:
//            pFlashlight = malloc(sizeof(Flashlight));
//            pFlashlight->m_item.m_name = "Flashlight";
//            pFlashlight->m_item.m_color = 5;
//            pFlashlight->m_item.m_symbol = 'F';
//            pFlashlight->m_item.m_assignedLevel = GetCurrentLevel();
//
//            pFlashlight->m_item.m_pos.x = pos.x;
//            pFlashlight->m_item.m_pos.y = pos.y;
//
//            pFlashlight->m_range = rand() % (6 + 1 - 3) + 3;
//            AssignStepsBeforeRecharge(pFlashlight);
//
//            s_pWorld->m_pLevelItems[index] = pFlashlight;
//            break;
//        case 1:
//            pBattery = malloc(sizeof(Battery));
//            pBattery->m_item.m_name = "Battery";
//            pBattery->m_item.m_color = 3;
//            pBattery->m_item.m_symbol = 'B';
//            pBattery->m_item.m_assignedLevel = GetCurrentLevel();
//
//            pBattery->m_item.m_pos.x = pos.x;
//            pBattery->m_item.m_pos.y = pos.y;
//
//            pBattery->m_rechargeAmount = BATTERY_UPGRADE;
//            s_pWorld->m_pLevelItems[index] = pBattery;
//            break;
//        case 2:
//            pHealthPack = malloc(sizeof(HealthPack));
//            pHealthPack->m_item.m_name = "HealthPack";
//            pHealthPack->m_item.m_color = 12;
//            pHealthPack->m_item.m_symbol = 'H';
//            pHealthPack->m_item.m_assignedLevel = GetCurrentLevel();
//
//            pHealthPack->m_item.m_pos.x = pos.x;
//            pHealthPack->m_item.m_pos.y = pos.y;
//
//            pHealthPack->m_healthAmount = HEALTH_UPGRADE;
//            s_pWorld->m_pLevelItems[index] = pHealthPack;
//            break;
//        default:
//            printf("Error! File: World.c. Function: CreateItem(int typeOfItem)\n");
//            break;
//        }
//        s_pWorld->m_generatedItemPoints[index] = pos;
//        s_pWorld->m_levelItemTypes[levelItemTypesIndex] = itemType;
//    }
//    s_pWorld->m_itemGeneration += ITEMS_PER_LEVEL;
//    s_pWorld->m_itemGenerationAssistance = i;
//}

//static void AssignItemNewPosition(Point *pPrevPos)
//{
//    pPrevPos->x = rand() % ((COLUMNS - 1) + 1 - 0) + 0;
//    pPrevPos->y = rand() % ((ROWS - 1) + 1 - 0) + 0;
//}

//static int FindDuplicate(Point *pPoint, Point pointToCheck)
//{
//    /// Step 1. Check the pointer with the normal point variable. If there is a match give the pointer new values.
//    if(pPoint->x == pointToCheck.x
//       && pPoint->y == pointToCheck.y)
//    {
//        AssignItemNewPosition(pPoint);
//        return TRUE;
//    }
//
//    /// Step 2. Check item with door.
//    else if(pPoint->x == s_pWorld->m_pDoor->m_coord.x
//            && pPoint->y == s_pWorld->m_pDoor->m_coord.y)
//    {
//        AssignItemNewPosition(pPoint);
//        return TRUE;
//    }
//
//    /// Step 3. Check item with player.
//    else if(pPoint->x == GetPlayerPosition().x
//            && pPoint->y == GetPlayerPosition().y)
//    {
//        AssignItemNewPosition(pPoint);
//        return TRUE;
//    }
//
//    /// (Optional Step). Return false if no duplicate was found.
//    return FALSE;
//}
//
//static int CheckGeneratedItemsForDuplicatePositions()
//{
//    int i, t;
//    for(i = 0; i < ITEMS_PER_LEVEL; ++i)
//    {
//        Point *pPoint = &s_pWorld->m_generatedItemPoints[i];
//
//        for(t = 0; t < ITEMS_PER_LEVEL; ++t)
//        {
//            if(i != t)
//            {
//                Point pointToCompare = s_pWorld->m_generatedItemPoints[t];
//
//                if(FindDuplicate(pPoint, pointToCompare))
//                {
//                    return TRUE;
//                }
//            }
//        }
//    }
//    return FALSE;
//}

/// Give the player items they pick up in the level. STRUCT DECLARATIONS HERE.
//static void GivePlayerItem(int itemType)
//{
//    Flashlight *pFlashlight;
//    Battery *pBattery;
//    HealthPack *pHealthPack;
//    void *pItem;
//
//    switch(itemType)
//    {
//    case 0:
//        pFlashlight = malloc(sizeof(Flashlight));
//        pFlashlight->m_item.m_name = "Flashlight";
//        pFlashlight->m_item.m_symbol = 'F';
//        pFlashlight->m_range = rand() % (6 + 1 - 2) + 2;
//        AssignStepsBeforeRecharge(pFlashlight);
//        pItem = pFlashlight;
//        break;
//    case 1:
//        pBattery = malloc(sizeof(Battery));
//        pBattery->m_item.m_name = "Battery";
//        pBattery->m_item.m_symbol = 'B';
//        pBattery->m_rechargeAmount = BATTERY_UPGRADE;
//        pItem = pBattery;
//        break;
//    case 2:
//        pHealthPack = malloc(sizeof(HealthPack));
//        pHealthPack->m_item.m_name = "HealthPack";
//        pHealthPack->m_item.m_symbol = 'H';
//        pHealthPack->m_healthAmount = HEALTH_UPGRADE;
//        pItem = pHealthPack;
//        break;
//    default:
//        printf("Error! File: World.c. Function: GivePlayerItem(int itemType).\n");
//        break;
//    }
//    AddItemToInventory(pItem, itemType);
//}

/// Used in function GatherFlashlightPoints frequently, we must clear out old coordinates. before loading in new ones for the flashlight to work properly.
static void ClearFlashlightPoints()
{
    int n;
    for(n = 0; n < SIZE_OF_FL_POINTS; ++n)
    {
        if(s_pWorld->m_flashLightPoints[n].x != ERROR_INDICATOR &&
           s_pWorld->m_flashLightPoints[n].y != ERROR_INDICATOR)
        {
            s_pWorld->m_flashLightPoints[n].x = ERROR_INDICATOR;
            s_pWorld->m_flashLightPoints[n].y = ERROR_INDICATOR;
        }
    }
}

static int CrossReferencePoint(int x, int y)
{
    /// Step 1. Check if first flashlight point entry is valid.
    if((s_pWorld->m_flashLightPoints[0].x == ERROR_INDICATOR || s_pWorld->m_flashLightPoints[0].x < 0)
       && (s_pWorld->m_flashLightPoints[0].y == ERROR_INDICATOR || s_pWorld->m_flashLightPoints[0].y < 0))
    {
        return FALSE;
    }

    /// Step 2. If step 1 is not true then proceed to check array for duplicates.
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_pWorld->m_flashLightPoints[i].x > ERROR_INDICATOR
           && s_pWorld->m_flashLightPoints[i].y > ERROR_INDICATOR)
        {
            if(s_pWorld->m_flashLightPoints[i].x == x
               && s_pWorld->m_flashLightPoints[i].y == y)
            {
                /// We found a value that has already been entered. So don't go through with inputing the new one.
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void SetFlashLightPoint(int x, int y)
{
    /// Step 1. Get empty index in flashlight point array with the function GetEmptyIndexForFlashlightPoints().
    int index = GetEmptyIndexForFlashlightPoints();

    /** Step 2. Cross reference the x and y we received here. If CrossReference(x, y) returns true then do not add x and y into the array because
    they were already entered before. If CrossReference(x, y) returns false, we'll add them. */
    if(!CrossReferencePoint(x, y))
    {
        s_pWorld->m_flashLightPoints[index].x = x;
        s_pWorld->m_flashLightPoints[index].y = y;
    }
}

/// Very important algorithm, key to making the flashlight effect work.
static void GatherFlashlightPoints()
{
    /**
    This function will calculate and gather all points to create the flashlight effect.
    1) Check if the players has steps available before having to recharge their battery.
    2) Get flashlight range.
    3) Clear the array m_flashLightPoints, we don't want values to stack and we run out of array room. We do this with the ClearFlashlightPoints() function.
    4) Get starting position for this algorithm. (Why the left middle though?)
    5) Get the northwest position with the flashlights range in mind.

    6) Add the coordinates from the northwest position to the starting position, into the array m_flashlightPoints.
    7) Calculate northeast position with the flashlights range in mind.
    */

    /// This equals 0, return. Players flashlight has no more battery left. Clear all points and exit.
    if(!CheckBatteryPower())
    {
        ClearFlashlightPoints();
        return;
    }

    /// Step 2. Range determines how many "spaces" the flashlight will illuminate.
    int flRange = GetFlashLight()->m_range;

    /// Step 3. MUST reset every time we run this algorithm or else the points will stack, and eventually the array will overflow.
    ClearFlashlightPoints();

    /// Step 4. We'll start calculating the starting position with the players position.
    Point savePlayerPos = GetPlayerPosition();

    int startingPos;
    if(savePlayerPos.x <= flRange) /// Top left corner check. If players pos is <= flRange, we're at the top left corner.
    {
        startingPos = 0;
    }

    else
    {
        startingPos = savePlayerPos.x - flRange;
    }

    SetFlashLightPoint(startingPos, savePlayerPos.y);

    /// Step 5. Get northwest position keeping in mind the flashlights range.
    Point northWestPoint;
    northWestPoint.x = startingPos; /// 17.
    if(savePlayerPos.y - flRange <= 0)
    {
        northWestPoint.y = 0;
    }
    else
    {
        northWestPoint.y = savePlayerPos.y - flRange;
    }

    SetFlashLightPoint(northWestPoint.x, northWestPoint.y);

    /// Step 6.
    Point p;
    int begin = northWestPoint.y;
    int end = savePlayerPos.y;
    while(begin < end)
    {
        p.x = begin;
        p.y = northWestPoint.x;

        SetFlashLightPoint(p.y, p.x);

        ++begin;
    }

    /// (Adjust Steps Here) Step 6.
    Point northEastPoint;
    northEastPoint.x = startingPos;
    int loop = savePlayerPos.y + 1;
    int endPoint = 0;
    if(savePlayerPos.y + flRange >= COLUMNS - 1)
    {
        northEastPoint.y = COLUMNS - 1;
    }
    else
    {
        northEastPoint.y = savePlayerPos.y + flRange;
    }
    endPoint = northEastPoint.y;

    SetFlashLightPoint(northEastPoint.x, northEastPoint.y);

    while(loop < endPoint)
    {
        northEastPoint.x = loop;
        int savePoint = northWestPoint.x;

        SetFlashLightPoint(savePoint, northEastPoint.x); /// UNTESTED
        ++loop;
    }

    int columns = northWestPoint.y;
    int rows = northWestPoint.x;
    int maxRows = 0;
    ++rows;

    if(savePlayerPos.x >= (ROWS - 1) - flRange)
    {
        maxRows = 19;
    }

    else
    {
        maxRows = savePlayerPos.x + flRange;
    }

    while(columns <= northEastPoint.y && rows <= maxRows)
    {
        SetFlashLightPoint(rows, columns);

        ++columns;

        if(columns == northEastPoint.y + 1)
        {
            columns = northWestPoint.y;
            ++rows;
        }
    }
}

//static void SendEnemyFactoryData()
//{
//    /** Step 1. Gather all points. Include
//    a) Generated items.
//    b) Player coordinates.
//    c) Door coordinates. (Use DistributeMessage).
//    d) FL points (When creating enemies, this ensures we don't create them in the players flashlight space.) */
//    GatherFlashlightPoints();
//
//    /// Step 2. min is a bookmark that keeps the place of where generated item points we want data from.
//    int min;
//    switch(GetCurrentLevel())
//    {
//    case 0: /// 0 - 3
//        min = 0;
//        break;
//    case 1: /// 4 - 7
//        min = 4;
//        break;
//    case 2: /// 8 - 11
//        min = 8;
//        break;
//    case 3: /// 12 - 15
//        min = 12;
//        break;
//    case 4: /// 16 - 19
//        min = 16;
//        break;
//    case 5: /// 20 - 23
//        min = 20;
//        break;
//    default:
//        printf("Error! File: World.c. Function: SendMonsterFactoryPoints().\n");
//        break;
//    }
//
//    /// Step 3. Begin assigning values.
//    int i;
//    for(i = 0; i < ITEMS_PER_LEVEL; ++i)
//    {
//        s_pWorld->m_enemyFactoryMainCoordinates[i] = s_pWorld->m_generatedItemPoints[min];
//        ++min;
//    }
//
//    /// Step 4. Save player position.
//    s_pWorld->m_enemyFactoryMainCoordinates[4] = GetPlayerPosition();
//
//    /// Step 5. Save door position.
//    s_pWorld->m_enemyFactoryMainCoordinates[5] = s_pWorld->m_pDoor->m_coord;
//
//    /// Step 6. Send data to the enemy factory so it knows what points to NOT assign enemies.
//    MessageData(s_pWorld->m_enemyFactoryMainCoordinates, s_pWorld->m_flashLightPoints);
//}

static void GenerateEnemies()
{
    int i;
    /// Step 1. Receive the amount of enemies to be generated based on the current level.
    Point enemiesToGenerate = AmountOfEnemiesToGenerate();

    for(i = 0; i < enemiesToGenerate.x; ++i)
    {
        /// Step 2. Find an empty position for m_pEnemies.
        int index = GetPointerArrayPos(s_pWorld->m_pEnemies, MAX_ENEMIES);
        if(index == ERROR_INDICATOR)
        {
            printf("Enemy generation array full!\n");
            return;
        }

        /// Step 3. Find an empty position for m_enemyTypes.
        int enemyTypesIndex = GetIntArrayPos(s_pWorld->m_enemyTypes, MAX_ENEMIES);

        /// Step 4. Save the enemy and it's type in the following arrays.
        s_pWorld->m_pEnemies[index] = GetEnemy(enemiesToGenerate.y, GetCurrentLevel());
        s_pWorld->m_enemyTypes[enemyTypesIndex] = enemiesToGenerate.y;
    }
}

/// Checks to see if a coordinate being printed in the loop in the update function is a flashlight coordinate, if so, alter color to create the flashlight effect.
static int DisplayFlashlight(int x, int y)
{
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_pWorld->m_flashLightPoints[i].x != ERROR_INDICATOR &&
           s_pWorld->m_flashLightPoints[i].y != ERROR_INDICATOR)
        {
            if(s_pWorld->m_flashLightPoints[i].x == x &&
               s_pWorld->m_flashLightPoints[i].y == y)
            {
                return TRUE;
            }
        }

        else
        {
            break;
        }
    }
    return FALSE;
}

//static int CheckItemLevel(int *pItemLevel)
//{
//    /// Return True if item's assigned level matches current level. Return False if otherwise.
//    switch(*pItemLevel == GetCurrentLevel() ? TRUE : FALSE)
//    {
//    case 1:
//        return TRUE;
//        break;
//    case 0:
//        return FALSE;
//        break;
//    default:
//        printf("Error! File: World.c. Function: CheckItemLevel(int *pItemLevel)\n");
//        break;
//    }
//    return ERROR_INDICATOR;
//}

//static int UpdateGeneratedItems(int x, int y)
//{
//    Flashlight *pFlashlight;
//    Battery *pBattery;
//    HealthPack *pHealthPack;
//
//    /// Step 1. Go through the item coordinates for matching position.
//    int i;
//    for(i = 0; i < MAX_ITEMS; ++i)
//    {
//        if(x == s_pWorld->m_generatedItemPoints[i].y
//           && y == s_pWorld->m_generatedItemPoints[i].x)
//        {
//            break;
//        }
//    }
//
//    /// Step 2. If i is equal to MAX_ITEMS, no matching item position was found. Exit this function.
//    switch(i == MAX_ITEMS ? TRUE : FALSE)
//    {
//    case 1:
//        return 0;
//        break;
//    case 0:
//        break;
//    default:
//        printf("Error! File: World.c. Function: UpdateGeneratedItems(int x, int y) Part 1.\n");
//        break;
//    }
//
//    /// Step 3. Save type of item we found.
//    int itemType = s_pWorld->m_levelItemTypes[i];
//
//    /// Step 4. Set up other necessary variables.
//    int assignedLevel;
//    int color;
//    char symbol;
//
//    /// Step 5. Get information of matching item.
//    switch(itemType)
//    {
//    case 0:
//        pFlashlight = s_pWorld->m_pLevelItems[i];
//
//        assignedLevel = pFlashlight->m_item.m_assignedLevel;
//        color = pFlashlight->m_item.m_color;
//        symbol = pFlashlight->m_item.m_symbol;
//        break;
//    case 1:
//        pBattery = s_pWorld->m_pLevelItems[i];
//
//        assignedLevel = pBattery->m_item.m_assignedLevel;
//        color = pBattery->m_item.m_color;
//        symbol = pBattery->m_item.m_symbol;
//        break;
//    case 2:
//        pHealthPack = s_pWorld->m_pLevelItems[i];
//
//        assignedLevel = pHealthPack->m_item.m_assignedLevel;
//        color = pHealthPack->m_item.m_color;
//        symbol = pHealthPack->m_item.m_symbol;
//        break;
//    default:
//        printf("Error! File: World.c. Function: UpdateGeneratedItems(int x, int y) Part 2.\n");
//        break;
//    }
//
//    /// Step 6. Possibly the most important step, check if the item is even assigned to the current level.
//    if(!CheckItemLevel(&assignedLevel))
//    {
//        return FALSE;
//    }
//
//    /// Step 7. If step 6 does not fail then  apply text coloring and print out item symbol.
//    AdjustTextColor(color);
//    printf("%c", symbol);
//
//    return TRUE;
//}

/// Simple collision detection system for the items on the current level. Runs in O(n).
//static void ItemCollisionDetection()
//{
//    int i;
//    for(i = 0; i < MAX_ITEMS; ++i)
//    {
//        if(GetPlayerPosition().x == s_pWorld->m_generatedItemPoints[i].y &&
//           GetPlayerPosition().y == s_pWorld->m_generatedItemPoints[i].x)
//        {
//            break;
//        }
//    }
//
//    switch(i == MAX_ITEMS ? TRUE : FALSE)
//    {
//    case 1:
//        return;
//        break;
//    case 0:
//        break;
//    default:
//        printf("Error! File: World.c. Function: ItemCollisionDetection().\n");
//        break;
//    }
//
//    int itemType = s_pWorld->m_levelItemTypes[i];
//
//    s_pWorld->m_pLevelItems[i] = NULL;
//    s_pWorld->m_levelItemTypes[i] = ERROR_INDICATOR;
//    s_pWorld->m_generatedItemPoints[i].x = ERROR_INDICATOR;
//    s_pWorld->m_generatedItemPoints[i].y = ERROR_INDICATOR;
//
//    switch(itemType)
//    {
//    case 0:
//        GivePlayerItem(0);
//        break;
//    case 1:
//        GivePlayerItem(1);
//        break;
//    case 2:
//        GivePlayerItem(2);
//        break;
//    default:
//        printf("Error! File: World.c. Function: CollisionDetection().\n");
//        break;
//    }
//}

static void UpdateScreen()
{
    int x, y;
    for(x = 0; x < ROWS; ++x)
    {
        for(y = 0; y < COLUMNS; ++y)
        {
            if(x == GetPlayerPosition().x && y == GetPlayerPosition().y)
            {
                printf("%c", GetPlayer());
            }

            else if(DrawEnemies(x, y))
            {
            }

            else
            {
                if(DisplayFlashlight(x, y))
                {
                    ///UpdateGeneratedItems(int x, int y)
                    if(UpdateGeneratedItems(x, y))
                    {
                        AdjustTextColor(7);
                    }

//                    if(UpdateGeneratedItems(x, y))
//                    {
//                        AdjustTextColor(7);
//                    }

                    else
                    {
                        if(x == s_pWorld->m_pDoor->m_coord.y && y == s_pWorld->m_pDoor->m_coord.x)
                        {
                            AdjustTextColor(4);
                            printf("%c", s_pWorld->m_pDoor->m_symbol);
                            AdjustTextColor(7);
                        }

                        else
                        {
                            AdjustTextColor(15);
                            printf("%c", s_pWorld->m_area[x][y]);
                            AdjustTextColor(7);
                        }
                    }
                }

                else
                {
                    AdjustTextColor(0);
                    printf("%c", s_pWorld->m_area[x][y]);
                    AdjustTextColor(7);
                }
            }
        }
        printf("\n");
    }
    PlayerGUI();
}

static int DoorCollisionDetection()
{
    /// Step 1. Check if door coordinates match players.
    if(GetPlayerPosition().y == s_pWorld->m_pDoor->m_coord.x
       && GetPlayerPosition().x == s_pWorld->m_pDoor->m_coord.y)
    {
        /// Step 2. If coordinates match, prepare for next level. In order to do that I must:

        /// a) Advance to the next level.
        NextLevel();

        if(IsGameOver())
            return TRUE;

        /// b) Clear flashlight points.
        ClearFlashlightPoints();

        /// c) Reset players position.
        ResetPlayerPositionToDefault();

        /// d) Generate the next levels items. (Must review the current implementation.)
        //GenerateLevelItems();

        /// e) Generate a new door.
        GenerateDoor();

        /// f) Send factory new information.
        ///SendEnemyFactoryData();

        /// g) Generate the next levels enemies.
        GenerateEnemies();

        /// h) Clear screen.
        system("cls");

        /// i) Be sure to give enemies a chance to update their strategies.
        UpdateEnemies(UpdateStrategy);

        /// j) Update screen (along with player GUI).
        UpdateScreen();
    }
    return FALSE;
}


/**
                                          THE ABOVE STATIC FUNCTIONS ARE C STYLE "PRIVATE" AND
                                        DECLARED BEFORE THE PRIMARY LIST OF FUNCTIONS IN WORLD.H
*/

void WorldInit()
{
    s_pWorld = malloc(sizeof(WorldManager));
    s_pWorld->m_pDoor = malloc(sizeof(Door));
    s_pWorld->m_gameRunning = TRUE;
    ///s_pWorld->m_itemGeneration = ITEMS_PER_LEVEL;
    ///s_pWorld->m_itemGenerationAssistance = 0;

    pMessageData = malloc(sizeof(EMData));

    int x, y;
    for(x = 0; x < ROWS; ++x)
    {
        for(y = 0; y < COLUMNS; ++y)
        {
            s_pWorld->m_area[x][y] = '.';
        }
    }

    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        s_pWorld->m_flashLightPoints[i].x = ERROR_INDICATOR;
        s_pWorld->m_flashLightPoints[i].y = ERROR_INDICATOR;
    }

//    for(i = 0; i < MAX_ITEMS; ++i)
//    {
//        s_pWorld->m_pLevelItems[i] = NULL;
//        s_pWorld->m_levelItemTypes[i] = ERROR_INDICATOR;
//        s_pWorld->m_generatedItemPoints[i].x = ERROR_INDICATOR;
//        s_pWorld->m_generatedItemPoints[i].y = ERROR_INDICATOR;
//    }

    for(i = 0; i < MAX_ENEMIES; ++i)
    {
        s_pWorld->m_pEnemies[i] = NULL;
        s_pWorld->m_enemyTypes[i] = ERROR_INDICATOR;
    }

    InitLevelManagement();

    InitItemManagement();

    GenerateDoor();

    ///GenerateLevelItems();

    Register(s_pWorld, 1);

    GivePlayerItem(0);

    ///SendEnemyFactoryData(); /// Gathers the flashlight points early to assist enemies in finding valid positions.

    GenerateEnemies();

    ///while(CheckGeneratedItemsForDuplicatePositions())
        ///continue;
}

Door * GetDoor()
{
    return s_pWorld->m_pDoor;
}

void UpdateGame()
{
    while(GetPlayerHealth() > 0 && s_pWorld->m_gameRunning)
    {
        GatherFlashlightPoints();

        UpdateEnemies(UpdateStrategy);

        UpdateScreen();

        ///ItemCollisionDetection();

        if(DoorCollisionDetection())
        {
            s_pWorld->m_gameRunning = FALSE;
        }

        else
        {
            if(UpdatePlayer() == 'q')
            {
                s_pWorld->m_gameRunning = FALSE;
            }

            else
            {
                UpdateEnemies(UpdateMovement);
                system("cls");
            }
        }
    }

    if(GetPlayerHealth() <= 0)
    {
        printf("You were killed by the monsters!\n");
        getch();
    }
}

void WorldCleanMemory()
{
    int a;
//    int a;
//    for(a = 0; a < MAX_ITEMS; ++a)
//    {
//        if(s_pWorld->m_pLevelItems[a] != NULL)
//        {
//            free(s_pWorld->m_pLevelItems[a]);
//            s_pWorld->m_pLevelItems[a] = 0;
//        }
//    }

    for(a = 0; a < MAX_ENEMIES; ++a)
    {
        if(s_pWorld->m_pEnemies[a] != NULL)
        {
            free(s_pWorld->m_pEnemies[a]);
            s_pWorld->m_pEnemies[a] = 0;
        }
    }

    free(s_pWorld->m_pDoor);
    s_pWorld->m_pDoor = 0;

    free(s_pWorld);
    s_pWorld = 0;

    free(pMessageData);
    pMessageData = 0;
}
