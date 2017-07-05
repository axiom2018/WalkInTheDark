/// LightMovement.c
#include "LightMovement.h"
#include "Definitions.h"
#include "LightMovementEnums.h"
#include "LightMovementDirections.h"
#include "UniversalMovementFunctions.h"
#include "Boolean.h"
#include "Player.h"
#include "ArrayOperations.h"
#include <stdlib.h>

/// Use of function pointers for running the direction calculation tests.
typedef Directions (*DirectionCalculations)(Point playerPos, Point *pEnemyPos);
static DirectionCalculations EnemyDirectionCalculations[DIRECTION_CALCULATIONS];
/// Initialization variable.
static int s_initLightMovement = FALSE;
/// Array for surrounding coordinate gathering.
static Point s_vacantSpaces[MAX_SPACES];
/// Condition variable for while loop. Have we went through all tests and failed? If so, break from loop.
static int s_maxFailedDirections;
/// Save the initial direction along with it's two cousins here.
static Directions s_directionAndCousins[DIRECTION_CALCULATIONS];
/// Index tracker for array above.
static int s_dacIndex;

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
    pointArr[index].x = p.x;
    pointArr[index].y = p.y;
}

static void EmptyPointArray(Point pointArr[])
{
    int i;
    for(i = 0; i < MAX_SPACES; ++i)
    {
        pointArr[i].x = ERROR_INDICATOR;
        pointArr[i].y = ERROR_INDICATOR;
    }
}

static void EmptyDirectionSystem()
{
    /// Step 1. Reset dac index.
    s_dacIndex = 0;

    /// Step 2. Reset back to default value of failed directions.
    s_maxFailedDirections = 0;

    /// Step 3. Reset s_directionAndCousins.
    int i;
    for(i = 0; i < DIRECTION_CALCULATIONS; ++i)
    {
        s_directionAndCousins[i] = Invalid;
    }
}

static int TestedAllDirections()
{
    if(s_maxFailedDirections == DIRECTION_CALCULATIONS)
    {
        return TRUE;
    }
    return FALSE;
}

static Point CorrelateDirection(Directions dir, Point *pEnemyPos)
{
    Point pos;

    /// Step 1. Run switch to determine desired enemy position based off direction given.
    switch(dir)
    {
    case North:
        pos.x = pEnemyPos->x;
        pos.y = pEnemyPos->y - 1;
        break;
    case South:
        pos.x = pEnemyPos->x;
        pos.y = pEnemyPos->y + 1;
        break;
    case East:
        pos.x = pEnemyPos->x + 1;
        pos.y = pEnemyPos->y;
        break;
    case West:
        pos.x = pEnemyPos->x - 1;
        pos.y = pEnemyPos->y;
        break;
    case NorthWest:
        pos.x = pEnemyPos->x - 1;
        pos.y = pEnemyPos->y - 1;
        break;
    case NorthEast:
        pos.x = pEnemyPos->x + 1;
        pos.y = pEnemyPos->y - 1;
        break;
    case SouthWest:
        pos.x = pEnemyPos->x - 1;
        pos.y = pEnemyPos->y + 1;
        break;
    case SouthEast:
        pos.x = pEnemyPos->x + 1;
        pos.y = pEnemyPos->y + 1;
        break;
    default:
        printf("Error! File: LightMovement.c Function: CorrelateDirection(Directions dir, Point *pEnemyPos).\n");
        break;
    }

    /// Step 2. Return point.
    return pos;
}

static Directions SouthCalculation(Point playerPos, Point *pEnemyPos)
{
    Directions dir;

    if(playerPos.x > pEnemyPos->y
            && playerPos.y == pEnemyPos->x)
    {
        /// Player is directly below the enemy, sharing the same X position but a different y.
        dir = South;
        return dir;
    }

    else if(playerPos.x > pEnemyPos->y
            && playerPos.y < pEnemyPos->x)
    {
        /// Player is below the enemy and to the left.
        dir = SouthWest;
        return dir;
    }

    else if(playerPos.x > pEnemyPos->y
            && playerPos.y > pEnemyPos->x)
    {
        /// Player is below the enemy and to the right.
        dir = SouthEast;
        return dir;
    }

    return Invalid;
}

