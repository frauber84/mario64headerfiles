// warning: this file is outdated, use explode.h instead

/*  Mario 64 header file v0.2 by messiaen. 
**
**  The information contained in this file is a compilation from many sources, 
**  especially Cellar Dweller's, Nagra's, Yoshielectron's notes along with 
**  original research by me. This is to be used with the mips-gcc "N64"
**  Toolchain set by ZZT32. The makefile will also convert the binary output
**  to a Nemu64 code.
**
**  You can a tutorial on setting up the N64 Toolchain on Windows
**  at http://code.google.com/p/gzrt/wiki/Nintendo64ToolchainSetup
**
**  If you are using an *nix environment, check this Wiki for info about the
**  appropriate MIPS-R4300i binutils:
**
**  http://en.wikibooks.org/wiki/N64_Programming
**
**  You can contact me at the Jul SM64 Hacking forum @ http://jul.rustedlogic.net/
*/  

/* Constants - struct pointers */

#define M64_CURR_OBJ_PTR            0x80361160
#define M64_FIRST_OBJ_STRUCT        0x8033D488   /* Pointer to the first object (out of 240) in the circular linked list */
#define M64_MARIO_STRUCT            0x8033B170   /* read from 0x8032d93c */
#define M64_MARIO_OBJ_PTR           0x80361158   /* Pointer to Mario OBJ struct in RAM */

/* Constants - misc pointers */

#define M64_DISPLAY_STATS_FLAGS     0x8032b26a
#define M64_SEGMENT_TABLE           0x8032b400
#define M64_CURRENT_LEVEL_ID        0x8032ddf8

/* Function prototypes (check out linker script for addresses) */

extern void  PrintXY(u16 x, u16 y, char* text);
extern void  ExplodeObject(u32 obj_ptr);
extern int   ShakeScreen(u32 argument);
extern int   DeactivateObject(u32 obj_pointer);
extern int   SpawnObj(u32 obj_pointer, u16 model_id, u32 behavior);   /* obj_pointer = always M64_CURR_OBJ_PTR -> returrns pointer for spawned object */
extern int   CreateStar(float x, float y, float z);
extern void  DmaCopy(u32 dst, u32 bottom, u32 top);
extern void  CreateTextBox(u16 msg_ID);
extern void  PlaySound(u32 argument);
extern void PrintInt (u16 x, u16 y, char* text, u32 value);
extern int   StopMario(u16 arg);  /* 1 = stop mario  2 = do nothing??  (needs to be tested more)*/ 
extern int   SegmentedToVirtual(u32 segmented_pointer);   /* returns RAM pointer of a segmented address*/
extern int   PrintText(u32 x_pos, char *text, u32 fade); /* used in Credits. a2 = a float value ? */
extern void  SetModel(u16 model_ID);  /* change how the object looks */
extern void  ScaleObject(float global_scaling_factor);
extern void  ScaleXYZ(u32 obj_pointer, float x, float y, float z);
extern int  PrintRegularText(u32 x, u32 y, char *table_text_pointer);  /* needs to be tested */ 
extern int SetObjAnimation(u16 animation_index);

