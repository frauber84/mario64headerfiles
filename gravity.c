#include <n64.h>
#include "explode.h"

void PrintFloat(int x, int y, float number, int decimalpoints);
MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
int *first_run = (void*)0x80500000;
float *gravity = (void*)0x80500004;
int *counter = (void*)0x80500008;
u32 *test = (void*)0x80500010;

int PatchList[11] =
{
    0x80256526,
    0x802566A2,
    0x802566BE,
    0x80256726,
    0x80256742,
    0x802567FE,
    0x8025681A,
    0x802569B6,
    0x802569D2,
    0x802E451A,
    0x802E4902,
};


void _start ( void )
{

    asm volatile("la $gp, _gp");
    if (*first_run == 0)
    {
        *gravity = -75;
        *first_run = 1;
    }
    *counter += 1;
    
     PrintInt(20, 20, "gravity ", 0);
     PrintFloat(140, 20, *gravity, 2);
     
     Pad *Controller = (void*)0x8033AF90;
     
     if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter > 3)
     {
         *gravity -= 1;
         *counter = 0;
     }
     if (Mario->pad->currentButton & BUTTON_D_RIGHT  && *counter > 3)
     {
         *gravity += 1;
         *counter = 0;
     }
     if (Mario->pad->currentButton & BUTTON_D_DOWN && *counter > 3)
     {
         *gravity -= 5;
         *counter = 0;
     }
     if (Mario->pad->currentButton & BUTTON_D_UP && *counter > 3)
     {
         *gravity += 5;
         *counter = 0;
     }
     
     int *patch = (void*)0x80500004;
//J     PrintInt(40, 40, "%x", (*patch) >> 16);
     *test = (*patch) >> 16;
     
     for (int i = 0; i < 11; i++)
     {
         u16 *ptr = (void*)PatchList[i];
         *ptr = (*patch) >> 16;
     }
     
}

void PrintFloat(int x, int y, float number, int decimalpoints)
{
    
    int i;
    int mult = 1;
    
    for (i = 0; i < decimalpoints; i++)
    {
        mult = mult * 10;
    }
    
    int integer = (int)number;
    float fractional = (number - integer);
    int fraction = (int)(fractional * (mult));
    
    if (fraction < 0) fraction = (-fraction);
     
    PrintInt(x, y, "%d", integer);
    
    if (integer < 0) x += 25;
    
    if (integer >= 10)  x+= 25;
    if (integer >= 100)  x+= 25;
    if (integer >= 1000)  x+= 25;
    if (integer >= 10000)  x+= 25;
    
    PrintInt(x+25, y, "*%d", fraction);
     
}
