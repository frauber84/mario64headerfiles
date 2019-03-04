#include <n64.h>
#include <explode.h>

    struct object       **Obj                 = (void*)M64_CURR_OBJ_PTR;
    struct mario_struct *Mario                = (void*)M64_MARIO_OBJ_PTR;
 
#define INITIAL_Y_POS     150
#define MAX_Y_POS         200
#define MIN_Y_POS         10

void _start ( void )
{
     
    asm volatile("la $gp, _gp");

    u8 *pad        = (void*)0x8033AFA0;    
    u8 *score_1    = (void*)0x80404000;
    u8 *score_2    = (void*)0x80404001;
    u8 *direction  = (void*)0x80404002;
    u8 *y_pos_1    = (void*)0x80404003;
    u8 *y_pos_2    = (void*)0x80404004;
    u8 *set_pos    = (void*)0x80404005;
    u8 *ball_x_pos = (void*)0x80404006;
    u8 *ball_y_pos = (void*)0x80404007;
    u8 *ball_direction_x   = (void*)0x80404008;
    u8 *ball_direction_y   = (void*)0x80404009;
    u8 *ball_speed         = (void*)0x8040400A;
    u8 *ball_speed_counter = (void*)0x8040400B;
    
    if (*set_pos == 0)
    {
        *y_pos_1 = INITIAL_Y_POS;
        *y_pos_2 = INITIAL_Y_POS;        
        *ball_x_pos = 150;
        *ball_y_pos = 150;
        *ball_speed = 1;
        *ball_speed_counter = 0;
        *set_pos = 1;
    }
    
    PrintInt(30, 275, "%d", *score_1);
    PrintInt(270, 275, "%d", *score_2);    
    
    PrintXY(25, *y_pos_1, "I");
    PrintXY(25, *y_pos_1 +5 , "I");
    PrintXY(25, *y_pos_1 +10, "I");
    PrintXY(25, *y_pos_1 +15, "I");        
    
    PrintXY(275, *y_pos_2, "I");
    PrintXY(275, *y_pos_2 +5 , "I");
    PrintXY(275, *y_pos_2 +10, "I");
    PrintXY(275, *y_pos_2 +15, "I");  
    
    // "ball"
    
    *ball_speed_counter += 1;
    
    if (*ball_speed_counter >= 180) 
    {
        *ball_speed += 1;
        *ball_speed_counter = 0;
        
        if (*ball_speed > 6) *ball_speed = 6;
    }
    
    PrintXY(*ball_x_pos, *ball_y_pos, "*");
    
    if (*ball_direction_x == 0)
    {
        *ball_x_pos += *ball_speed;
    }
    else if (*ball_direction_x == 1)
    {
        *ball_x_pos -= *ball_speed;
    }
    
    if (*ball_direction_y == 0)
    {
        *ball_y_pos += *ball_speed;
    }
    else if (*ball_direction_y == 1)
    {
        *ball_y_pos -= *ball_speed;
    }
    
    if (*ball_x_pos >= 255) *ball_direction_x = 1;
    else if (*ball_x_pos <= 25)  *ball_direction_x = 0;
    if (*ball_y_pos >= 215) *ball_direction_y = 1;
    else if (*ball_y_pos <= 10)  *ball_direction_y = 0;

        
    if ( *pad & 1)
    {
        if ( ! (*y_pos_1 >= MAX_Y_POS) ) *y_pos_1 += 3;
    }
    
    if ( *pad & 2)
    {
        if ( ! (*y_pos_1 <= MIN_Y_POS ) ) *y_pos_1 -= 3;
    }
    
    if ( *pad & 4)
    {
        if ( ! (*y_pos_2 >= MIN_Y_POS ) ) *y_pos_2 += 3;
    }

    if ( *pad & 8)
    {
        if ( ! (*y_pos_2 <= MIN_Y_POS ) ) *y_pos_2 -= 3;
    }
    
    
    

}
