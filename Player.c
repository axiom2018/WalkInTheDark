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
#include "Switch.h"
#include "InventoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    char m_symbol;
    int m_health;
    Point m_playerPosition;
} Player;

/// Static Player pointer for re-use.
static Player *s_pPlayer;

/// Giving the players some helpful advice during the game.
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
        printf("Find the door %c to go to next level.\n\n", GetDoor()->m_symbol);
        printf("Press -q- to return to game.\n\n");

        char input = _getch();
        if(input == 'q')
            break;
    }
}

static void DisplayInventory()
{
    ShowInventory();
}

/// When the player moves, make sure it's not "off the level".
static void CheckBounds(char *input)
{
    /// Step 1. Initialize variables.
    int rows = ROWS - 1;
    int columns = COLUMNS - 1;

    /// Step 2. Process input.
    switch(*input)
    {
    case 'a':
        if(s_pPlayer->m_playerPosition.y - 1 < 0)
            return;

        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.y -= 1;
        break;
    case 'd':
        if(s_pPlayer->m_playerPosition.y + 1 > columns)
            return;

        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.y += 1;
        break;
    case 'w':
        if(s_pPlayer->m_playerPosition.x - 1 < 0)
            return;

        DecreaseStepsBeforeRecharge();
        s_pPlayer->m_playerPosition.x -= 1;
        break;
    case 's':
        if(s_pPlayer->m_playerPosition.x + 1 > rows)
            return;

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
        break;
    }
}

/// Free memory.
void PlayerCleanMemory()
{
    InventoryManagementCleanMemory();

    free(s_pPlayer);
    s_pPlayer = 0;
}

/// Referenced in FlashlightManagement.c
int CheckBatteryPower()
{
    if(CheckBattery())
        return TRUE;

    return FALSE;
}

/// Referenced in InventoryManagement.c,
void AddHealth(int amount)
{
    s_pPlayer->m_health += amount;
}

/// Allow player input.
char UpdatePlayer()
{
    char input = _getch();
    CheckBounds(&input);
    return input;
}

/// Delegate the task to add item inventory to InventoryManagement.c.
void AddItemToInventory(void *pItem, int itemType)
{
    AddToInventory(pItem, itemType);
}

/// Referenced in DarkMovement.c and LightMovement.c.
void AdjustHealth(int x)
{
    s_pPlayer->m_health = x;
}

/// After advancing to a new level, reset the players position. Referenced in World.c.
void ResetPlayerPositionToDefault()
{
    s_pPlayer->m_playerPosition.x = ROWS - 1;
    s_pPlayer->m_playerPosition.y = COLUMNS - 1;
}

/// Simple GUI for player to know the controls.
void PlayerGUI()
{
    printf("\nHealth: %d - Steps before battery dies: %d Max inventory space: %d\n",
           s_pPlayer->m_health, ReturnFlashLight()->m_stepsBeforeRecharge, MAX_INVENTORY_SPACE);
    printf("[o]Open Inventory [i]Instructions [t]Tips [q]Quit");
}

/// Referenced in InventoryManagement.c
Flashlight *GetFlashLight()
{
    return ReturnFlashLight();
};

/// Referenced in UpdateEnemy.c and World.c.
int GetPlayerHealth()
{
    return s_pPlayer->m_health;
}

/// Multiple other files reference this function.
Point GetPlayerPosition()
{
    return s_pPlayer->m_playerPosition;
}

/// Referenced in World.c.
char GetPlayer()
{
    return s_pPlayer->m_symbol;
}

void InitPlayer()
{
    /// Step 1. Set up and initialize all variables.
    s_pPlayer = malloc(sizeof(Player));
    s_pPlayer->m_health = DEFAULT_PLAYER_HEALTH;
    s_pPlayer->m_playerPosition.x = ROWS - 1;
    s_pPlayer->m_playerPosition.y = COLUMNS - 1;
    s_pPlayer->m_symbol = '$';

    /// Step 2. Initialize the actual inventory manager.
    InitInventoryManagement();
}
