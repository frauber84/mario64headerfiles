#include <n64.h>
#include "explode.h"
#define DEBUG 0

int *INIT = (void*)0x8060005c;
int *last_segment7 = (void*)0x80600060;

void _start ( void )
{
    asm volatile("la $gp, _gp");

    if (*INIT == 0)
    {
        DmaCopy(0x80700000, 0x7f4000, 0x7fdd98); 
        u32 *SegmentB = (void*)0x8033b42c;
        
        *SegmentB = 0x00700000;
        
        int segment7 = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF;
        *last_segment7 = segment7;

        *INIT = 1;

    }
    
    int segment_7 = SegmentedToVirtual(7 * 0x01000000) & 0xFFFFFF;
    if (segment_7 != *last_segment7) *INIT = 0;
    
    *last_segment7 = segment_7;
    
}
