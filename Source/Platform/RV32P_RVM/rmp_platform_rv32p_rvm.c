/******************************************************************************
Filename    : rmp_platform_rv32p_rvm.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The platform specific file for RV32 with physical address space.
              Any subsets that include RV32I are supported by this port, and this
              is also compatible with processors that support RV32C in addition
              to RV32G.
              Note that processors only supporting RV32C are not covered by
              this port and should use the RV32CP port instead.
******************************************************************************/

/* Include *******************************************************************/
/* The virtual machine configs are here */
#include "rvm_guest.h"

#define __HDR_DEF__
#include "Platform/RV32P_RVM/rmp_platform_rv32p_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "Platform/RV32P_RVM/rmp_platform_rv32p_rvm.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Platform/RV32P_RVM/rmp_platform_rv32p_rvm.h"

#define __HDR_PUBLIC__
#include "Kernel/rmp_kernel.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:_RMP_Stack_Init ***************************************************
Description : Initiate the process stack when trying to start a process. Never
              call this function in user application.
Input       : rmp_ptr_t Stack - The stack address of the thread.
              rmp_ptr_t Size - The stack size of the thread.
              rmp_ptr_t Entry - The entry address of the thread.
              rmp_ptr_t Param - The argument to pass to the thread.
Output      : None.
Return      : rmp_ptr_t - The adjusted stack location.
******************************************************************************/
rmp_ptr_t _RMP_Stack_Init(rmp_ptr_t Stack,
                          rmp_ptr_t Size,
                          rmp_ptr_t Entry,
                          rmp_ptr_t Param)
{
    rmp_ptr_t End;
    struct RMP_RV32P_RVM_Stack* Ptr;

    /* Compute & align stack - full descending */
    End=RMP_ROUND_DOWN(Stack+Size, 4U);
    Ptr=(struct RMP_RV32P_RVM_Stack*)(End-sizeof(struct RMP_RV32P_RVM_Stack));

    /* This is where PC is saved */
    Ptr->PC=Entry;
    /* We always initialize the mstatus register to initialize
     * the FPU, but whether it is present depends on the processor */
    Ptr->MSTATUS=0x1880U|0x2000U;
    /* We always initialize GP to a known value.
     * If a thread modifies this later (by itself), it is fine */
    Ptr->X3_GP=(rmp_ptr_t)(&_RMP_Global);
    /* x10 for arguments */
    Ptr->X10_A0=Param;

    /* Fill the rest for ease of identification */
    Ptr->X1_RA=0x01010101U;
    Ptr->X4_TP=0x04040404U;
    Ptr->X5_T0=0x05050505U;
    Ptr->X6_T1=0x06060606U;
    Ptr->X7_T2=0x07070707U;
    Ptr->X8_S0_FP=0x08080808U;
    Ptr->X9_S1=0x09090909U;
    Ptr->X11_A1=0x11111111U;
    Ptr->X12_A2=0x12121212U;
    Ptr->X13_A3=0x13131313U;
    Ptr->X14_A4=0x14141414U;
    Ptr->X15_A5=0x15151515U;
    Ptr->X16_A6=0x16161616U;
    Ptr->X17_A7=0x17171717U;
    Ptr->X18_S2=0x18181818U;
    Ptr->X19_S3=0x19191919U;
    Ptr->X20_S4=0x20202020U;
    Ptr->X21_S5=0x21212121U;
    Ptr->X22_S6=0x22222222U;
    Ptr->X23_S7=0x23232323U;
    Ptr->X24_S8=0x24242424U;
    Ptr->X25_S9=0x25252525U;
    Ptr->X26_S10=0x26262626U;
    Ptr->X27_S11=0x27272727U;
    Ptr->X28_T3=0x28282828U;
    Ptr->X29_T4=0x29292929U;
    Ptr->X30_T5=0x30303030U;
    Ptr->X31_T6=0x31313131U;

    return (rmp_ptr_t)Ptr;
}
/* End Function:_RMP_Stack_Init **********************************************/

/* Function:_RMP_Lowlvl_Init **************************************************
Description : Initialize the low level hardware of the system. This is adapted from
              Arduino and FE310 examples.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Lowlvl_Init(void)
{
    RVM_Virt_Tim_Reg(RMP_Tim_Handler);
    RVM_Virt_Ctx_Reg(RMP_Ctx_Handler);
}
/* End Function:_RMP_Lowlvl_Init *********************************************/

