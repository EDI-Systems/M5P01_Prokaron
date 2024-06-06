/*
// TI File $Revision: /main/4 $
// Checkin $Date: July 9, 2009   17:39:29 $
//###########################################################################
//
// FILE:    DSP281x_Headers_nonBIOS.cmd
//
// TITLE:   DSP281x Peripheral registers linker command file 
//
// DESCRIPTION: 
// 
//          This file is for use in Non-BIOS applications.
//
//          Linker command file to place the peripheral structures 
//          used within the DSP28 headerfiles into the correct memory
//          mapped locations.
//
//          This version of the file includes the PieVectorTable structure.
//          For BIOS applications, please use the DSP281x_Headers_BIOS.cmd file
//          which does not include the PieVectorTable structure.
//
//###########################################################################
// $TI Release: DSP281x C/C++ Header Files V1.20 $
// $Release Date: July 27, 2009 $
//###########################################################################
*/

MEMORY
{
 PAGE 0:    /* Program Memory */

 PAGE 1:    /* Data Memory */
 
   DEV_EMU     : origin = 0x000880, length = 0x000180     /* device emulation registers */
   PIE_VECT    : origin = 0x000D00, length = 0x000100     /* PIE Vector Table */
   FLASH_REGS  : origin = 0x000A80, length = 0x000060     /* FLASH registers */
   CSM         : origin = 0x000AE0, length = 0x000010     /* code security module registers */
   XINTF       : origin = 0x000B20, length = 0x000020     /* external interface registers */
   CPU_TIMER0  : origin = 0x000C00, length = 0x000008     /* CPU Timer0 registers */
   CPU_TIMER1  : origin = 0x000C08, length = 0x000008     /* CPU Timer1 registers */
   CPU_TIMER2  : origin = 0x000C10, length = 0x000008     /* CPU Timer2 registers */
   PIE_CTRL    : origin = 0x000CE0, length = 0x000020     /* PIE control registers */
   ECANA       : origin = 0x006000, length = 0x000040     /* eCAN control and status registers */ 
   ECANA_LAM   : origin = 0x006040, length = 0x000040     /* eCAN local acceptance masks */
   ECANA_MOTS  : origin = 0x006080, length = 0x000040     /* eCAN message object time stamps */
   ECANA_MOTO  : origin = 0x0060C0, length = 0x000040     /* eCAN object time-out registers */
   ECANA_MBOX  : origin = 0x006100, length = 0x000100     /* eCAN mailboxes */
   SYSTEM      : origin = 0x007010, length = 0x000020     /* System control registers */
   SPIA        : origin = 0x007040, length = 0x000010     /* SPI registers */
   SCIA        : origin = 0x007050, length = 0x000010     /* SCI-A registers */
   XINTRUPT    : origin = 0x007070, length = 0x000010     /* external interrupt registers */
   GPIOMUX     : origin = 0x0070C0, length = 0x000020     /* GPIO mux registers */
   GPIODAT     : origin = 0x0070E0, length = 0x000020     /* GPIO data registers */
   ADC         : origin = 0x007100, length = 0x000020     /* ADC registers */
   EVA         : origin = 0x007400, length = 0x000040     /* Event Manager A registers */
   EVB         : origin = 0x007500, length = 0x000040     /* Event Manager B registers */
   SCIB        : origin = 0x007750, length = 0x000010     /* SCI-B registers */
   MCBSPA      : origin = 0x007800, length = 0x000040     /* McBSP registers */
   CSM_PWL     : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations. */
}

 
SECTIONS
{
   PieVectTableFile : > PIE_VECT,   PAGE = 1

/*** Peripheral Frame 0 Register Structures ***/
   DevEmuRegsFile    : > DEV_EMU,     PAGE = 1
   FlashRegsFile     : > FLASH_REGS,  PAGE = 1
   CsmRegsFile       : > CSM,         PAGE = 1
   XintfRegsFile     : > XINTF,       PAGE = 1
   CpuTimer0RegsFile : > CPU_TIMER0,  PAGE = 1 
   CpuTimer1RegsFile : > CPU_TIMER1,  PAGE = 1 
   CpuTimer2RegsFile : > CPU_TIMER2,  PAGE = 1  
   PieCtrlRegsFile   : > PIE_CTRL,    PAGE = 1      

/*** Peripheral Frame 1 Register Structures ***/
   ECanaRegsFile     : > ECANA,       PAGE = 1
   ECanaLAMRegsFile  : > ECANA_LAM    PAGE = 1   
   ECanaMboxesFile   : > ECANA_MBOX   PAGE = 1
   ECanaMOTSRegsFile : > ECANA_MOTS   PAGE = 1
   ECanaMOTORegsFile : > ECANA_MOTO   PAGE = 1

/*** Peripheral Frame 2 Register Structures ***/
   SysCtrlRegsFile   : > SYSTEM,      PAGE = 1
   SpiaRegsFile      : > SPIA,        PAGE = 1
   SciaRegsFile      : > SCIA,        PAGE = 1
   XIntruptRegsFile  : > XINTRUPT,    PAGE = 1
   GpioMuxRegsFile   : > GPIOMUX,     PAGE = 1
   GpioDataRegsFile  : > GPIODAT      PAGE = 1
   AdcRegsFile       : > ADC,         PAGE = 1
   EvaRegsFile       : > EVA,         PAGE = 1
   EvbRegsFile       : > EVB,         PAGE = 1
   ScibRegsFile      : > SCIB,        PAGE = 1
   McbspaRegsFile    : > MCBSPA,      PAGE = 1
   
/*** Code Security Module Register Structures ***/
   CsmPwlFile        : > CSM_PWL,     PAGE = 1
}


/******************* end of file ************************/
