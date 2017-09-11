/// ArrayOperations.h
#ifndef ARRAYOPERATIONS_H_INCLUDED
#define ARRAYOPERATIONS_H_INCLUDED
#include "Point.h"
#include "LevelInfo.h"

/// ArrayOperations supply different project files with a one stop shop of array manipulation functions. Player, World, and more benefit.

int GetPointerArrayPos(void *arr[], int arrSize);
int GetIntArrayPos(int arr[], int arrSize);
int GetPointArrayPos(Point arr[], int arrSize);
void ResetPointArray(Point arr[], int arrSize);
void SetLevelInfoArray(LevelInfo arr[], int arrSize);

#endif // ARRAYOPERATIONS_H_INCLUDED