struct mario_struct                /* 8033b170 */
{
    u32    status;
    u32    flags;                   /* cap & other flags */
    u32    _0x08;
    u32    action;                  /* see Romanian Girl list */
    u32    previous_action;         /* 0x10 */
    u32    _0x14;
    u16    _0x18;
    u16    _0x1a;
    u32    _0x1c;    
    float  _0x20;                   /* 0x20 */
    u16    _0x26;
    s16    hitstun;                 /* hitstun counter (how long Mario will stays invencible after getting hit */
    u32    _0x28;
    u32    _0x2c;    
    u32    _0x30;                   /* 0x30 */
    u32    _0x34;
    u32    _0x38;
    float  x_pos;                   /* re-check */
    float  y_pos;                   /* 0x40 */
    float  z_pos;                   /* re-check */
    u32    _0x48;
    u32    _0x4c;   
    u32    _0x50;                   /* 0x50 */
    u32    speed;
    u32    _0x58;
    u32    _0x5c;    
    u32    _0x60;                   /* 0x60 */
    u32    _0x64;
    struct  *curr_collision_triangle;   // struct size = 0x2c - >> u16 0x00 = collision type
    float  _0x6c;    
    float  _0x70;                   /* 0x70 - distance from ground Y ? (recheck) */
    u32    _0x74;                   
    u32    _0x78;
    u32    _0x7c;    
    u32    _0x80;                   /* 0x80 */
    u32    _0x84;
    u32    Mario_obj_ptr;
    u32    _0x8c_ptr;    
    u32    _0x90_ptr;               /* 0x90 */
    u32    _camera_struct_ptr;      /* not 100% sure about this one */
    u32    _0x98_ptr;               /* 0x8033B3B0 */
    u32    _Pad_struct_ptr;         /* this should be used for reading controller input  */
    u32    _0xa0_ptr;               /* 0x8033B080 */
    u32    _0xa4;
    s16    coins;                   /* 0xa8 */
    s16    stars;                   /* 0xaa */
    s16    lifes;                   /* 0xac */
    s16    power;                   /* 0xae */
    u16    constant_ground_distance;   /* usually 0xBD */
    u16    misc_timer;              /* on any value other than zero it will decrease until zero */
    u32    _0xb4;
    u32    _0xb8;
    float  _0xbc;    
    float  _0xc0;                   /* 0xc0 */     
};

