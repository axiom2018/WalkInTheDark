/**

Name: Walk in the Dark. (Version 2)
Author: Omar Moodie.
Date:

Website: https://omarscode.wordpress.com/
*/

#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "MediatorPattern.h"
#include "EnemyFactory.h"
#include "World.h"
#include "Player.h"
#include "Boolean.h"
#include "ItemManagement.h"

int main()
{
    srand(time(NULL));

    MediatorInit();
    EnemyFactoryInit();
    PlayerInit();
    WorldInit();

    UpdateGame();

    ItemManagementCleanMemory();
    MediatorCleanMemory();
    EnemyFactoryCleanMemory();
    PlayerCleanMemory();
    WorldCleanMemory();

    getch();
    return 0;
}
