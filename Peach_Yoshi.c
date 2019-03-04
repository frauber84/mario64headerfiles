#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"

#define PI 3.14159
#define DEBUG 0

extern s16 _8037a9a8(float y, float x);

Level *CurrentLevel = (void*)0x8033B90c;
MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
Object  **Obj    = (void*)M64_CURR_OBJ_PTR;
u32  *CurrentObject = (void*)M64_CURR_OBJ_PTR;

Object *Yoshi;
Object *Peach;
void ControlPlayer(Object *Player, Pad *GamePad);

#define OVERLAP 95
#define FIRE_RATE  40

Object *Fire;

int *counter = (void*)0x80600000;
int *counter2 = (void*)0x80600008;
float *camera_x = (void*)0x80600038;
float *camera_y = (void*)0x8060003c;
float *camera_z = (void*)0x80600040;
float *distance = (void*)0x8033c5a4; // real camera distance (find struct?)
int *camerapreset = (void*)0x8060004c;
int *sound = (void*)0x80600050;
int *instrument[16];
int *segment[0x17];
int *function_id = (void*)0x80600054;
int *last_segment7 = (void*)0x80600060;
int *FireCounter = (void*)0x80600064;
int *CameraOnObject = (void*)0x80600068;
int *ObjectDistanceSource = (void*)0x80600070;
int *AnimationIndex2P = (void*)0x80600074;
int *INIT = (void*)0x80600074;
int *speed = (void*)0x80600100;
int *speed_counter = (void*)0x80600104;
int *air = (void*)0x80600108;
int *air_counter = (void*)0x8060010C;

void CheckFireCollision(Object *Source);

