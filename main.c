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
