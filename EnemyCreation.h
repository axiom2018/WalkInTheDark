/// EnemyCreation.h
#ifndef ENEMYCREATION_H_INCLUDED
#define ENEMYCREATION_H_INCLUDED
#include "Point.h"

/// Delegate the responsibility of creating enemies and assigning their proper values.
void * RequestToCreateEnemy(int type, int levelAssignment, Point pos);

#endif // ENEMYCREATION_H_INCLUDED
