#include <n64.h>
#include <explode.h>

    struct object       **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct mario_struct *Mario                = (void*)M64_MARIO_OBJ_PTR;
    

#define UP_SPEED  90
void _start ( void )
{
     
     // 4066
     
    asm volatile("la $gp, _gp");
    
    u8 *count = (void*)0x80405000;
    u8 *on_air = (void*)0x80405001;

    if ( (*Obj)->_0x110 == 1 )   /* if mario is on top of trampoline */
    {
        *count += 1;
        (*Obj)->y_pos += 20;
        Mario->y_pos += 22;
        *on_air = 0;
        
        if (count == 1)
        {
            PlaySound(0x60086001);
        }
        
        if (*count > 3)
        {
            *on_air = 1;
        }
    }
    else if ( (*Obj)->_0x110 == 0)
    {
         *count = 0;
    }
    
    if (*on_air != 0)
    {
        Mario->y_pos += 90;
        
        if (*on_air == 1)
        {
            Mario->action = 0x01000882;
        }
         
        *on_air += 1;
        
        if (*on_air > 6) (UP_SPEED - ( (*on_air  - 6) * 5) );
        
        if (*on_air > 15) *on_air = 0;

    }
    


}
