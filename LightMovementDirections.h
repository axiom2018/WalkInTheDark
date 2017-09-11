/// LightMovementDirections.h
#ifndef LIGHTMOVEMENTDIRECTIONS_H_INCLUDED
#define LIGHTMOVEMENTDIRECTIONS_H_INCLUDED

/// Referenced in LightMovement.c to help the enemy with moving through light points.

typedef enum
{
    North,
    South,
    East,
    West,
    NorthWest,
    NorthEast,
    SouthWest,
    SouthEast,
    Invalid
} Directions;

#endif // LIGHTMOVEMENTDIRECTIONS_H_INCLUDED
