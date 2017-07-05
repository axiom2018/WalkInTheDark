/// LevelManagement.c
#include "LevelManagement.h"
#include "Definitions.h"
#include "Boolean.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

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

static Levels s_level = ERROR_INDICATOR;

void InitLevelManagement()
{
    if(s_level != One)
        s_level = One;
}

int GetCurrentLevel()
{
    return (int)s_level;
}

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

int IsGameOver()
{
    if(s_level == GameOver)
    {
        system("cls");
        printf("You survived and escaped! Congratulations!\n");
        return TRUE;
    }
    return FALSE;
}