/* Function:_RMP_Plat_Hook ****************************************************
Description : Platform-specific hook for system initialization.
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Plat_Hook(void)
{
    /* Check header validity - guarantees that the header is not optimized out.
     * ALL VMs are guaranteed to have three entries: Vector, User and Stub */
    RVM_ASSERT(RVM_Desc[0]==RVM_MAGIC_VIRTUAL);
    RVM_ASSERT(RVM_Desc[1]==3U);
    /* Enable interrupt, we've finished all initialization */
    RVM_Hyp_Int_Ena();
}
/* End Function:_RMP_Plat_Hook ***********************************************/

/* Function:RMP_Putchar *******************************************************
Description : Print a character to the debug console.
Input       : char Char - The character to print.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
#if(RVM_DEBUG_PRINT!=0U)
    RVM_Putchar(Char);
#endif
}
/* End Function:RMP_Putchar **************************************************/

/* Function:RMP_Int_Enable ****************************************************
Description : Enable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Enable(void)
{
    RVM_Hyp_Int_Ena();
}
/* End Function:RMP_Int_Enable ***********************************************/

/* Function:RMP_Int_Disable ***************************************************
Description : Disable interrupts.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Disable(void)
{
    RVM_Hyp_Int_Dis();
}
/* End Function:RMP_Int_Disable **********************************************/

/* Function:RMP_Int_Mask ******************************************************
Description : Mask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Mask(void)
{
    RVM_Virt_Int_Mask();
}
/* End Function:RMP_Int_Mask *************************************************/

/* Function:RMP_Int_Unmask ****************************************************
Description : Unmask interrupts that may do sends.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Int_Unmask(void)
{
    RVM_Virt_Int_Unmask();
}
/* End Function:RMP_Int_Unmask ***********************************************/

/* Function:_RMP_Yield ********************************************************
Description : Trigger a yield to another thread. This will trigger the software
              interrupt in RISC-V.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
/* Use "const" to make sure this initializer is in code flash - this will
 * be optimized out when fast context switching is not enabled */
volatile struct RVM_Param* const RMP_RV32P_RVM_Usr_Param=&(RVM_STATE->Usr);
void _RMP_Yield(void)
{
#if(RMP_RV32P_RVM_FAST_YIELD!=0U)
    if(RVM_STATE->Vct_Act!=0U)
        RVM_Virt_Yield();
    else
    /* Selecting TVD implies RVF */
#if(RMP_RV32P_RVM_COP_RVD!=0U)
        _RMP_RV32P_RVM_Yield_RVD();
#elif(RMP_RV32P_RVM_COP_RVF!=0U)
        _RMP_RV32P_RVM_Yield_RVF();
#else
        _RMP_RV32P_RVM_Yield_NONE();
#endif
#else
    RVM_Virt_Yield();
#endif
}
/* End Function:_RMP_Yield ***************************************************/

/* Function:RMP_Ctx_Handler ***************************************************
Description : The context switch interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Ctx_Handler(void)
{
    rmp_ptr_t* SP;
    rmp_ptr_t MSTATUS;
    
    /* ADDI     sp,sp,-31*4         Save registers
     * SW       x31,30*4(sp)
     * SW       x30,29*4(sp)
     * SW       x29,28*4(sp)
     * SW       x28,27*4(sp)
     * SW       x27,26*4(sp)
     * SW       x26,25*4(sp)
     * SW       x25,24*4(sp)
     * SW       x24,23*4(sp)
     * SW       x23,22*4(sp)
     * SW       x22,21*4(sp)
     * SW       x21,20*4(sp)
     * SW       x20,19*4(sp)
     * SW       x19,18*4(sp)
     * SW       x18,17*4(sp)
     * SW       x17,16*4(sp)
     * SW       x16,15*4(sp)
     * SW       x15,14*4(sp)
     * SW       x14,13*4(sp)
     * SW       x13,12*4(sp)
     * SW       x12,11*4(sp)
     * SW       x11,10*4(sp)
     * SW       x10,9*4(sp)
     * SW       x9,8*4(sp)
     * SW       x8,7*4(sp)
     * SW       x7,6*4(sp)
     * SW       x6,5*4(sp)
     * SW       x5,4*4(sp)
     * SW       x4,3*4(sp)
     * SW       x3,2*4(sp)
     * SW       x1,1*4(sp)
     * CSRR     a0,mepc
     * SW       a0,1*4(sp)
     * CSRR     a0,mstatus */
    SP=((rmp_ptr_t*)(RVM_REG->Reg.X2_SP))-31U;
    SP[30U]=RVM_REG->Reg.X31_T6;
    SP[29U]=RVM_REG->Reg.X30_T5;
    SP[28U]=RVM_REG->Reg.X29_T4;
    SP[27U]=RVM_REG->Reg.X28_T3;
    SP[26U]=RVM_REG->Reg.X27_S11;
    SP[25U]=RVM_REG->Reg.X26_S10;
    SP[24U]=RVM_REG->Reg.X25_S9;
    SP[23U]=RVM_REG->Reg.X24_S8;
    SP[22U]=RVM_REG->Reg.X23_S7;
    SP[21U]=RVM_REG->Reg.X22_S6;
    SP[20U]=RVM_REG->Reg.X21_S5;
    SP[19U]=RVM_REG->Reg.X20_S4;
    SP[18U]=RVM_REG->Reg.X19_S3;
    SP[17U]=RVM_REG->Reg.X18_S2;
    SP[16U]=RVM_REG->Reg.X17_A7;
    SP[15U]=RVM_REG->Reg.X16_A6;
    SP[14U]=RVM_REG->Reg.X15_A5;
    SP[13U]=RVM_REG->Reg.X14_A4;
    SP[12U]=RVM_REG->Reg.X13_A3;
    SP[11U]=RVM_REG->Reg.X12_A2;
    SP[10U]=RVM_REG->Reg.X11_A1;
    SP[9U]=RVM_REG->Reg.X10_A0;
    SP[8U]=RVM_REG->Reg.X9_S1;
    SP[7U]=RVM_REG->Reg.X8_S0_FP;
    SP[6U]=RVM_REG->Reg.X7_T2;
    SP[5U]=RVM_REG->Reg.X6_T1;
    SP[4U]=RVM_REG->Reg.X5_T0;
    SP[3U]=RVM_REG->Reg.X4_TP;
    SP[2U]=RVM_REG->Reg.X3_GP;
    SP[1U]=RVM_REG->Reg.X1_RA;
    SP[0U]=RVM_REG->Reg.PC;
    MSTATUS=RVM_REG->Reg.MSTATUS;   /* Read mstatus to decide FPU status, but don't save yet */