struct object                /* Regular objects, Mario also has its own struct like this */
{
    u16    graph_node_type;        /* 0x00 */
    u16    graph_flags;
    u32    *prev;                  /* previous linked list object */
    u32    *next;                  /* next linked list object */ 
    u32    graph_parent;
    u32    graph_child;            /* 0x10 */
    u32    geo_layout_ptr; 
    u32    _0x18;
    u32    _0x1c;
    float  float_0x20;             /* 0x20 */
    float  float_0x24;
    float  float_0x28;
    float  x_scaling;
    float  y_scaling;              /* 0x30 */
    float  z_scaling;
    u16    _0x38;
    u16    _0x3a;
    u32    animation;              /* 0x3c - current animation */
    u16    _0x40;                  /* 0x40 */
    u16    _0x42;
    u32    _0x44;
    u32    _0x48;
    u32    _0x4c;
    u32    matrix_ptr;             /* 0x50 */
    float  float_0x54;
    float  float_0x58;
    float  float_0x5c;
    u32    next_object_ptr;        /* 0x60: re-check this */
    u32    _0x64;
    u32    next_object_ptr2;       /* 0x68: re-check this (child_obj) */
    u32    _0x6c;
    u32    _0x70;                  /* 0x70 */
    u16    active;                 /* 0x0000 = inactive, 0x0101 = active */
    u16    _0x76;                  /* collision flag according to YE */
    u32    collided_obj_ptr;      /* according to YE, pointer to object collided with */
    u32    _0x7c;
    u32    _0x80;                  /* 0x80 */
    u32    _0x84;
    u32    _0x88;
    u32    obj_flags;
    u32    _0x90;                  /* 0x90 */
    u32    _0x94;
    u32    _0x98;
    u32    _0x9c;
    float  x_pos;                  /* 0xa0 */
    float  y_pos;
    float  z_pos;
    u32    _0xac;
    u32    _0xb0;                  /* 0xb0 */
    u32    _0xb4;
    float  _0xb8;
    u32    _0xbc;
    u32    _0xc0;                  /* 0xc0 */
    u32    collision_rot_x;          /* collision rotation (float?) */
    u32    collision_rot_y;
    u32    collision_rot_z;
    float  x_rotation;              /* 0xd0 */
    float  y_rotation;
    float  z_rotation;
    u32    _0xd8;
    u32    _0xe0;                  /* 0xe0 */
    float  _0xe4;
    u32    _0xe8;
    u32    _0xec;
    u32    _0xf0;                  /* 0xf0 */
    u32    obj_type;               /* obj type for some behaviors (ie, ice bully) */
    u32    _0xf8;
    u32    _0xfc;
    u32    _0x100;                 /* 0x100 */
    u32    _0x104;
    u32    _0x108;
    u32    _0x10c;
    u32    _0x110;                 /* 0x110 */
    u32    _0x114;
    u32    _0x118;
    u32    _0x11c;
    u32    animation_ptr;          /* 0x120 = (set by 0x27 26 behavior command) entry for animation? */
    u32    _0x124;
    float  _0x128;
    float  _0x12c;
    u32    interaction;            /* 0x130 
                                      00 = Something Solid. Can't grab. Mario walks around, Can jump over.
                                      01 = Crashed when jumping at it, Used by Hoot.
                                      02 = Grabbing
                                      04 = Going through door
                                      08 = Knocks mario back and dissappears. No damage.
                                      10 = Something Solid, Can't grab, Mario walks around, Can't jump over, Seems somewhat thin..
                                      40 = Climbing 
                                   */
    u32    _0x134;
    u32    _0x138;
    u32    _0x13c;
    u32    _0x140;                 /* 0x140 */
    u32    behav_param;            /* behav param */
    u32    _0x148;
    u32    action;
    u32    _0x150;                 /* 0x150 */
    u32    timer;
    float  _0x158;                 /* also some sort of distance from mario? */
    float  distance_from_mario;
    u32    _0x160;                 /* 0x160 */
    float  _0x164_x;
    float  _0x168_y;
    float  _0x16c_z;
    float  _0x170;                 /* 0x170 */
    float  _0x174;
    u32    _0x178;
    u32    transparency;
    u32    damage_to_mario;        /* According to YE, "How many segments of damage to do to Mario for objects that cause him harm" */
    u32    health;                 /* Health (ie, for King bob-omb and whomp */
    u32    behav_param2;           /* re-check */
    u32    _0x18c;
    u32    _0x190;                 /* 0x190 */
    float  collision_distance;
    u32    _0x198;
    float  disappear_distance;
    u32    _0x1a0;                 /* 0x1a0 */
    u32    _0x1a4;
    u32    _0x1a8;
    u32    _0x1ac;
    u32    _0x1b0;                 /* 0x1b0 */
    u32    _0x1b4;
    u32    _0x1b8;
    u32    _0x1bc;
    u32    _0x1c0;                 /* 0x1c0 */
    u32    _0x1c4;
    u32    _0x1c8;
    u32    script_ptr;
    u32    stack_index;            /* 0x1d0 */
    u32    stack;
    u32    _0x1d8;
    u32    _0x1dc;
    u32    _0x1e0;                 /* 0x1e0 */
    u32    _0x1e4;
    u32    _0x1e8;
    u32    _0x1ec;
    u32    _0x1f0;                 /* 0x1f0 */
    u16    _0x1f4;
    u16    _0x1f6;
    float  col_sphere_x;
    float  col_sphere_y;
    float  _0x200;                 /* 0x200 */
    float  _0x204;
    float  _0x208;
    u32    behavior_script_entry;
    u32    _0x210;                 /* 0x210 */
    u32    collide_obj_ptr;        /* pointer to another object (collision happening)?. 
                                   Can be used to detect if Mario is on top of the object by comparing
                                   value with Mario's pointer */
    u32    collision_ptr;
    u32    _0x21c;
    u32    _0x220;                 /* 0x220 */
    u32    _0x224;
    u32    _0x228;
    u32    _0x22c;
    u32    _0x230;                 /* 0x230 */
    u32    _0x234;
    u32    _0x238;
    u32    _0x23c;
    u32    _0x240;                 /* 0x240 */
    u32    _0x244;
    u32    _0x248;
    u32    _0x24c;
    u32    _0x250;                 /* 0x250 */
    u32    _0x254;
    u32    _0x258;
    u32    behav_param_copy_ptr;
};
