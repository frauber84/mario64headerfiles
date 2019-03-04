#include <n64.h>
#include "explode.h"
#include "levels.h"

#define SWAP(a,b)   { int t; t=a; a=b; b=t; }  // Macro for swapping
#define OVERLAP  85
#define FIRE_RATE  100

    u32   *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    Object **Obj = (void*)M64_CURR_OBJ_PTR;
    MarioStruct *Mario = (void*)0x8033b170;    
    Object *Shell, *Shell2, *Shell3, *CurrentShell = 0;

    struct MarioPos
    {
        float x,y,z;
    };
    
void bubble_srt(int a[], int n);

void bubble_srt( int a[], int n )  
{   
    int i, j;
       
    for(i = 0; i < n; i++)
    {              
        for(j = 1; j < (n-i); j++)
        {              
           if(a[j-1] > a[j])
              SWAP(a[j-1],a[j]);   
        }
    }
}

void _start ( void )
{
     
    asm volatile("la $gp, _gp");  // ensure correct data pointer is loaded

    int pseudostack, pseudostack2;
    int i, j, k, num_obj = 0;   
    u32 *counter = (void*)0x80500000;
    u32 *firecounter = (void*)0x80500008;
    u32 *restart_counter = (void*)0x80500004;
    u32 *shell_pointer = (void*)0x80500030;
    u32 *shell_pointer2 = (void*)0x80500034;
    u32 *shell_pointer3 = (void*)0x80500038;
    u32 *follow_pointer = (void*)0x80500040;
    u32 *follow_pointer2 = (void*)0x80500044;
    u32 *follow_pointer3 = (void*)0x80500048;
    
    u32 *sound_call = (void*)0x802E8EAC; // NOP'd to get rid of the amp sound
    u32 shell_behavior = SegmentedToVirtual(0x13003388);
    u32 throwing_shell_behavior = SegmentedToVirtual(0x13004148);
    u32 *list_pointer = (void*)0x80600000;
    
    u16 *current_level = (void*)0x8032ddf8;
    
    if (*restart_counter == 1)
    {
        *counter = 0;
        *restart_counter = 0;
    }
    if (*current_level == BOB_OMB_BATTLEFIELD) *counter += 1;
    
    *firecounter += 1;
    
    if ( !(!Shell & !Shell2 & !Shell3) )
    {
        if (*firecounter > FIRE_RATE )
        {
            PrintXY(100, 20, "Ready to fire  ");
        }
        else
        {
            PrintXY(60, 20, "WAIT   ");
            PrintInt(*firecounter+78, 20, ",      ", *firecounter); // debug
        }
    }
    
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
        
        if (*current_level == BOB_OMB_BATTLEFIELD) 
        {
            Shell3->x_scaling = 1.5;
            Shell3->y_scaling = 1.5;
            Shell3->z_scaling = 1.5;                
//            Shell3->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC); // bed reta shell
        }
        
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

        if (*current_level == BOB_OMB_BATTLEFIELD) 
        {
            Shell2->x_scaling = 1.5;
            Shell2->y_scaling = 1.5;
            Shell2->z_scaling = 1.5;                
//            Shell2->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC); // bed reta shell
        }
        
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
        
        if (*current_level == BOB_OMB_BATTLEFIELD) 
        {
            Shell->x_scaling = 1.5;
            Shell->y_scaling = 1.5;
            Shell->z_scaling = 1.5;                
//            Shell->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC); // bed reta shell
        }
        
        *shell_pointer = (void*)Shell;
        
        goto End;
    }
    End:
        
    *sound_call = 0; // ** screw dynamic recompilers
    
    /* this has to be rewritten so there are three different 'follow' pointers */
    u16 *homing_patches = (void*)0x802F3EB6;
    *homing_patches = 0x8060;
    homing_patches = (void*)0x802F3EC2;
    *homing_patches = 0x0008;    
    homing_patches = (void*)0x802F3ED6;
    *homing_patches = 0x8060;    
    homing_patches = (void*)0x802F3EDA;
    *homing_patches = 0x0008;    
    homing_patches = (void*)0x802F3F02;
    *homing_patches = 0x8060;    
    homing_patches = (void*)0x802F3F0E;
    *homing_patches = 0x0008;    
    homing_patches = (void*)0x802F3F56;
    *homing_patches = 0x8060;    
    homing_patches = (void*)0x802F3F5A;
    *homing_patches = 0x0008;    
    homing_patches = (void*)0x802F3EDE;
    *homing_patches = 0x41C8; // height of the shell when getting near the enemies
    homing_patches = (void*)0x802F3FF8;
    *homing_patches = 0x2400;  // nope function to cancel "1-up" collision with Mario
    
    // keep shells circling Mario
    
    if (Shell)
    {
        
        Shell->_0x164_x = Mario->MarioObj->x_pos;
        Shell->_0x168_y = Mario->MarioObj->y_pos;        
        Shell->_0x16c_z = Mario->MarioObj->z_pos;        
        
        Shell->action = 0x02;
    }
    
    if (Shell2)
    {
        
        Shell2->_0x164_x = Mario->MarioObj->x_pos;
        Shell2->_0x168_y = Mario->MarioObj->y_pos;        
        Shell2->_0x16c_z = Mario->MarioObj->z_pos;        
        
        Shell2->action = 0x02;        
    }
    if (Shell3)
    {
               
        Shell3->_0x164_x = Mario->MarioObj->x_pos;
        Shell3->_0x168_y = Mario->MarioObj->y_pos;        
        Shell3->_0x16c_z = Mario->MarioObj->z_pos;        
               
        Shell3->action = 0x02;        
    }
    

    pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
    *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
    
    /* check for collision */
    
    for (i = 0; i < 240; i++)
    {   
               
        if ( (*Obj)->behavior_script_entry == shell_behavior || (*Obj)->behavior_script_entry == throwing_shell_behavior )
        {
             pseudostack2 = *CurrentObject;
             CurrentShell = *CurrentObject;
             *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
             
                 for (j = 0; j < 240; j++)
                 {
                 
                     if (    ( (*Obj)->animation != 0 /* || (*Obj)->collision_ptr != 0 */  ) && (*Obj)->animation != 0x80060030 && (*Obj)->behavior_script_entry != shell_behavior && (*Obj)->behavior_script_entry != throwing_shell_behavior )
                     {
                         if ( (*Obj)->x_pos > CurrentShell->x_pos - OVERLAP && (*Obj)->x_pos < CurrentShell->x_pos + OVERLAP && (*Obj)->y_pos > CurrentShell->y_pos - OVERLAP && (*Obj)->y_pos < CurrentShell->y_pos + OVERLAP && (*Obj)->z_pos > CurrentShell->z_pos - OVERLAP && (*Obj)->z_pos < CurrentShell->z_pos + OVERLAP && (*Obj)->geo_layout_ptr !=  0x800F0860) 
                         {
                             SpawnObj( (*Obj), 0x00cd, 0x13003510);  // explosion
                             (*Obj)->active = 0;
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
    
    // if button is pressed, create the throwing shell with 1-up homing in behavior
    
    if (Mario->pad->currentButton & BUTTON_D_UP && *firecounter > FIRE_RATE && !(!Shell & !Shell2 & !Shell3) )
    {

        // find nearest object using bubble sort
        
        pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */
    
        *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
        list_pointer = (void*)0x80600010;
     
        for (i = 0; i < 240; i++)
        {   
            
            if ( (*Obj)->animation != 0 /* || (*Obj)->collision_ptr != 0 */  && (*Obj)->behavior_script_entry != shell_behavior && Obj != Mario->MarioObj )
            {
                *list_pointer = *CurrentObject;
                list_pointer++;
                *list_pointer = (*Obj)->distance_from_mario;
                list_pointer++;
                
                num_obj++;
            }
            *CurrentObject = (*Obj)->next;
            
        }
        list_pointer = (void*)0x80600000;
        *list_pointer = num_obj;
        
        int array[num_obj];
        
        for (i = 0; i < num_obj; i++)
        {
           u32 *distance = (void*)0x80600010 + 4 + (i*8);
           array[i] = *distance;
        }
        bubble_srt(array, num_obj);
        
        while (array[0] != *list_pointer) list_pointer++; // match object with the correct pointer
        
        list_pointer--; // back 4 bytes to get the object pointer
        int closest_object = *list_pointer;
        
        list_pointer = (void*)0x80600008;
        *list_pointer = closest_object;
        
        Object *ClosestObject = (void*)closest_object;
        
        *CurrentObject = (void*)pseudostack;  /* Restoring before returning */

        // transform one of the shells into a throwring shell
        
        if (Shell3) 
        {
                    
            Object *ThrowingShell = SpawnObj( (*Obj), 0x00be, 0x13004148);
            
            if (*current_level == BOB_OMB_BATTLEFIELD) 
            {
                ThrowingShell->x_scaling = 1.5;
                ThrowingShell->y_scaling = 1.5;
                ThrowingShell->z_scaling = 1.5;
//                ThrowingShell->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC);
            }            
            
            ThrowingShell->action = 0x01;  // bypass animation, throw it right away
            ThrowingShell->x_pos = Shell3->x_pos;
            ThrowingShell->y_pos = Shell3->y_pos;
            ThrowingShell->z_pos = Shell3->z_pos;
            
            Shell3->active = 0;
            *shell_pointer3 = 0;
        }
        else if (Shell2)
        {
             
            Object *ThrowingShell = SpawnObj( (*Obj), 0x00be, 0x13004148);
            
            if (*current_level == BOB_OMB_BATTLEFIELD) 
            {
                ThrowingShell->x_scaling = 1.5;
                ThrowingShell->y_scaling = 1.5;
                ThrowingShell->z_scaling = 1.5;
//                ThrowingShell->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC);
            }            
            
            ThrowingShell->action = 0x01;
            ThrowingShell->x_pos = Shell2->x_pos;
            ThrowingShell->y_pos = Shell2->y_pos;
            ThrowingShell->z_pos = Shell2->z_pos;
            
            Shell2->active = 0;
            *shell_pointer2 = 0;            
             
        }
        else if (Shell)
        {
             
            Object *ThrowingShell = SpawnObj( (*Obj), 0x00be, 0x13004148);
            
            if (*current_level == BOB_OMB_BATTLEFIELD) 
            {
                ThrowingShell->x_scaling = 1.5;
                ThrowingShell->y_scaling = 1.5;
                ThrowingShell->z_scaling = 1.5;
//                ThrowingShell->geo_layout_ptr = SegmentedToVirtual(0x0F000ADC);
            }            
            
            ThrowingShell->action = 0x01;
            ThrowingShell->x_pos = Shell->x_pos;
            ThrowingShell->y_pos = Shell->y_pos;
            ThrowingShell->z_pos = Shell->z_pos;
            
            Shell->active = 0;
            *shell_pointer = 0;            
             
        }
        
        *firecounter = 0;
    
    }
    
    if (Mario->pad->currentButton & BUTTON_D_DOWN && (!Shell & !Shell2 & !Shell3) && ( *current_level == BOB_OMB_BATTLEFIELD ) )
    {
        *restart_counter = 1;  // spawn new shells
    }
    

}
