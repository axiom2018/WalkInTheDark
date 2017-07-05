/// UniversalMovementFunctions.h
#ifndef UNIVERSALMOVEMENTFUNCTIONS_H_INCLUDED
#define UNIVERSALMOVEMENTFUNCTIONS_H_INCLUDED
#include "Point.h"
#include "EnemyMoveData.h"

/** These for helping an enemy with their movement and it does not matter if they are working with light movement or dark movement. There are some functions
in which both types of movement will need to have access too. An example would be checking if the desired position is in bounds of the maximum rows and
columns, aka, not falling off the edge of the world. */

int RunMovementDelayTest(EMData *pData);
int RunPositionTests(EMData *pData, Point *pDesiredEnemyPos);
int PlayerCollision(Point pPlayerPos, Point pEnemyPos);

#endif // UNIVERSALMOVEMENTFUNCTIONS_H_INCLUDED