#if((RMP_RV32P_RVM_COP_RVD!=0U)||(RMP_RV32P_RVM_COP_RVF!=0U))
    /* LUI      a1,4                See if FPU is used (mstatus.fs[1]==1)
     * AND      a1,a1,a0
     * BEQZ     a1,_RMP_RV32P_Yield_Save_Skip */
    if(MSTATUS)
    {
#endif
#if(RMP_RV32P_RVM_COP_RVD!=0U)
        /* ADDI     sp,sp,-65*4     FPU active, saving context
         * FRCSR    a1
         * SW       a1,32*8(sp)
         * FSD      f31,31*8(sp)
         * FSD      f30,30*8(sp)
         * FSD      f29,29*8(sp)
         * FSD      f28,28*8(sp)
         * FSD      f27,27*8(sp)
         * FSD      f26,26*8(sp)
         * FSD      f25,25*8(sp)
         * FSD      f24,24*8(sp)
         * FSD      f23,23*8(sp)
         * FSD      f22,22*8(sp)
         * FSD      f21,21*8(sp)
         * FSD      f20,20*8(sp)
         * FSD      f19,19*8(sp)
         * FSD      f18,18*8(sp)
         * FSD      f17,17*8(sp)
         * FSD      f16,16*8(sp)
         * FSD      f15,15*8(sp)
         * FSD      f14,14*8(sp)
         * FSD      f13,13*8(sp)
         * FSD      f12,12*8(sp)
         * FSD      f11,11*8(sp)
         * FSD      f10,10*8(sp)
         * FSD      f9,9*8(sp)
         * FSD      f8,8*8(sp)
         * FSD      f7,7*8(sp)
         * FSD      f6,6*8(sp)
         * FSD      f5,5*8(sp)
         * FSD      f4,4*8(sp)
         * FSD      f3,3*8(sp)
         * FSD      f2,2*8(sp)
         * FSD      f1,1*8(sp)
         * FSD      f0,0*8(sp) */
        SP-=65U;
        SP[64U]=RMP_FPU_RVD->FCSR;
        SP[63U]=RMP_FPU_RVD->F31[1U];
        SP[62U]=RMP_FPU_RVD->F31[0U];
        SP[61U]=RMP_FPU_RVD->F30[1U];
        SP[60U]=RMP_FPU_RVD->F30[0U];
        SP[59U]=RMP_FPU_RVD->F29[1U];
        SP[58U]=RMP_FPU_RVD->F29[0U];
        SP[57U]=RMP_FPU_RVD->F28[1U];
        SP[56U]=RMP_FPU_RVD->F28[0U];
        SP[55U]=RMP_FPU_RVD->F27[1U];
        SP[54U]=RMP_FPU_RVD->F27[0U];
        SP[53U]=RMP_FPU_RVD->F26[1U];
        SP[52U]=RMP_FPU_RVD->F26[0U];
        SP[51U]=RMP_FPU_RVD->F25[1U];
        SP[50U]=RMP_FPU_RVD->F25[0U];
        SP[49U]=RMP_FPU_RVD->F24[1U];
        SP[48U]=RMP_FPU_RVD->F24[0U];
        SP[47U]=RMP_FPU_RVD->F23[1U];
        SP[46U]=RMP_FPU_RVD->F23[0U];
        SP[45U]=RMP_FPU_RVD->F22[1U];
        SP[44U]=RMP_FPU_RVD->F22[0U];
        SP[43U]=RMP_FPU_RVD->F21[1U];
        SP[42U]=RMP_FPU_RVD->F21[0U];
        SP[41U]=RMP_FPU_RVD->F20[1U];
        SP[40U]=RMP_FPU_RVD->F20[0U];
        SP[39U]=RMP_FPU_RVD->F19[1U];
        SP[38U]=RMP_FPU_RVD->F19[0U];
        SP[37U]=RMP_FPU_RVD->F18[1U];
        SP[36U]=RMP_FPU_RVD->F18[0U];
        SP[35U]=RMP_FPU_RVD->F17[1U];
        SP[34U]=RMP_FPU_RVD->F17[0U];
        SP[33U]=RMP_FPU_RVD->F16[1U];
        SP[32U]=RMP_FPU_RVD->F16[0U];
        SP[31U]=RMP_FPU_RVD->F15[1U];
        SP[30U]=RMP_FPU_RVD->F15[0U];
        SP[29U]=RMP_FPU_RVD->F14[1U];
        SP[28U]=RMP_FPU_RVD->F14[0U];
        SP[27U]=RMP_FPU_RVD->F13[1U];
        SP[26U]=RMP_FPU_RVD->F13[0U];
        SP[25U]=RMP_FPU_RVD->F12[1U];
        SP[24U]=RMP_FPU_RVD->F12[0U];
        SP[23U]=RMP_FPU_RVD->F11[1U];
        SP[22U]=RMP_FPU_RVD->F11[0U];
        SP[21U]=RMP_FPU_RVD->F10[1U];
        SP[20U]=RMP_FPU_RVD->F10[0U];
        SP[19U]=RMP_FPU_RVD->F9[1U];
        SP[18U]=RMP_FPU_RVD->F9[0U];
        SP[17U]=RMP_FPU_RVD->F8[1U];
        SP[16U]=RMP_FPU_RVD->F8[0U];
        SP[15U]=RMP_FPU_RVD->F7[1U];
        SP[14U]=RMP_FPU_RVD->F7[0U];
        SP[13U]=RMP_FPU_RVD->F6[1U];
        SP[12U]=RMP_FPU_RVD->F6[0U];
        SP[11U]=RMP_FPU_RVD->F5[1U];
        SP[10U]=RMP_FPU_RVD->F5[0U];
        SP[9U]=RMP_FPU_RVD->F4[1U];
        SP[8U]=RMP_FPU_RVD->F4[0U];
        SP[7U]=RMP_FPU_RVD->F3[1U];
        SP[6U]=RMP_FPU_RVD->F3[0U];
        SP[5U]=RMP_FPU_RVD->F2[1U];
        SP[4U]=RMP_FPU_RVD->F2[0U];
        SP[3U]=RMP_FPU_RVD->F1[1U];
        SP[2U]=RMP_FPU_RVD->F1[0U];
        SP[1U]=RMP_FPU_RVD->F0[1U];
        SP[0U]=RMP_FPU_RVD->F0[0U];
#elif(RMP_RV32P_COP_RVF!=0U)
        /* ADDI    sp,sp,-33*4      FPU active, saving context
         * FRCSR   a1
         * SW      a1,32*4(sp)
         * FSW     f31, 31*4(sp)
         * FSW     f30, 30*4(sp)
         * FSW     f29, 29*4(sp)
         * FSW     f28, 28*4(sp)
         * FSW     f27, 27*4(sp)
         * FSW     f26, 26*4(sp)
         * FSW     f25, 25*4(sp)
         * FSW     f24, 24*4(sp)
         * FSW     f23, 23*4(sp)
         * FSW     f22, 22*4(sp)
         * FSW     f21, 21*4(sp)
         * FSW     f20, 20*4(sp)
         * FSW     f19, 19*4(sp)
         * FSW     f18, 18*4(sp)
         * FSW     f17, 17*4(sp)
         * FSW     f16, 16*4(sp)
         * FSW     f15, 15*4(sp)
         * FSW     f14, 14*4(sp)
         * FSW     f13, 13*4(sp)
         * FSW     f12, 12*4(sp)
         * FSW     f11, 11*4(sp)
         * FSW     f10, 10*4(sp)
         * FSW     f9, 9*4(sp)
         * FSW     f8, 8*4(sp)
         * FSW     f7, 7*4(sp)
         * FSW     f6, 6*4(sp)
         * FSW     f5, 5*4(sp)
         * FSW     f4, 4*4(sp)
         * FSW     f3, 3*4(sp)
         * FSW     f2, 2*4(sp)
         * FSW     f1, 1*4(sp)
         * FSW     f0, 0*4(sp) */
        SP-=33U;
        SP[32U]=RMP_FPU_RVF->FCSR;
        SP[31U]=RMP_FPU_RVF->F31;
        SP[30U]=RMP_FPU_RVF->F30;
        SP[29U]=RMP_FPU_RVF->F29;
        SP[28U]=RMP_FPU_RVF->F28;
        SP[27U]=RMP_FPU_RVF->F27;
        SP[26U]=RMP_FPU_RVF->F26;
        SP[25U]=RMP_FPU_RVF->F25;
        SP[24U]=RMP_FPU_RVF->F24;
        SP[23U]=RMP_FPU_RVF->F23;
        SP[22U]=RMP_FPU_RVF->F22;
        SP[21U]=RMP_FPU_RVF->F21;
        SP[20U]=RMP_FPU_RVF->F20;
        SP[19U]=RMP_FPU_RVF->F19;
        SP[18U]=RMP_FPU_RVF->F18;
        SP[17U]=RMP_FPU_RVF->F17;
        SP[16U]=RMP_FPU_RVF->F16;
        SP[15U]=RMP_FPU_RVF->F15;
        SP[14U]=RMP_FPU_RVF->F14;
        SP[13U]=RMP_FPU_RVF->F13;
        SP[12U]=RMP_FPU_RVF->F12;
        SP[11U]=RMP_FPU_RVF->F11;
        SP[10U]=RMP_FPU_RVF->F10;
        SP[9U]=RMP_FPU_RVF->F9;
        SP[8U]=RMP_FPU_RVF->F8;
        SP[7U]=RMP_FPU_RVF->F7;
        SP[6U]=RMP_FPU_RVF->F6;
        SP[5U]=RMP_FPU_RVF->F5;
        SP[4U]=RMP_FPU_RVF->F4;
        SP[3U]=RMP_FPU_RVF->F3;
        SP[2U]=RMP_FPU_RVF->F2;
        SP[1U]=RMP_FPU_RVF->F1;
        SP[0U]=RMP_FPU_RVF->F0;
#endif
#if((RMP_RV32P_RVM_COP_RVD!=0U)||(RMP_RV32P_RVM_COP_RVF!=0U))
    }
