#include <n64.h>
#include <explode.h>
#include "levels.h"

#define MIN_VALUE  0x3C800000
#define MAX_VALUE  0x42C00000
#define MIN_VALUE2 0xBC800000
#define MAX_VALUE2 0xC2C00000

    struct object       **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct object       **Mario                = (void*)0x80331158;
    struct mario_struct  *MarioStruct          = (void*)0x8033b170;
 

void _start ( void )
{
    
    asm volatile("la $gp, _gp");
        
    u32 *patch1 = (void*)0x8037903C;
    u32 *patch2 = (void*)0x80379060;
    
    u32 *magic_pointer = 0x80406000;
    u8 *flag           = 0x80405000;
    
    u8 *mushroom_flag = 0x80407004;
    u8 *music_flag    = 0x80407008;
    
    u16 *music_tempo = (void*)0x80222622;
    
    u32 *colors = (void *)0x8007EC20;
    
    *patch1 = 0x3C018040;
    *patch2 = 0x3C018040;
    
    int i;
    

    for (i = 0; i < 0x5c; i++)
    {
        *colors += 0x100;
        colors += 1;    /* Point to next word */
    }
    
    
    if (*flag == 0)
    {
        *magic_pointer = MIN_VALUE;
        *flag = 1;
    }
    
    if (*magic_pointer > MAX_VALUE && *flag == 1 ) *flag = 2;

    if (*magic_pointer < MIN_VALUE && *flag == 2 ) *flag = 3;
    
    if (*magic_pointer > MAX_VALUE2 && *flag == 4) *flag = 5;

    if (*magic_pointer < MIN_VALUE2 && *flag == 5) *flag = 0;    
    
    
    if (*flag == 1 || *flag == 4)
    {
        *magic_pointer += 0x100000;
    }
    else if (*flag == 2 || *flag == 5)
    {
         *magic_pointer -= 0x100000;
    }
    else if (*flag == 3)
    {
        *magic_pointer = MIN_VALUE2;
        *flag = 4;
    }
    
    if (*music_tempo > 0x8000)
    {
        *music_flag = 2;
    }
    else if (*music_tempo < 0x0300 )
    {
        *music_flag = 1;
    }
    
    if (*music_flag == 2)
    {
        *music_tempo -= 0x70;
    }
    else if (*music_flag == 1 || *music_flag == 0)
    {
        *music_tempo += 0x70;
    }
    
    if (*mushroom_flag == 2)
    {
        *patch1 = 0x3C018038;
        *patch2 = 0x3C018038;
    }

    
    
}   


