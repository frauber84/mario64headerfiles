#include <n64.h>
#include "explode.h"


//
void _start ( void )
{

    asm volatile("la $gp, _gp");
   
 
//   int (*function)(int, int, int, int) = (void*)0x8029053C;
    int (*function2)(int) = (void*)0x80290d90;
   
   int *LevelCamera = (void*)0x8033cbd0;
   
//   function(*LevelCamera, 0xE000, 0x2000, 0xE000);

   function2(*LevelCamera);

}
