/// World.c
#include "World.h"
#include "Player.h"
#include "LevelManagement.h"
#include "ArrayOperations.h"
#include "Flashlight.h"
#include "HealthPack.h"
#include "Battery.h"
#include "Definitions.h"
#include "Werewolf.h"
#include "Witch.h"
#include "Banshee.h"
#include "EnemyMoveData.h"
#include "EnemyProcedure.h"
#include "ItemManagement.h"
#include "EnemyManagement.h"
#include "FlashlightManagement.h"
#include "EnemyFactory.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/// Defined structure that holds all item related variable data needed.
typedef struct
{
    char m_area[ROWS][COLUMNS];
    int m_gameRunning;
    Door *m_pDoor;
} WorldManager;

/// Static manager pointer for re-use.
static WorldManager *s_pWorld;

/// Every "level" will have a door you must get to in order to advance the game. Case 0 puts the door top left, case 1, top right, case 2, bottom left.
static void GenerateDoor()
{
    s_pWorld->m_pDoor->m_symbol = DOOR_SYMBOL;

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
                continue;
            }

            else
            {
                if(DisplayFlashlight(x, y))
                {
                    if(UpdateGeneratedItems(x, y))
                    {
                        AdjustTextColor(DEFAULT_COLOR);
                    }

                    else
                    {
                        if(x == s_pWorld->m_pDoor->m_coord.y && y == s_pWorld->m_pDoor->m_coord.x)
                        {
                            AdjustTextColor(DOOR_COLOR);
                            printf("%c", s_pWorld->m_pDoor->m_symbol);
                            AdjustTextColor(DEFAULT_COLOR);
                        }

                        else
                        {
                            AdjustTextColor(FLASHLIGHT_COLOR);
                            printf("%c", s_pWorld->m_area[x][y]);
                            AdjustTextColor(DEFAULT_COLOR);
                        }
                    }
                }

                else
                {
                    AdjustTextColor(0);
                    printf("%c", s_pWorld->m_area[x][y]);
                    AdjustTextColor(DEFAULT_COLOR);
                }
            }
        }

        printf("\n");
    }

    PlayerGUI();
}

/// Check if player has hit the door and if so, set up the next level.
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
        GenerateLevelItems();

        /// e) Generate a new door.
        GenerateDoor();

        /// f) Send factory new information.
        UpdateEnemyFactoryData();

        /// g) Generate the next levels enemies.
        GenerateEnemies();

        /// h) Clear screen.
        system("cls");

        GatherFlashlightPoints();

        /// i) Be sure to give enemies a chance to update their strategies.
        UpdateEnemyStrategy(UpdateStrategy);

        /// j) Update screen (along with player GUI).
        UpdateScreen();
    }

    return FALSE;
}

/// Call memory cleaning functions.
void WorldCleanMemory()
{
    ItemManagementCleanMemory();

    EnemyFactoryCleanMemory();

    PlayerCleanMemory();

    EnemyManagementCleanMemory();

    free(s_pWorld->m_pDoor);
    s_pWorld->m_pDoor = 0;

    free(s_pWorld);
    s_pWorld = 0;
}

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

void UpdateGame()
{
    while(GetPlayerHealth() > 0 && s_pWorld->m_gameRunning)
    {
        GatherFlashlightPoints();

        UpdateEnemyStrategy(UpdateStrategy);

        UpdateScreen();

        ItemCollisionDetection();

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
                UpdateEnemyStrategy(UpdateMovement);
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

/// Referenced in EnemyManagement.c, ItemManagement.c, Player.c, and UpdateEnemy.c.
Door * GetDoor()
{
    return s_pWorld->m_pDoor;
}

/// Initialize everything needed for the game to function properly.
void WorldInit()
{
    /// Step 1. Initialize variables.
    s_pWorld = malloc(sizeof(WorldManager));
    s_pWorld->m_pDoor = malloc(sizeof(Door));
    s_pWorld->m_gameRunning = TRUE;

    /// Step 2. Decorate game board.
    int x, y;
    for(x = 0; x < ROWS; ++x)
    {
        for(y = 0; y < COLUMNS; ++y)
        {
            s_pWorld->m_area[x][y] = '.';
        }
    }

    /// Step 3. Initialize other entities.

    InitItemManagement();

    InitEnemyFactory();

    InitPlayer();

    InitFlashlightManagement();

    InitEnemyManagement();

    InitLevelManagement();

    /// Step 4. Set up rest of game.

    GenerateDoor();

    GenerateLevelItems();

    GivePlayerItem(0);

    UpdateEnemyFactoryData();

    GenerateEnemies();

    while(CheckGeneratedItemsForDuplicatePositions())
        continue;
}