void _start ( void )
{
    asm volatile("la $gp, _gp");
    
    
    Pad *Controller2 = (void*)0x8033AFAC;
    Pad *Controller3 = (void*)0x8033AFC8;

    *counter2 = *counter2 + 1;

    //patch object distance calculation
    u32 *apointer = (void*)0x80385C2C;
    *apointer =  0x3C058060;  // LUI A1, 0x8060
    apointer++;
    *apointer =  0x8CA50070; //  LW A1, 0x0070 (A1)  Load ObjectDistanceSource
    
    u32 *MarioVariable = (void*)0x80361158;
    *ObjectDistanceSource = *MarioVariable;

    /* fire stuff */    
    u32 *fire_floats_pointer = SegmentedToVirtual(0x130011A8);
    u16 *fire_patch = 0x802af4da; // some pointers to fireball routine patches
    u32 *fire_patch2 = 0x802AF564;
    *fire_patch = 0x3F80; // affects fireball initial trajectory
    *fire_patch2 = 0; // nopes the write to obj->0xb8 which fixes the fireball speed
    *fire_floats_pointer = 0x001EFD30;  // patching behavior to alter physics
     fire_floats_pointer++;
    *fire_floats_pointer = 0xFF9A03E8;
    fire_floats_pointer++;    
    *fire_floats_pointer = 0x03E800C8;    
    
    //nope camera
    u32 *nopbyte = (void*)0x80254318;
    *nopbyte = 0x00000000;
    
    CheckInit:
    if (*INIT == 0)
    {
        u32 *ptr = (void*)0x80750000;
        *ptr = 0x00005000;
        ptr++;
        ptr++;
        *ptr = 0x80750010;
        ptr++;
        *ptr = 0x80750010;
        DmaCopy(0x80700000, 0x1700000, 0x1724200); // copy yoshi/peach modified bank to 0x80700000
        DmaCopy(0x80730000, 0x1730000, 0x1740000); // layout/behavior
        
        Object *NewObject = SpawnObj( (*Obj), 0x01, 0x80730700);  /* give yoshi a temp behavior */
         NewObject->animation_ptr = 0x807241E8; // yoshi
        NewObject->geo_layout_ptr = ProcessGeoLayout(0x80750000, 0x80730468);   // yoshi
        NewObject->x_pos += 75;
        
        int pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */        
        *CurrentObject = NewObject;
        SetObjAnimation(1);
        *CurrentObject = pseudostack;  /* Saving because this global variable will be messed up */ 
        
        Yoshi = NewObject;
        
        Yoshi->col_sphere_x = (float)0xa0;
        Yoshi->col_sphere_y = (float)0x96;
        Yoshi->_0xe4 = 0x02;
        
        Object *NewObject2 = SpawnObj( (*Obj), 0x01, 0x807306A0);  /* give peach a temp behavior */
        NewObject2->animation_ptr = 0x8071C504; // peach
        NewObject2->geo_layout_ptr = ProcessGeoLayout(0x80750000, 0x80730410);   // peach
        NewObject2->x_pos += 175;
        NewObject2->transparency = 0xFFFFFFFF;
        
        pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */        
        *CurrentObject = NewObject2;
        SetObjAnimation(1);
        *CurrentObject = pseudostack;  /* Saving because this global variable will be messed up */ 
        
        Peach = NewObject2;
        
        Peach->col_sphere_x = (float)0xa0;
        Peach->col_sphere_y = (float)0x96;
        Peach->_0xe4 = 0x02;


        
        
        int segment7 = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF;
        *last_segment7 = segment7;
        

        *INIT = 1;

    }
    
    int segment_7 = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF;
    
    if (segment_7 != *last_segment7) 
    {
        *INIT = 0;
        goto CheckInit;
    }
    
    *last_segment7 = segment_7;
    
    Yoshi->_0x9c = 0; // enable collision sphere
    Peach->_0x9c = 0; // enable collision sphere
    ControlPlayer(Yoshi, Controller2);
    ControlPlayer(Peach, Controller3);

    /* gravity */    
    int pseudostack = *CurrentObject;
    *CurrentObject = Yoshi;
    int (*DoGravity)(void) = (void*)0x802e4e90;
    DoGravity();
    *CurrentObject = Peach;
    DoGravity();
    *CurrentObject = pseudostack;

    Camera *camera = Mario->camera;     
    camera->x = Mario->x_pos;
    camera->y = Mario->y_pos;
    camera->z = Mario->z_pos;
        
    PrintInt(20, 120, "peach 80%x", ((int)Peach & 0x00FFFFFF) );
    PrintInt(20, 40, "yoshi 80%x", ((int)Yoshi & 0x00FFFFFF) );
    
    Yoshi->distance_from_mario = 0;
    Yoshi->collision_distance = 32000;
    Yoshi->drawing_distance = 32000;
    Peach->distance_from_mario = 0;
    Peach->collision_distance = 32000;
    Peach->drawing_distance = 32000;

    *FireCounter = *FireCounter + 1;
    if (Controller2->currentButton & BUTTON_B && *FireCounter > FIRE_RATE)    
    {
        pseudostack = *CurrentObject;
        *CurrentObject = Yoshi;
        Object *ChildObj = SpawnObj( (*Obj), 0x90, 0x13001184);   /* Mario fireball behavior */
        ChildObj->x_scaling = 3.5;
        ChildObj->y_scaling = 3.5;
        ChildObj->z_scaling = 3.5;
        ChildObj->speed = 30;
        PlaySound(0x60086001);
        *CurrentObject = (void*)pseudostack;
        *FireCounter = 0;
    }

    CheckFireCollision(Yoshi);

    if (Controller2->currentButton & BUTTON_Z && *counter2 > 15 ) 
    {
       if (*CameraOnObject == 0 ) *CameraOnObject = 1;
       else if (*CameraOnObject == 1 ) *CameraOnObject = 2;
       else if (*CameraOnObject == 2 ) *CameraOnObject = 0;
       *counter2 = 0;
   }
        
       PrintInt(20, 20, "Camera", 0);
       if (*CameraOnObject == 1)
       {
           *ObjectDistanceSource = Yoshi; // calculate distances
           Camera *camera = Mario->camera;     
           camera->x = Yoshi->x_pos;
           camera->y = Yoshi->y_pos;
           camera->z = Yoshi->z_pos;
           PrintInt(110, 20, "Yoshi", 0);
       }
       else if (*CameraOnObject == 2)
       {
           *ObjectDistanceSource = Peach; // calculate distances
           Camera *camera = Mario->camera;     
           camera->x = Peach->x_pos; 
           camera->y = Peach->y_pos;
           camera->z = Peach->z_pos;
           PrintInt(110, 20, "Peach", 0);
       }
       else
       {
           PrintInt(110, 20, "Mario", 0);
       }

       float distanceObj = DistanceFromObject(Yoshi, Mario->MarioObj);       
//       float *camera6ac = (void*)0x8033c6ac;
//       PrintInt(20, 100, "cam %d", (int)*camera6ac);
       PrintInt(20, 80, "dist %d", (int)distanceObj);
       PrintInt(20, 60, "cdist %d", (int)*distance);
       
       if (distanceObj > 1500 )
       {
           if (*distance < 100) *distance += 1.9;
       }
       else if (distanceObj > 1000 && distanceObj <= 1500)
       {
           if (*distance < 85) *distance += 1.5;
       }
       else if (distanceObj > 500 && distanceObj <= 1000)
       {
           if (*distance < 67) *distance += 1.4;
       }
       else if (distanceObj > 0 && distanceObj <= 500)
       {
           if (*distance < 50) *distance += 0.7;
       }


       
      int OVERLAP2 = 125;
      
      if ( Mario->x_pos > Yoshi->x_pos - OVERLAP2 && Mario->x_pos < Yoshi->x_pos + OVERLAP2 && Mario->y_pos > Yoshi->y_pos - OVERLAP2 && Mario->y_pos < Yoshi->y_pos + OVERLAP2 && Mario->z_pos > Yoshi->z_pos - OVERLAP2 && Mario->z_pos < Yoshi->z_pos + OVERLAP2 && Mario->y_pos > Mario->ground_y + 30 )
      {
           if (*air == 0 && Mario->action == 0x03000881) *air = 1; // double jump = make backflip
           else if (Mario->action == 0x008008A9 && FireCounter > FIRE_RATE)  // stomp = big fireball
           {
            pseudostack = *CurrentObject;
            *CurrentObject = Yoshi;
            Object *ChildObj = SpawnObj( (*Obj), 0x90, 0x13001184);   /* Mario fireball behavior */
            ChildObj->x_scaling = 10;
            ChildObj->y_scaling = 10;
            ChildObj->z_scaling = 10;
            ChildObj->y_pos += 40;
            ChildObj->speed = 100;
            PlaySound(0x60086001);
            *CurrentObject = (void*)pseudostack;
            *FireCounter = 0;

           }
      }

      if ( Mario->x_pos > Yoshi->x_pos - 150 && Mario->x_pos < Yoshi->x_pos + 150 && Mario->y_pos > Yoshi->y_pos - 150 && Mario->y_pos < Yoshi->y_pos + 150 && Mario->z_pos > Yoshi->z_pos - 150 && Mario->z_pos < Yoshi->z_pos + 150 && ( Mario->action == 0x00800380 || Mario->action == 0x00800457 ) )
      {
           Mario->action = 0x00020338;
           Mario->power -= 1;
           
      }
      

      
      if (*air == 1)
      {
          *air_counter += 1;
          
          if (*air_counter == 1) Mario->y_pos += 80;
          else Mario->y_pos += 40;
          Mario->y_speed = 20;
          
          if (*air_counter > 7)
          {
             Mario->action = 0x108008A4; // spin
             *air = 0;
             *air_counter = 0;    
          }
               
      }
      
       
    
    
}

