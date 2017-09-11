/// FlashlightManagement.c
#include "FlashlightManagement.h"
#include "Definitions.h"
#include "ArrayOperations.h"
#include "Switch.h"
#include "Player.h"
#include "Boolean.h"
#include "Player.h"
#include <stdio.h>

/// No need to declare structure like other interfaces, just a static array with points.
static Point s_flashLightPoints[SIZE_OF_FL_POINTS];

/// Assist with setting flashlight points.
static int GetEmptyIndexForFlashlightPoints()
{
    int flPointIndex = GetPointArrayPos(s_flashLightPoints, SIZE_OF_FL_POINTS);

    if(UseSwitch(flPointIndex, ERROR_INDICATOR))
        return ERROR_INDICATOR;

    return flPointIndex;
}

/// Prevent from adding duplicate values.
static int CrossReferencePoint(int x, int y)
{
    /// Step 1. Check if first flashlight point entry is valid.
    if((s_flashLightPoints[0].x == ERROR_INDICATOR || s_flashLightPoints[0].x < 0)
       && (s_flashLightPoints[0].y == ERROR_INDICATOR || s_flashLightPoints[0].y < 0))
    {
        return FALSE;
    }

    /// Step 2. If step 1 is not true then proceed to check array for duplicates.
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_flashLightPoints[i].x > ERROR_INDICATOR
           && s_flashLightPoints[i].y > ERROR_INDICATOR)
        {
            if(s_flashLightPoints[i].x == x
               && s_flashLightPoints[i].y == y)
            {
                /// We found a value that has already been entered. So don't go through with inputing the new one.
                return TRUE;
            }
        }
    }

    return FALSE;
}

/// Assign flashlight points.
static void SetFlashLightPoint(int x, int y)
{
    /// Step 1. Get empty index in flashlight point array with the function GetEmptyIndexForFlashlightPoints().
    int index = GetEmptyIndexForFlashlightPoints();

    /** Step 2. Cross reference the x and y we received here. If CrossReference(x, y) returns true then do not add x and y into the array because
    they were already entered before. If CrossReference(x, y) returns false, we'll add them. */
    if(!CrossReferencePoint(x, y))
    {
        s_flashLightPoints[index].x = x;
        s_flashLightPoints[index].y = y;
    }
}

/// Empty flashlight points to ensure no values stack. Use in GatherFlashlightPoints() frequently.
void ClearFlashlightPoints()
{
    ResetPointArray(s_flashLightPoints, SIZE_OF_FL_POINTS);

    /**
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_flashLightPoints[i].x != ERROR_INDICATOR &&
           s_flashLightPoints[i].y != ERROR_INDICATOR)
        {
            s_flashLightPoints[i].x = ERROR_INDICATOR;
            s_flashLightPoints[i].y = ERROR_INDICATOR;
        }
    }
    */
}

/// Very important algorithm, key to making the flashlight effect work.
void GatherFlashlightPoints()
{
    /// Step 1. This equals 0, return. Players flashlight has no more battery left. Clear all points and exit.
    if(!CheckBatteryPower())
    {
        ClearFlashlightPoints();
        return;
    }

    /// Step 2. Range determines how many "spaces" the flashlight will illuminate.
    int flRange = GetFlashLight()->m_range;

    /// Step 3. MUST reset every time we run this algorithm or else the points will stack, and eventually the array will overflow.
    ClearFlashlightPoints();

    /// Step 4. We'll start calculating the starting position with the players position.
    Point savePlayerPos = GetPlayerPosition();

    int startingPos;
    if(savePlayerPos.x <= flRange) /// Top left corner check. If players pos is <= flRange, we're at the top left corner.
    {
        startingPos = 0;
    }

    else
    {
        startingPos = savePlayerPos.x - flRange;
    }

    SetFlashLightPoint(startingPos, savePlayerPos.y);

    /// Step 5. Get northwest position keeping in mind the flashlights range.
    Point northWestPoint;
    northWestPoint.x = startingPos; /// 17.
    if(savePlayerPos.y - flRange <= 0)
    {
        northWestPoint.y = 0;
    }
    else
    {
        northWestPoint.y = savePlayerPos.y - flRange;
    }

    SetFlashLightPoint(northWestPoint.x, northWestPoint.y);

    /// Step 6. Northwest calculation.
    Point p;
    int begin = northWestPoint.y;
    int end = savePlayerPos.y;
    while(begin < end)
    {
        p.x = begin;
        p.y = northWestPoint.x;

        SetFlashLightPoint(p.y, p.x);

        ++begin;
    }

    /// Step 7. Northeast calculation.
    Point northEastPoint;
    northEastPoint.x = startingPos;
    int loop = savePlayerPos.y + 1;
    int endPoint = 0;
    if(savePlayerPos.y + flRange >= COLUMNS - 1)
    {
        northEastPoint.y = COLUMNS - 1;
    }
    else
    {
        northEastPoint.y = savePlayerPos.y + flRange;
    }
    endPoint = northEastPoint.y;

    SetFlashLightPoint(northEastPoint.x, northEastPoint.y);

    /// Step 8. Assist with Northeast calculation.
    while(loop < endPoint)
    {
        northEastPoint.x = loop;
        int savePoint = northWestPoint.x;

        SetFlashLightPoint(savePoint, northEastPoint.x);
        ++loop;
    }

    /// Step 9. Row calculation.
    int columns = northWestPoint.y;
    int rows = northWestPoint.x;
    int maxRows = 0;
    ++rows;

    if(savePlayerPos.x >= (ROWS - 1) - flRange)
    {
        maxRows = RESET_MAX_ROWS;
    }

    else
    {
        maxRows = savePlayerPos.x + flRange;
    }

    /// Step 10. Final point calculation.
    while(columns <= northEastPoint.y && rows <= maxRows)
    {
        SetFlashLightPoint(rows, columns);

        ++columns;

        if(columns == northEastPoint.y + 1)
        {
            columns = northWestPoint.y;
            ++rows;
        }
    }
}

/// Return flash light point array.
Point * GetFlashLightPoints()
{
    return s_flashLightPoints;
}

/// Check if coordinate being printed in the loop in the update function is a flashlight coordinate.
int DisplayFlashlight(int x, int y)
{
    int i;
    for(i = 0; i < SIZE_OF_FL_POINTS; ++i)
    {
        if(s_flashLightPoints[i].x != ERROR_INDICATOR &&
           s_flashLightPoints[i].y != ERROR_INDICATOR)
        {
            if(s_flashLightPoints[i].x == x &&
               s_flashLightPoints[i].y == y)
            {
                return TRUE;
            }
        }

        else
        {
            break;
        }
    }

    return FALSE;
}

void InitFlashlightManagement()
{
    ClearFlashlightPoints();
}
