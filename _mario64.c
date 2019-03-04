#include <n64.h>
#include "explode.h"
#include "levels.h"
#include "animations.h"

#define PI 3.14159
#define DEBUG 0
// checkout: 0x8033b06c = display list buffer?

/* Music stuff */
#define NINST_POINTER 0x801D0370      /* table for instrument sets */
#define NULL_CHANNEL 0x80225bd8

/* Pong of Death stuff */
#define INITIAL_Y_POS     110
#define MAX_Y_POS         200
#define MIN_Y_POS         10

/* Bubble Sort */
#define SWAP(a,b)   { int t; t=a; a=b; b=t; }  // Macro for swapping

/* animation pointers */
#define  BOBOMB                0x0802396c
#define  GOOMBA                0x0801DA4C
#define  KOOPA                 0x06011364
#define  CHUCKYA               0x0C00C070
#define  CHAINCHOMP            0x06025178
#define  DOOR                  0x030156C0
#define  PEACH                 0x0501c504
#define  SPINDRIFT             0x05002d68
#define  HEAVEHO               0x0501534c
#define  BOWSER                0x060577e0
#define  PENGUIN               0x05008b74
#define  SCUTTLEBUG            0x06015064
#define  LAKITU                0x060058F8
#define  EVILLAKITU            0x050144D4
#define  TOAD                  0x0600FC48
#define  PIRANHA               0x0601C31C
#define  WHOMP                 0x06020A04
#define  AMP                   0x08004034
#define  BUTTERFLY             0x030056B0
#define  OWL                   0x05005768
#define  BULLY                 0x0500470C
#define  MIPS                  0x06015724
#define  YOSHI                 0x050241E8
#define  BAT                   0x060070D0
#define  FLYGUY                0x08011A64
#define  WIGGLER               0x0500EC8C
#define  MONTYMOLE             0x05007248
#define  MRBLIZZARD            0x0500D118
#define  DORRIE                0x0600F638
#define  HAUNTEDCHAIR          0x05005784
#define  MADPIANO              0x05009B14
#define  FLYINGBOOKEND         0x05002540
#define  KLEPTO                0x05008CFC
#define  BIRD                  0x050009E8
#define  CLAMSHELL             0x05001744
#define  SKEETER               0x06007DE0


