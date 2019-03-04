#include <n64.h>
#include "explode.h"



int main()
void _start ( void )
{
    asm volatile("la $gp, _gp");


    Level *CurrentLevel = (void*)0x8033B90c;
    MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
    Object  **Obj    = (void*)M64_CURR_OBJ_PTR;
    u32 *CurrentObject = (void*)M64_CURR_OBJ_PTR;


}
