/// LevelManagement.h
#ifndef LEVELMANAGEMENT_H_INCLUDED
#define LEVELMANAGEMENT_H_INCLUDED

/// Delegate the responsibility of managing levels to this interface. All entities that need knowledge of levels will use this interface.

void InitLevelManagement();
int GetCurrentLevel();
void NextLevel();
int IsGameOver();

#endif // LEVELMANAGEMENT_H_INCLUDED
