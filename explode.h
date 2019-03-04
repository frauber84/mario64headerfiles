/*  Mario 64 header file rev1 by messiaen. 
**
**  The information contained in this file is a compilation from many sources, 
**  especially Cellar Dweller's, Nagra's, Yoshielectron's notes along with 
**  original research by me. This is to be used with a C compiler targetting MIPS.
**
**  You can a find tutorial on setting up a N64 MIPS-GCC Toolchain on Windows
**  at http://code.google.com/p/gzrt/wiki/Nintendo64ToolchainSetup (ZZT32 we lurve you)
**
**  If you are using an *nix environment, check this Wiki for info about the
**  appropriate MIPS-R4300i binutils:
**
**  http://en.wikibooks.org/wiki/N64_Programming
**
**  You can contact me at the Jul SM64 Hacking forum @ http://jul.rustedlogic.net/
*/  

/* Constants - struct pointers */
#define M64_CURR_OBJ_PTR            0x80361160   /* Pointer to object being currectly processed */
#define M64_FIRST_OBJ_STRUCT        0x8033D488   /* Pointer to the first object (out of 240) in the circular linked list */
#define M64_MARIO_STRUCT            0x8033B170   /* you can read it from 0x8032d93c */
#define M64_MARIO_OBJ_PTR           0x80361158   /* Pointer to Mario OBJ struct in RAM */
#define M64_LEVEL_STRUCT            0x8033B90c

/* Constants - misc pointers */
#define M64_DISPLAY_STATS_FLAGS     0x8032b26a
#define M64_SEGMENT_TABLE           0x8033b400
#define M64_CURRENT_LEVEL_ID        0x8032ddf8 /* u16 */
#define M64_GEO_LAYOUT_PTR_TABLE    0x8032ddc4 /* Pointer to pointer */
#define DEBUG_FLAG1                 0x8032d598

/* graph flags */
#define BILLBOARD 4
#define INVISIBLE 0  /* recheck */
#define VISIBLE   1  /* recheck */


//library function:

extern int sprintf ( char * str, const char * format, ... );          
          
/* Functions */
extern int   CreateMessageBox(u16 flags, u16 rotate_to_mario, u16 type_of_dialog, u16 message_id);
/* CreateMessageBox                                          */
/* return value = 0x00 -> dialog is happening                */
/*                0x01 -> dialog is over (choice #1)         */
/*                0x02 -> dialog is over (choice #2)         */
/*                0x03 -> normal dialog is over              */
/* type of dialog = 0xA1 -> save related (wing blocks?)      */
/*                  0xA2 -> regular dialog                   */ 
/*                  0xA3 -> two choices                      */ 
/*   You may want to set an wrapper function for             */
/*   CreateMessageBox (check yoshi.c for an example)         */

extern int   CreateStar(float x, float y, float z);   /* returns pointer for spawned object */
extern void  CopyObjParams(u32 *dest, u32 *source);  /* copies X,Y,Z + rotation from another object */
extern void  CopyObjPosition(u32 *dest, u32 *source);
extern void  CopyObjRotation(u32 *dest, u32 *source);
extern void  CopyObjScaling(u32 *dest, u32 *source);
extern int   DeactivateObject(u32 obj_pointer); /* kills current object */
extern float DistanceFromObject(u32 object1, u32 object2);  /* usually object 1 = (*Obj) and object 2 = Mario */
extern void  DmaCopy(u32 dst, u32 bottom, u32 top);
extern void  ExplodeObject(u32 obj_ptr);
extern void  PlaySound(u32 argument);
extern void  HideObject();  /* hides current object by ORing 0x01 at offset 0x02 */
extern void  UnHideObject(); /* ORs 0x10*/
extern int   RotateTorwardsMario(int current_rotation, int rotation_speed, int arg2);
extern void  ScaleObject(float global_scaling_factor);
extern void  ScaleXYZ(u32 obj_pointer, float x, float y, float z);
extern void  SetModel(u16 model_ID);  /* change how the object looks */
extern int   SetObjAnimation(u16 animation_index);
extern int   ShakeScreen(u16 argument); /* argument = 1 to 4 (?) */
extern int   SpawnObj(u32 obj_pointer, u16 model_id, u32 behavior);   /* returns pointer for spawned object */
extern int   CheckObjBehavior(u32 behavior_segmented_pointer); /* return 1 if behavior == arg, else 0  */
extern int   CheckObjBehavior2(u32 obj_pointer, u32 behavior_segmented_pointer); /* return 1 if behavior == arg, else 0  */
extern void  SetObjBehavior(u32 obj_pointer, u32 behavior_segmented_pointer); /* 0x802a14c4 */
extern int   IsMarioStepping(); /* returns 1 if Mario is on TOP of a solid object, else 0 */
extern void  ProcessCollision(); /* 0x803938cc, usually called from behaviors */
extern int   SetMarioAction(u32 mario_struct_pointer, u32 action, u32 unk_arg);  /* to Do: check return values */

