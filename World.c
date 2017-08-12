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
    Point m_factoryPoints[COORDINATES_TO_SEND];
} WorldManager;

/// Static manager pointer for re-use.
static WorldManager *s_pWorld;

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
                        AdjustTextColor(7);
                    }

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
        GenerateLevelItems();

        /// e) Generate a new door.
        GenerateDoor();

        /// f) Send factory new information.
        UpdateEnemyFactoryData();

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

void WorldInit()
{
    s_pWorld = malloc(sizeof(WorldManager));
    s_pWorld->m_pDoor = malloc(sizeof(Door));
    s_pWorld->m_gameRunning = TRUE;

    int x, y;
    for(x = 0; x < ROWS; ++x)
    {
        for(y = 0; y < COLUMNS; ++y)
        {
            s_pWorld->m_area[x][y] = '.';
        }
    }

    InitItemManagement();

    InitEnemyFactory();

    InitPlayer();

    InitFlashlightManagement();

    InitEnemyManagement();

    InitItemManagement();

    InitLevelManagement(); ///

    GenerateDoor();

    GenerateLevelItems();

    GivePlayerItem(0);

    UpdateEnemyFactoryData();

    GenerateEnemies();

    while(CheckGeneratedItemsForDuplicatePositions())
        continue;
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
