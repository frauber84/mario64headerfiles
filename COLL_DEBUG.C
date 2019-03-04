#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"

#define PI 3.14159
#define DEBUG 0
// checkout: 0x8033b06c = display list buffer?


extern s16 _8037a9a8(float y, float x);
/* Soundlist is incomplete, done using a modified Project64 which dumps PlaySound function arguments  */
Level *CurrentLevel = (void*)0x8033B90c;
MarioStruct *Mario = (void*)M64_MARIO_STRUCT;
Object  **Obj    = (void*)M64_CURR_OBJ_PTR;
u32  *CurrentObject = (void*)M64_CURR_OBJ_PTR;

#define OVERLAP 95
#define FIRE_RATE  40

Object *Fire;

int *counter = (void*)0x80600000;
int *music = (void*)0x80600004;
int *counter2 = (void*)0x80600008;
int *selection = (void*)0x80600010;
int *page = (void*)0x8060000c;
u16 *chmenu = (void*)0x80600014;
int *step = (void*)0x80600018;
int *modelid = (void*)0x8060001c;
int *behavior = (void*)0x80600020;
int *bparam1 = (void*)0x80600024;
int *bparam2 = (void*)0x80600028;
int *distancetomario = (void*)0x8060002c;
int *obj_model_id = (void*)0x80600030;
int *obj_behav = (void*)0x80600034;
float *camera_x = (void*)0x80600038;
float *camera_y = (void*)0x8060003c;
float *camera_z = (void*)0x80600040;
int *freeze = (void*)0x80600044;
float *cameradistance = (void*)0x80600048;
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

void MenuPage04();
void ProcessSelection();

void _start ( void )
{
    asm volatile("la $gp, _gp");

    *counter2 = *counter2 + 1;

    if (Mario->pad->currentButton & BUTTON_L && *counter2 > 3)        
    {
                                  
        if (Mario->pad->currentButton & BUTTON_D_LEFT)
        {
            *page -= 1;
            if (*page < 4) *page = 4;
            *counter2 = 0;
            *counter = 0;
        }
        else if (Mario->pad->currentButton & BUTTON_D_RIGHT)
        {
            *page += 1;
            if (*page > 4) *page = 4;
            *counter2 = 0;
            *counter = 0;
        }
        else if (Mario->pad->currentButton & BUTTON_R)  /* easter egg :P */
        {
            *page = 4;
            *counter2 = 0;
            *counter = 0;
        }
        
    }
    
    switch (*page)
    {
           case 0:
                PrintInt(10, 30, "sm64 super debug ", 0);
                PrintInt(10, 10, "by frauber aka messiaen", 0);
                break;
           
           case 1:
           case 2:
           case 3:
           case 4:
                MenuPage04();
                break;
           default:
                MenuPage04();
                break;
                
    }
    
}

void MenuPage04()
{

    if (*counter == 0)
    {
        *selection = 6;
        *counter = 1;
    }    

    ProcessSelection (6, 7, 0);
    PrintInt(10, (*selection * 20), "*", 0);
    
    switch (*selection)
    {
           
        case 7:
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                if ( ! (CurrentLevel->terrain_type <= 0 ) ) CurrentLevel->terrain_type -= 1;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                if ( ! (CurrentLevel->terrain_type >= 7) ) CurrentLevel->terrain_type += 1;
                *counter2 = 0;
            }
        break;
        
        case 6:
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                if ( ! (Mario->curr_collision_triangle->collision_type <= 0) ) Mario->curr_collision_triangle->collision_type -= 1;
                if ( Mario->curr_collision_triangle->collision_type == 0x64 )Mario->curr_collision_triangle->collision_type = 0x3f;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                Mario->curr_collision_triangle->collision_type += 1;
                if ( (Mario->curr_collision_triangle->collision_type > 0x3f) ) Mario->curr_collision_triangle->collision_type = 0x65;
                *counter2 = 0;
            }
        break;
        
    }
    
  
//    u32 *nop_byte3 = (void*)0x80251BF8;
//    *nop_byte3 = 0;
    
    PrintInt(25, 180, "PTR 80%x", (int)Mario->curr_collision_triangle & 0xFFFFFF);
    PrintInt(25, 160, "LV TERRAIN %x", CurrentLevel->terrain_type);
    PrintInt(25, 140, "TYPE %x",    Mario->curr_collision_triangle->collision_type);
    PrintInt(25, 120, "N1 %x", (float)Mario->curr_collision_triangle->normal_x);
    PrintInt(25, 100, "N2 %x", (float)Mario->curr_collision_triangle->normal_y);
    PrintInt(25, 80, "N3 %x", (float)Mario->curr_collision_triangle->normal_z);
    PrintInt(25, 60, "ymin %d",        Mario->curr_collision_triangle->ymin);    
    PrintInt(185, 60, "ymax %d",        Mario->curr_collision_triangle->ymax);        
    PrintInt(25, 40, "Negdot %d",        (int)Mario->curr_collision_triangle->negdot);    
    PrintInt(25, 20, "GROUND Y %d", (int)Mario->ground_y);
    
    Mario->curr_collision_triangle->collision_type = 0x15;
    
    Mario->curr_collision_triangle->negdot = -(Mario->curr_collision_triangle->normal_x*((float)Mario->curr_collision_triangle->vertex1_x) + Mario->curr_collision_triangle->normal_y*((float)Mario->curr_collision_triangle->vertex1_y) + Mario->curr_collision_triangle->normal_z*((float)Mario->curr_collision_triangle->vertex1_z));

    
    Mario->curr_collision_triangle->ymin = -8192;
    Mario->curr_collision_triangle->ymax = +8192;
    
}


void ProcessSelection(int min, int max, int swap_buttons)
{
    
    if (Mario->pad->currentButton & BUTTON_D_UP && *counter2 > 3)    
    {
        
        if (swap_buttons)
        {
            *selection = *selection - 1;
            if (*selection < min) *selection = min;
        }
        else
        {
            *selection = *selection + 1;
            if (*selection > max) *selection = max;
        }
        
        *counter2 = 0;
    }
    else if (Mario->pad->currentButton & BUTTON_D_DOWN && *counter2 > 3)    
    {
         
        if (swap_buttons)
        {
            *selection = *selection + 1;
            if (*selection > max) *selection = max;
        }
        else
        {
            *selection = *selection - 1;
            if (*selection < min) *selection = min;
        }
        
        *counter2 = 0;
    }
}