extern s16 _8037a9a8(float y, float x);
/* Soundlist is incomplete, done using a modified Project64 which dumps PlaySound function arguments  */
u32 SoundList[] = { 0x70140081, 0x700b0081, 0x2432ff81, 0x701eff81, 0x70060081, 0x70110081, 0x70070081, 0x7023ff81, 0x60104001, 0x50514001, 0x80504001, 0x90524001, 0x40010001, 0x2428ff81, 0x6002ff01, 0x4568081, 0x45e8081, 0x45f8081, 0x7017a081, 0x701a8081, 0x24048081, 0x444a081, 0x4088081, 0x24118081, 0x7016a081, 0x70040081, 0x70130081, 0x70050081, 0x6108081, 0x4008081, 0x24008081, 0x241e8081, 0x4378081, 0x24038081, 0x242e8081, 0x4358081, 0x24228081, 0x4608081, 0x14000001, 0x24248081, 0x24018081, 0x45bff81, 0x4098081, 0x6118081, 0x14010001, 0x4018081, 0x430c081, 0x300b0081, 0x4338081, 0x447a081, 0x4316081, 0x4028081, 0x40b8081, 0x6138081, 0x14030001, 0x4038081, 0x241f8081, 0x7002ff81, 0x7000f881, 0x7003ff81, 0x700f0081, 0x60028001, 0x3004c081, 0x3005c081, 0x505f8091, 0x38118081, 0x445a081, 0x24058081, 0x24308081, 0x41b8081, 0x240bd081, 0x7024ff81, 0x502f0081, 0x400a0001, 0x50270081, 0x50584081, 0x24028081, 0x4198081, 0x444b081, 0x38300081, 0x3041c081, 0x50200081, 0x4188081, 0x5060b081, 0x30364081, 0x400d0001, 0x30390081, 0x60086001, 0x303a0081, 0x802e2081, 0x78289081, 0x2413d081, 0x3065c081, 0x300f0081, 0x30400081, 0x240c8081, 0x39280081, 0x70150081, 0x70222081, 0x43d8081, 0x4408081, 0x43e8081, };
u32 Behaviors[519] =
{
0x13000000, 0x13000054, 0x1300008c, 0x130000ac, 0x130000f8, 
0x13000118, 0x13000144, 0x13000174, 0x13000194, 0x130001ac, 
0x130001cc, 0x130001f4, 0x13000254, 0x13000278, 0x1300029c, 
0x130002b8, 0x130002c4, 0x130002e4, 0x13000338, 0x130003bc, 
0x13000400, 0x13000428, 0x13000444, 0x1300046c, 0x13000494, 
0x130004a8, 0x130004e4, 0x13000500, 0x13000528, 0x13000584, 
0x130005a8, 0x130005b4, 0x130005d8, 0x13000600, 0x13000624, 
0x13000638, 0x1300066c, 0x130006a4, 0x130006d8, 0x130006e0, 
0x13000708, 0x13000720, 0x1300075c, 0x13000780, 0x130007a0, 
0x130007dc, 0x130007f8, 0x1300080c, 0x13000830, 0x13000888, 
0x130008d0, 0x130008ec, 0x1300090c, 0x1300091c, 0x13000940, 
0x13000964, 0x13000984, 0x130009a4, 0x130009e0, 0x13000a14, 
0x13000a34, 0x13000a54, 0x13000a64, 0x13000a78, 0x13000a98, 
0x13000abc, 0x13000ad8, 0x13000ae8, 0x13000afc, 0x13000b0c, 
0x13000b58, 0x13000b8c, 0x13000bc8, 0x13000c04, 0x13000c28, 
0x13000c44, 0x13000c64, 0x13000c84, 0x13000cc8, 0x13000cfc, 
0x13000d30, 0x13000d6c, 0x13000d98, 0x13000db4, 0x13000dd8, 
0x13000dfc, 0x13000e24, 0x13000e3c, 0x13000e40, 0x13000e58, 
0x13000e70, 0x13000e88, 0x13000eac, 0x13000ed0, 0x13000f08, 
0x13000f14, 0x13000f2c, 0x13000f48, 0x13000f9c, 0x13000fc8, 
0x13001000, 0x13001030, 0x13001064, 0x13001098, 0x130010b8, 
0x130010d8, 0x13001108, 0x13001124, 0x13001168, 0x13001184, 
0x130011d0, 0x130011ec, 0x13001214, 0x13001254, 0x1300127c, 
0x13001298, 0x130012b4, 0x130012f4, 0x13001318, 0x13001340, 
0x13001368, 0x13001390, 0x130013a8, 0x130013c4, 0x130013dc, 
0x13001408, 0x1300142c, 0x13001448, 0x13001468, 0x13001478, 
0x13001484, 0x130014ac, 0x130014bc, 0x130014e0, 0x13001518, 
0x13001548, 0x130015a4, 0x130015c0, 0x130015e4, 0x13001608, 
0x13001634, 0x13001650, 0x1300167c, 0x130016ac, 0x130016b8, 
0x130016e4, 0x13001714, 0x13001744, 0x13001778, 0x1300179c, 
0x130017f4, 0x13001820, 0x13001828, 0x13001850, 0x130018cc, 
0x13001904, 0x13001920, 0x13001958, 0x13001984, 0x130019c8, 
0x13001a0c, 0x13001a30, 0x13001a74, 0x13001aa4, 0x13001ae8, 
0x13001b2c, 0x13001b54, 0x13001b70, 0x13001b88, 0x13001bb4, 
0x13001bd4, 0x13001bf4, 0x13001c04, 0x13001c34, 0x13001c58, 
0x13001c7c, 0x13001c8c, 0x13001cb0, 0x13001d0c, 0x13001d14, 
0x13001d40, 0x13001d70, 0x13001d78, 0x13001da8, 0x13001dcc, 
0x13001e04, 0x13001e30, 0x13001e4c, 0x13001e6c, 0x13001e94, 
0x13001ec4, 0x13001ef8, 0x13001f30, 0x13001f3c, 0x13001f68, 
0x13001f90, 0x13001fbc, 0x13002018, 0x13002038, 0x13002068, 
0x13002088, 0x130020d8, 0x130020e0, 0x130020e8, 0x1300213c, 
0x1300214c, 0x1300215c, 0x13002178, 0x13002194, 0x130021c0, 
0x130021e4, 0x1300220c, 0x13002250, 0x1300227c, 0x1300229c, 
0x130022b8, 0x130022d8, 0x13002308, 0x13002338, 0x13002388, 
0x130023a4, 0x130023d0, 0x130023ec, 0x1300241c, 0x1300244c, 
0x1300246c, 0x13002480, 0x130024ac, 0x130024dc, 0x13002500, 
0x13002528, 0x13002558, 0x13002568, 0x13002588, 0x130025c0, 
0x130025e0, 0x130025f8, 0x13002620, 0x13002634, 0x13002650, 
0x13002684, 0x1300269c, 0x130026d4, 0x13002710, 0x13002768, 
0x1300277c, 0x13002790, 0x130027d0, 0x130027e4, 0x130027f4, 
0x13002804, 0x1300286c, 0x13002898, 0x130028cc, 0x130028fc, 
0x1300292c, 0x13002968, 0x13002998, 0x130029b4, 0x130029e4, 
0x13002a10, 0x13002a20, 0x13002a48, 0x13002a54, 0x13002a5c, 
0x13002a7c, 0x13002aa4, 0x13002ad0, 0x13002af0, 0x13002b08, 
0x13002b14, 0x13002b5c, 0x13002ba0, 0x13002bb8, 0x13002bcc, 
0x13002c14, 0x13002c60, 0x13002c7c, 0x13002cb0, 0x13002ce0, 
0x13002d28, 0x13002d50, 0x13002d7c, 0x13002db0, 0x13002dc0, 
0x13002e04, 0x13002e20, 0x13002e3c, 0x13002e58, 0x13002ea8, 
0x13002ec0, 0x13002ef8, 0x13002f40, 0x13002fa0, 0x13002fc0, 
0x13002fe4, 0x13003008, 0x1300302c, 0x13003048, 0x13003068, 
0x130030a4, 0x130030d4, 0x13003104, 0x13003134, 0x13003158, 
0x13003174, 0x130031ac, 0x130031dc, 0x13003228, 0x13003274, 
0x130032a8, 0x130032c8, 0x130032e0, 0x13003324, 0x13003354, 
0x13003388, 0x130033bc, 0x130033ec, 0x13003420, 0x13003454, 
0x1300345c, 0x13003464, 0x1300346c, 0x13003474, 0x1300347c, 
0x13003484, 0x130034c4, 0x13003510, 0x13003558, 0x13003588, 
0x130035b0, 0x13003604, 0x1300360c, 0x13003614, 0x1300362c, 
0x13003660, 0x13003694, 0x130036c8, 0x13003700, 0x13003738, 
0x13003750, 0x13003798, 0x130037e0, 0x130037ec, 0x1300381c, 
0x13003840, 0x13003868, 0x13003888, 0x130038b0, 0x130038d0, 
0x130038e8, 0x13003910, 0x13003940, 0x13003970, 0x130039a0, 
0x130039d4, 0x13003a08, 0x13003a30, 0x13003a58, 0x13003a80, 
0x13003aa4, 0x13003ac8, 0x13003ae0, 0x13003b00, 0x13003b30, 
0x13003b60, 0x13003b98, 0x13003bb4, 0x13003bec, 0x13003c0c, 
0x13003c30, 0x13003c44, 0x13003c58, 0x13003c7c, 0x13003c90, 
0x13003ca4, 0x13003cb8, 0x13003ce4, 0x13003d0c, 0x13003d34, 
0x13003d4c, 0x13003d74, 0x13003da0, 0x13003db8, 0x13003dd8, 
0x13003df8, 0x13003e1c, 0x13003e3c, 0x13003e64, 0x13003e8c, 
0x13003eac, 0x13003ee4, 0x13003efc, 0x13003f1c, 0x13003f40, 
0x13003f78, 0x13003fa4, 0x13003fdc, 0x13004010, 0x13004044, 
0x1300407c, 0x130040b4, 0x130040ec, 0x13004124, 0x13004148, 
0x13004180, 0x130041a4, 0x130041bc, 0x130041f0, 0x13004218, 
0x13004244, 0x13004270, 0x13004284, 0x130042b4, 0x130042e4, 
0x13004314, 0x13004348, 0x13004370, 0x130043a0, 0x130043c4, 
0x130043e0, 0x13004400, 0x1300442c, 0x1300444c, 0x13004470, 
0x13004494, 0x130044b8, 0x130044e0, 0x130044fc, 0x13004538, 
0x13004580, 0x130045d0, 0x130045f8, 0x13004634, 0x13004668, 
0x13004698, 0x130046dc, 0x1300472c, 0x13004770, 0x1300478c, 
0x130047e4, 0x1300481c, 0x13004868, 0x13004898, 0x130048e0, 
0x13004918, 0x13004954, 0x13004988, 0x130049ac, 0x130049c8, 
0x13004a00, 0x13004a58, 0x13004a78, 0x13004ab0, 0x13004af4, 
0x13004b1c, 0x13004b44, 0x13004b6c, 0x13004b8c, 0x13004ba8, 
0x13004bd4, 0x13004bf0, 0x13004c24, 0x13004c5c, 0x13004c94, 
0x13004ccc, 0x13004cf8, 0x13004d28, 0x13004d64, 0x13004d90, 
0x13004dbc, 0x13004e08, 0x13004e4c, 0x13004e78, 0x13004ea0, 
0x13004ecc, 0x13004ef8, 0x13004f10, 0x13004f28, 0x13004f40, 
0x13004f78, 0x13004f90, 0x13004fd4, 0x13005024, 0x1300506c, 
0x130050b4, 0x130050d4, 0x130050f4, 0x13005120, 0x13005158, 
0x1300518c, 0x130051ac, 0x130051e0, 0x1300521c, 0x1300525c, 
0x130052b4, 0x130052d0, 0x13005310, 0x13005354, 0x13005380, 
0x130053c4, 0x130053dc, 0x130053f4, 0x13005414, 0x13005440, 
0x13005468, 0x130054a0, 0x130054b8, 0x130054ec, 0x13005504, 
0x13005528, 0x1300556c, 0x13005598, 0x130055dc, 0x13005610, 
0x13005638, 0x1300565c, 0x13005680, 0x130056a4, 
};

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

