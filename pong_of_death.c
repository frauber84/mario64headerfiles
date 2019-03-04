#include <n64.h>
#include <explode.h>
#include "levels.h"

/* Pong of Death v1.0 by messiaen */

    struct object       **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct object       **Mario                = (void*)0x80331158;
    struct mario_struct  *MarioStruct          = (void*)0x8033b170;
 
#define INITIAL_Y_POS     110
#define MAX_Y_POS         200
#define MIN_Y_POS         10

void _start ( void )
{
    
    asm volatile("la $gp, _gp");

    u16 *CurrentLevel     = (void*)0x8032ddf8;
    u8 *pad               = (void*)0x8033AFA0;    
    u16 *score_1           = (void*)0x80404000;
    u16 *score_2           = (void*)0x80404002;
    u32 *other_counter     = (void*)0x80404004;
    u32 *y_pos_1           = (void*)0x8040400C;
    u32 *y_pos_2           = (void*)0x80404010;
    u32 *set_pos           = (void*)0x80404014;
    u32 *ball_x_pos        = (void*)0x80404018;
    u32 *ball_y_pos        = (void*)0x8040401C;
    u32 *ball_direction_x  = (void*)0x80404020;
    u32 *ball_direction_y  = (void*)0x80404024;
    u32 *ball_speed        = (void*)0x80404028;
    u8 *run_once           = (void*)0x8040402C;
    u8 *next_level         = (void*)0x8040402D;
    u8 *level              = (void*)0x8040402E;
    u8 *pad_speed          = (void*)0x8040402F;
    u32 *count_before_start = (void*)0x80404030;  
    u16 *display_stats = (void*)0x8033b26a;
    u32 *restart_counter    = (void*)0x80404034;
    u32 *first_run          = (void*)0x80404038;
    u32 *crazy_shot         = (void*)0x8040403C;
    

   if (*first_run == 0)
   {
     *next_level = 1;
     *first_run = 1;
     *level = 0;
   }

  if (*next_level == 0 )
  {    
  
    *other_counter += 3;
    *display_stats = 0;                                    
    
    if (*run_once == 0)
    {
        *y_pos_1 = INITIAL_Y_POS;
        *y_pos_2 = INITIAL_Y_POS;        
        *run_once = 1;
        *set_pos = 0;
        *count_before_start = 0;
        *ball_speed = 4;
        *pad_speed = 2;
        *score_1 = 0;
        *score_2 = 0;
        *restart_counter = 0;
        *level += 1;

        if (*level >= 3 )
        {
            *ball_speed += (*level - 3);
        }
        if (*level >= 8 )
        {
           *ball_speed = 9;
        }
        

    }
    
    if (*set_pos == 0)
    {
        if (*ball_direction_x == 0 || *ball_direction_x == 2)
        {
            *ball_x_pos = 50;
            *ball_direction_x = 0;
        }
        else if (*ball_direction_x == 1 || *ball_direction_x == 3)
        {
            *ball_x_pos = 250;
            *ball_direction_x = 1;
        }
        
        if (*ball_direction_y == 2) *ball_direction_y = 0;
        if (*ball_direction_y == 3) *ball_direction_y = 1;
        
        *ball_y_pos = INITIAL_Y_POS;
        *count_before_start = 0;        
        
        if (*score_2 > 9 )
        {
            *next_level = 1;
            *level = 0;
            MarioStruct->power = 0;            
        }
        else if (*score_1 > 9 )
        {
            u32 *current_object = (void*)M64_CURR_OBJ_PTR;
            u32 pseudo_stack = *current_object;
            
            if (*CurrentLevel != INSIDE_CASTLE) 
            {
                              
                if (*other_counter % 5 + 1 == 3 ) 
                {
                    struct object *ChildObject = SpawnObj( (*Obj), 0xBE, 0x13001f3c); // koppa shell
                }
                else if ( *other_counter % 5 + 1 == 4 ) 
                {
                    struct object *ChildObject = SpawnObj( (*Obj), 0x88, 0x13003dd8); // metap-cap                     
                }
                else if ( *other_counter % 5 + 1 == 2 ) 
                {
                    struct object *ChildObject = SpawnObj( (*Obj), 0x86, 0x13003db8); // wing-cap                     
                }
                else
                {
                     struct object *ChildObject = SpawnObj( (*Obj), 0xD4, 0x13003fdc); // 1-up
                }
                


            }
            
            *next_level = 1;            

        }
        
        *set_pos = 1;
        *crazy_shot = 0;
    }


    /* print paddles and score */    
    PrintInt(5, 215, "%d", *score_1);
    PrintInt(305, 215, "%d", *score_2);    
    
    PrintXY(25, *y_pos_1, "I");
    PrintXY(25, *y_pos_1 +5 , "I");
    PrintXY(25, *y_pos_1 +10, "I");
    PrintXY(25, *y_pos_1 +15, "I");        
    
    PrintXY(275, *y_pos_2, "I");
    PrintXY(275, *y_pos_2 +5 , "I");
    PrintXY(275, *y_pos_2 +10, "I");
    PrintXY(275, *y_pos_2 +15, "I");  
    
    /* print the ball */
    
    PrintXY(*ball_x_pos, *ball_y_pos, "+");

    *count_before_start += 1;
    
    if (*count_before_start > 30)
    {
        if (*ball_direction_x == 0)
        {
            *ball_x_pos += *ball_speed;
        }
        else if (*ball_direction_x == 1)
        {
            *ball_x_pos -= *ball_speed;
        }
        else if (*ball_direction_x == 2)
        {
            *ball_x_pos += *ball_speed + 2;
        }
        else if (*ball_direction_x == 3)
        {
            *ball_x_pos -= *ball_speed + 2;
        }
        
        if (*ball_direction_y == 0)
        {
            *ball_y_pos += *ball_speed;
        }
        else if (*ball_direction_y == 1)
        {
            *ball_y_pos -= *ball_speed;
        }
        else if (*ball_direction_y == 2)
        {
            *ball_y_pos += *ball_speed + 2;
        }
        else if (*ball_direction_y == 3)
        {
            *ball_y_pos -= *ball_speed + 2;
        }
        
        if (*crazy_shot == 1) 
        {
            if (*ball_direction_x == 0 || *ball_direction_x == 2) *ball_x_pos += 4;
            if (*ball_direction_x == 1 || *ball_direction_x == 3) *ball_x_pos -= 4;
            PrintXY(150, 100, "OMFG SUPERSHOT");
        }
        
        
        
    }
    
    /*check for collision with paddles and reverse directions */ 
        
    if (*ball_x_pos >= 262) 
    {   
       if ( !  (  (*y_pos_2 + 20 < *ball_y_pos - 8)  || (*y_pos_2 - 8 > *ball_y_pos + 8 ) ) ) /* rectangle collision */
       {
           
           if (*other_counter % 101 + 1 > 40 )  /* randomize for different reflection angles */
           {
               if (*ball_direction_x == 0 ) *ball_direction_x = 1;
               else if (*ball_direction_x == 2 ) *ball_direction_x = 1;
               
               *crazy_shot = 0;
           }
           else if (*other_counter % 101 + 1 < 40 ) 
           {
               if (*ball_direction_x == 0 ) *ball_direction_x = 3;
               else if (*ball_direction_x == 2 ) *ball_direction_x = 3;
               
               *crazy_shot = 0;
           }
           
           if (*other_counter %101 + 1 < 5)
           {
               *crazy_shot = 1;
           }
           
           if (*count_before_start % 100 + 1 > 30 ) 
           {
               if (*ball_direction_y == 2) *ball_direction_y = 0;
               else if (*ball_direction_y == 3) *ball_direction_y = 1;
           }
           else if (*count_before_start % 100 + 1 < 30 ) 
           {
               if (*ball_direction_y == 0) *ball_direction_y = 2;
               else if (*ball_direction_y == 1) *ball_direction_y = 3;
           }
           
           PlaySound(0x40020001);
       }
       else
       {
           
           if (*ball_x_pos >= 270) 
           {
               *score_1 += 1;
               *ball_direction_x = 1;
               *set_pos = 0;
           }
       }
    }
    else if (*ball_x_pos <= 33)
    {
       if ( !  (  (*y_pos_1 + 20 < *ball_y_pos - 8)  || (*y_pos_1 - 8 > *ball_y_pos + 8 ) ) ) /* rectangle collision */
       {
           if (*other_counter % 101 + 1 > 40 )   
           {
               if (*ball_direction_x == 1 ) *ball_direction_x = 0;
               else if (*ball_direction_x == 3 ) *ball_direction_x = 0;
               
               *crazy_shot = 0;
           }
           else if (*other_counter % 101 + 1 < 40 ) 
           {
               if (*ball_direction_x == 1 ) *ball_direction_x = 2;
               else if (*ball_direction_x == 3 ) *ball_direction_x = 2;
               
               *crazy_shot = 0;               
           }
           
           if (*other_counter %101 + 1 < 4)
           {
               *crazy_shot = 1;
           }


           if (*count_before_start % 100 + 1 > 30 ) 
           {
               if (*ball_direction_y == 2) *ball_direction_y = 0;
               else if (*ball_direction_y == 3) *ball_direction_y = 1;
           }
           else if (*count_before_start % 100 + 1 < 30 ) 
           {
               if (*ball_direction_y == 0) *ball_direction_y = 2;
               else if (*ball_direction_y == 1) *ball_direction_y = 3;
           }
           
           PlaySound(0x40020001);
       }
       else
       {
           if (*ball_x_pos <= 27)            
           {
               *score_2 += 1;           
               *set_pos = 0;
           }
       }
    }
    
    if (*ball_y_pos >= 215)
    {
        if (*ball_direction_y == 0) *ball_direction_y = 1;
        else if (*ball_direction_y == 2) *ball_direction_y = 3;
    }
    else if (*ball_y_pos <= 8)
    {
        if (*ball_direction_y == 1) *ball_direction_y = 0;
        else if (*ball_direction_y == 3) *ball_direction_y = 2;
    }
        
    if ( *pad & 4)
    {
        if ( ! (*y_pos_1 >= MAX_Y_POS) ) *y_pos_1 += ( 3 + *pad_speed );
    }
    if ( *pad & 8)
    {
        if ( ! (*y_pos_1 <= MIN_Y_POS ) ) *y_pos_1 -= (3 + *pad_speed );
    }

    /*  Computer AI */
    
    if (*ball_x_pos >= 140 - (*level * 8 ) )
    {
        if (*ball_direction_y == 0 || *ball_direction_y == 2)
        {
            if ( ! (*y_pos_2 >= MAX_Y_POS ) ) *y_pos_2 += *ball_speed;
            
            /* compensate opposite direction when the ball is near */
            if (*ball_x_pos > 215 - (*level * 2 ) )
            {
                if (*y_pos_2 > *ball_y_pos && !(*y_pos_2 <= MIN_Y_POS) && !(*ball_direction_x == 1 || *ball_direction_x == 3) ) *y_pos_2 -= (*ball_speed * 2);
            }            
        }
        else if (*ball_direction_y == 1 || *ball_direction_y == 3)
        {
            if ( ! (*y_pos_2 <= MIN_Y_POS ) )*y_pos_2 -= *ball_speed;
            
            if (*ball_x_pos > 215 - (*level * 2 ) )
            {
                if (*y_pos_2 < *ball_y_pos && !(*y_pos_2 >= MAX_Y_POS) && !(*ball_direction_x == 1 || *ball_direction_x == 3) ) *y_pos_2 += (*ball_speed * 2);
            }            
            
        }
        
    if (*y_pos_2 < MIN_Y_POS) *y_pos_2 = MIN_Y_POS + 1;
    if (*y_pos_2 > MAX_Y_POS) *y_pos_2 = MAX_Y_POS - 1;


    }
    
  } //  "quit pong" flag conditional
  else if (*next_level == 1)
  {
     *restart_counter += 1;
     
     PrintXY(150, *restart_counter + 50,  "GET READY FOR");
     PrintXY(150, *restart_counter + 30,  "PONG OF DEATH");
     PrintXY(150, *restart_counter + 10,  "  STAGE      ");
     PrintInt(150, *restart_counter + 10, "  %d  ", *level + 1);
     
     
     if (*restart_counter > 150)
     {
         *next_level = 0;
         *run_once = 0;
         *set_pos = 0;
     }
  }

    
}   
    


