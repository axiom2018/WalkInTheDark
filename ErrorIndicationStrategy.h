/// ErrorIndicationStrategy.h
#ifndef ERRORINDICATIONSTRATEGY_H_INCLUDED
#define ERRORINDICATIONSTRATEGY_H_INCLUDED

/// ?

/// And in FlashlightManagement.c

/** In this project there are multiple areas where an array must be reset to the ERROR_INDICATOR (located in Definitions.h).
For example in LightMovement.c, Step 6 in the MovementLight function, the function EmptyPointArray is called and the parameter
it passes in is the Point array "s_vacantSpaces". This is just one of a plethora of examples where a strategy pattern

No need for strategy, just do this in the ArrayOperations.h/.c*/

#endif // ERRORINDICATIONSTRATEGY_H_INCLUDED