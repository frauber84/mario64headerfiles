/* ========================================================================
 *
 * n64ld.x
 *
 * GNU Linker script for building an image that is set up for the N64
 * but still has the data factored into sections.  It is not directly
 * runnable, and it contains the debug info if available.  It will need
 * a 'loader' to perform the final stage of transformation to produce
 * a raw image.
 *
 * Copyright (c) 1999 Ground Zero Development, All rights reserved.
 * Developed by Frank Somers <frank@g0dev.com>
 * Modifications by hcs (halleyscometsoftware@hotmail.com)
 *
 * $Header: /cvsroot/n64dev/n64dev/lib/alt-libn64/n64ld.x,v 1.2 2006/08/11 15:54:11 halleyscometsw Exp $
 *
 * ========================================================================
 */

OUTPUT_FORMAT ("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH (mips)
EXTERN (_start)
ENTRY (_start)

SECTIONS {
   /* Start address of code is 1K up in uncached, unmapped RAM.  We have
    * to be at least this far up in order to not interfere with the cart
    * boot code which is copying it down from the cart
    */

   . = 0x80400000 ;

   sprintf = 0x8032255c ;
   CreateMessageBox = 0x802A4BE4 ;
   CreateTextBox = 0x802d8d08 ;
   CreateStar = 0x802F2B88 ;
   CheckObjBehavior = 0x802a14fc;
   CheckObjBehavior2 = 0x802A1554;
   SetObjBehavior = 0x802a14c4;
   CopyObjParams = 0x8029F0E0 ;
   CopyObjPosition = 0x8029F120 ;
   CopyObjRotation = 0x8029F148 ;
   CopyObjScaling = 0x8029F3A4 ;
   DistanceFromObject = 0x8029e27c ;
   DeactivateObject = 0x802a0568 ; 
   DmaCopy = 0x80278504 ;
   ExplodeObject = 0x802e6af8 ; 
   GetSegmentBase = 0x80277f20 ;
   HideObject = 0x8029F620 ;
   UnHideObject = 0x8029f6bc ;
   PlaySound = 0x802ca190;
   PrintInt = 0x802d62d8 ;
   PrintXY = 0x802d66c0 ; 
   RotateTorwardsMario = 0x8029e530 ;
   ScaleObject = 0x8029F430;
   ScaleXYZ =  0x8029F3D0;
   SegmentedToVirtual = 0x80277f50;
   SetModel = 0x802a04c0;
   SetSegmentBase = 0x80277ee0 ;
   SetObjAnimation = 0x8029f464;
   ShakeScreen = 0x802a50fc ;
   SpawnObj = 0x8029edcc ;

   SetMarioAction = 0x80252cf4;
   SetMarioAnimation = 0x80279084;

   IsMarioStepping = 0x802A3CFC ;

   DynamicIndexCopy = 0x8027868c ;
  
   ProcessGeoLayout = 0x8037e0b4;
 

   MoveRelated = 0x802a2320 ;

   PreMoveObj = 0x802a1308;
   MoveObj = 0x802A2348 ;


   MoveObj2 = 0x802A2644 ;
   UnknownMove = 0x8029E714 ;

   _8037a9a8 = 0x8037a9a8;


   StopMario = 0x8028bd34 ;
   PrintText = 0x802d8844;
   PrintRegularText = 0x802d7e88;

   sinf = 0x80325480;
   cosf = 0x80325310;
   sqrtf = 0x80323a50;

   ProcessCollision = 0x803839cc;

   SetMusic = 0x80320544;
   SetInstrument = 0x8031cfd4;

   /* The text section carries the app code and its relocation addr is
    * the first byte of the cart domain in cached, unmapped memory
    */

   .text : {
      FILL (0)

      __text_start = . ;
      *(.init)
      *(.text)
      *(.ctors)
      *(.dtors)
      *(.rodata)
      *(.fini)
      __text_end  = . ;
   }


   /* Data section has relocation address at start of RAM in cached,
    * unmapped memory, but is loaded just at the end of the text segment,
    * and must be copied to the correct location at startup
    */

   .data : {
      /* Gather all initialised data together.  The memory layout
       * will place the global initialised data at the lowest addrs.
       * The lit8, lit4, sdata and sbss sections have to be placed
       * together in that order from low to high addrs with the _gp symbol 
       * positioned (aligned) at the start of the sdata section.
       * We then finish off with the standard bss section
       */

      FILL (0xaa)

      __data_start = . ;
         *(.data)
         *(.lit8)
         *(.lit4) ;
     /* _gp = ALIGN(16) + 0x7ff0 ;*/
/*	 _gp = . + 0x7ff0; */
	 . = ALIGN(16);
	 _gp = . ;
         *(.sdata)
      __data_end = . ;
/*
      __bss_start = . ;
         *(.scommon)
         *(.sbss)
         *(COMMON)
         *(.bss)
      /* XXX Force 8-byte end alignment and update startup code */

      __bss_end = . ;
*/
   }
   
   .bss (NOLOAD) :  {
       	__bss_start = . ;
       	*(.scommon)
	*(.sbss)
	*(COMMON)
	*(.bss)
	__bss_end = . ;
	end = . ;
   }

}