int *AnimationIndex2P = (void*)0x80600074;
MarioAnimation *Animation2P = (void*)0x80600080;



u32 FunctionList[16] = { 0x8030DC70,   /* 0 = snufit */
                         0x8030D598,   /* 1 = fire spewer */
                         0x8030D2F0,   /* 2 = p. plant */
                         0x8030C8EC,   /* 3 = bookshelf thing */
                         0x8030BC90,   /* 4 = haunted chair */
                         0x8030B2F4,   /* 5 = dorrie */
                         0x803094D0,   /* 6 = rotation */
                         0x80309354,   /* 7 = sliding platform */
                         0x802FF96C,   /* 8 = goomba walk */
                         0x802FF040,   /* 9 = fly guy  */
                         0x802FE8B4,   /* 10 = bat */
                         0x802FE3B0,   /* 11 = pokey */
                         0x802F79B0,   /* 12 = pendulum */
                         0x802F7264,   /* 13 = falling pillar */
                         0x802EB9D0,   /* 14 = bully */
                         0x8030D598,   /* 15 = fire spewer */
                       };                               

MusicController *Music = (void*)0x80222618;

void MenuPage01();
void MenuPage02();
void MenuPage03();
void MenuPage04();
void MenuPage05();
void MenuPage06();
void MenuPage07();
void MenuPage08();
void MenuPage09();
void ProcessSelection();
void PongOfDeath();
void bubble_srt(int a[], int n);
void CheckFireCollision(Object *Source);

void _start ( void )
{
    asm volatile("la $gp, _gp");

    *counter2 = *counter2 + 1;

    //patch object distance calculation
    u32 *apointer = (void*)0x80385C2C;
    *apointer =  0x3C058060;  // LUI A1, 0x8060
    apointer++;
    *apointer =  0x8CA50070; //  LW A1, 0x0070 (A1)  Load ObjectDistanceSource
    
    u32 *MarioVariable = (void*)0x80361158;
    *ObjectDistanceSource = *MarioVariable;
    
    Animation2P->AnimationDMATable = 0x80064040;
    Animation2P->TargetAnimationPtr = 0x80500000;

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
    

    if (Mario->pad->currentButton & BUTTON_L && *counter2 > 3)        
    {
                                  
        if (Mario->pad->currentButton & BUTTON_D_LEFT)
        {
            *page -= 1;
            if (*page < 0) *page = 9;
            *counter2 = 0;
            *counter = 0;
        }
        else if (Mario->pad->currentButton & BUTTON_D_RIGHT)
        {
            *page += 1;
            if (*page > 9) *page = 0;
            *counter2 = 0;
            *counter = 0;
        }
        else if (Mario->pad->currentButton & BUTTON_R)  /* easter egg :P */
        {
            *page = 666;
            *counter2 = 0;
            *counter = 0;
        }
        
    }
    
    *distance = *distance + *cameradistance; // update camera
    Camera *camera = Mario->camera;     
    camera->x = Mario->x_pos;
    camera->y = Mario->y_pos;
    camera->z = Mario->z_pos;
        
    switch (*page)
    {
           case 0:
                PrintInt(10, 30, "sm64 super debug ", 0);
                PrintInt(10, 10, "by frauber aka messiaen", 0);
                break;
           
           case 1:
                MenuPage01();   // music
                break;
           case 2:
                MenuPage02();  // music channels
                break;

           case 3:
                MenuPage03(); // music instruments
                break;
           
           case 4:
                MenuPage04();
                break;
                
           case 5: // control obj
                MenuPage05();
                break;

           case 6:  //spawn
                MenuPage06();
                break;
                
           case 7:
                MenuPage07(); // camera
                break;

           case 8:
                MenuPage08(); // banks
                break;
                
           case 9:
                MenuPage09(); // controller stuff
                break;
                

           case 666:
                PongOfDeath();
                break;
                
           default:
                PrintInt(10, 30, "sm64 super debug", 0);
                PrintInt(10, 10, "by frauber aka messiaen", 0);                
                break;
                
    }
    
}

void MenuPage01()
{
     
    if (*counter == 0)
    {
        *selection = 2;
        *counter = 1;
    }    

    ProcessSelection(2, 6, 0);     
    
    u8 *ninst = (void*)NINST_POINTER + (*music *2) + 1;
    u8 *ninst2 = (void*) (*ninst + 0x801D0370 + 1) ;
    
    switch (*selection)
    {
           
        case 2: // music//
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                if (!(*music <= 0x00)) *music = *music - 1;
        
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                if (!(*music >= 0x22)) *music = *music + 1;        
        
                *counter2 = 0;
            }
            break;
        
        case 3: // Play Music
        
            if ( (Mario->pad->currentButton & BUTTON_D_LEFT ||  Mario->pad->currentButton & BUTTON_D_RIGHT) && *counter2 > 3 )
            { 
                SetMusic(0, *music, 0);  // only layer 0 is used to avoid having to deal with three music controllers
                *counter2 = 0; 
            }
            break;
        
        case 4: //ninst

            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)        
            {
               *ninst2 = *ninst2 - 1;
               if (*ninst2 < 0) *ninst2 = 0;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
               *ninst2 = *ninst2 +1;
               if (*ninst2 > 0x25) *ninst2 = 0x25;
                *counter2 = 0;
            }
            
            break;
            
        case 5: //tempo

            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)        
            {
               Music->tempo -= 48;
               if (Music->tempo < 0) Music->tempo = 0;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
               Music->tempo += 48;
               if (Music->tempo/48 > 300) Music->tempo = 300 * 48;
                *counter2 = 0;
            }
            
            break;

        case 6: // sound

            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                *sound = *sound - 1;
                if (*sound < 0) *sound = 0;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)    
            {
                *sound = *sound + 1;
                *counter2 = 0;
            }
            if ( Mario->pad->currentButton & BUTTON_L && *counter2 > 3 )
            { 
                PlaySound(SoundList[*sound]);
                *counter2 = 0; 
            }        
        
    }
      
    PrintInt(10, (*selection * 20), "*", 0);
    PrintInt(25,120, "Play Sound %x", *sound);    
    PrintInt(25,100, "Tempo %d BPM", Music->tempo/48);
    PrintInt(25,80, "NInst %x", *ninst2);        
    PrintInt(25,60, "Play", 0);
    PrintInt(25,40, "Music %x", *music);    
         
}

