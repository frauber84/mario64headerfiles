#include <n64.h>
#include "explode.h"

#define SWAP(a,b)   { int t; t=a; a=b; b=t; }  // Macro for swapping

#define OVERLAP  85

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
       
    for(i = 0; i < n; i++)         // Make a pass through the array for each element
    {              
        for(j = 1; j < (n-i); j++) // Go through the array beginning to end
        {              
           if(a[j-1] > a[j])       // If the the first number is greater, swap it 
              SWAP(a[j-1],a[j]);   
        }
    }
}
  

void _start ( void )
{
     
     
    asm volatile("la $gp, _gp");

    int pseudostack, pseudostack2;
    int i, j, k, num_obj = 0;   
    u32 *counter = (void*)0x80500000;
    u32 *restart_counter = (void*)0x80500004;
    u32 *shell_pointer = (void*)0x80500030;
    u32 *shell_pointer2 = (void*)0x80500034;
    u32 *shell_pointer3 = (void*)0x80500038;
    u32 *cycle_count = (void*)0x80500080; 
    u32 *sound_call = (void*)0x802E8EAC; // noped to get rid of the amp sound
    u32 shell_behavior = SegmentedToVirtual(0x13003388);
    u32 *follow = (void*)0x80700000;
    
    u32 *list_pointer = (void*)0x80600000;
    
    struct MarioPos *Old_pos = (void*)0x80500060;
    struct MarioPos *Current_pos = (void*)0x80500070;

    *cycle_count += 1;

    if (*cycle_count == 2) *cycle_count = 0;
    
    Current_pos->x = Mario->MarioObj->x_pos;
    Current_pos->y = Mario->MarioObj->y_pos;
    Current_pos->z = Mario->MarioObj->z_pos;

        
    if (*restart_counter == 1)
    {
        *counter = 0;
        *restart_counter = 0;
    }
    *counter += 1;
    
    goto End;
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
        
        goto End;
    }
    End:
        
    if (*counter == 1)
    {
        Old_pos->x = Mario->MarioObj->x_pos;
        Old_pos->y = Mario->MarioObj->y_pos;
        Old_pos->z = Mario->MarioObj->z_pos;
    }
    
    *sound_call = 0; // ** dynamic recompilers
    
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
    
//    if (!Shell & !Shell2 & !Shell3) *restart_counter = 1;

    if (Mario->pad->currentButton & BUTTON_D_UP)
    {

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
        
        while (array[0] != *list_pointer) list_pointer++;
        
        list_pointer--;
        int closest_object = *list_pointer;
        
        list_pointer = (void*)0x80600008;
        *list_pointer = closest_object;
        
        Object *ClosestObject = (void*)closest_object;
        
        ClosestObject->x_scaling += 0.1;
        ClosestObject->y_scaling += 0.1;
        ClosestObject->z_scaling += 0.1;  
        ClosestObject->col_sphere_x += 0.1;
        ClosestObject->col_sphere_y += 0.1;
                      
        
    *CurrentObject = (void*)pseudostack;  /* Restoring before returning */
    
    }
    else if (Mario->pad->currentButton & BUTTON_D_DOWN)
    {
        u32 *t = (void*)0x80600008;
        Object *ClosestObject = *t;
        ClosestObject->x_scaling -= 0.1;
        ClosestObject->y_scaling -= 0.1;
        ClosestObject->z_scaling -= 0.1;      
        ClosestObject->col_sphere_x -= 0.1;
        ClosestObject->col_sphere_y -= 0.1;
    }                                  
    else if (Mario->pad->currentButton & BUTTON_D_RIGHT)
    {
        u32 *t = (void*)0x80600008;
        Object *ClosestObject = *t;
        ClosestObject->y_pos += 25;        

    }                                  
    else if (Mario->pad->currentButton & BUTTON_D_LEFT)
    {
        u32 *t = (void*)0x80600008;
        Object *ClosestObject = *t;

        pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */        
        
        *CurrentObject = ClosestObject;
        
        SetModel(0x01);
        
        CurrentObject = pseudostack;  /* Saving because this global variable will be messed up */        
        
        ClosestObject->animation = 0x80060030;
        


    }           
   else if (Mario->pad->currentButton & BUTTON_L)
   {
       *follow = 1;    
   }
   else if (Mario->pad->currentButton & BUTTON_R)
   {
       *follow = 0;    
   }
   
   
    if (*follow == 1)
    {
        u32 *t = (void*)0x80600008;
        Object *ClosestObject = *t;
        
        if ( Old_pos->x != Current_pos->x ) ClosestObject->x_pos += (Current_pos->x - Old_pos->x);
        if ( Old_pos->y != Current_pos->y ) ClosestObject->y_pos += (Current_pos->y - Old_pos->y);             
        if ( Old_pos->z != Current_pos->z ) ClosestObject->z_pos += (Current_pos->z - Old_pos->z);         
        
        
    }     


}
