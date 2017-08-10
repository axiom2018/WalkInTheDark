/// LevelManagement.c
#include "LevelManagement.h"
#include "Definitions.h"
#include "Boolean.h"
#include "Switch.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

/// Levels in enumeration form.
typedef enum
{
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    GameOver
} Levels;

/// Static Level variable for re-use.
static Levels s_level = ERROR_INDICATOR;

/// Initialization.
void InitLevelManagement()
{
    if(s_level != One)
        s_level = One;
}

/// Return level to clients.
int GetCurrentLevel()
{
    return (int)s_level;
}

/// Advance game level when the following function is called.
void NextLevel()
{
    switch(s_level)
    {
    case One:
        s_level = Two;
        break;
    case Two:
        s_level = Three;
        break;
    case Three:
        s_level = Four;
        break;
    case Four:
        s_level = Five;
        break;
    case Five:
        s_level = Six;
        break;
    case Six:
        s_level = GameOver;
        break;
    default:
        printf("Error! File: LevelManagement.c. Function: NextLevel().\n");
        break;
    }
}

/// Check if game is over.
int IsGameOver()
{
    if(UseSwitch(s_level, GameOver))
    {
        system("cls");
        printf("You survived and escaped! Congratulations!\n");
        return TRUE;
    }

    return FALSE;
}
