#include <n64.h>
#include "explode.h"

MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
u32  *CurrentObject = (void*)M64_CURR_OBJ_PTR;
Object  **Obj    = (void*)M64_CURR_OBJ_PTR;

void _start ( void )
{
    asm volatile("la $gp, _gp");
    
    int save = (void*)*CurrentObject;
    int i = 0;
    *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
        
    for (i = 0; i < 240; i++)
    {   
            
        if (  (*Obj)->y_speed < -10 )
        {
              (*Obj)->y_speed = -10;
        }
        else if  (  (*Obj)->y_speed > 10 )
        {
              (*Obj)->y_speed = 10;
        }
        
        
        *CurrentObject = (*Obj)->next;
    }
    
   if (Mario->y_speed < -10) Mario->y_speed = - 10;
   if (Mario->y_speed > 10) Mario->y_speed = 10;
    

    *CurrentObject = (void*)save;
            
}