static Directions WestEastCalculation(Point playerPos, Point *pEnemyPos)
{
    Directions dir;

    if(playerPos.x == pEnemyPos->y
            && playerPos.y < pEnemyPos->x)
    {
        /// Player is on the same y axis and to the left of the enemy.
        dir = West;
        return dir;
    }

    else if(playerPos.x == pEnemyPos->y
            && playerPos.y > pEnemyPos->x)
    {
        /// Player is on the same y axis and to the right of the enemy.
        dir = East;
        return dir;
    }

    return Invalid;
}

static Directions NorthCalculation(Point playerPos, Point *pEnemyPos)
{
    Directions dir;

    /// Step 1. Calculate proper direction.
    if(playerPos.x < pEnemyPos->y
       && playerPos.y == pEnemyPos->x)
    {
        dir = North; /// Player is directly above the enemy, sharing the same X position but different y.
        return dir;
    }

    else if(playerPos.x < pEnemyPos->y
            && playerPos.y < pEnemyPos->x)
    {
        dir = NorthWest; /// Player is to the top left of the enemy.
        return dir;
    }

    else if(playerPos.x < pEnemyPos->y
            && playerPos.y > pEnemyPos->x)
    {
        dir = NorthEast; /// Player is to the top right of the enemy.
        return dir;
    }

    /// Step 2. Return Invalid if player and enemy have no matching position relationships above.
    return Invalid;
}

static void AssignCousinsToDirection()
{
    /// Step 1. Use switch to determine the cousins of the primary direction already in the array s_directionAndCousins.
    switch(s_directionAndCousins[0])
    {
    case North:
        s_directionAndCousins[1] = NorthWest;
        s_directionAndCousins[2] = NorthEast;
        break;
    case South:
        s_directionAndCousins[1] = SouthWest;
        s_directionAndCousins[2] = SouthEast;
        break;
    case East:
        s_directionAndCousins[1] = NorthEast;
        s_directionAndCousins[2] = SouthEast;
        break;
    case West:
        s_directionAndCousins[1] = NorthWest;
        s_directionAndCousins[2] = SouthWest;
        break;
    case NorthWest:
        s_directionAndCousins[1] = West;
        s_directionAndCousins[2] = North;
        break;
    case NorthEast:
        s_directionAndCousins[1] = North;
        s_directionAndCousins[2] = East;
        break;
    case SouthWest:
        s_directionAndCousins[1] = West;
        s_directionAndCousins[2] = South;
        break;
    case SouthEast:
        s_directionAndCousins[1] = East;
        s_directionAndCousins[2] = South;
        break;
    default:
        printf("Error! File: LightMovement.c Function: AssignCousinsToDirection().\n");
        break;
    }
}

static void CalculateDirection(EMData *pData)
{
    /// Step 1. Set up variable to save potential direction.
    Directions dir;

    /// Step 2. Run functions to find matching position relationship. Guaranteed to find matching position.
    int i;
    for(i = 0; i < DIRECTION_CALCULATIONS; ++i)
    {
        dir = EnemyDirectionCalculations[i](pData->m_playerPos, pData->m_pEnemyPos);
        if(dir != Invalid)
            break;
    }

    /// Step 3. We have acquired the primary direction, assign to array.
    s_directionAndCousins[0] = dir;

    /// Step 4. Assign cousins after the primary direction.
    AssignCousinsToDirection();

    while(!TestedAllDirections())
    {
        /// Step 5. Match direction with coordinates. Simple.
        Point pos = CorrelateDirection(s_directionAndCousins[s_dacIndex], pData->m_pEnemyPos);

        /// Step 6. Begin UniversalMovementFunctions tests.
        if(!RunPositionTests(pData, &pos))
        {
            pData->m_pEnemyPos->x = pos.x;
            pData->m_pEnemyPos->y = pos.y;
            break;
        }

        else
        {
            /// Direction has failed, add it to a failed direction array.
            ++s_maxFailedDirections;
            ++s_dacIndex;
        }
    }
}

static void DetectPlayerCollision(EMData *pData)
{
    if(PlayerCollision(pData->m_playerPos, *pData->m_pEnemyPos))
    {
        int assignedDamage = pData->m_playerHealth - pData->m_enemyDamageRate;
        AdjustHealth(assignedDamage);
    }
}

