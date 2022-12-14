/******************************************************************************
Filename    : rmp_hook_rvm.c
Author      : pry 
Date        : 08/09/2019
Licence     : The Unlicense; see LICENSE for details.
Description : The blank project hook file for RMP under RVM hypervisor.
******************************************************************************/

/* Includes ******************************************************************/
#include "rmp.h"
#include "rvm_guest.h"
/* End Includes **************************************************************/

/* Begin Function:RMP_Init ****************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{

}

void RMP_Init_Idle(void)
{
    RVM_Hyp_Wait_Vect();
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

