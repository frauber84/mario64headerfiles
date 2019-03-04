#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"

/* remove this to get rid of debug prints */
#define DEBUG      

/* globals */
Object **Obj = (void*)M64_CURR_OBJ_PTR;
MarioStruct *Mario = (void*)M64_MARIO_STRUCT;   
u32 *result = (void*)0x80500000; /* temporary, this variable should be inside the object struct */

/* function prototypes */
void Action_00(void);
void Action_02(void);
void Action_04(void);
void ProcessJoyPad(void);
int  ShowDialog(u16 distance, u16 param1, u16 param2, u16 x_position, u16 message_ID);
int  Yoshi_Jump(u16 step, u16 time);

/* entry-point for the behavior (0x80400000) */
void _start ( void )
{
    asm volatile("la $gp, _gp");  /* to ensure the correct data pointer is loaded */
    
    /*  initialization. in this example, we'll use offset 0x254 of the object  */
    /*  struct to hold some variables, since it seems unused by most objects   */
    if ( (*Obj)->_0x254 == 0 )
    {
        (*Obj)->action = 0;
        
        /* x,y,z position would be normally set by the 0x24 behavior, but since */
        /* we are hijacking an existing object, we have to change its position  */
        (*Obj)->x_pos = -3389;
        (*Obj)->y_pos = -510;        
        (*Obj)->z_pos = -1617;
        
        SetObjAnimation( YOSHI_STANDING );
        
        (*Obj)->_0x254 = 1;
        
        }

    
    /* calculates distance from Mario */
    (*Obj)->distance_from_mario = DistanceFromObject( (*Obj), Mario->MarioObj);
    
 

/* rotation toward Mario doesn't work properly yet ( ToDo: investigate further ) */

//  if ( (*Obj)->distance_from_mario < 1000 )
//  {
//      (*Obj)->rot_y = RotateTorwardsMario( (*Obj)->rot_y, 0x100, (*Obj)->_0x160 );
//  }

    
    /* process action value */
    switch ( (*Obj)->action )
    {
           case -1:
                /* gives Mario some time if he refuses Yoshi's first dialog */
                ShakeScreen(2);
                if ( (*Obj)->timer > 60 ) (*Obj)->action = 0;
                break;
                
           case 0:
                Action_00();
                break;

           case 1:
               if ( ShowDialog(250, 2, 1, 0xA2, 0x00) == 1)
               {
                   (*Obj)->action = 2;
                   SetObjAnimation( YOSHI_WALKING );
               }
               break;
                
           case 2:
                Action_02();
                break;
                
           case 3:
                if ( ShowDialog(250, 2, 1, 0xA2, 0x01) == 1) (*Obj)->action = 4;
                break;
                
           case 4:
                Action_04();
                break;
                
           case 5:
                if ( ShowDialog(250, 2, 1, 0xA2, 0x02) == 1) (*Obj)->action = 6;
                break;

           case 6:
                if ( Yoshi_Jump(13, 10) == 1 ) (*Obj)->action = 7;
                break;
                
           case 7:
                if ( ShowDialog(250, 2, 1, 0xA2, 0x03) == 1) (*Obj)->action = 8;
                break;
                
           case 8:
                ProcessJoyPad();
                break;
                
           case 9:
               if ( Yoshi_Jump(13, 10) == 1 )
               {
                   (*Obj)->action = 8;
                   (*Obj)->_0x254 += 1;
                   
                   if ( (*Obj)->_0x254 == 12 ) (*Obj)->action = 10;
                   
               }
               break;
               
           case 10:
                if ( ShowDialog(250, 2, 1, 0xA2, 0x04) == 1) (*Obj)->action = 11;
                break;
                
           case 11:
                if ( (*Obj)->timer < 100)
                {
                    (*Obj)->x_scaling += 0.05;
                    (*Obj)->y_scaling += 0.05;
                    (*Obj)->z_scaling += 0.05;
                }
                else if ( (*Obj)-> timer > 100 )
                {
                    ExplodeObject ( (*Obj) );
                }
                break;
                
           default:
               break;
    }

#ifdef DEBUG    
    PrintInt(20, 20, "action %d", (*Obj)->action);
    PrintInt(20, 40, "timer %d", (*Obj)->timer);
    PrintInt(20, 60, "254 %d", (*Obj)->_0x254);
    PrintInt(20, 80, "distance %d", (*Obj)->distance_from_mario);
    PrintInt(20, 100, "choice %d", *result);
#endif

}

void Action_00 (void)
{

    if ( (*Obj)->distance_from_mario < 250 )
    {
        *result = CreateMessageBox(2, 11, 0xA3, 0x08);
    
        if (*result == 1)            /* choice no. 1 */
        {
            (*Obj)->action = 1;
        }
        else if (*result == 2)       /* choice no. 2 */
        {
            (*Obj)->action = -1;
            Mario->action = 0x00020338;  /* give him a shock! */
        }
    }
    
}

/* makes Yoshi walks a bit */
void Action_02 (void)
{
     if ( (*Obj)->timer < 250 )
     {
         (*Obj)->x_pos += 19;
     }
     else
     {
         SetObjAnimation ( YOSHI_STANDING );
         (*Obj)->action = 3;
     }
}

/* spawns a few 1-up mushrooms */
void Action_04(void)
{
    if ( (*Obj)->timer > 50 )
    {
        (*Obj)->timer = 0;
        Object *NewObject = SpawnObj( (*Obj), 0x00D4, 0x13004148);
         
        NewObject->action = 1;
        NewObject->y_pos += 100;
        NewObject->x_scaling = 3;
        NewObject->y_scaling = 3;
        NewObject->z_scaling = 3;
         
        (*Obj)->_0x254 += 1;
    }
     
    if ( (*Obj)->_0x254 == 6 )
    {
        (*Obj)->action = 5;
    }
}

void ProcessJoyPad (void)
{

     if (Mario->pad->currentButton & BUTTON_D_UP)
     {
         (*Obj)->action = 9; /* Yoshi Jump */
     }
     
}

int ShowDialog(u16 distance, u16 param1, u16 param2, u16 x_position, u16 message_ID)
{
    if ( (*Obj)->distance_from_mario < distance )
    {
        if ( CreateMessageBox(param1, param2, x_position, message_ID) == 3 ) return 1;  /* dialog has ended */
        else return 0; /* dialog is happening */
    }
}

/* Sucky jump function ;) */
int Yoshi_Jump(u16 step, u16 time)
{
    
    if ( (*Obj)-> timer == 0 )      SetObjAnimation ( YOSHI_JUMPING );
    else if ( (*Obj)-> timer == 9 ) SetObjAnimation ( YOSHI_STANDING );
    
    if ( (*Obj)->timer < time )
    {
         (*Obj)->y_pos += step;
    }
    else if ( (*Obj)->timer > time)
    {
         (*Obj)->y_pos -= step;
    }
     
    if ( (*Obj)->timer > (time*2) )
    {
        SetObjAnimation ( YOSHI_STANDING );
        (*Obj)->y_pos = -510;                  /* hacky and ugly way to ensure correct Y position */
        return 1;                               /* jump has ended */
    }
    
    return 0; /* jump is still hapenning */
}
