/// Switch.c
#include "Switch.h"
#include "Boolean.h"
#include "Definitions.h"
#include <stdio.h>

int UseSwitch(int first, int second)
{
    switch(first == second ? TRUE : FALSE)
    {
    case 0:
        return FALSE;
    case 1:
        return TRUE;
    default:
        printf("Error! File: Switch.c, Function: UseSwitch().\n");
        break;
    }

    return ERROR_INDICATOR;
}