void MenuPage02()
{
    int i;
    
    if (*counter == 0)
    {
        *selection = 0;
        *counter = 1;
    }    
     
    ProcessSelection(0, 15, 1);
    
    for (i = 0; i <= 7; i++)
    {
       PrintInt(25, 180 - (i*20), "CH %02d", i);
       if (Music->Channel[i]->volume == -1 || Music->Channel[i] == (void*)NULL_CHANNEL) PrintInt(100, 180 - (i*20), "OFF", 0);
       else  PrintInt(100, 180 - (i*20), "ON", 0);
    }
    
    for (i = 8; i <= 15; i++)
    {
       PrintInt(175, 180 - ( (i-8) *20), "CH %02d", i);
       if (Music->Channel[i]->volume == -1  || Music->Channel[i] == (void*)NULL_CHANNEL) PrintInt(250, 180 - ( (i-8) *20), "OFF", 0);
       else  PrintInt(250, 180 - ( (i-8) *20), "ON", 0);
    }
 
    if (*selection <= 7)
    {
        PrintInt(10, 180 - (*selection * 20), "*", 0);
    }
    else 
    {
         PrintInt(160, 180 - ( (*selection-8) * 20), "*", 0);
    }
    
    
    if ( (Mario->pad->currentButton & BUTTON_D_RIGHT ) && *counter2 > 3 )
    {   
        
        if (Music->Channel[*selection]->volume != -1)
        {
            
            Music->Channel[*selection]->volume = -1;
            *counter2 = 0;
        }
    }
    if ( (Mario->pad->currentButton & BUTTON_D_LEFT ) && *counter2 > 3 )
    {   
        Music->Channel[*selection]->volume = 0.8;
        *counter2 = 0;
    }
     
}

