#include "RMP.h"

//#define RUN_IN_VM      /* Run this test in RVM virtual machine? */

/* When running these tests, we assume that the counting timer is already enabled */
#define TEST_YIELD       /* Make yield tests */
#define TEST_MAIL        /* Do mailbox tests */
#define TEST_SEM         /* Do semaphore tests */
#define TEST_MAIL_INT    /* Do mailbox interrupt tests */
#define TEST_SEM_INT     /* Do semaphore interrupt tests */

/* How to read counter */
#define COUNTER_READ()   (TIM2->CNT)

#ifdef RUN_IN_VM
const struct RVM_Hdr_Pgtbl RMP_Pgtbl[1]=
{
    /* The first page table */
    {
        0                                                                      /* ptr_t Parent */,
        0x00000000                                                             /* ptr_t Addr */,
        RME_PGTBL_ORDER(RME_PGTBL_SIZE_512M,RME_PGTBL_NUM_8)                   /* ptr_t Order */,
        {                                                                      /* u8 Flags[8] */
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM
        }
    }
};

const struct RVM_Image RMP_Image=
{
    RVM_MAGIC                                                                  /* const ptr_t Magic */,
    "VM Domain 01"                                                             /* s8 Name[16] */,
    _RMP_Entry                                                                 /* void* User_Entry */,
    &RMP_User_Stack                                                            /* void* User_Stack */,
    0x100                                                                      /* ptr_t User_Size */,
    _RMP_Int                                                                   /* void* Int_Entry */,
    &RMP_Int_Stack                                                             /* void* Int_Stack */,
    0x400                                                                      /* ptr_t Int_Size */,
    
    &RMP_Param                                                                 /* struct RVM_Param* Param */,
    &RMP_Regs                                                                  /* struct RVM_Regs* Regs */,
    &RMP_Flag                                                                  /* struct RVM_Int_Flag* Int_Flags */,
    
    RMP_Console                                                                /* void* Console_Buf */,
    RMP_KERNEL_DEBUG_MAX_STR                                                   /* ptr_t Console_Size */,
    
    1                                                                          /* ptr_t Prio */,
    10                                                                         /* ptr_t Slices */,
    
    1                                                                          /* ptr_t Pgtbl_Num */,
    RMP_Pgtbl                                                                  /* const struct RVM_Hdr_Pgtbl* Pgtbl */,
    0                                                                          /* const struct RVM_Image* const * const Next_Image; */
};
#endif

ptr_t Stack_1[256];
struct RMP_Thd Thd_1={0};
ptr_t Stack_2[256];
struct RMP_Thd Thd_2={0};
struct RMP_Sem Sem_1={0};
ptr_t Time;
//TIM_HandleTypeDef TIM2_Handle={0};
//TIM_HandleTypeDef TIM4_Handle={0};

void Timer_Init(void)
{
//    /* Initialize timer 2 to run at the same speed as the CPU */
//    TIM2_Handle.Instance=TIM2;
//    TIM2_Handle.Init.Prescaler=0;
//    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
//    TIM2_Handle.Init.Period=(unsigned int)(-1);
//    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
//    HAL_TIM_Base_Init(&TIM2_Handle);
//    __HAL_RCC_TIM2_CLK_ENABLE();
//    __HAL_TIM_ENABLE(&TIM2_Handle);
}

void Int_Init(void)
{
//    /* Initialize timer 2 to run at the same speed as the CPU */
//    TIM4_Handle.Instance=TIM4;
//    TIM4_Handle.Init.Prescaler=0;
//    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
//    TIM4_Handle.Init.Period=10000;
//    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
//	TIM4_Handle.Init.RepetitionCounter=0;
//    HAL_TIM_Base_Init(&TIM4_Handle);
//    __HAL_RCC_TIM4_CLK_ENABLE();
//    __HAL_TIM_ENABLE(&TIM4_Handle);
//	/* Clear interrupt pending bit, because we used EGR to update the registers */
//	__HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
//	HAL_TIM_Base_Start_IT(&TIM4_Handle);
}

void Int_Disable(void)
{
    /* Disable timer 4 interrupt */
    NVIC_DisableIRQ(TIM4_IRQn);
}

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance==TIM4) 
//    {
//		/* Set the interrupt priority */
//		NVIC_SetPriority(TIM4_IRQn,0xFF);
//		/* Enable timer 4 interrupt */
//		NVIC_EnableIRQ(TIM4_IRQn);
//		/* Enable timer 4 clock */
//		__HAL_RCC_TIM4_CLK_ENABLE();
//	}
//}

void Int_Handler(void);

void TIM4_IRQHandler(void)
{
    TIM4->SR=~TIM_FLAG_Update;//TIM_FLAG_UPDATE;
    Int_Handler();
}



