/// ItemManagement.h
#ifndef ITEMMANAGEMENT_H_INCLUDED
#define ITEMMANAGEMENT_H_INCLUDED
#include "EnemyMoveData.h"
#include "Point.h"

/// Handles all item related activities.
void InitItemManagement();
void GenerateLevelItems();
int UpdateGeneratedItems(int x, int y);
void UpdateEnemyItemData(EMData *pData);
void ItemCollisionDetection();
void GivePlayerItem(int itemType);
Point GetItemPoint(int index);
int CheckGeneratedItemsForDuplicatePositions();
void ItemManagementCleanMemory();

#endif // ITEMMANAGEMENT_H_INCLUDED
