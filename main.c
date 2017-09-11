/** Name: Walk in the Dark. (Version 2)
    Author: Omar Moodie.
    Date: 9/11/2017

    Website: https://omarscode.wordpress.com/ */

#include <stdlib.h>
#include <time.h>
#include "World.h"

int main()
{
    srand(time(NULL));

    WorldInit();

    UpdateGame();

    WorldCleanMemory();

    return 0;
}
