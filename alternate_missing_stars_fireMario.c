#include <n64.h>
#include "explode.h"

#define OVERLAP 160
#define BOWSER_BATTLE_3    0x0022
#define FIRE_RATE          70

    u32               *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    Object     **Obj                 = (void*)M64_CURR_OBJ_PTR;
    Object     *Fire;
    MarioStruct *Mario = (void*)M64_MARIO_STRUCT;

void _start ( void )
{

    asm volatile("la $gp, _gp");
    
    int i, j,k  = 0;
    int pseudostack;
    int pseudostack2;
    int pseudostack3;
    
    u8 *pad     = (void*)0x8033AFA0;
    u32 *Flames = (void*)0x80500004;
    u32 *ColorFlag = (void*)0x80500008;
    u32 *Counter = (void*)0x80500010;
    
    *Flames = -3;
    
    pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
    *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
    
    u32 flame_behavior = SegmentedToVirtual(0x130001F4);   // originally 0x13001184 
    
    if (*ColorFlag == 0)
    {
        /* fire mario colors! */
        u32 *Color1 = SegmentedToVirtual(0x04000000);
        u32 *Color2 = SegmentedToVirtual(0x04000008);
        u32 *Color3 = SegmentedToVirtual(0x04000018);
        u32 *Color4 = SegmentedToVirtual(0x04000020);
        
        u32 *OldColor1 = (void*)0x80500030;
        u32 *OldColor2 = (void*)0x80500034;
        u32 *OldColor3 = (void*)0x80500038;
        u32 *OldColor4 = (void*)0x8050003c;

        *OldColor1 = *Color1;
        *OldColor2 = *Color2;
        *OldColor3 = *Color3;
        *OldColor4 = *Color4;
        
        *Color1 = 0x50000000;
        *Color2 = 0xFF000000;
        *Color3 = 0x50505000;
        *Color4 = 0xFFFFFF00;
        
        *ColorFlag = 1;
    }

    for (i = 0; i < 240; i++)
    {   
               
        if ( (*Obj)->behavior_script_entry == flame_behavior )
        {
             *Flames += 1;
             pseudostack2 = *CurrentObject;
             Fire = *CurrentObject;
             *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
             
                 for (j = 0; j < 240; j++)
                 {
                 
                         if ( (*Obj)->x_pos > Fire->x_pos - OVERLAP && (*Obj)->x_pos < Fire->x_pos + OVERLAP && (*Obj)->y_pos > Fire->y_pos - OVERLAP && (*Obj)->y_pos < Fire->y_pos + OVERLAP && (*Obj)->z_pos > Fire->z_pos - OVERLAP && (*Obj)->z_pos < Fire->z_pos + OVERLAP && Obj != Mario->MarioObj && ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 ) && (*Obj)->behavior_script_entry != flame_behavior && (*Obj)->animation != 0x80060030 && (*Obj)->health != 666) 
                         {
                             SpawnObj( (*Obj), 0x00cd, 0x13003510);  // explosion
                             Fire->active = 0;
                             (*Obj)->active = 0;
                             (*Obj)->health = 666;
                         }
                     
                    *CurrentObject = (*Obj)->next;
                 }
                 
            *CurrentObject = pseudostack2;
        }
        
        *CurrentObject = (*Obj)->next;
    }
    *CurrentObject = pseudostack;
    
    *Counter += 1;

    if (*Counter > FIRE_RATE )
    {
        PrintXY(60, 20, "Ready   ");
        
        if  (*pad & 1)
        {
            SpawnObj( (*Obj), 0x90, 0x130001f4);   /* Mario fireball behavior */
            PlaySound(0x60086001);
            *Counter = 0;
        }
        
    }
    else
    {
        PrintXY(60, 20, "Charging");
        PrintInt(*Counter+108, 20, ",      ", *Counter); // debug
    }
    

   

}
