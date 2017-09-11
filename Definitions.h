/// Definitions.h
#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

/** It's easier to make a file with definitions that several entities might need instead of declaring them in a single .c file.
    Also helps avoid hard coding actual values into functions.

    (Keep track of where each definition is used for reference.)
*/

/// Shared.
#define ERROR_INDICATOR -1
#define COORDINATES_TO_SEND 6
#define SIZE_OF_FL_POINTS 200
#define MAX_ENEMIES 15
#define MAX_ITEMS 24
#define MAX_INVENTORY_SPACE 10
#define POINT_ARRAY 3
#define POINT_ARRAY_MID 2
#define MAX_SPACES 8
#define DEFAULT_COLOR 7

/// ItemCreation.c
#define HEALTH_UPGRADE 40
#define BATTERY_UPGRADE 40
#define FLASHLIGHT_ITEM_COLOR 5
#define BATTERY_ITEM_COLOR 3
#define HEALTH_PACH_ITEM_COLOR 12

/// ItemManagement.c
#define PLUS_ONE 1
#define RANGE_MIN 120
#define RANGE_MID 130
#define RANGE_MAX 150
#define MAX_MIN 180
#define MAX_MIN_PLUS 190
#define MAX_MED 200
#define MAX_RANGE 210
#define ITEMS_PER_LEVEL 4

/// World.c
#define ROWS 20
#define COLUMNS 30
#define DOOR_SYMBOL 178
#define DOOR_COLOR 4
#define FLASHLIGHT_COLOR 15

/// EnemyManagement.c (EM = EnemyManagement)
#define EM_LEVEL_ONE_INDEX 0
#define EM_LEVEL_TWO_INDEX 4
#define EM_LEVEL_THREE_INDEX 8
#define EM_LEVEL_FOUR_INDEX 12
#define EM_LEVEL_FIVE_INDEX 16
#define EM_LEVEL_SIX_INDEX 20

/// EnemyCreation.c
#define BANSHEE_DAMAGE_HIGH 45
#define BANSHEE_DAMAGE_LOW 40
#define BANSHEE_SYMBOL 234
#define BANSHEE_MOVEMENT_DELAY 1
#define WITCH_DAMAGE_HIGH 25
#define WITCH_DAMAGE_LOW 20
#define WITCH_SYMBOL 237
#define WITCH_MOVEMENT_DELAY 2
#define WEREWOLF_DAMAGE_HIGH 15
#define WEREWOLF_DAMAGE_LOW 10
#define WEREWOLF_SYMBOL 64
#define WEREWOLF_MOVEMENT_DELAY 3

/// Player.c
#define DEFAULT_PLAYER_HEALTH 100

/// LightMovement.c
#define DIRECTION_CALCULATIONS 3

/// DarkMovement.c
#define MAX_DARK_MOVEMENT_POSITIONS 7

/// EnemyFactory.c
#define MAX_ENEMY_FACTORY_TESTS 3

/// UniversalMovementFunctions.c
#define MAX_TESTS 4

/// InventoryManagement.c
#define FLASHLIGHT_FIRST_RANGE 3

/// FlashlightManagement.c
#define RESET_MAX_ROWS 19

#endif // SHAREDBEHAVIOR_H_INCLUDED