extern int  ProcessGeoLayout(u32 *dest, u32 segmented_address);   /* 0x8037e0b4 */


/* Music Related */
extern int  SetMusic(u32 layer, u16 song_index, u32 a2); /* possible layers = 0 (main bgmusic), 1 (other musics) or 2 (sfx) */
extern int  SetInstrument(u32 *chan_ptr, u8 instrument_index);

/* these functions need to be tested further */
extern void  CreateTextBox(u16 msg_ID);
extern int   PrintText(u32 x_pos, char *text, u32 fade); /* used in Credits. a2 = a float value ? */
extern int   PrintRegularText(u32 x, u32 y, char *table_text_pointer);  /* needs to be tested */ 
extern int   StopMario(u16 arg);  /* 1 = stop mario  2 = do nothing??

/* print functions */
extern void  PrintInt (u16 x, u16 y, char* text, u32 value);
extern int   PrintRegularText(u32 x, u32 y, char *table_text_pointer);  /* needs to be tested more, used at credits?*/ 
extern void  PrintXY(u16 x, u16 y, char* text);

/* memory functions */
extern int   SegmentedToVirtual(u32 segmented_pointer);   /* returns RAM pointer of a segmented address*/
extern int   GetSegmentBase(int segment);
extern int   SetSegmentBase(int segment, void *base);  /* sets segment pointer table */
extern u8    *DynamicIndexCopy(u32 index, u32 begin, u32 end, u32 what);

/* math stuff */
extern float sqrtf(float x);
extern float sinf(float x);
extern float cosf(float x);

typedef struct anim /* unfinishied */
{
    u16 framecount; /* 0x08 */
    u32 pointer;    /* 0x0c */
    u32 pointer2;   /* 0x10 */
} Animation;

typedef struct anim2
{
    u32 *AnimationDMATable;    // 0xd1 items, each item 8 bytes in lenght
    u32 Current_DMA;           // not sure
    u32 TargetAnimationPtr;    // 0x80060030 - gets copied to MarioObj->animation (that's where's animation data is DMAed)
    u32 padding;
} MarioAnimation;

extern int   SetMarioAnimation(MarioAnimation *AnimStruct, u16 index); /* returns 1 if animation has changed, 0 if its the same as before) */



typedef struct Music2
{
   u32 _0x00;
   u32 _0x04;
   u32 _0x08;
   u32 _0x0c;
   u32 _0x10;
   u32 _0x14;
   u32 _0x18;
   u32 _0x1c;
   f32 volume;       /* 1 = 0x7f  -  0x20 */
   f32 _0x024_maybe_pan;
   f32 _0x028;
   f32 pitch_transposition;
   u32 _0x30;
   u32 _0x34;
   u32 _0x38;
   u32 instrument;   /* pointer to instrument */
   /* stuff missing here */
   u32 Vibrato;      /* 0x70 */

} ChannelStruct;

typedef struct Music1
{
    u32 _0x00;
    u32 _0x04;
    u16 _0x08;
    u16 tempo;        /* 0x0a */
    u32 _0x0c;
    u32 _0x10;
    u32 pointer_seq_head;
    f32 volume;
    u32 _0x1c;
    f32 _0x20;
    f32 _0x24;
    u32 _0x28;
    ChannelStruct *Channel[15];
    u32 _0x7c_sequence_pointer;
} MusicController;          /* Layer 0 = 0x80222618 */

