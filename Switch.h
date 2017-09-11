/// Switch.h
#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

/** Several instances in the code could benefit from using an interface that allows the client to determine certain values for switch statements
quickly. The current implementation has instances of: switch(i == MAX_ITEMS ? TRUE : FALSE). Here, the variable i and MAX_ITEMS are kind of
hard coded in. The new implementation will allow the client to pass in 2 variables to use in the switch, making the code shorter and much easier
to manage. */

int UseSwitch(int first, int second);

#endif // SWITCH_H_INCLUDED
