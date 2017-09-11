/** Name: Walk in the Dark. (Version 2)
    Author: Omar Moodie.
    Date:

    Website: https://omarscode.wordpress.com/ */

#include <stdlib.h>
#include <time.h>
/// #include <conio.h>
#include "World.h"

int main()
{
    srand(time(NULL));

    WorldInit();

    UpdateGame();

    WorldCleanMemory();

    /// getch();
    return 0;
}