#endif

    /* ADDI     sp,sp,-4            Save mstatus
     * SW       a0,0*4(sp) */
    SP--;
    SP[0U]=MSTATUS;

    /* Store all the user-accessible hypercall structure to stack */
    SP-=5U;
    SP[4U]=RVM_STATE->Usr.Param[3];
    SP[3U]=RVM_STATE->Usr.Param[2];
    SP[2U]=RVM_STATE->Usr.Param[1];
    SP[1U]=RVM_STATE->Usr.Param[0];
    SP[0U]=RVM_STATE->Usr.Number;

    /* .option  push
     * .option  norelax
     * LA       gp,_RMP_Global      Load gp for kernel - defined by linker script
     * .option  pop
     * LA       a0,RMP_SP_Cur       Save the sp to control block
     * SW       sp,(a0) */
    RMP_SP_Cur=(rmp_ptr_t)SP;


    /* LA       sp,_RMP_Stack       Load sp for kernel - defined by linker script
     * CALL     _RMP_Run_High       Get the highest ready task */
    _RMP_Run_High();

    /* LA       a0,RMP_SP_Cur       Load the sp from control block
     * LW       sp,(a0) */
    SP=(rmp_ptr_t*)RMP_SP_Cur;

    /* Load the user-accessible hypercall structure to stack */
    RVM_STATE->Usr.Number=SP[0U];
    RVM_STATE->Usr.Param[0]=SP[1U];
    RVM_STATE->Usr.Param[1]=SP[2U];
    RVM_STATE->Usr.Param[2]=SP[3U];
    RVM_STATE->Usr.Param[3]=SP[4U];
    SP+=5U;

    /* LW       a0,0*4(sp)          Read mstatus to decide FPU status, but don't load yet
     * ADDI     sp,sp,4 */
    MSTATUS=SP[0U];
    SP++;

