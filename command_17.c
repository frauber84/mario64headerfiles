#include <n64.h>
#include <explode.h>

typedef struct cmd17
{
   u8 command;
   u8 lenght;
   u8 extend_flag;
   u8 segment;
   u32 start_address;
   u32 end_address;
} LevelCommand17;

void _start ( void )
{
    asm volatile("la $gp, _gp");
    LevelCommand17 **Cmd = (void*)0x8038BE28;
    u32 *script_pointer = (void*)0x8038BE28;

    if ( (*Cmd)->extend_flag == 1 )
    {
        u32 *segment = (void*)0x8033b400 + ((*Cmd)->segment * 4);

        switch( (*Cmd)->segment)
        {
            case 0x0E:
                DmaCopy(0x80420000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x00420000;
                break;

            /*
            case 0x07:
                DmaCopy(0x80520000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x00520000;
                break;
                
           case 0x06:
                DmaCopy(0x80550000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x00550000;
                break;
     
                
            */

            case 0x05:
                DmaCopy(0x80520000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x00520000;
                break;

            
              //805b5000 (next free space)

            case 0x013:
                DmaCopy(0x807b0000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x007b0000;
                break;
                
            case 0x0A:
                DmaCopy(0x807c0000, (*Cmd)->start_address , (*Cmd)->end_address);
    	        *segment = 0x007c0000;
                break;
                
        }

    }
    else
    {
    	DynamicIndexCopy( (*Cmd)->segment, (*Cmd)->start_address , (*Cmd)->end_address , 0);

    }
    *script_pointer = *script_pointer + (*Cmd)->lenght;

}
