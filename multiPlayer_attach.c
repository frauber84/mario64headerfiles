#include <n64.h>
#include "explode.h"
#include "levels.h"
#define PI 3.14159

#define SWAP(a,b)   { int t; t=a; a=b; b=t; }  // Macro for swapping

extern s16 _8037a9a8(float y, float x);

Level *CurrentLevel = (void*)0x8033B90c;
MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
Object  **Obj    = (void*)M64_CURR_OBJ_PTR;
u32  *CurrentObject = (void*)M64_CURR_OBJ_PTR;

int *counter = (void*)0x80600000;
int *counter2 = (void*)0x80600008;
int *distancetomario = (void*)0x8060002c;
int *obj_behav = (void*)0x80600034;
float *distance = (void*)0x8033c5a4; // real camera distance (find struct?)
int *camerapreset = (void*)0x8060004c;
int *function_id = (void*)0x80600054;
int *last_segment7 = (void*)0x80600060;
int *segment = (void*)0x80600260;
float *cameradistance = (void*)0x80600048;
int *speed = (void*)0x80600100;
int *speed_counter = (void*)0x80600104;
int *ObjectDistanceSource = (void*)0x80600070;
int *CameraOnObject = (void*)0x80600068;
int *gravity = (void*)0x80600180;


void MenuPage05();
void ProcessSelection();
void ControlPlayer_Jump(Object *object, Pad *gamepad);
void ControlPlayer_Move(Object *object, Pad *gamepad);
void bubble_srt(int a[], int n);

void _start ( void )
{
    asm volatile("la $gp, _gp");

    *counter2 = *counter2 + 1;
    
    Camera *camera = Mario->camera;     
    camera->x = Mario->x_pos;
    camera->y = Mario->y_pos;
    camera->z = Mario->z_pos;
    
    *segment = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF; // check if level has changed
        
                MenuPage05();
    *last_segment7 = *segment;

    
}


