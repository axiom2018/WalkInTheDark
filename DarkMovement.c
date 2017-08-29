/// DarkMovement.c
#include "DarkMovement.h"
#include "Definitions.h"
#include "UniversalMovementFunctions.h"
#include "Boolean.h"
#include "Player.h"
#include "ArrayOperations.h"
#include <stdlib.h>

/// Save vacant spaces found for later use.
static Point s_vacantSpaces[MAX_SPACES];
/// Initialization help.
static int s_initVacantSpaces = FALSE;

/// Reset the vacant spaces to default values to be sure no old values remain.
static void EmptyVacantSpacesArray()
{
    int i;
    for(i = 0; i < MAX_SPACES; ++i)
    {
        s_vacantSpaces[i].x = ERROR_INDICATOR;
        s_vacantSpaces[i].y = ERROR_INDICATOR;
    }
}

/// When doing point calculations, this function will add a point to the desired array.
static void AddPointToArray(Point pointArr[], Point p)
{
    /// Step 1. Get vacant position with an array operation.
    int index = GetPointArrayPos(pointArr, MAX_SPACES);

    /// Step 2. Return if no vacant spaces.
    if(index == ERROR_INDICATOR)
    {
        return;
    }

    /// Step 3. Add point to array if Step 2 did not cause an early exit.
    s_vacantSpaces[index].x = p.x;
    s_vacantSpaces[index].y = p.y;
}

/// Assign the enemy a certain position in the vacant spaces array.
static void AppointEnemyPosition(EMData *pData)
{
    /// Step 1. Begin loop.
    while(TRUE)
    {
        /// Step 2. Get random position from array, we are guaranteed to have a maximum of 8 since we don't start testing until this point.
        int index = rand() % (MAX_DARK_MOVEMENT_POSITIONS + 1 - 0) + 0;

        /// Step 3. Begin testing.
        if(!RunPositionTests(pData, &s_vacantSpaces[index]))
        {
            /// (Optional Step) If Step 3 returns false, we can safely assign the desired position to the enemy.
            *pData->m_pEnemyPos = s_vacantSpaces[index];
            break;
        }

        else
        {
            /// (Optional Step) If Step 3 returns true, we cannot grant this position. Continue until one is found.
            continue;
        }
    }
}

/// If enemy shares position with player, damage player.
static void DetectPlayerCollision(EMData *pData)
{
    if(PlayerCollision(pData->m_playerPos, *pData->m_pEnemyPos))
    {
        int assignedDamage = pData->m_playerHealth - pData->m_enemyDamageRate;
        AdjustHealth(assignedDamage);
    }
}

/// Calculate all possible positions enemy can move, which are above.
static void CalculateHigh(EMData *pData)
{
    /// Step 1. Create points and assign values.
    Point topLeft, topMiddle, topRight;
    topLeft.x = pData->m_pEnemyPos->x - 1;
    topLeft.y = pData->m_pEnemyPos->y - 1;

    topMiddle.x = pData->m_pEnemyPos->x;
    topMiddle.y = pData->m_pEnemyPos->y - 1;

    topRight.x = pData->m_pEnemyPos->x + 1;
    topRight.y = pData->m_pEnemyPos->y - 1;

    /// Step 2. Create array of the points.
    Point HighPoints[POINT_ARRAY];
    HighPoints[0] = topLeft;
    HighPoints[1] = topMiddle;
    HighPoints[2] = topRight;

    /// Step 3. Test each one to see if it's possible the enemy can move there. If so, add point to s_vacantSpacesArray.
    int i;
    for(i = 0; i < POINT_ARRAY; ++i)
    {
        AddPointToArray(s_vacantSpaces, HighPoints[i]);
    }
}

/// Calculate all possible positions enemy can move, on both sides.
static void CalculateMid(EMData *pData)
{
    /// Step 1. Create points and assign values.
    Point leftMid, rightMid;
    leftMid.x = pData->m_pEnemyPos->x - 1;
    leftMid.y = pData->m_pEnemyPos->y;

    rightMid.x = pData->m_pEnemyPos->x + 1;
    rightMid.y = pData->m_pEnemyPos->y;

    /// Step 2. Create array for points.
    Point MidPoints[POINT_ARRAY_MID];
    MidPoints[0] = leftMid;
    MidPoints[1] = rightMid;

    /// Step 3. Test each one to see if it's possible the enemy can move there. If so, add point to s_vacantSpacesArray.
    int i;
    for(i = 0; i < POINT_ARRAY_MID; ++i)
    {
        AddPointToArray(s_vacantSpaces, MidPoints[i]);
    }
}

/// Calculate all possible positions enemy can move, which are below.
static void CalculateLow(EMData *pData)
{
    /// Step 1. Create points and assign values.
    Point bottomLeft, bottomMiddle, bottomRight;
    bottomLeft.x = pData->m_pEnemyPos->x - 1;
    bottomLeft.y = pData->m_pEnemyPos->y + 1;

    bottomMiddle.x = pData->m_pEnemyPos->x;
    bottomMiddle.y = pData->m_pEnemyPos->y + 1;

    bottomRight.x = pData->m_pEnemyPos->x + 1;
    bottomRight.y = pData->m_pEnemyPos->y + 1;

    /// Step 2. Create array for points.
    Point LowPoints[POINT_ARRAY];
    LowPoints[0] = bottomLeft;
    LowPoints[1] = bottomMiddle;
    LowPoints[2] = bottomRight;

    /// Step 3. Test each one to see if it's possible the enemy can move there. If so, add point to s_vacantSpacesArray.
    int i;
    for(i = 0; i < POINT_ARRAY; ++i)
    {
        AddPointToArray(s_vacantSpaces, LowPoints[i]);
    }
}

/// Set the values of the s_vacantSpaces array ourselves to avoid junk values.
static void InitializeVacantSpacesArray()
{
    int i;
    for(i = 0; i < MAX_SPACES; ++i)
    {
        s_vacantSpaces[i].x = ERROR_INDICATOR;
        s_vacantSpaces[i].y = ERROR_INDICATOR;
    }

    s_initVacantSpaces = TRUE;
}

void MovementDark(EMData *pData)
{
    /// Step 1. Must initialize array.
    if(!s_initVacantSpaces)
    {
        InitializeVacantSpacesArray();
    }

    /// Step 2. Premature check for player We're checking if the player walked into enemy and not vice versa.
    DetectPlayerCollision(pData);

    /// Step 3. Insert movement delay check function here.
    if(!RunMovementDelayTest(pData))
    {
        return;
    }

    /// Step 4. Calculate vacant spaces above enemy, to the left, right, and below the enemy.
    CalculateHigh(pData);
    CalculateMid(pData);
    CalculateLow(pData);

    /// Step 5. After calculating positions, assign the enemy a new position.
    AppointEnemyPosition(pData);

    /// Step 6. Check if enemy bumped into player.
    DetectPlayerCollision(pData);

    /// Step 7. Dump all elements in arrays in preparation for the next time the function runs.
    EmptyVacantSpacesArray();
}
