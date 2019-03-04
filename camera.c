#include <n64.h>
#include "explode.h"

   struct mario_obj  *Mario                = (void*)M64_MARIO_OBJ_PTR;    

void _start ( void )
{
    asm volatile("la $gp, _gp");
    
/*    u32 *nopbyte = (void*)0x80254318;
    
    *nopbyte = 0x00000000;
    
    float *camera_x = (void*)0x8033C524;
    float *camera_y = (void*)0x8033C528;
    float *camera_z = (void*)0x8033C52C;
    
    *camera_x = Mario->x_pos;
    *camera_y = Mario->y_pos;
    *camera_z = Mario->z_pos;
    
*/    

    float *camera_distance = (void*)0x8033c5a4;    
    
    *camera_distance = -50;
    
/*    u8 *pad     = (void*)0x8033AFA0;
    
    if (*pad & 1 )
    {
        *camera_distance += 3;
    }    
    else if (*pad & 2 )
    {
        *camera_distance -= 3;
    }    
*/    
    
}
