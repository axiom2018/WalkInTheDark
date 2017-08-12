/// DrawEnemy.h
#ifndef DRAWENEMY_H_INCLUDED
#define DRAWENEMY_H_INCLUDED
#include "Werewolf.h"
#include "Banshee.h"
#include "Witch.h"

/// In order to help keep the EnemyManagement.c file as short and concise as possible, this interface will assist in enemy drawing.
int DrawWerewolf(Werewolf *pWerewolf, int x, int y);
int DrawBanshee(Banshee *pBanshee, int x, int y);
int DrawWitch(Witch *pWitch, int x, int y);

#endif // DRAWENEMY_H_INCLUDED