void CheckFireCollision(Object *Source)
{
    
    Object *SourceObject = Source;
    int pseudostack, pseudostack2, i, k, j;
    
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
                 
                     if (    ( (*Obj)->animation != 0 || (*Obj)->collision_ptr != 0 ) && (*Obj)->animation != 0x80060030 && ( (*Obj)->x_pos != Source->x_pos) )
                     {
                         if ( (*Obj)->x_pos > Fire->x_pos - OVERLAP && (*Obj)->x_pos < Fire->x_pos + OVERLAP && (*Obj)->y_pos > Fire->y_pos - OVERLAP && (*Obj)->y_pos < Fire->y_pos + OVERLAP && (*Obj)->z_pos > Fire->z_pos - OVERLAP && (*Obj)->z_pos < Fire->z_pos + OVERLAP && (*Obj)->geo_layout_ptr !=  0x800F0860) 
                         {
                             SpawnObj( (*Obj), 0x00cd, 0x13003510);  // explosion
                             (*Obj)->active =0;
                             Fire->active = 0;
                             goto GetOut1;
                         }
                     }
                     
                    *CurrentObject = (*Obj)->next;
                 }
            
            GetOut1:     
            *CurrentObject = pseudostack2;
        }
        
        *CurrentObject = (*Obj)->next;
    }
    *CurrentObject = (void*)pseudostack;  /* Restoring before returning */         
}


void ControlPlayer(Object *Player, Pad *GamePad)
{
        int angle = (int)Player->y_rotation/180 + 180;
        int dir = 0;
        
        if (angle < 0) angle = 0;
        if (angle > 360) angle = 360;

        if (angle >= 0 && angle < 90)          dir = 1;
        else if (angle >= 90 && angle < 180)   dir = 2;
        else if (angle >= 180 && angle < 270)  dir = 3;
        else if (angle >= 270 && angle <= 360) dir = 4;
        

        // rotate object        
        if (GamePad->x != 0  || GamePad->y != 0)
        {
            Player->y_rotation = _8037a9a8( -(GamePad->y), GamePad->x);
            Player->y_rotation2 = _8037a9a8(-(GamePad->y), GamePad->x);        
        }
        
     float distanceObj = DistanceFromObject(Player, Mario->MarioObj);
        
        switch (dir)
        {
            case 1:
                Player->z_pos -= (int)GamePad->y * 0.4f * cosf( angle *PI/180.0);
                Player->x_pos -= (int)GamePad->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 2:
                Player->z_pos += (int)GamePad->x * 0.4f * cosf( angle *PI/180.0);
                Player->x_pos += (int)GamePad->x * 0.4f * sinf( angle *PI/180.0);
                break;

                
            case 3:
                Player->z_pos += (int)GamePad->y * 0.4f * cosf( angle *PI/180.0);
                Player->x_pos += (int)GamePad->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 4:
                Player->z_pos -= (int)GamePad->x * 0.4f * cosf( angle *PI/180.0);
                Player->x_pos -= (int)GamePad->x * 0.4f * sinf( angle *PI/180.0);
                break;
                
        }
        
        if (GamePad->currentButton & BUTTON_A && *speed == 0)
        {
            Player->y_speed += 5;
            
            if (Player->y_speed > 25) *speed = 1;
        }
        
        if (*speed == 1)
        {
            *speed_counter += 1;
            
            if (*speed_counter > 30)
            {
               *speed_counter = 0;
               *speed = 0;
            }
        }
}
