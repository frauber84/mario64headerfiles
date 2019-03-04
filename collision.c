    struct object       **Obj                 = (void*)M64_CURR_OBJ_PTR;
 

#define COLLISION_TABLE 0x80410000

void _start ( void )
{

 
    u16 i = 0;
    u16 ModelID = 0;
    u32 *pointer_table = (void*)COLLISION_TABLE + (Level * TABLE_LENGHT);
    u32 *table_value;
  
    
    /* 0x8033DDC4 = model ID geo layout table */ 

    for (i = 0; i < 0x100; i++ )
    {
 
        u32 *model_layout_ptr = 0x8033DC44 + (i * 4) ;

        if ( (*Obj)->geo_layout_ptr  == *model_layout_ptr )
        {
            ModelID = i;
            break;
        }
    }

    table_value = pointer_table + (ModelID * 8);

    if (*table_table != 0)
    {
        (*Obj)->collision_ptr = SegmentedToVirtual(*table_value);
    }

    table_value++; // increment a word to the pointer (collision distance)

    if (*table_table != 0)
    {
        (*Obj)->collision_distance = *table_value; // check if cast/conversion is needed
    }
   


}