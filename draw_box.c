#include <stdio.h>
#include "mips.h"


typedef void (*F1)
(
    u32,
    u32,
    u32,
    u32
);
typedef void (*F2)
(
    u32,
    u32,
    u32,
    u32
);

struct data_t
{
    u64 * a;
    u64 * b;
    u64 * c;
};

static F1 f1 = (F1)0x802D7070;
static F2 f2 = (F2)0x802D7280;

static u32 
args1[] =
{
    1,
    0,          /* X: 0   */
    0x43700000, /* Y: 240 */
    0
},
args2[] =
{
    2,
    0x40266666, /* W: 2.59 */
    0x4059999A, /* H: 3.40 */
    0x3F800000  /* ?: 1.00 */
};

static struct data_t * data = (struct data_t*)0x8033B06C;


void
makeSquare ( float x, float y, float w, float h, u8 r, u8 g, u8 b, u8 a )
{
    u32 * d, * ix, * iy, * iw, * ih;
    float fx, fy, fw, fh;
    const float w_max = 2.525f;
    const float h_max = 3.325f;
    
    fx = (float)x;
    fy = (float)y;
    ix = (u32*)&fx;
    iy = (u32*)&fy;
    
    fw = (w_max * (float)w) / 320.0f;
    fh = (h_max * (float)h) / 240.0f;
    iw = (u32*)&fw;
    ih = (u32*)&fh;
    
    /* Call functions */
    f1( args1[0], *ix, *iy, args1[3] );
    f2( args2[0], *iw, *ih, args2[3] );
    
    /* Manipulate memory */
    d = (u32*)&(data->a++)[0];               
    d[0] = 0xFB000000;
    d[1] = (r << 24) | (g << 16) | (b << 8) | (a);
    d = (u32*)&(data->a++)[0];   
    d[0] = 0x06000000;
    d[1] = 0x02010000 + 7240;
    d = (u32*)&(data->a++)[0];
    d[0] = 0xBD000000;
    d[1] = 0x00000000;
}
