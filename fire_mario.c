#include <n64.h>
#include "explode.h"

#define OVERLAP 85

    u32               *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    struct object     **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct object     *Fire;

void _start ( void )
{

    asm volatile("la $gp, _gp");

    int i, j,k  = 0;
    int pseudostack;
    int pseudostack2;
    u8 *pad = 0x8033AFA0;
    static u32 b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25;
    b25 = SegmentedToVirtual(0x13003174);     
    b24 = SegmentedToVirtual(0x13003510); 
    b23 = SegmentedToVirtual(0x13000940); 
    b22 = SegmentedToVirtual(0x130008EC); 
    b21 = SegmentedToVirtual(0x1300091C); 
    b20 = SegmentedToVirtual(0x1300090C); 
    b19 = SegmentedToVirtual(0x13001184);  // flame
    b18 = SegmentedToVirtual(0x130024AC);
    b17 = SegmentedToVirtual(0x13000A54);
    b16 = SegmentedToVirtual(0x13000A98);
    b15 = SegmentedToVirtual(0x13002B08);
    b14 = SegmentedToVirtual(0x130002B8);
    b13 = SegmentedToVirtual(0x13002C14);
    b12 = SegmentedToVirtual(0x13002CE0);
    b11 = SegmentedToVirtual(0x13000428);
    b10 = SegmentedToVirtual(0x13002DC0);
    b9 = SegmentedToVirtual(0x130011EC);
    b8 = SegmentedToVirtual(0x13002D50);
    b7 = SegmentedToVirtual(0x13002D7C);
    b6 = SegmentedToVirtual(0x13001390);
    b5 = SegmentedToVirtual(0x13000E58);
    b4 = SegmentedToVirtual(0x13000D98);
    b3 = SegmentedToVirtual(0x13000E3C);
    b2 = SegmentedToVirtual(0x13000E24);
    b1 = SegmentedToVirtual(0x13000AD8);  
    
    if  (*pad == 1)
    {
        pseudostack = (*Obj)->x_pos;
        pseudostack2 = (*Obj)->z_pos;
        
        (*Obj)->x_pos += 180; // offseting the position so that it doesn't hurt Mario right away
        (*Obj)->z_pos += 180;
        
        SpawnObj( (*Obj), 0x90, 0x13001184);  /* Flame (has to be scaled for better effect)*/
        
        (*Obj)->x_pos = pseudostack;
        (*Obj)->z_pos = pseudostack2;
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
                 
                     if ( (*Obj)->behavior_script_entry != b1 && (*Obj)->behavior_script_entry != b2 && (*Obj)->behavior_script_entry != b3 && (*Obj)->behavior_script_entry != b4 && (*Obj)->behavior_script_entry != b5 && (*Obj)->behavior_script_entry != b6 && (*Obj)->behavior_script_entry != b7 && (*Obj)->behavior_script_entry != b8 && (*Obj)->behavior_script_entry != b9 && (*Obj)->behavior_script_entry != b10 && (*Obj)->behavior_script_entry != b11 && (*Obj)->behavior_script_entry != b12 && (*Obj)->behavior_script_entry != b13 && (*Obj)->behavior_script_entry != b14 && (*Obj)->behavior_script_entry != b15 && (*Obj)->behavior_script_entry != b16 && (*Obj)->behavior_script_entry != b17 && (*Obj)->behavior_script_entry != b18 && (*Obj)->behavior_script_entry != b19 && (*Obj)->behavior_script_entry != b20 && (*Obj)->behavior_script_entry != b21 && (*Obj)->behavior_script_entry != b22 && (*Obj)->behavior_script_entry != b23 && (*Obj)->behavior_script_entry != b24 && (*Obj)->behavior_script_entry != b25)
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
