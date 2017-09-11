/// ItemCreation.c
#include "ItemCreation.h"
#include "LevelManagement.h"
#include "Definitions.h"
#include <stdlib.h>
#include <stdio.h>

void CreateFlashlight(Flashlight *pFlashlight, Point p)
{
    pFlashlight->m_item.m_name = "Flashlight";
    pFlashlight->m_item.m_color = FLASHLIGHT_ITEM_COLOR;
    pFlashlight->m_item.m_symbol = 'F';
    pFlashlight->m_item.m_assignedLevel = GetCurrentLevel();

    pFlashlight->m_item.m_pos.x = p.x;
    pFlashlight->m_item.m_pos.y = p.y;

    pFlashlight->m_range = rand() % (6 + 1 - 3) + 3;
}

void CreateBattery(Battery *pBattery, Point p)
{
    pBattery->m_item.m_name = "Battery";
    pBattery->m_item.m_color = BATTERY_ITEM_COLOR;
    pBattery->m_item.m_symbol = 'B';
    pBattery->m_item.m_assignedLevel = GetCurrentLevel();

    pBattery->m_item.m_pos.x = p.x;
    pBattery->m_item.m_pos.y = p.y;

    pBattery->m_rechargeAmount = BATTERY_UPGRADE;
}

void CreateHealthPack(HealthPack *pHealthPack, Point p)
{
    pHealthPack->m_item.m_name = "HealthPack";
    pHealthPack->m_item.m_color = HEALTH_PACH_ITEM_COLOR;
    pHealthPack->m_item.m_symbol = 'H';
    pHealthPack->m_item.m_assignedLevel = GetCurrentLevel();

    pHealthPack->m_item.m_pos.x = p.x;
    pHealthPack->m_item.m_pos.y = p.y;

    pHealthPack->m_healthAmount = HEALTH_UPGRADE;
}
