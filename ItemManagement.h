/// ItemManagement.h
#ifndef ITEMMANAGEMENT_H_INCLUDED
#define ITEMMANAGEMENT_H_INCLUDED
#include "EnemyMoveData.h"

/// Handles all item related activities.
void InitItemManagement();
int UpdateGeneratedItems(int x, int y);
void UpdateEnemyItemData(EMData *pData);
void GenerateLevelItems();
void GivePlayerItem(int itemType);
void ItemCollisionDetection();
void ItemManagementCleanMemory();

#endif // ITEMMANAGEMENT_H_INCLUDED
