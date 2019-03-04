#include <n64.h>
#include <explode.h>

    u32               *CurrentObject        = (void*)M64_CURR_OBJ_PTR;
    Object            **Obj                 = (void*)M64_CURR_OBJ_PTR;
    
void _start ( void )
{
    asm volatile("la $gp, _gp");
    
    u32 *water = (void*)0x80361184;
    s16 *waterlevel = (void*)*water + 0x0c;
    u32 *count = (void*)0x807efff4;
    *count +=1;
    
    
    if (*count > 501)
    {
        *count = 0;
    }
    
    if (*count < 250)
    {
        *waterlevel -= 10;
    }
    else if (*count > 250)
    {
        *waterlevel += 10;
    }

    
    int i, stack;
    u16 *Status = (void*)0x80660000;
    u32 *BoxCount = (void*)0x80660004;
    u32 *CoinCount = (void*)0x80660008;
    static int DoStuff = 0;
        
    stack = *CurrentObject;
    *BoxCount = 0;
    *CoinCount = 0;
    
    int BoxSwitch = SegmentedToVirtual(0x08012D70);
    int CoinBehavior = SegmentedToVirtual(0x1300091c);
    
    if ( (*Obj)->action == 2 && *Status == 0) *Status = 1;  // change to coin
    if ( (*Obj)->action == 0 && *Status == 2) *Status = 3;  // change back to breakable box
    
//    PrintInt(40, 40, "Status %d", *Status);
//    PrintInt(40, 20, "Action %d", (*Obj)->action);
    
    if ( *Status == 1 || *Status == 3)           
    {
        *CurrentObject = (void*)M64_FIRST_OBJ_STRUCT;
        for (i = 0; i < 240; i++)
        {          
            if ( (*Obj)->collision_ptr == BoxSwitch && (*Obj)->_0x254 != 666)
            {
                DoStuff = 1;
                *BoxCount = *BoxCount + 1;
                u32 *BoxPtr = (void*)0x80670000 + (*BoxCount * 4);
                *BoxPtr = (*Obj);
                (*Obj)->_0x254 = 666;
             }
             else if( (*Obj)->behavior_script_entry == CoinBehavior && (*Obj)->_0x254 != 666)
             {
                 
                DoStuff = 1;
                *CoinCount = *CoinCount + 1;
                u32 *CoinPtr = (void*)0x80680000 + (*CoinCount * 4);
                *CoinPtr = (*Obj);
                (*Obj)->_0x254 = 666;
             }
             
            *CurrentObject = (*Obj)->next;
         }
        if ( *Status == 1 ) *Status = 2;
        else if ( *Status == 3 ) *Status = 0;
    }
    
    if (DoStuff == 1) 
    {    
         DoStuff = 0;
         
         for (i = 0; i < *BoxCount; i++)
         {
             u32 *ObjPtr = (void*)0x80670000 + ( (i+1) * 4);
             Object *TempObj = *ObjPtr;
         
             float x = TempObj->x_pos;
             float y = TempObj->y_pos;
             float z = TempObj->z_pos;
             Object *ChildObj = SpawnObj( (*Obj), 0x74, 0x1300091c);  
             
             TempObj->active = 0;
             TempObj->graph_flags = 0;

                 
             ChildObj->x_pos = x;
             ChildObj->y_pos = y;
             ChildObj->z_pos = z;
             ChildObj->_0x254 = 0;
             
         }
         
         for (i = 0; i < *CoinCount; i++)
         {
             u32 *ObjPtr = (void*)0x80680000 + ( (i+1) * 4);
             Object *TempObj = *ObjPtr;
         
             float x = TempObj->x_pos;
             float y = TempObj->y_pos;
             float z = TempObj->z_pos;
             Object *ChildObj = SpawnObj( (*Obj), 0x81, 0x130014E0); 
             
             TempObj->active = 0;
             TempObj->graph_flags = 0;

                 
             ChildObj->x_pos = x;
             ChildObj->y_pos = y;
             ChildObj->z_pos = z;
             ChildObj->_0x254 = 0;
             
         }
         
    }
        
    

    static int BoxCount2 = 0;
    static int CoinCount2 = 0;
    
    if (*BoxCount != 0) BoxCount2 = *BoxCount;
    if (*CoinCount != 0) CoinCount2 = *CoinCount;

    PrintInt(20, 20, "BoxCount %d", BoxCount2);
    PrintInt(20, 40, "CoinCount %d", CoinCount2);
    
    *BoxCount = 0;
    *CoinCount = 0;
    
    for (i = 0; i < 255; i++)
    {
        u32 *Ptr1 = (void*)0x80670000 + (i * 4);
        *Ptr1 = 0;
        u32 *Ptr2 = (void*)0x80680000 + (i * 4);
        *Ptr2 = 0;
    }
    
    
    #define PI 3.14159
    
    int *nope_rotation = (void*)0x802E77C0;
    int *nope2 = (void*)0x802E77D0;
    
    *nope_rotation = 0;
    *nope2 = 0;
    
    static int random = 0;
    random += 3;
    
    if (*waterlevel % 10 + 1 > 6) random+= 1;
    
    
    int PinkBobOmb = SegmentedToVirtual(0x130031DC);
    for (i = 0; i < 240; i++)
    {          
       if( (*Obj)->behavior_script_entry == PinkBobOmb && (*Obj)->action != 2)
       {
           
           (*Obj)->y_rotation += 180;
           
           int angle = (int)(*Obj)->y_rotation/180 + 180;
           
           (*Obj)->z_pos -= 15 * 0.3f * cosf( angle *PI/180.0);
           (*Obj)->x_pos -= 15 * 0.3f * sinf( angle *PI/180.0);
           
//           PrintInt(20, 100, "BobAction %d", (*Obj)->action);
//           PrintInt(20, 120, "BobAngle  %x", (*Obj)->y_rotation);

       }
      *CurrentObject = (*Obj)->next;
   }





    *CurrentObject = (void*)stack;   
 
}
