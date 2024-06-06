/*
// TI File $Revision: /main/3 $
// Checkin $Date: April 23, 2007   09:09:27 $
//###########################################################################
//
// FILE:    F2812_EzDSP_RAM_lnk.cmd
//
// TITLE:   Linker Command File For F2812 eZdsp examples that run out of RAM
//          This linker file assumes the user is booting up in Jump to H0 mode
//
//###########################################################################
// $TI Release: $
// $Release Date: $
//###########################################################################
*/

/* ======================================================
// For Code Composer Studio V2.2 and later
// ---------------------------------------
// In addition to this memory linker command file, 
// add the header linker command file directly to the project. 
// The header linker command file is required to link the
// peripheral structures to the proper locations within 
// the memory map.
//
// The header linker files are found in <base>\DSP281x_Headers\cmd
//   
// For BIOS applications add:      DSP281x_Headers_BIOS.cmd
// For nonBIOS applications add:   DSP281x_Headers_nonBIOS.cmd    
========================================================= */

/* ======================================================
// For Code Composer Studio prior to V2.2
// --------------------------------------
// 1) Use one of the following -l statements to include the 
// header linker command file in the project. The header linker
// file is required to link the peripheral structures to the proper 
// locations within the memory map                                    */

/* Uncomment this line to include file only for non-BIOS applications */
/* -l DSP281x_Headers_nonBIOS.cmd */

/* Uncomment this line to include file only for BIOS applications */
/* -l DSP281x_Headers_BIOS.cmd */

/* 2) In your project add the path to <base>\DSP281x_headers\cmd to the
   library search path under project->build options, linker tab, 
   library search path (-i).
/*========================================================= */



MEMORY
{
PAGE 0 :
   /* For this example, H0 is split between PAGE 0 and PAGE 1 */  
   /* BEGIN is used for the "boot to HO" bootloader mode      */
   /* RESET is loaded with the reset vector only if           */
   /* the boot is from XINTF Zone 7.  Otherwise reset vector  */
   /* is fetched from boot ROM. See .reset section below      */
   
   RAMM0      : origin = 0x000000, length = 0x000800 /* 0x000400 */
   BEGIN      : origin = 0x3F8000, length = 0x000002
   RAML0	 : origin = 0x008000, length = 0x002000
   RAMH0     : origin = 0x3F8002, length = 0x001FFE /* PRAMH0 0x000FFE */
   RESET      : origin = 0x3FFFC0, length = 0x000002           

         
PAGE 1 : 

   /* For this example, H0 is split between PAGE 0 and PAGE 1 */

   /* RAMM1    : origin = 0x000400, length = 0x000400 */
   /* DRAMH0   : origin = 0x3f9000, length = 0x001000 */
}
 
 
SECTIONS
{
   /* Setup for "boot to H0" mode: 
      The codestart section (found in DSP28_CodeStartBranch.asm)
      re-directs execution to the start of user code.  
      Place this section at the start of H0  */

   codestart        : > BEGIN,       PAGE = 0
   ramfuncs         : > RAMH0       PAGE = 0 /* PRAMH0 */
   .text            : > RAMH0,      PAGE = 0 /* PRAMH0 */
   .cinit           : > RAMH0,      PAGE = 0 /* PRAMH0 */
   .pinit           : > RAMH0,      PAGE = 0 /* PRAMH0 */
   .switch          : > RAMM0,       PAGE = 0
   .reset           : > RESET,       PAGE = 0, TYPE = DSECT /* not used, */
   
   ramfuncs         : LOAD = FLASHD,
                      RUN = RAMH0,
                      LOAD_START(_RamfuncsLoadStart),
                      LOAD_END(_RamfuncsLoadEnd),
                      RUN_START(_RamfuncsRunStart),
                      PAGE = 0

   .stack           : > RAML0,       PAGE = 0 /* RAMM1 */
   .ebss            : > RAML0,      PAGE = 0
   .econst          : > RAMM0,      PAGE = 0 /* DRAMH0 */
   .esysmem         : > RAMM0,      PAGE = 0 /* DRAMH0 */

     
}
