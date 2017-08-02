/// ItemCreation.c
#include "ItemCreation.h"
#include "LevelManagement.h"
#include "Definitions.h" /// BATTERY_UPGRADE & HEALTH_UPGRADE
#include <stdlib.h>
#include <stdio.h>

void CreateFlashlight(Flashlight *pFlashlight, int index, Point p)
{
    pFlashlight->m_item.m_name = "Flashlight";
    pFlashlight->m_item.m_color = 5;
    pFlashlight->m_item.m_symbol = 'F';
    pFlashlight->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Item: %s, Assigned level: %d\n", pFlashlight->m_item.m_name,
            pFlashlight->m_item.m_assignedLevel);

    pFlashlight->m_item.m_pos.x = p.x;
    pFlashlight->m_item.m_pos.y = p.y;

    pFlashlight->m_range = rand() % (6 + 1 - 3) + 3;
}

void CreateBattery(Battery *pBattery, int index, Point p)
{
    pBattery->m_item.m_name = "Battery";
    pBattery->m_item.m_color = 3;
    pBattery->m_item.m_symbol = 'B';
    pBattery->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Item: %s, Assigned level: %d\n", pBattery->m_item.m_name,
            pBattery->m_item.m_assignedLevel);

    pBattery->m_item.m_pos.x = p.x;
    pBattery->m_item.m_pos.y = p.y;

    pBattery->m_rechargeAmount = BATTERY_UPGRADE;
}

void CreateHealthPack(HealthPack *pHealthPack, int index, Point p)
{
    pHealthPack->m_item.m_name = "HealthPack";
    pHealthPack->m_item.m_color = 12;
    pHealthPack->m_item.m_symbol = 'H';
    pHealthPack->m_item.m_assignedLevel = GetCurrentLevel();
    printf("Item: %s, Assigned level: %d\n", pHealthPack->m_item.m_name,
            pHealthPack->m_item.m_assignedLevel);

    pHealthPack->m_item.m_pos.x = p.x;
    pHealthPack->m_item.m_pos.y = p.y;

    pHealthPack->m_healthAmount = HEALTH_UPGRADE;
}
