#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"



void _start ( void )
{
     
    asm volatile("la $gp, _gp"); 
    
    int *object = (void*)0x80500000;
    
    
    if (*object != 0 )
    {
        Object  **Obj = (void*) *object;
        PrintInt(25, 160, "b0 %d", (int) (*Obj)->_0xb0);
        PrintInt(25, 140, "b4 %d", (int) (*Obj)->_0xb4);
        PrintInt(25, 120, "b8 %d", (int) (*Obj)->_0xb8);                        
        PrintInt(25, 100, "20 %d", (int) (*Obj)->_0x20);
        PrintInt(25, 80, "24 %d", (int) (*Obj)->_0x24);
        PrintInt(25, 60, "28 %d", (int) (*Obj)->_0x28);        
        PrintInt(25, 40, "e4 %d", (int) (*Obj)->_0xe4);
        PrintInt(25, 20, "f8 %d", (int) (*Obj)->_0xf8);
    }
    

}
