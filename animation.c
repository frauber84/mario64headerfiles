#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"
#define SWAP(a,b)   { int t; t=a; a=b; b=t; }  // Macro for swapping

#define  BOBOMB                0x0802396c
#define  GOOMBA                0x0801DA4C
#define  KOOPA                 0x06011364
#define  CHUCKYA               0x0C00C070
#define  CHAINCHOMP            0x06025178
#define  DOOR                  0x030156C0
#define  PEACH                 0x0501c504
#define  SPINDRIFT             0x05002d68
#define  HEAVEVO               0x0501534c
#define  BOWSER                0x060577e0
#define  PENGUIN               0x05008b74
#define  SCUTTLEBUG            0x06015064

u32          *CurrentObject = (void*)M64_CURR_OBJ_PTR;
Object       **Obj          = (void*)M64_CURR_OBJ_PTR;
MarioStruct  *Mario         = (void*)M64_MARIO_STRUCT;


void bubble_srt(int a[], int n);

void _start ( void )
{
     
    asm volatile("la $gp, _gp"); 
    int pseudostack, pseudostack2;
    int i, j, k, num_obj = 0;   
    u32 *list_pointer = (void*)0x80600000; 
    int *AnimationPtr = (void*)0x80700000;   
    
    if (Mario->pad->currentButton & BUTTON_D_UP )
    {
        pseudostack = *CurrentObject;
    
        *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
        list_pointer = (void*)0x80600010;
     
        for (i = 0; i < 240; i++)
        {   
            
            if ( (*Obj)->animation != 0 && Obj != Mario->MarioObj )
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
        
        *AnimationPtr = (void*) SegmentedToVirtual(ClosestObject->animation_ptr);
        
        *CurrentObject = (void*)pseudostack;
        
        

    }
        
    list_pointer = (void*)0x80600008;
    
    if ( *list_pointer != 0 )
    {
       
        Object *ClosestObject = (void*) *list_pointer;
       
        switch ( ClosestObject->animation_ptr ) 
        {
              
           case BOBOMB:
               PrintXY(30, 100, "BobOmb");
               break;
           
           case KOOPA:
                PrintXY(30, 100, "Koopa");
                break;

           case CHAINCHOMP:
                PrintXY(30, 100, "ChainChomp");
                break;
              
           case BOWSER:
                PrintXY(30, 100, "Bowser");
                break;
              
           case DOOR:
                PrintXY(30, 100, "Door");
                break;
           
           case GOOMBA:
                PrintXY(30, 100, "Goomba");
                break;
              
           default:
               break;
              
              
        }
       
        PrintInt(30, 60, "frame %d", (int)ClosestObject->anim_current_frame );
       
        PrintInt(30, 80, "animation 80%x", (int)*AnimationPtr & 0xFFFFFF);
       
              
    }
    
    
}

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
