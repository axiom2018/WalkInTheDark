/// ArrayOperations.c
#include "Definitions.h"
#include "Point.h"
#include <windows.h>

/// Locate an empty position in a pointer array.
int GetPointerArrayPos(void *arr[], int arrSize)
{
    int i;
    for(i = 0; i < arrSize; ++i)
    {
        if(arr[i] == NULL)
        {
            return i;
        }
    }

    return ERROR_INDICATOR;
}

/// Locate an empty position in an integer array.
int GetIntArrayPos(int arr[], int arrSize)
{
    int i;
    for(i = 0; i < arrSize; ++i)
    {
        if(arr[i] == ERROR_INDICATOR)
        {
            return i;
        }
    }

    return ERROR_INDICATOR;
}

/// Locate an empty position in a Point array.
int GetPointArrayPos(Point arr[], int arrSize)
{
    int i;
    for(i = 0; i < arrSize; ++i)
    {
        if(arr[i].x == ERROR_INDICATOR
           && arr[i].y == ERROR_INDICATOR)
        {
           return i;
        }
    }

    return ERROR_INDICATOR;
}