void MenuPage05()
{
    
    int pseudostack, pseudostack2;
    int i, j, k, num_obj = 0;   
    u32 *list_pointer = (void*)0x80608000; 
    
    if (Mario->pad->currentButton & BUTTON_L && Mario->pad->currentButton & BUTTON_D_UP )
    {
                                  
        pseudostack = *CurrentObject;
    
        *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
        list_pointer = (void*)0x80608010;
     
        for (i = 0; i < 240; i++)
        {   
            
            if (  ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 ) && Obj != Mario->MarioObj )
            {
                *list_pointer = *CurrentObject;
                list_pointer++;
                *list_pointer = (*Obj)->distance_from_mario;
                list_pointer++;
                    
                num_obj++;
            }
            *CurrentObject = (*Obj)->next;
            
        }
        list_pointer = (void*)0x80608000;
        *list_pointer = num_obj;
        
        int array[num_obj];
        
        for (i = 0; i < num_obj; i++)
        {
           u32 *distance = (void*)0x80608010 + 4 + (i*8);
           array[i] = *distance;
        }
        bubble_srt(array, num_obj);
        
        while (array[0] != *list_pointer) list_pointer++;
        
        list_pointer--;
        int closest_object = *list_pointer;
        
        list_pointer = (void*)0x80608008;
        *list_pointer = closest_object;
        
        Object *ClosestObject = (void*)closest_object;
        
        *CurrentObject = (void*)pseudostack;
        
        *obj_behav = ClosestObject->behavior_script_entry;
        
    }
        
    list_pointer = (void*)0x80608008;
    
    if ( *list_pointer != 0 )
    {

        Object *ClosestObject = (void*) *list_pointer;

        PrintInt(25, 200, "PTR 80%x", (int)ClosestObject & 0xFFFFFF );
        PrintInt(25, 160, "DIST %d", (int)DistanceFromObject( ClosestObject, Mario->MarioObj) );        
        
        //Pad *Controller2 = (void*)0x8033AF90; //  for player2
        Pad *Controller2 = (void*)0x8033AFAC; 
        
        ControlPlayer_Jump(ClosestObject, Controller2);
        ControlPlayer_Move(ClosestObject, Controller2);

        int pseudostack34 = *CurrentObject;
        *CurrentObject = ClosestObject;
        
        if (*gravity == 1)
        {
            int (*function234)(void) = (void*)0x802e4e90;
           function234();
           *CurrentObject = pseudostack34;
           
           if (ClosestObject->_0xe4 == 0)
            {
            /* copied from Yoshi */
                ClosestObject->_0xe4 = 2.2;
                ClosestObject->_0x170 = 0.9;
                ClosestObject->_0x174 = 1.3;
            }
            /* gravity */
           
           PrintInt (20, 100, "grav hack", 0);
        }
        
        /* camera stuff */
       
       u32 *MarioVariable = (void*)0x80361158;
       *ObjectDistanceSource = *MarioVariable;
       
        //nope camera
        u32 *nopbyte = (void*)0x80254318;
        *nopbyte = 0x00000000;

/*       

 This wil be done manually (0x80600068) 
        if (Controller2->currentButton & BUTTON_Z && *counter2 > 15 ) 
        {
           if (*CameraOnObject == 0 ) *CameraOnObject = 1;
           else if (*CameraOnObject == 1 ) *CameraOnObject = 0;
           *counter2 = 0;
           
        }
*/        
        
        if (Controller2->currentButton & BUTTON_B && *counter2 > 15 )    
        {
           if (*gravity == 0 ) *gravity = 1;
           else if (*gravity == 1 ) *gravity = 0;
           *counter2 = 0;
        } 
        

    //patch object distance calculation
    u32 *apointer = (void*)0x80385C2C;
    *apointer =  0x3C058060;  // LUI A1, 0x8060
    apointer++;
    *apointer =  0x8CA50070; //  LW A1, 0x0070 (A1)  Load ObjectDistanceSource


        Camera *camera = Mario->camera;     
        camera->x = Mario->x_pos;
        camera->y = Mario->y_pos;
        camera->z = Mario->z_pos;


//       PrintInt(20, 20, "Camera", 0);
       if (*CameraOnObject == 1)
       {
           *ObjectDistanceSource = ClosestObject; // calculate distances
           Camera *camera = Mario->camera;     
           camera->x = ClosestObject->x_pos;
           camera->y = ClosestObject->y_pos;
           camera->z = ClosestObject->z_pos;
           PrintInt(110, 20, "2P", 0);
       }
       else
       {
           PrintInt(110, 20, "Mario", 0);
       }

        
        if (ClosestObject->active == 0 || ClosestObject->graph_node_type != 0x0018 || *obj_behav != ClosestObject->behavior_script_entry)
        {
            *list_pointer = 0;
           u32 *MarioVariable = (void*)0x80361158;
           *ObjectDistanceSource = *MarioVariable;
           *CameraOnObject == 0;
           *gravity = 0;

        }
        
        if (*segment != *last_segment7) *list_pointer = 0;
        
              
    }
    else
    {
        PrintInt(10, 30, "L and D UP to attach", 0);
        *list_pointer = 0;

        u32 *MarioVariable = (void*)0x80361158;
        *ObjectDistanceSource = *MarioVariable;
        *CameraOnObject == 0;
        *gravity = 0;

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

void ControlPlayer_Jump(Object *object, Pad *gamepad)
{
     
        if (gamepad->currentButton & BUTTON_A && *speed == 0)
        {
            object->y_speed += 5;
            
            if (object->y_speed > 25) *speed = 1;
        }
        
        if (*speed == 1)
        {
            *speed_counter += 1;
            
            if (*speed_counter > 120)
            {
               *speed_counter = 0;
               *speed = 0;
            }
        }

}

void ControlPlayer_Move(Object *object, Pad *gamepad)
{
        
        int angle = (int)object->y_rotation/180 + 180;
        int dir = 0;
        
        if (angle < 0) angle = 0;
        if (angle > 360) angle = 360;

        if (angle >= 0 && angle < 90)          dir = 1;
        else if (angle >= 90 && angle < 180)   dir = 2;
        else if (angle >= 180 && angle < 270)  dir = 3;
        else if (angle >= 270 && angle <= 360) dir = 4;
        
        // rotate object        
        if (gamepad->x != 0  || gamepad->y != 0)
        {
            object->y_rotation = _8037a9a8( -(gamepad->y), gamepad->x);
            object->y_rotation2 = _8037a9a8(-(gamepad->y), gamepad->x);        
        }
        
     float distanceObj = DistanceFromObject(object, Mario->MarioObj);
        
        switch (dir)
        {
            case 1:
                object->z_pos -= (int)gamepad->y * 0.4f * cosf( angle *PI/180.0);
                object->x_pos -= (int)gamepad->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 2:
                object->z_pos += (int)gamepad->x * 0.4f * cosf( angle *PI/180.0);
                object->x_pos += (int)gamepad->x * 0.4f * sinf( angle *PI/180.0);
                break;

                
            case 3:
                object->z_pos += (int)gamepad->y * 0.4f * cosf( angle *PI/180.0);
                object->x_pos += (int)gamepad->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 4:
                object->z_pos -= (int)gamepad->x * 0.4f * cosf( angle *PI/180.0);
                object->x_pos -= (int)gamepad->x * 0.4f * sinf( angle *PI/180.0);
                break;
                
        }
        
}