typedef struct collision_triangle
{
    u16 collision_type;   /* check collision.txt */
    u16 _0x02;
    u8 flag;
    u8 _0x05;
    s16 ymin;
    s16 ymax;
    s16 vertex1_x, vertex1_y, vertex1_z;   /* 0x0a */
    s16 vertex2_x, vertex2_y, vertex2_z;   /* 0x10 */
    s16 vertex3_x, vertex3_y, vertex3_z;   /* 0x16 */
    float normal_x;
    float normal_y;
    float normal_z;
    float negdot;
    u32 _0x2c;   /* unused? */
} CollisionTriangle;

typedef struct pad_struct  /* from nagra */
{
	s16 stick_x;
    s16 stick_y;
	float x;
    float y;
    float z;
	u16 currentButton;      /* 0x10 */
    u16 previousButton;
	u32 *statusData;      	/* 0x14 */
	u32 *controllerData;	/* 0x18 */
} Pad;

typedef struct camera_struct  /* mario->camera (0x8033C520) */
{
    u32 mario_action;       /* copied from mario->action */
    float x;               /* also copied from Mario struct */ 
    float y;
    float z;
    s16 _0x10_mario_0x2c;  /* 0x10 */
    u16 rotation;          /* again copied from Mario struct */
    s16 _0x14_mario_0x30;
    s16 _0x16_mario_0x32;
    u32 _0x18;
    u16 _0x1c;
    u16 camera_setting;    /* 0x06 = door opening   0x09 = triggers initial peach animation */
    
     /* incomplete, many other members left ?? */ 
} Camera;

typedef struct level_struct /* 0x8033B90c, from Cellar Dweller's notes */
{
    s16 _0x00;
    s16 terrain_type;
    u32 geo_layout_ptr;
    u32 collision_ptr;
    u32 _0x0c;
    u32 mini_objects_ptr;         /* 0x10 pointer to an array of objects defined by command 0x39 */
    u32 warp_links_head;
    u32 _0x18;
    u32 _0x1c;
    u32 *objects_head;           /* 0x24 objects linked list head */
    u32 LevelCameraPointer;   /* Level Camera Pointer (generated at run-time) */
    u32 _0x28;
    u32 _0x2c;
    u32 _0x30;
    u8 _0x34;         /* set by level command 0x30 */
    u8 _0x35;
    s16 music_param;      /* 0x36 */
    s16 music_param2;     /* 0x38 title screen,etc) */
    /* more? */
} Level;

