#include <n64.h>
#include "explode.h"

#define OVERLAP 95
#define FIRE_RATE  40

    u32               *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    struct object     **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct object     *Fire;
    struct mario_struct *Mario = 0x8033b170;

void _start ( void )
{

    asm volatile("la $gp, _gp");
    int i, j,k  = 0;
    int pseudostack;
    int pseudostack2;
    u8 *pad = 0x8033AFA0;
    u32 *Counter  = (void*)0x80500000;   
    u32 *run_once = (void*)0x80500004; 
    u32 *fire_floats_pointer = SegmentedToVirtual(0x130011A8);


    if (*run_once == 0)
    {
        u16 *fire_patch = 0x802af4da; // some pointers to fireball routine patches
        u32 *fire_patch2 = 0x802AF564;
        u32 *Color1 = SegmentedToVirtual(0x04000000);
        u32 *Color2 = SegmentedToVirtual(0x04000008);
        u32 *Color3 = SegmentedToVirtual(0x04000018);
        u32 *Color4 = SegmentedToVirtual(0x04000020);

        *Color1 = 0x50000000;
        *Color2 = 0xFF000000;
        *Color3 = 0x50505000;
        *Color4 = 0xFFFFFF00;

        *fire_patch = 0x3F80; // affects fireball initial trajectory
        *fire_patch2 = 0; // nopes the write to obj->0xb8 which fixes the fireball speed
        
        *fire_floats_pointer = 0x001EFD30;  // patching behavior to alter physics
        fire_floats_pointer++;
        *fire_floats_pointer = 0xFF9A03E8;
        fire_floats_pointer++;    
        *fire_floats_pointer = 0x03E800C8;
        
        
        
        *run_once = 1;
    }
    
    *Counter += 1;


    //debug stuff


    if (Mario->action & 0x30000000) PrintXY(60, 60, "ON WATER");            
 
 
//    PrintInt(10, 80, "X %d", (int)Mario->x_pos);
//    PrintInt(10, 100, "Y %d", (int)Mario->y_pos);
//    PrintInt(10, 120, "Z %d", (int)Mario->z_pos);
 
//    if (*pad & 0x80) PrintXY(10, 160, "1");    
//    if (*pad & 0x40) PrintXY(35, 160, "2");
//    if (*pad & 0x20) PrintXY(55, 160, "3");
    PrintInt(10, 140, "SPD %d", (int)Mario->speed);
//    PrintInt(10, 60, "ACTION %x", (int)Mario->action);
//    PrintInt(10, 40, "GROUND Y %d", (int)Mario->_0x70);
  
    
    // end debug
    
    if (*Counter > FIRE_RATE )
    {
        PrintXY(100, 20, "Ready to fire  ");
        
        if  (*pad & 1)
        {
            u32 MarioSpeed = (int)Mario->speed;
                        
            *fire_floats_pointer = 0x001EFD30;  // normal 'gravity' for fire

            if (*pad & 0x40)  *fire_floats_pointer = 0x001EFE60;
            
            if (*pad & 0x20)  *fire_floats_pointer = 0x001EFA00;
            
            struct object *ChildObj = SpawnObj( (*Obj), 0x90, 0x13001184);   /* Mario fireball behavior */
            
            if (MarioSpeed >= 0)            
            {
                ChildObj->_0xb8 = 15 + MarioSpeed;
            }
            else
            {
               ChildObj->_0xb8 = 15;
            }
            
           if (Mario->action & 0x30000000)   // on water
            {
                ChildObj->_0xb8 = 5;
            }
            
            if (*pad & 0x80)
            {
                ChildObj->x_scaling = 3;
                ChildObj->y_scaling = 3;
                ChildObj->z_scaling = 3;
            }
            
            PlaySound(0x60086001);
            *Counter = 0;
        }
        
    }
    else
    {
        PrintXY(60, 20, "WAIT   ");
        PrintInt(*Counter+78, 20, ",      ", *Counter); // debug
    }
    
    pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
    *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
    
    u32 flame_behavior = SegmentedToVirtual(0x13001184);

    for (i = 0; i < 240; i++)
    {   
               
        if ( (*Obj)->behavior_script_entry == flame_behavior )
        {
             pseudostack2 = *CurrentObject;
             Fire = *CurrentObject;
             *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
             
                 for (j = 0; j < 240; j++)
                 {
                 
                     if (    ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 ) && (*Obj)->animation != 0x80060030   )
                     {
                         if ( (*Obj)->x_pos > Fire->x_pos - OVERLAP && (*Obj)->x_pos < Fire->x_pos + OVERLAP && (*Obj)->y_pos > Fire->y_pos - OVERLAP && (*Obj)->y_pos < Fire->y_pos + OVERLAP && (*Obj)->z_pos > Fire->z_pos - OVERLAP && (*Obj)->z_pos < Fire->z_pos + OVERLAP && (*Obj)->geo_layout_ptr !=  0x800F0860) 
                         {
                             SpawnObj( (*Obj), 0x00cd, 0x13003510);  // explosion
                             (*Obj)->active =0;
                             Fire->active = 0;
                         }
                     }
                     
                    *CurrentObject = (*Obj)->next;
                 }
                 
            *CurrentObject = pseudostack2;
        }
        
        *CurrentObject = (*Obj)->next;
    }
    *CurrentObject = (void*)pseudostack;  /* Restoring before returning */

}
