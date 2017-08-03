/// Definitions.h
#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

/** It's easier to make a file with definitions that several entities might need instead of declaring them in a single .c file.
    Also helps avoid hard coding actual values into functions.

    (Keep track of where each definition is used for reference.)
*/

#define ROWS 20
#define COLUMNS 30
#define ERROR_INDICATOR -1
#define ITEMS_PER_LEVEL 4
#define COORDINATES_TO_SEND 6
#define SIZE_OF_FL_POINTS 200
#define MAX_ENEMIES 15
#define MAX_ITEMS 24
#define MAX_DARK_MOVEMENT_POSITIONS 7
#define EPA_MAX_FUNCTIONS 3
#define POINT_ARRAY 3
#define POINT_ARRAY_MID 2
#define MAX_SPACES 8
#define MAX_INVENTORY_SPACE 10
#define HEALTH_UPGRADE 40
#define BATTERY_UPGRADE 40
#define SUBSCRIBERS 2
#define MAX_TESTS 4
#define MAX_ENEMY_FACTORY_TESTS 3
#define DIRECTION_CALCULATIONS 3
#define SAVE_COUSIN_DIRECTIONS 2

/// ItemManagement.c
#define PLUS_ONE 1
#define RANGE_MIN 120
#define RANGE_MID 130
#define RANGE_MAX 150
#define MAX_MIN 180
#define MAX_MIN_PLUS 190
#define MAX_MED 200
#define MAX_RANGE 210
#define DEFAULT_COLOR 7

#endif // SHAREDBEHAVIOR_H_INCLUDED