typedef struct object_struct           /* Regular objects, Mario also has its own struct like this */
{
    u16    graph_node_type;        /* 0x00 */
    u16    graph_flags;
    struct object_struct *prev;                  /* previous linked list object */
    struct object_struct *next;                  /* next linked list object */ 
    u32    graph_parent;
    u32    graph_child;            /* 0x10 */
    u32    geo_layout_ptr;         /* 0x14 */
    u32    _0x18;
    u32    _0x1c;
    float  _0x20;             /* 0x20 */
    float  _0x24;
    float  _0x28;
    float  x_scaling;              /* 0x2c */
    float  y_scaling;              /* 0x30 */
    float  z_scaling;
    u16    _0x38;
    u16    _0x3a;
    u32    animation;              /* 0x3c - current animation */
    u16    anim_current_frame;     /* 0x40 */
    u16    anim_timer;             /* timer, animation related? */
    u16    anim_current_frame_copy;
    u16    _0x46;
    u32    _0x48;
    u32    _0x4c;
    u32    matrix_ptr;             /* 0x50 */
    float  float_0x54;
    float  float_0x58;
    float  float_0x5c;
    struct object_struct  *next_object_ptr;        /* 0x60: re-check this */
    u32    _0x64;
    struct object_struct  *next_object_ptr2;       /* 0x68: re-check this (child_obj) */
    u32    _0x6c;
    u32    _0x70;                  /* 0x70 */
    u16    active;                 /* 0x0000 = inactive, 0x0101 = active */
    u16    _0x76;                  /* collision flag according to YE */
    struct object_struct  *collided_obj_ptr;      /* according to YE, pointer to object collided with */
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
    float  x_speed;  /* x increment? */
    float  y_speed;                  /* 0xb0 */
    float  z_speed;  /* z_increment? */
    float  speed;
    u32    _0xbc;
    u32    _0xc0;                  /* 0xc0 */
    u32    x_rotation;             /* 0xc4 - rotation triplet */
    u32    y_rotation;             /* 0xc8 */
    u32    z_rotation;
    u32    x_rotation2;            /* rotation copy (collision?) 0xd0 */
    u32    y_rotation2;            /* 0xd4 */
    u32    z_rotation2;
    u32    _0xd8;
    u32    _0xe0;                  /* 0xe0 */
    float  _0xe4;         /* gravity related? y_speed - 0xe4 ? */
    u32    _0xe8;
    u32    _0xec;
    u32    _0xf0;                  /* 0xf0 */
    u32    _0xf4;               /* obj type for some behaviors (ie, ice bully), for AMPS, radius of rotation */
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
    u32    animation_ptr;        /* 0x120 = (set by 0x27 26 behavior command) entry for animation? */
    u32    _0x124;                  /* in some behaviors, action related? */
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
    u32    _0x150;                 /* 0x150 = also reset when action changes */
    u32    timer;                  /* always incremented. When action changes, it's set to 0 */
    float  _0x158;                 
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
    u32    previous_action;        /* used to reset the 0x154 timer */
    u32    _0x190;                 /* 0x190 */
    float  collision_distance;     /*  NOTE: if collision_distance < disappear_distance then disappear_distance = collision_distance */
    u32    _0x198;
    float  drawing_distance;
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
    u32    collision_ptr;          /* set by behavior script (0x2A command) */
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
} Object;

typedef struct mario_struct                /* 8033b170 */
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
    u16    _0x24;                    /* rotation related, if bit 1 of status is set, 0x24 is copied to 0x2e */
    s16    hitstun;                 /* hitstun counter (how long Mario stays invencible after getting hit */
    u32    _0x28;
    s16    _0x2c;
    u16    rotation;                /* divide it by 180 to get the angle? */
    s16    _0x30;                   /* 0x30 */
    s16    _0x32;    
    u32    _0x34;
    u32    _0x38;
    float  x_pos;                   /* 0x3c */
    float  y_pos;                   /* 0x40 */ 
    float  z_pos;
    float  x_speed;
    float  y_speed; 
    float  z_speed;                 /* 0x50. The next four floats are related to speed/acelleration */
    float  speed;
    float  _0x58;
    float  _0x5c;    
    u32    _0x60;                   /* 0x60 */
    u32    _0x64;
    CollisionTriangle  *curr_collision_triangle;  /* current triangle mario is stepping in */
    float  _0x6c;    
    float  ground_y;                   /* 0x70 - ground Y */
    u32    _0x74;                   
    u32    _0x78;
    u32    _0x7c;    
    u32    _0x80;                   /* 0x80 */
    u32    _0x84;
    Object   *MarioObj;
    u32    _0x8c_ptr;    
    u32    Mario_level_command;     /* 0x90 = 8033b4b0 = Information read from the Level command that sets Mario*/
    Camera *camera;
    u32    _0x98_ptr;               /* 0x8033B3B0 */
    Pad    *pad;                    /* pointer to controller struct  controller 1 = 8033AF90   controller2 = 8033AFAC*/
    MarioAnimation *MarioAnimationStruct;   /* 0x8033B080 */
    u32    _0xa4;
    s16    coins;                   /* 0xa8 */
    s16    stars;                   /* 0xaa */
    s16    lifes;                   /* 0xac */
    s16    power;                   /* 0xae */
    u16    constant_ground_distance;   /* usually 0xBD */
    u16    misc_timer;              /* on any value other than zero it will decrease until zero (also, drains mario energy?) */
    u32    cap_timer;
    u32    _0xb8;
    float  _0xbc;    
    float  _0xc0;                   /* 0xc0 */     
} MarioStruct;