void MenuPage03()
{
    int i;
    
    if (*counter == 0)
    {
        *selection = 0;
        
        for (i = 0; i <=16; i++)
        {
            instrument[i] = (void*)(0x8060a000 + (i*4) );
        }
        
        *counter = 1;
    }    
     
    ProcessSelection(0, 15, 1);    
    
    for (i = 0; i <= 7; i++)
    {
       PrintInt(25, 180 - (i*20), "CH %02d", i);
       if (Music->Channel[i]->volume == -1 || Music->Channel[i] == (void*)NULL_CHANNEL) PrintInt(100, 180 - (i*20), "OFF", 0);
       else PrintInt(100, 180 - (i*20), "%d", *instrument[i]);
    }
    
    for (i = 8; i <= 15; i++)
    {
       PrintInt(175, 180 - ( (i-8) *20), "CH %02d", i);
       if (Music->Channel[i]->volume == -1 || Music->Channel[i] == (void*)NULL_CHANNEL) PrintInt(250, 180 - ( (i-8) * 20), "OFF", 0);       
       else PrintInt(250, 180 - ( (i-8) *20),  "%d", *instrument[i]);
    }
 
    if (*selection <= 7)
    {
        PrintInt(10, 180 - (*selection * 20), "*", 0);
    }
    else 
    {
         PrintInt(160, 180 - ( (*selection-8) * 20), "*", 0);
    }
    
    
    if ( (Mario->pad->currentButton & BUTTON_D_RIGHT ) && *counter2 > 3 )
    {   
        
        if (Music->Channel[*selection]->volume != -1 || Music->Channel[*selection] != (void*)NULL_CHANNEL)
        {
            *instrument[*selection] = *instrument[*selection] + 1;
            if (*instrument[*selection] == 20) *instrument[*selection] = 0x7f;
            if (*instrument[*selection] > 0x7f) *instrument[*selection] = 0x7f;
            SetInstrument(Music->Channel[*selection], *instrument[*selection]);
        }
        *counter2 = 0;
            
    }
    if ( (Mario->pad->currentButton & BUTTON_D_LEFT ) && *counter2 > 3 )
    {   

        if (Music->Channel[*selection]->volume != -1 || Music->Channel[*selection] != (void*)NULL_CHANNEL)
        {
            *instrument[*selection] = *instrument[*selection] - 1;
            if (*instrument[*selection] < 0) *instrument[*selection] = 0x7f;        
            if (*instrument[*selection] == 0x7e) *instrument[*selection] = 19;
            SetInstrument(Music->Channel[*selection], *instrument[*selection]);
        }
        *counter2 = 0;        
        
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
    
    PrintInt(25, 140, "LV TERRAIN %x", CurrentLevel->terrain_type);
    PrintInt(25, 120, "TYPE %x",    Mario->curr_collision_triangle->collision_type);
    PrintInt(25, 100, "V1 %d",      Mario->curr_collision_triangle->vertex1_x);
    PrintInt(160, 100, "%d",        Mario->curr_collision_triangle->vertex1_y);
    PrintInt(240, 100, "%d",        Mario->curr_collision_triangle->vertex1_z);
    PrintInt(25, 80, "V2 %d",       Mario->curr_collision_triangle->vertex2_x);
    PrintInt(160, 80, "%d",         Mario->curr_collision_triangle->vertex2_y);
    PrintInt(240, 80, "%d",         Mario->curr_collision_triangle->vertex2_z);
    PrintInt(25, 60, "V3 %d",       Mario->curr_collision_triangle->vertex3_x);
    PrintInt(160, 60, "%d",         Mario->curr_collision_triangle->vertex3_y);
    PrintInt(240, 60, "%d",         Mario->curr_collision_triangle->vertex3_z);     
    PrintInt(25, 40, "GROUND Y %d", (int)Mario->ground_y);
    
}

void MenuPage05()
{
    
    float tempf[6];
    int   tempi[6];
    int pseudostack, pseudostack2;
    int i, j, k, num_obj = 0;   
    u32 *list_pointer = (void*)0x80608000; 
    
     int (*function)(int) = (void*)FunctionList[*function_id];
    
    if (*counter == 0)
    {
        *obj_model_id = 0xc0;
        
        u32 *mid = (void*)(0x8033b4d0 + (*obj_model_id * 4));
        
        if (*mid == 0)
        {
            while ( *mid != 0)
            { 
                *obj_model_id = *obj_model_id - 1;
                mid--;
            } 
        }
        
        *selection = 1;
        *counter = 1;
        *step = 15;
    }    
    
    if (Mario->pad->currentButton & BUTTON_L && Mario->pad->currentButton & BUTTON_D_UP )
    {
                                  
        if (*selection == 14) *selection = 13;
        
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
        
        int model_index;
        u32 *model_table = (void*)(0x8033b4d0);
        
        *obj_model_id = 0;
        while ( ClosestObject->geo_layout_ptr != *model_table)
        {
            *obj_model_id = *obj_model_id +1;
            model_table++;
        }
        
        *CurrentObject = (void*)pseudostack;
        
        *obj_behav = ClosestObject->behavior_script_entry;
        
    }
        
    list_pointer = (void*)0x80608008;
    
    if ( *list_pointer != 0 )
    {

        ProcessSelection(0, 14, 0);
        
        Object *ClosestObject = (void*) *list_pointer;

        switch ( ClosestObject->animation_ptr ) 
        {
              
           case BOBOMB:       PrintInt(25, 180, "BobOmb", 0);          break;
           case KOOPA:        PrintInt(25, 180, "Koopa", 0);           break;
           case CHAINCHOMP:   PrintInt(25, 180, "ChainChomp", 0);      break;
           case BOWSER:       PrintInt(25, 180, "Bowser", 0);          break;
           case DOOR:         PrintInt(25, 180, "Door", 0);            break;
           case GOOMBA:       PrintInt(25, 180, "Goomba", 0);          break;
           case CHUCKYA:      PrintInt(25, 180, "Chuckya", 0);         break;
           case HEAVEHO:      PrintInt(25, 180, "Heave-ho", 0);        break;
           case PENGUIN:      PrintInt(25, 180, "Penguin", 0);         break;
           case PEACH:        PrintInt(25, 180, "Peach", 0);           break;
           case SPINDRIFT:    PrintInt(25, 180, "SpinDrift", 0);       break;
           case LAKITU:       PrintInt(25, 180, "Lakitu", 0);          break;           
           case EVILLAKITU:   PrintInt(25, 180, "Evil Lakitu", 0);     break;                      
           case TOAD:         PrintInt(25, 180, "Toad", 0);            break;
           case PIRANHA:      PrintInt(25, 180, "Piranha", 0);         break;
           case WHOMP:        PrintInt(25, 180, "Whomp", 0);           break;
           case AMP:          PrintInt(25, 180, "Amp", 0);             break;
           case BUTTERFLY:    PrintInt(25, 180, "Butterfly", 0);       break;
           case OWL:          PrintInt(25, 180, "Owl", 0);             break;           
           case BULLY:        PrintInt(25, 180, "Bully", 0);           break;           
           case MIPS:         PrintInt(25, 180, "MIPS", 0);            break;           
           case YOSHI:        PrintInt(25, 180, "Yoshi", 0);           break;           
           case BAT:          PrintInt(25, 180, "Bat", 0);             break;           
           case FLYGUY:       PrintInt(25, 180, "Fly guy", 0);         break;                      
           case WIGGLER:      PrintInt(25, 180, "Wiggler", 0);         break;                                 
           case MONTYMOLE:    PrintInt(25, 180, "Monty Mole", 0);      break;                                            
           case MRBLIZZARD:   PrintInt(25, 180, "mr blizzard", 0);     break;
           case DORRIE:       PrintInt(25, 180, "Dorrie", 0);          break;
           case HAUNTEDCHAIR: PrintInt(25, 180, "Haunted Chair", 0);   break;
           case MADPIANO:     PrintInt(25, 180, "Mad Piano", 0);       break;
           case FLYINGBOOKEND:PrintInt(25, 180, "Flying BookEnd", 0);  break;
           case KLEPTO:       PrintInt(25, 180, "Klepto", 0);          break;
           case BIRD:         PrintInt(25, 180, "Bird", 0);            break;          
           case CLAMSHELL:    PrintInt(25, 180, "Clam shell", 0);      break; 
           case SKEETER:      PrintInt(25, 180, "Skeeter", 0);         break; 
           /* missing chilly boss and maybe other undefined behaviors*/
           default:          
                 if (ClosestObject->animation_ptr) PrintInt(25, 180, "ANIM OBJ", 0);
                 else if ( (ClosestObject->collision_ptr || ClosestObject->_0x9c == 0 ) ) PrintInt(25, 180, "SOLID OBJ", 0);
           
                  break;

        }


        PrintInt(25, 100, "CUR 80%x", (int)ClosestObject->animation & 0xFFFFFF );
        PrintInt(210, 180, "bhv %x", (u32)( (ClosestObject->behavior_script_entry - GetSegmentBase(0x13) ) & 0xFFFF) );
        PrintInt(145, 160, "ANM 80%x", SegmentedToVirtual(ClosestObject->animation_ptr) & 0xFFFFFF );        
        PrintInt(25, 160, "DIST %d", (int)DistanceFromObject( ClosestObject, Mario->MarioObj) );        
        PrintInt(25, 140, "ACT %d", ClosestObject->action);      
        PrintInt(145, 140, "FRAME %d", (int)ClosestObject->anim_current_frame);
//        PrintInt(25, 120, "test", 0);   //14
//        PrintInt(125, 120, "FUNC %d", *function_id); //13   
//      PrintInt(25, 120, "mid %x", *obj_model_id);  // 12
//        PrintInt(25, 80, "ROT", 0);
//        PrintInt(85, 80, "X", 0);        // 11
//        PrintInt(125, 80, "Y", 0);          
//        PrintInt(165, 80, "Z", 0);        
        
//        PrintInt(25, 60, "ScalE", 0); // 8
//        PrintInt(85, 60, "X", 0);
//        PrintInt(125, 60, "Y", 0);                
//        PrintInt(165, 60, "Z", 0);    
            
//        PrintInt(25, 40, "X %d", (int)ClosestObject->x_pos);  // 4
//        PrintInt(125, 40, "Y %d", (int)ClosestObject->y_pos);   
//        PrintInt(225, 40, "Z %d", (int)ClosestObject->z_pos);
//        PrintInt(25, 20, "STEP %d", *step);  // 1
//        PrintInt(200, 20, "KILL", *step);  // 0
        
        switch (*selection)
        {
         
            case 0: 
//              PrintInt(185, 20, "*", 0);
                if (Mario->pad->currentButton & BUTTON_L && *counter2 > 2)    
                {
                    int pseudostack;
                    pseudostack = *CurrentObject;
                    *CurrentObject = ClosestObject;
                    ExplodeObject( (*Obj) );
                    
                    *CurrentObject = (void*)pseudostack;
                } 
            break;        
           
            case 1: 
//              PrintInt(10, 20, "*", 0);
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    *step -= 5;
                    
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                     *step += 5;
                    *counter2 = 0;
                } 
            break;        
            
            case 2: 
//              PrintInt(210, 40, "*", 0);                 
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->z_pos -= *step;                
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->z_pos += *step;
                    *counter2 = 0;
                } 
            break;        
            
            case 3: 
//              PrintInt(110, 40, "*", 0);                                  
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->y_pos -= *step;                
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->y_pos += *step;
                    *counter2 = 0;
                } 
            break;        

            case 4:
//              PrintInt(10, 40, "*", 0);            
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->x_pos -= *step;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->x_pos += *step;
                    *counter2 = 0;
                } 
            break;      
            
            case 5: 
//              PrintInt(150, 60, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->z_scaling -= 0.10;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->z_scaling += 0.10;
                    *counter2 = 0;
                } 
            break;                
            
            case 6: 
