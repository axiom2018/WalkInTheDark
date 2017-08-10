/// MediatorPattern.h
#ifndef MEDIATORPATTERN_H_INCLUDED
#define MEDIATORPATTERN_H_INCLUDED
#include "Point.h"

/// C style Mediator pattern.

void InitMediator();
void Register(void *pMember, int memberShipType);
void MessageData(Point mainCoordinates[], Point flPoints[]);
void *GetEnemy(int typeOfMonster, int levelToAssign);
void MediatorCleanMemory();

#endif // MEDIATORPATTERN_H_INCLUDED
