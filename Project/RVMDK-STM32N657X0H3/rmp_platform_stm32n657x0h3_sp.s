/*****************************************************************************
Filename    : rmp_platform_stm32n657x0h3_sp.s
Author      : pry
Date        : 02/11/2026
Licence     : The Unlicense; see LICENSE for details.
Description : The classic stack symbols for the MicroLIB startup of the
              RVMDK-STM32N657X0H3 project. The ST CMSIS C startup for the
              STM32N6 has no STACK area of its own (it relies on the scatter
              ARM_LIB_STACK region), while the MicroLIB entry code still
              expects the legacy "__initial_sp" absolute symbol - provide it
              here, matching the top of the ARM_LIB_STACK region.
******************************************************************************/

    .global             __initial_sp
    .set                __initial_sp, 0x34100000

    .end