//              PrintInt(110, 60, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->y_scaling -= 0.10;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->y_scaling += 0.10;
                    *counter2 = 0;
                } 
            break;                
            
            case 7: 
//              PrintInt(60, 60, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->x_scaling -= 0.10;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->x_scaling += 0.10;
                    *counter2 = 0;
                } 
            break;                

            case 8:
//              PrintInt(10, 60, "*", 0);                              
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->x_scaling -= 0.10;
                    ClosestObject->y_scaling -= 0.10;
                    ClosestObject->z_scaling -= 0.10;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->x_scaling += 0.10;
                    ClosestObject->y_scaling += 0.10;
                    ClosestObject->z_scaling += 0.10;
                    *counter2 = 0;
                } 
            break;  
            
            case 9:
//              PrintInt(150, 80, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->x_rotation -= 15 * 180;
                    ClosestObject->x_rotation2 -= 15 * 180;               
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->x_rotation += 15 * 180;
                    ClosestObject->x_rotation2 += 15 * 180;
                    *counter2 = 0;
                } 
            break;                
            
            case 10: 
//              PrintInt(110, 80, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->y_rotation -= 15 * 180;
                    ClosestObject->y_rotation2 -= 15 * 180;
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->y_rotation += 15 * 180;
                    ClosestObject->y_rotation2 += 15 * 180;

                    *counter2 = 0;
                } 
            break;                
            
            case 11: 
//              PrintInt(60, 80, "*", 0);                             
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    ClosestObject->z_rotation -= 15 * 180;
                    ClosestObject->z_rotation2 -= 15 * 180;

                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    ClosestObject->z_rotation += 15 * 180;
                    ClosestObject->z_rotation2 += 15 * 180;

                    *counter2 = 0;
                } 
            break;                
            
            
            case 12:
//              PrintInt(10, 100, "*", 0);            
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                    *obj_model_id -= 1;
                    if (*obj_model_id < 0 ) *obj_model_id = 0;
                    
                    u32 *mid = (void*)(0x8033b4d0 + (*obj_model_id * 4));
                    if (*mid != 0) ClosestObject->geo_layout_ptr = *mid;
                    
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    *obj_model_id += 1;
                    if (*obj_model_id > 0xff ) *obj_model_id = 0xff;

                    u32 *mid = (void*)(0x8033b4d0 + (*obj_model_id * 4));
                    if (*mid != 0) ClosestObject->geo_layout_ptr = *mid;
                    
                    *counter2 = 0;
                } 
                 
                 break;   

            case 13:
//              PrintInt(110, 120, "*", 0);            
                if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 2)    
                {
                                              
                    *function_id -= 1;
                    if (*function_id < 0) *function_id = 0;                    
                    
                    *counter2 = 0;
                } 
                if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)    
                {
                    *function_id += 1;
                    if (*function_id > 15) *function_id = 15;
                    *counter2 = 0;
                } 
                break;

                 
            case 14:
                
//              PrintInt(10, 120, "*", 0);            
                pseudostack = *CurrentObject;
                *CurrentObject = ClosestObject;
                
                /* ensure compatibilty (but limiting many behaviors) */
                tempf[0] = (*Obj)->x_scaling;
                tempf[1] = (*Obj)->y_scaling;
                tempf[2] = (*Obj)->z_scaling;
                tempi[0] = (*Obj)->geo_layout_ptr;
                tempi[1] = (*Obj)->animation_ptr;
                tempi[2] = (*Obj)->collision_ptr;
                
