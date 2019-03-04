#include <n64.h>
#include "explode.h"

#define OVERLAP  85

    u32   *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    Object **Obj = (void*)M64_CURR_OBJ_PTR;
    MarioStruct *Mario = (void*)0x8033b170;    
    Object *Shell, *Shell2, *Shell3, *CurrentShell = 0;
    
    u32 (*object_distance)[240] = (void*)0x80600000;
    
    struct MarioPos
    {
        float x,y,z;
    };

void _start ( void )
{
     
     
    asm volatile("la $gp, _gp");

    int pseudostack, pseudostack2;
    int i, j, k = 0;   
    u32 *counter = (void*)0x80500000;
    u32 *shell_pointer = (void*)0x80500030;
    u32 *shell_pointer2 = (void*)0x80500034;
    u32 *shell_pointer3 = (void*)0x80500038;
    u32 *cycle_count = (void*)0x80500004; 
    u32 *sound_call = (void*)0x802E8EAC; // noped to get rid of the amp sound
    struct MarioPos *Old_pos = (void*)0x80500010;
    struct MarioPos *Current_pos = (void*)0x80500020;
    u32 shell_behavior = SegmentedToVirtual(0x13003388);
    
    *cycle_count++;
    *counter += 1;
    
    if (*cycle_count == 2) *cycle_count = 0;
    
    Current_pos->x = Mario->MarioObj->x_pos;
    Current_pos->y = Mario->MarioObj->y_pos;
    Current_pos->z = Mario->MarioObj->z_pos;
    
    
    if (*counter > 40)
    {
        Shell = *shell_pointer;
        Shell2 = *shell_pointer2;
        Shell3 = *shell_pointer3;                
        goto End;        
    }
    if (*counter == 40)
    {
        Shell3 = SpawnObj( (*Obj), 0x00be, 0x13003388);  // koopa shell, amp behavior 
        Shell3->action = 0x02;
        Shell3->_0xf4 = 0x42f00000; // shell radius
        *shell_pointer3 = (void*)Shell3;
        Shell = *shell_pointer;
        Shell2 = *shell_pointer2;
        goto End;        
    }
    if (*counter > 15)
    {
        Shell = *shell_pointer;
        Shell2 = *shell_pointer2;
        goto End;        
    }
    if (*counter == 15)
    {
        Shell2 = SpawnObj( (*Obj), 0x00be, 0x13003388);  // koopa shell, amp behavior
        Shell2->action = 0x02;
        Shell2->_0xf4 = 0x42f00000; // shell radius
        *shell_pointer2 = (void*)Shell2;
        Shell = *shell_pointer;
        goto End;        
    }
    if (*counter > 1)
    {
        Shell = *shell_pointer;
        goto End;        
    }    
    if (*counter == 1)
    {
        Shell = SpawnObj( (*Obj), 0x00be, 0x13003388);  // koopa shell, amp behavior
        Shell->action = 0x02;
        Shell->_0xf4 = 0x42f00000; // shell radius
        *shell_pointer = (void*)Shell;
        
        Old_pos->x = Mario->MarioObj->x_pos;
        Old_pos->y = Mario->MarioObj->y_pos;
        Old_pos->z = Mario->MarioObj->z_pos;
        
        goto End;
    }
    End:
    
    *sound_call = 0; // ** dynamic recompilers
    
    if (Shell)
    {
        if ( Old_pos->x != Current_pos->x ) Shell->_0x164_x += (Current_pos->x - Old_pos->x);
        if ( Old_pos->y != Current_pos->y ) Shell->_0x168_y += (Current_pos->y - Old_pos->y);             
        if ( Old_pos->z != Current_pos->z ) Shell->_0x16c_z += (Current_pos->z - Old_pos->z);         
        Shell->action = 0x02;
    }
    
    if (Shell2)
    {
        if ( Old_pos->x != Current_pos->x ) Shell2->_0x164_x += (Current_pos->x - Old_pos->x);
        if ( Old_pos->y != Current_pos->y ) Shell2->_0x168_y += (Current_pos->y - Old_pos->y);             
        if ( Old_pos->z != Current_pos->z ) Shell2->_0x16c_z += (Current_pos->z - Old_pos->z);         
        Shell2->action = 0x02;        
    }
    if (Shell3)
    {
        if ( Old_pos->x != Current_pos->x ) Shell3->_0x164_x += (Current_pos->x - Old_pos->x);
        if ( Old_pos->y != Current_pos->y ) Shell3->_0x168_y += (Current_pos->y - Old_pos->y);             
        if ( Old_pos->z != Current_pos->z ) Shell3->_0x16c_z += (Current_pos->z - Old_pos->z);         
        Shell3->action = 0x02;        
    }
    
    if (*cycle_count == 0)
    {
        Old_pos->x = Mario->MarioObj->x_pos;
        Old_pos->y = Mario->MarioObj->y_pos;    
        Old_pos->z = Mario->MarioObj->z_pos;
    }
    
    pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
    *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
    
    /* check for collision */
    
    for (i = 0; i < 240; i++)
    {   
               
        if ( (*Obj)->behavior_script_entry == shell_behavior )
        {
             pseudostack2 = *CurrentObject;
             CurrentShell = *CurrentObject;
             *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
             
                 for (j = 0; j < 240; j++)
                 {
                 
                     if (    ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 ) && (*Obj)->animation != 0x80060030 && (*Obj)->behavior_script_entry != shell_behavior)
                     {
                         if ( (*Obj)->x_pos > CurrentShell->x_pos - OVERLAP && (*Obj)->x_pos < CurrentShell->x_pos + OVERLAP && (*Obj)->y_pos > CurrentShell->y_pos - OVERLAP && (*Obj)->y_pos < CurrentShell->y_pos + OVERLAP && (*Obj)->z_pos > CurrentShell->z_pos - OVERLAP && (*Obj)->z_pos < CurrentShell->z_pos + OVERLAP && (*Obj)->geo_layout_ptr !=  0x800F0860) 
                         {
                             SpawnObj( (*Obj), 0x00cd, 0x13003510);  // explosion
                             (*Obj)->active =0;
                             CurrentShell->active = 0;
                             
                             if (CurrentShell == Shell) *shell_pointer = 0;
                             if (CurrentShell == Shell2) *shell_pointer2 = 0;
                             if (CurrentShell == Shell3) *shell_pointer3 = 0;
                             
                         }
                     }
                     
                    *CurrentObject = (*Obj)->next;
                 }
                 
            *CurrentObject = pseudostack2;
        }
        
        *CurrentObject = (*Obj)->next;
    }
    
    
    *CurrentObject = (void*)pseudostack;  /* Restoring before returning */    
    
    if (!Shell & !Shell2 & !Shell3) *counter = -50;

    if (Mario->pad->currentButton & BUTTON_D_UP)
    {

        pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
        *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
     
        for (i = 0; i < 240; i++)
        {   
            
            if ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 )    
            {
                (*object_distance)[i] = (*Obj)->distance_from_mario;
            }
            else
            {
                (*object_distance0[i] = 0;
            }
            *CurrentObject = (*Obj)->next;
            
        }

    *CurrentObject = (void*)pseudostack;  /* Restoring before returning */
    
    }
    


}