static void CalculateHigh(Point *pEnemyPos)
{
    /// Step 1. Create points and assign values.
    Point topLeft, topMiddle, topRight;
    topLeft.x = pEnemyPos->x - 1;
    topLeft.y = pEnemyPos->y - 1;

    topMiddle.x = pEnemyPos->x;
    topMiddle.y = pEnemyPos->y - 1;

    topRight.x = pEnemyPos->x + 1;
    topRight.y = pEnemyPos->y - 1;

    /// Step 2. Create array of the points.
    Point HighPoints[POINT_ARRAY];
    HighPoints[0] = topLeft;
    HighPoints[1] = topMiddle;
    HighPoints[2] = topRight;

    /// Step 3. Assign to array s_vacantSpaces.
    int i;
    for(i = 0; i < POINT_ARRAY; ++i)
    {
        AddPointToArray(s_vacantSpaces, HighPoints[i]);
    }
}

static void CalculateMid(Point *pEnemyPos)
{
    /// Step 1. Create points and assign values.
    Point leftMid, rightMid;
    leftMid.x = pEnemyPos->x - 1;
    leftMid.y = pEnemyPos->y;

    rightMid.x = pEnemyPos->x + 1;
    rightMid.y = pEnemyPos->y;

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

static void CalculateLow(Point *pEnemyPos)
{
    /// Step 1. Create points and assign values.
    Point bottomLeft, bottomMiddle, bottomRight;
    bottomLeft.x = pEnemyPos->x - 1;
    bottomLeft.y = pEnemyPos->y + 1;

    bottomMiddle.x = pEnemyPos->x;
    bottomMiddle.y = pEnemyPos->y + 1;

    bottomRight.x = pEnemyPos->x + 1;
    bottomRight.y = pEnemyPos->y + 1;

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

static int IsPlayerWithinDistance(Point *pEnemyPos, Point playerPos)
{
    /// Step 2. Now we can loop through the s_vacantSpaces array to see if we have a matching position for the player.
    int i;
    for(i = 0; i < MAX_SPACES; ++i)
    {
        if(s_vacantSpaces[i].x == playerPos.y
           && s_vacantSpaces[i].y == playerPos.x)
        {
            /// (Optional Step) The player was in range of the enemy so the enemy will go right toward the player.
            pEnemyPos->x = playerPos.y;
            pEnemyPos->y = playerPos.x;
            return TRUE;
        }
    }
    /// Step 3. The player was not close enough to the enemy for the enemy to attack. Return false.
    return FALSE;
}

static void InitializeArrays()
{
    /// Step 1. Set every value to a default ERROR_INDICATOR.
    int i;
    for(i = 0; i < MAX_SPACES; ++i)
    {
        s_vacantSpaces[i].x = ERROR_INDICATOR;
        s_vacantSpaces[i].y = ERROR_INDICATOR;
    }

    /// Step 2. Initialize array of functions.
    EnemyDirectionCalculations[0] = NorthCalculation;
    EnemyDirectionCalculations[1] = WestEastCalculation;
    EnemyDirectionCalculations[2] = SouthCalculation;

    /// Step 3. Initialize how many direction tests are failed so far, 0.
    s_maxFailedDirections = 0;

    /// Step 4. Original direction will be saved later. May prove to be relevant or not.
    s_dacIndex = 0;

    /// Step 5. Be sure to set to true or the Initialization function will be called repeatedly.
    s_initLightMovement = TRUE;
}

void MovementLight(EMData *pData)
{
    /// Step 1. Must initialize arrays.
    if(!s_initLightMovement)
    {
        InitializeArrays();
    }

    /// Step 2. Premature check for player We're checking if the player walked into enemy and not vice versa.
    DetectPlayerCollision(pData);

    /// Step 3. Regardless of us finding available positions later, if the enemy is not set to move based on their delay then they won't get the chance.
    if(!RunMovementDelayTest(pData))
    {
        return;
    }

    CalculateHigh(pData->m_pEnemyPos);
    CalculateMid(pData->m_pEnemyPos);
    CalculateLow(pData->m_pEnemyPos);

    /// Step 4. If player is within enemy vicinity than the enemy will immediately go toward him. If not proceed to calculate position.
    if(!IsPlayerWithinDistance(pData->m_pEnemyPos, pData->m_playerPos))
    {
        CalculateDirection(pData);
    }

    /// Step 5. Check if enemy bumped into player.
    DetectPlayerCollision(pData);

    /// Step 6. Dump all elements in arrays in preparation for the next time the function runs.
    EmptyPointArray(s_vacantSpaces);

    /// Step 7. Dump all old data with direction system.
    EmptyDirectionSystem();
}
