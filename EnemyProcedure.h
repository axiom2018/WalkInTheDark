/// EnemyProcedure.h
#ifndef ENEMYPROCEDURE_H_INCLUDED
#define ENEMYPROCEDURE_H_INCLUDED

/// While updating enemies it is necessary to distinguish between what should be updated at a certain time.
typedef enum
{
    UpdateStrategy,
    UpdateMovement,
} EnemyUpdateProcedure;

#endif // ENEMYPROCEDURE_H_INCLUDED