#if((RMP_RV32P_RVM_COP_RVD!=0U)||(RMP_RV32P_RVM_COP_RVF!=0U))
    /* LUI      a1,4                See if FPU is used (mstatus.fs[1]==1)
     * AND      a1,a1,a0
     * BEQZ     a1,_RMP_RV32P_Yield_Load_Skip */
    if(MSTATUS)
    {
#endif
#if(RMP_RV32P_RVM_COP_RVD!=0U)
        /* FLD      f0,0*8(sp)      FPU active, loading context
         * FLD      f1,1*8(sp)
         * FLD      f2,2*8(sp)
         * FLD      f3,3*8(sp)
         * FLD      f4,4*8(sp)
         * FLD      f5,5*8(sp)
         * FLD      f6,6*8(sp)
         * FLD      f7,7*8(sp)
         * FLD      f8,8*8(sp)
         * FLD      f9,9*8(sp)
         * FLD      f10,10*8(sp)
         * FLD      f11,11*8(sp)
         * FLD      f12,12*8(sp)
         * FLD      f13,13*8(sp) 
         * FLD      f14,14*8(sp)
         * FLD      f15,15*8(sp)
         * FLD      f16,16*8(sp)
         * FLD      f17,17*8(sp)
         * FLD      f18,18*8(sp)
         * FLD      f19,19*8(sp)
         * FLD      f20,20*8(sp)
         * FLD      f21,21*8(sp)
         * FLD      f22,22*8(sp)
         * FLD      f23,23*8(sp)
         * FLD      f24,24*8(sp)
         * FLD      f25,25*8(sp)
         * FLD      f26,26*8(sp)
         * FLD      f27,27*8(sp)
         * FLD      f28,28*8(sp)
         * FLD      f29,29*8(sp)
         * FLD      f30,30*8(sp)
         * FLD      f31,31*8(sp)
         * LW       a1,32*8(sp)
         * FSCSR    a1
         * ADDI     sp,sp,65*4 */
        RMP_FPU_RVD->F0[0U]=SP[0U];
        RMP_FPU_RVD->F0[1U]=SP[1U];
        RMP_FPU_RVD->F1[0U]=SP[2U];
        RMP_FPU_RVD->F1[1U]=SP[3U];
        RMP_FPU_RVD->F2[0U]=SP[4U];
        RMP_FPU_RVD->F2[1U]=SP[5U];
        RMP_FPU_RVD->F3[0U]=SP[6U];
        RMP_FPU_RVD->F3[1U]=SP[7U];
        RMP_FPU_RVD->F4[0U]=SP[8U];
        RMP_FPU_RVD->F4[1U]=SP[9U];
        RMP_FPU_RVD->F5[0U]=SP[10U];
        RMP_FPU_RVD->F5[1U]=SP[11U];
        RMP_FPU_RVD->F6[0U]=SP[12U];
        RMP_FPU_RVD->F6[1U]=SP[13U];
        RMP_FPU_RVD->F7[0U]=SP[14U];
        RMP_FPU_RVD->F7[1U]=SP[15U];
        RMP_FPU_RVD->F8[0U]=SP[16U];
        RMP_FPU_RVD->F8[1U]=SP[17U];
        RMP_FPU_RVD->F9[0U]=SP[18U];
        RMP_FPU_RVD->F9[1U]=SP[19U];
        RMP_FPU_RVD->F10[0U]=SP[20U];
        RMP_FPU_RVD->F10[1U]=SP[21U];
        RMP_FPU_RVD->F11[0U]=SP[22U];
        RMP_FPU_RVD->F11[1U]=SP[23U];
        RMP_FPU_RVD->F12[0U]=SP[24U];
        RMP_FPU_RVD->F12[1U]=SP[25U];
        RMP_FPU_RVD->F13[0U]=SP[26U];
        RMP_FPU_RVD->F13[1U]=SP[27U];
        RMP_FPU_RVD->F14[0U]=SP[28U];
        RMP_FPU_RVD->F14[1U]=SP[29U];
        RMP_FPU_RVD->F15[0U]=SP[30U];
        RMP_FPU_RVD->F15[1U]=SP[31U];
        RMP_FPU_RVD->F16[0U]=SP[32U];
        RMP_FPU_RVD->F16[1U]=SP[33U];
        RMP_FPU_RVD->F17[0U]=SP[34U];
        RMP_FPU_RVD->F17[1U]=SP[35U];
        RMP_FPU_RVD->F18[0U]=SP[36U];
        RMP_FPU_RVD->F18[1U]=SP[37U];
        RMP_FPU_RVD->F19[0U]=SP[38U];
        RMP_FPU_RVD->F19[1U]=SP[39U];
        RMP_FPU_RVD->F20[0U]=SP[40U];
        RMP_FPU_RVD->F20[1U]=SP[41U];
        RMP_FPU_RVD->F21[0U]=SP[42U];
        RMP_FPU_RVD->F21[1U]=SP[43U];
        RMP_FPU_RVD->F22[0U]=SP[44U];
        RMP_FPU_RVD->F22[1U]=SP[45U];
        RMP_FPU_RVD->F23[0U]=SP[46U];
        RMP_FPU_RVD->F23[1U]=SP[47U];
        RMP_FPU_RVD->F24[0U]=SP[48U];
        RMP_FPU_RVD->F24[1U]=SP[49U];
        RMP_FPU_RVD->F25[0U]=SP[50U];
        RMP_FPU_RVD->F25[1U]=SP[51U];
        RMP_FPU_RVD->F26[0U]=SP[52U];
        RMP_FPU_RVD->F26[1U]=SP[53U];
        RMP_FPU_RVD->F27[0U]=SP[54U];
        RMP_FPU_RVD->F27[1U]=SP[55U];
        RMP_FPU_RVD->F28[0U]=SP[56U];
        RMP_FPU_RVD->F28[1U]=SP[57U];
        RMP_FPU_RVD->F29[0U]=SP[58U];
        RMP_FPU_RVD->F29[1U]=SP[59U];
        RMP_FPU_RVD->F30[0U]=SP[60U];
        RMP_FPU_RVD->F30[1U]=SP[61U];
        RMP_FPU_RVD->F31[0U]=SP[62U];
        RMP_FPU_RVD->F31[1U]=SP[63U];
        RMP_FPU_RVD->FCSR=SP[64U];
        SP+=65U;
#elif(RMP_RV32P_COP_RVF!=0U)
        /* FLW      f0,0*4(sp)      FPU active, loading context
         * FLW      f1,1*4(sp)
         * FLW      f2,2*4(sp)
         * FLW      f3,3*4(sp)
         * FLW      f4,4*4(sp)
         * FLW      f5,5*4(sp)
         * FLW      f6,6*4(sp)
         * FLW      f7,7*4(sp)
         * FLW      f8,8*4(sp)
         * FLW      f9,9*4(sp)
         * FLW      f10,10*4(sp)
         * FLW      f11,11*4(sp)
         * FLW      f12,12*4(sp)
         * FLW      f13,13*4(sp)
         * FLW      f14,14*4(sp)
         * FLW      f15,15*4(sp)
         * FLW      f16,16*4(sp)
         * FLW      f17,17*4(sp)
         * FLW      f18,18*4(sp)
         * FLW      f19,19*4(sp)
         * FLW      f20,20*4(sp)
         * FLW      f21,21*4(sp)
         * FLW      f22,22*4(sp)
         * FLW      f23,23*4(sp)
         * FLW      f24,24*4(sp)
         * FLW      f25,25*4(sp)
         * FLW      f26,26*4(sp)
         * FLW      f27,27*4(sp)
         * FLW      f28,28*4(sp)
         * FLW      f29,29*4(sp)
         * FLW      f30,30*4(sp)
         * FLW      f31,31*4(sp)
         * LW       a1,32*4(sp)
         * FSCSR    a1
         * ADDI     sp,sp,33*4 */
        RMP_FPU_RVF->F0=SP[0U];
        RMP_FPU_RVF->F1=SP[1U];
        RMP_FPU_RVF->F2=SP[2U];
        RMP_FPU_RVF->F3=SP[3U];
        RMP_FPU_RVF->F4=SP[4U];
        RMP_FPU_RVF->F5=SP[5U];
        RMP_FPU_RVF->F6=SP[6U];
        RMP_FPU_RVF->F7=SP[7U];
        RMP_FPU_RVF->F8=SP[8U];
        RMP_FPU_RVF->F9=SP[9U];
        RMP_FPU_RVF->F10=SP[10U];
        RMP_FPU_RVF->F11=SP[11U];
        RMP_FPU_RVF->F12=SP[12U];
        RMP_FPU_RVF->F13=SP[13U];
        RMP_FPU_RVF->F14=SP[14U];
        RMP_FPU_RVF->F15=SP[15U];
        RMP_FPU_RVF->F16=SP[16U];
        RMP_FPU_RVF->F17=SP[17U];
        RMP_FPU_RVF->F18=SP[18U];
        RMP_FPU_RVF->F19=SP[19U];
        RMP_FPU_RVF->F20=SP[20U];
        RMP_FPU_RVF->F21=SP[21U];
        RMP_FPU_RVF->F22=SP[22U];
        RMP_FPU_RVF->F23=SP[23U];
        RMP_FPU_RVF->F24=SP[24U];
        RMP_FPU_RVF->F25=SP[25U];
        RMP_FPU_RVF->F26=SP[26U];
        RMP_FPU_RVF->F27=SP[27U];
        RMP_FPU_RVF->F28=SP[28U];
        RMP_FPU_RVF->F29=SP[29U];
        RMP_FPU_RVF->F30=SP[30U];
        RMP_FPU_RVF->F31=SP[31U];
        RMP_FPU_RVF->FCSR=SP[32U];
        SP+=33U;
#endif
#if((RMP_RV32P_RVM_COP_RVD!=0U)||(RMP_RV32P_RVM_COP_RVF!=0U))
    }