//              function(0);
                
                (*Obj)->x_scaling = tempf[0];
                (*Obj)->y_scaling = tempf[1];
                (*Obj)->z_scaling = tempf[2];                                
                (*Obj)->geo_layout_ptr = tempi[0];
                (*Obj)->animation_ptr = tempi[1];
                (*Obj)->collision_ptr = tempi[2];
                
                    
                *CurrentObject = (void*)pseudostack;
                
                break;
                          
            
            default:
                break;
        }
        
        
        Pad *Controller2 = (void*)0x8033AFAC;
        
        int angle = (int)ClosestObject->y_rotation/180 + 180;
        int dir = 0;
        
        if (angle < 0) angle = 0;
        if (angle > 360) angle = 360;

        PrintInt(20, 120, "80%x", (u32)ClosestObject & 0xFFFFFF);         
        PrintInt(155, 120, "ANG %d", angle); 

        if (angle >= 0 && angle < 90)  { PrintInt(270, 120, "D1", angle);         dir=1;  } // +y
        else if (angle >= 90 && angle < 180) { PrintInt(270, 120, "D2", angle);   dir=2;  } // x
        else if (angle >= 180 && angle < 270) { PrintInt(270, 120, "D3", angle);  dir=3;  } // x
        else if (angle >= 270 && angle <= 360) { PrintInt(270, 120, "D4", angle); dir=4;  } // -y
        

        // rotate object        
        if (Controller2->x != 0  || Controller2->y != 0)
        {
            ClosestObject->y_rotation = _8037a9a8( -(Controller2->y), Controller2->x);
            ClosestObject->y_rotation2 = _8037a9a8(-(Controller2->y), Controller2->x);        
        }
        
        switch (dir)
        {
            case 1:
                ClosestObject->z_pos -= (int)Controller2->y * 0.4f * cosf( angle *PI/180.0);
                ClosestObject->x_pos -= (int)Controller2->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 2:
                ClosestObject->z_pos += (int)Controller2->x * 0.4f * cosf( angle *PI/180.0);
                ClosestObject->x_pos += (int)Controller2->x * 0.4f * sinf( angle *PI/180.0);
                break;

                
            case 3:
                ClosestObject->z_pos += (int)Controller2->y * 0.4f * cosf( angle *PI/180.0);
                ClosestObject->x_pos += (int)Controller2->y * 0.4f * sinf( angle *PI/180.0);
                break;

            case 4:
                ClosestObject->z_pos -= (int)Controller2->x * 0.4f * cosf( angle *PI/180.0);
                ClosestObject->x_pos -= (int)Controller2->x * 0.4f * sinf( angle *PI/180.0);
                break;
                
        }
        
        if (Controller2->currentButton & BUTTON_A)
        {
            ClosestObject->y_pos += 30;
//          ClosestObject->z_pos+=9.0f*cosf(angle*PI/180.0);
//  	    ClosestObject->x_pos+=9.0f*sinf(angle*PI/180.0);
            
        }
        
        *FireCounter = *FireCounter + 1;
        if (Controller2->currentButton & BUTTON_B && *FireCounter > FIRE_RATE)    
        {
            pseudostack = *CurrentObject;
            *CurrentObject = ClosestObject;
            Object *ChildObj = SpawnObj( (*Obj), 0x90, 0x13001184);   /* Mario fireball behavior */
            ChildObj->x_scaling = 3.5;
            ChildObj->y_scaling = 3.5;
            ChildObj->z_scaling = 3.5;
            ChildObj->_0xb8 = 30;
            PlaySound(0x60086001);
            *CurrentObject = (void*)pseudostack;
            *FireCounter = 0;
        }
        
        if (ClosestObject->active == 0 || *obj_behav != ClosestObject->behavior_script_entry) // ClsestObject->graph_node_type != 0x0018 || 
        {
            *list_pointer = 0;
        }
        
        CheckFireCollision(ClosestObject);
        
        if (Controller2->currentButton & BUTTON_Z && *counter2 > 15 ) 
        {
           if (*CameraOnObject == 0 ) *CameraOnObject = 1;
           else if (*CameraOnObject == 1 ) *CameraOnObject = 0;
           
           ClosestObject->animation = Animation2P->TargetAnimationPtr;
           pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */        
           *CurrentObject = ClosestObject;
           SetModel(0x01);
           SetMarioAnimation(Animation2P, *AnimationIndex2P);
           *CurrentObject = pseudostack;  /* Saving because this global variable will be messed up */        
           
           
           *counter2 = 0;
           
        }
        
       PrintInt(20, 20, "Camera", 0);
       if (*CameraOnObject == 1)
       {
           *ObjectDistanceSource = ClosestObject; // calculate distances
           Camera *camera = Mario->camera;     
           camera->x = ClosestObject->x_pos;
           camera->y = ClosestObject->y_pos;
           camera->z = ClosestObject->z_pos;
//           SetMarioAnimation(Animation2P, *AnimationIndex2P);
           PrintInt(110, 20, "2P", 0);
       }
       else
       {
           PrintInt(110, 20, "Mario", 0);
       }
       
       ClosestObject->distance_from_mario = 0;
       ClosestObject->collision_distance = 32000;
       ClosestObject->drawing_distance = 32000;
       
       PrintInt(20, 80, "M MID 80%x", Mario->MarioObj->geo_layout_ptr & 0xFFFFFFF);       
       PrintInt(20, 60, "M AN STRUCT 80%x", (int)Mario->MarioAnimationStruct &0xFFFFFF);
       PrintInt(20, 40, "M ANIM 80%x", Mario->MarioObj->animation & 0xFFFFFFF);

       pseudostack = *CurrentObject;  /* Saving because this global variable will be messed up */        
       *CurrentObject = ClosestObject;
       *CurrentObject = pseudostack;  /* Saving because this global variable will be messed up */        
              
    }
    else
    {
        PrintInt(10, 30, "L and D UP to attach", 0);
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

void MenuPage06()
{
     
    if (*counter == 0)
    {
        *selection = 2;
        *modelid = 0xc0;
        *bparam1 = 0;
        *bparam2 = 0;
        *behavior = 262;
        *distancetomario = 100;
        *counter = 1;
    }    
     
    ProcessSelection(2,7,0);
    
    switch (*selection)
    {
           
        case 2:
            if ( (Mario->pad->currentButton & BUTTON_D_LEFT || Mario->pad->currentButton & BUTTON_D_RIGHT) && *counter2 > 5)    
            {
                Object *ChildObject = SpawnObj( (*Obj), *modelid, Behaviors[*behavior]);
                
                ChildObject->x_pos += *distancetomario;
                ChildObject->z_pos += *distancetomario;                
                ChildObject->y_pos += *distancetomario/2;
                
                ChildObject->behav_param = *bparam1;
                ChildObject->behav_param2 = *bparam2;
                
                *counter2 = 0;
            }
            break;
            
        case 3:
            if ( Mario->pad->currentButton & BUTTON_D_LEFT  && *counter2 > 3)
            {
                
                *modelid -= 1;
                if (*modelid < 1) *modelid = 1;
                
                u32 *model_pointers = (void*)(0x8033B4D0 + (*modelid * 4));
                
                if (*model_pointers == 0) 
                {
                    while (*model_pointers == 0) 
                    {
                       model_pointers++;
                       *modelid -= 1;
                    }
                }
                
                *counter2 = 0;
            }
            
            
            if ( Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)    
            {
                *modelid += 1;
                if (*modelid > 0xff) *modelid = 0xff;
                
                u32 *model_pointers = (void*)(0x8033B4D0 + (*modelid * 4) );
                
                if (*model_pointers == 0)
                {
                   while (*model_pointers == 0)
                   {
                       *modelid += 1;
                       model_pointers++;
                   }
                }
                
                
                *counter2 = 0;
            } 
            break;
            
       case 4: // behavior             
            if ( Mario->pad->currentButton & BUTTON_D_LEFT  && *counter2 > 2)    
            {
                *behavior -= 1;
                if (*behavior < 0) *behavior = 0;
                *counter2 = 0;
            }
            if ( Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 2)
            {
                *behavior += 1;
                if (*behavior > 519) *behavior = 519;
                *counter2 = 0;
            }
            break;
            
       case 5: // b.param2
            if ( Mario->pad->currentButton & BUTTON_D_LEFT  && *counter2 > 3)    
            {
                *bparam2 -= 1;
                if (*bparam2 < 0) *bparam2 = 0;
                *counter2 = 0;
            }
            if ( Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)
            {
                *bparam2 += 1;
                *counter2 = 0;
            }
            break;
            
       case 6: // b.param1
            if ( Mario->pad->currentButton & BUTTON_D_LEFT  && *counter2 > 3)    
            {
                *bparam1 -= 1;
                if (*bparam1 < 0) *bparam1 = 0;
                *counter2 = 0;
            }
            if ( Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)
            {
                *bparam1 += 1;
                *counter2 = 0;
            }
            break;
            
            
       case 7: // distance             
            if ( Mario->pad->currentButton & BUTTON_D_LEFT  && *counter2 > 3)    
            {
                *distancetomario -= 30;
                *counter2 = 0;
            }
            if ( Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)    
            {
                *distancetomario += 30;
                *counter2 = 0;
            }
            break;
            
       default:
            break;
       
     }
     
    int *objcount = (void*)0x8033D270;
    if (*objcount > 120) PrintInt(25, 160, "warning: too many objs", 0);

    PrintInt(10, (*selection * 20), "*", 0);
    PrintInt(25,140, "DIST %d", *distancetomario); 
    PrintInt(25,120, "B PARAM1 %d", *bparam1);    
    PrintInt(25,100, "B PARAM2 %d", *bparam2);
    PrintInt(25,80, "BHV %x", Behaviors[*behavior] & 0xFFFF);
    PrintInt(25,60, "GFX %x", *modelid);
    PrintInt(25,40, "SPAWN", 0);    

}

void MenuPage07()
{

    Camera *camera = Mario->camera;     
    /*(0x8033C520) */
    
    /* look what's in (8033C698) */    
    
    /* 0x8133c5a4  */

    if (*counter == 0)
    {
        *selection = 2;
        *counter = 1;
    }    

    ProcessSelection (1, 7, 0);
    PrintInt(10, (*selection * 20), "*", 0);


    float *camera22 = (void*)0x8033C698;
    PrintInt(25, 200, "98 %d", (int)*camera22);
    camera22++;
    PrintInt(25, 180, "9c %d", (int)*camera22);
    camera22++;
    PrintInt(25, 160, "a0 %d", (int)*camera22);
    camera22++;
    PrintInt(150, 200, "a4 %d", (int)*camera22);
    camera22++;
    PrintInt(150, 180, "a8 %d", (int)*camera22);
    camera22++;
    PrintInt(150, 160, "ac %d", (int)*camera22);
    
    
    
    PrintInt(25, 140, "DIST %d", (int)*distance); //cameradistance);    
    if (*freeze == 0) PrintInt(25, 120, "FREEZE", 0);    
    else  PrintInt(25, 120, "UNFREEZE", 0);        
    PrintInt(25, 100, "SET PRESET %d", *camerapreset);    
    PrintInt(25, 80, "ROT %d", (int)camera->rotation/180);        
    PrintInt(25, 60, "X %d", (int)camera->x);
    PrintInt(25, 40, "Y %d", (int)camera->y);
    PrintInt(25, 20, "Z %d", (int)camera->z);
    
    // camera distance, where?
    
    switch (*selection)
    {
           
        case 1: 
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                *camera_z -= 15;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                *camera_z += 15;
                *counter2 = 0;
            }
        break;
        
        case 2: 
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                *camera_y -= 15;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                *camera_y += 15;
                *counter2 = 0;
            }
        break;
        
        case 3: 
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                *camera_x -= 15;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                *camera_x += 15;
                *counter2 = 0;
            }
        break;
        
        case 5:
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                
                *camerapreset -= 1;
                if (*camerapreset < 0 ) *camerapreset = 0;
                                          
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                 *camerapreset += 1;
                 *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_L && *counter2 > 3)        
            {
                 camera->camera_setting = *camerapreset;
                 *counter2 = 0;
            }
            
        break;
        
        case 6:
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 3)    
            {
                if (*freeze == 1) 
                {
                    *freeze = 0;
                    Mario->camera = 0x8033c520;
                }
                
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 3)        
            {
                if (*freeze == 0)
                {
                    *freeze = 1;
                    Mario->camera = 0x807c0000;                    
                }
                *counter2 = 0;
            }
        break;

        case 7:
            if (Mario->pad->currentButton & BUTTON_D_LEFT && *counter2 > 12)    
            {
                *cameradistance -= 0.3;
                *counter2 = 0;
            }
            if (Mario->pad->currentButton & BUTTON_D_RIGHT && *counter2 > 12)        
            {
                *cameradistance += 0.3;
                *counter2 = 0;
            }
        break;
        
        }
    
}


