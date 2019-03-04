#include <n64.h>
#include "explode.h"

MarioStruct *Mario = (void*)M64_MARIO_STRUCT;

void _start ( void )
{

    asm volatile("la $gp, _gp");
    
    char buffer[30];
    sprintf(buffer, "TESTE %f", Mario->speed);
    PrintXY(20, 20, buffer);
    

}
