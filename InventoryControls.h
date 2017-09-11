/// InventoryControls.h
#ifndef INVENTORYCONTROLS_H_INCLUDED
#define INVENTORYCONTROLS_H_INCLUDED

/// Delegate responsibility of controlling the Players inventory GUI with keys with this new interface.

void InitInventoryControls();
char ReturnIndicator(int currentIndex);
void InventoryKeyInstructions();
void IndicatorMotion(char *input);
void InventoryControlsCleanMemory();

#endif // INVENTORYCONTROLS_H_INCLUDED
