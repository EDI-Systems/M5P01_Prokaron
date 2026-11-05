/******************************************************************************
Filename    : rmp_stubs.c
Author      : pry
Date        : 03/11/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The two external symbols that Nuvoton's stock M2351 startup
              references but does not define. They exist so that the vendor
              startup file can be used verbatim (this port does not ship a
              custom startup).
              - ProcessHardFault: HardFault_Handler in startup_M2351.S does
                "MOV R0,LR / MRS R1,MSP / MRS R2,PSP / BLX ProcessHardFault"
                and then "BX R0", so the prototype must match Nuvoton's.
              - SH_Return: called from SH_DoCommand, the semihosting
                trampoline. This project never uses semihosting, so it only
                has to report "no debugger attached".
              The BSP defines both in StdDriver/src/retarget.c, but that is a
              944-line semihosting/printf unit; this file is the minimal
              replacement for a project that links nosys.
******************************************************************************/

/* Include *******************************************************************/
#include <stdint.h>
/* End Include ***************************************************************/

/* Function:ProcessHardFault **************************************************
Description : The hard fault entry of the Nuvoton startup. The Baseline profile
              has no separate MemManage/BusFault/UsageFault exceptions, so
              every fault lands here. Park the core; a debugger will show the
              PC inside this function, and the stacked frame is reachable from
              the MSP/PSP that the caller passed in.
Input       : uint32_t LR - The EXC_RETURN value at the fault.
              uint32_t MSP - The main stack pointer at the fault.
              uint32_t PSP - The process stack pointer at the fault.
Output      : None.
Return      : None - this function does not return.
******************************************************************************/
uint32_t ProcessHardFault(uint32_t LR, uint32_t MSP, uint32_t PSP)
{
    (void)LR;
    (void)MSP;
    (void)PSP;

    while(1)
    {
        /* Park forever */
    }
}
/* End Function:ProcessHardFault *********************************************/

/* Function:SH_Return *********************************************************
Description : The semihosting return trampoline. Returning zero means "no ICE
              debugger connected", which is always true for this project.
Input       : int32_t R0 - Semihosting R0.
              int32_t R1 - Semihosting R1.
              int32_t* Out - Semihosting output pointer.
Output      : None.
Return      : int32_t - Always 0.
******************************************************************************/
int32_t SH_Return(int32_t R0, int32_t R1, int32_t* Out)
{
    (void)R0;
    (void)R1;
    (void)Out;

    return 0;
}
/* End Function:SH_Return ****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
