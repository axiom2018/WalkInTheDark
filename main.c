/**

Name: Walk in the Dark. (Version 1)
Author: Omar Moodie.
Date:

https://omarscode.wordpress.com/
*/

#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "MediatorPattern.h"
#include "EnemyFactory.h"
#include "World.h"
#include "Player.h"
#include "Boolean.h"

void SwitchTest(int first, int second)
{
    switch(first == second ? TRUE : FALSE)
    {
    case 1:
        printf("True!\n");
        break;
    case 0:
        printf("False!\n");
        break;
    default:
        printf("Error!\n");
        break;
    }
}

int main()
{
    srand(time(NULL));

    MediatorInit();
    EnemyFactoryInit();
    PlayerInit();
    WorldInit();

    UpdateGame();

    MediatorCleanMemory();
    EnemyFactoryCleanMemory();
    PlayerCleanMemory();
    WorldCleanMemory();

    getch();
    return 0;
}