#endif

    /* 
     * LI       a1,0x1880           Load mstatus - force M mode with enabled interrupt
     * OR       a0,a0,a1
     * CSRW     mstatus,a0 */
    RVM_REG->Reg.MSTATUS=MSTATUS;

    /* LW       a0,0*4(sp)          Load pc
     * CSRW     mepc,a0
     * LW       x1,1*4(sp)          Load registers
     * LW       x3,2*4(sp)
     * LW       x4,3*4(sp)
     * LW       x5,4*4(sp)
     * LW       x6,5*4(sp)
     * LW       x7,6*4(sp)
     * LW       x8,7*4(sp)
     * LW       x9,8*4(sp)
     * LW       x10,9*4(sp)
     * LW       x11,10*4(sp)
     * LW       x12,11*4(sp)
     * LW       x13,12*4(sp)
     * LW       x14,13*4(sp)
     * LW       x15,14*4(sp)
     * LW       x16,15*4(sp)
     * LW       x17,16*4(sp)
     * LW       x18,17*4(sp)
     * LW       x19,18*4(sp)
     * LW       x20,19*4(sp)
     * LW       x21,20*4(sp)
     * LW       x22,21*4(sp)
     * LW       x23,22*4(sp)
     * LW       x24,23*4(sp)
     * LW       x25,24*4(sp)
     * LW       x26,25*4(sp)
     * LW       x27,26*4(sp)
     * LW       x28,27*4(sp)
     * LW       x29,28*4(sp)
     * LW       x30,29*4(sp)
     * LW       x31,30*4(sp)
     * ADDI     sp,sp,31*4 */
    SP[0U]=RVM_REG->Reg.PC;
    SP[1U]=RVM_REG->Reg.X1_RA;
    SP[2U]=RVM_REG->Reg.X3_GP;
    SP[3U]=RVM_REG->Reg.X4_TP;
    SP[4U]=RVM_REG->Reg.X5_T0;
    SP[5U]=RVM_REG->Reg.X6_T1;
    SP[6U]=RVM_REG->Reg.X7_T2;
    SP[7U]=RVM_REG->Reg.X8_S0_FP;
    SP[8U]=RVM_REG->Reg.X9_S1;
    SP[9U]=RVM_REG->Reg.X10_A0;
    SP[10U]=RVM_REG->Reg.X11_A1;
    SP[11U]=RVM_REG->Reg.X12_A2;
    SP[12U]=RVM_REG->Reg.X13_A3;
    SP[13U]=RVM_REG->Reg.X14_A4;
    SP[14U]=RVM_REG->Reg.X15_A5;
    SP[15U]=RVM_REG->Reg.X16_A6;
    SP[16U]=RVM_REG->Reg.X17_A7;
    SP[17U]=RVM_REG->Reg.X18_S2;
    SP[18U]=RVM_REG->Reg.X19_S3;
    SP[19U]=RVM_REG->Reg.X20_S4;
    SP[20U]=RVM_REG->Reg.X21_S5;
    SP[21U]=RVM_REG->Reg.X22_S6;
    SP[22U]=RVM_REG->Reg.X23_S7;
    SP[23U]=RVM_REG->Reg.X24_S8;
    SP[24U]=RVM_REG->Reg.X25_S9;
    SP[25U]=RVM_REG->Reg.X26_S10;
    SP[26U]=RVM_REG->Reg.X27_S11;
    SP[27U]=RVM_REG->Reg.X28_T3;
    SP[28U]=RVM_REG->Reg.X29_T4;
    SP[29U]=RVM_REG->Reg.X30_T5;
    SP[30U]=RVM_REG->Reg.X31_T6;
    RVM_REG->Reg.X2_SP=(rmp_ptr_t)(SP+31U);

    /* MRET */
    return;
}
/* End Function:RMP_Ctx_Handler **********************************************/

/* Function:RMP_Tim_Handler ***************************************************
Description : The OS tick timer interrupt routine.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Tim_Handler(void)
{
    /* Update or reset the next tick
     * RMP_RV32P_TIM_CLR(); */

    _RMP_Tim_Handler(1U);
}
/* End Function:RMP_Tim_Handler **********************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