void MenuPage09()
{

    Pad *Controller = (void*)0x8033AFAC;

    PrintInt(15,160,  "Controller2 status", 0);
    PrintInt(15,180,  "ANGLE %d", _8037a9a8( -(Controller->y), Controller->x)/180 );
    PrintInt(15,140,  "old %d", (int)Controller->previousButton);    
    PrintInt(15,120,  "but %d", (int)Controller->currentButton);
    PrintInt(15,100,  "z %d", (int)Controller->z);    
    PrintInt(15,80,   "y %d", (int)Controller->y);
    PrintInt(15,60,   "x %d", (int)Controller->x);    
    PrintInt(15,40,   "stick_y %x", Controller->stick_y);    
    PrintInt(15,20,   "stick_x %x", Controller->stick_x);
 
 
}
void MenuPage08()
{

    int i;
    
    if (*counter == 0)
    {
        for (i = 1; i <= 0x17; i++)
        {
            segment[i] = (void*)(0x8060b000 + (i*4) );
            *segment[i] = SegmentedToVirtual(i * 0x01000000) & 0xFFFFFF;
        }
        
        *last_segment7 = *segment[7];
        
        *counter = 1;
    }    
    
    *segment[7] = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF;
    
    if (*segment[7] |= *last_segment7) *counter = 0;


    PrintInt(15,220, "S1 %06x", *segment[1]);
    PrintInt(15,200, "S2 %06x", *segment[2]);
    PrintInt(15,180, "S3 %06x", *segment[3]);
    PrintInt(15,160, "S4 %06x", *segment[4]);
    PrintInt(15,140, "S5 %06x", *segment[5]);
    PrintInt(15,120, "S6 %06x", *segment[6]);
    PrintInt(15,100, "S7 %06x", *segment[7]);
    PrintInt(15,80,  "S8 %06x", *segment[8]);
    PrintInt(15,60,  "S9 %06x", *segment[9]);
    PrintInt(15,40,  "SA %06x", *segment[0xa]);
    PrintInt(15,20,  "SB %06x", *segment[0xb]);
    
    PrintInt(145,220, "SgC %06x", *segment[0xc]);
    PrintInt(145,200, "SgD %06x", *segment[0xd]);
    PrintInt(145,180, "SgE %06x", *segment[0xe]);
    PrintInt(145,160, "SgF %06x", *segment[0xf]);
    PrintInt(145,140, "S10 %06x", *segment[0x10]);
    PrintInt(145,120, "S11 %06x", *segment[0x11]);
    PrintInt(145,100, "S12 %06x", *segment[0x12]);
    PrintInt(145,80,  "S13 %06x", *segment[0x13]);
    PrintInt(145,60,  "S14 %06x", *segment[0x14]);
    PrintInt(145,40,  "S15 %06x", *segment[0x15]);
    PrintInt(145,20,  "S16 %06x", *segment[0x16]);

}



void PongOfDeath ( void )
{

    u16 *CurrentLevel     = (void*)0x8032ddf8;
    u8 *pad               = (void*)0x8033AFA0;    
    u16 *score_1           = (void*)0x80604000;
    u16 *score_2           = (void*)0x80604002;
    u32 *other_counter     = (void*)0x80604004;
    u32 *y_pos_1           = (void*)0x8060400C;
    u32 *y_pos_2           = (void*)0x80604010;
    u32 *set_pos           = (void*)0x80604014;
    u32 *ball_x_pos        = (void*)0x80604018;
    u32 *ball_y_pos        = (void*)0x8060401C;
    u32 *ball_direction_x  = (void*)0x80604020;
    u32 *ball_direction_y  = (void*)0x80604024;
    u32 *ball_speed        = (void*)0x80604028;
    u8 *run_once           = (void*)0x8060402C;
    u8 *next_level         = (void*)0x8060402D;
    u8 *level              = (void*)0x8060402E;
    u8 *pad_speed          = (void*)0x8060402F;
    u32 *count_before_start = (void*)0x80604030;  
    u16 *display_stats = (void*)0x8033b26a;
    u32 *restart_counter    = (void*)0x80604034;
    u32 *first_run          = (void*)0x80604038;
    u32 *crazy_shot         = (void*)0x8060403C;
    

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
            Mario->power = 0;            
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
        
    if ( Mario->pad->currentButton & BUTTON_D_UP)
    {
        if ( ! (*y_pos_1 >= MAX_Y_POS) ) *y_pos_1 += ( 3 + *pad_speed );
    }
    if ( Mario->pad->currentButton & BUTTON_D_DOWN)
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
