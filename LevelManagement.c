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

/// Advance game level when the following function is called.
void NextLevel()
{
    /// Step 1. Save the current variable in integer format.
    int curLevel = (int)s_level;
    printf("Current level in integer format: %d\n", curLevel);

    /// Step 2. Increment previously made integer.
    ++curLevel;
    printf("Increment integer: %d\n", curLevel);

    /// Step 3. Convert back to enumeration.
    s_level = (Levels)curLevel;
    printf("New level: %d\n", (int)s_level);

    /**
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
    */
}

/// Return level to clients.
int GetCurrentLevel()
{
    return (int)s_level;
}

/// Initialization.
void InitLevelManagement()
{
    if(s_level != One)
        s_level = One;
}
