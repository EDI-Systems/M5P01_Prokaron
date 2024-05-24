/******************************************************************************
Filename    : rmp_kernel.c
Author      : pry
Date        : 04/02/2018
Licence     : The Unlicense; see LICENSE for details.
Description : The RMP RTOS single-file kernel.
              This is a single-process kernel that does nothing but simple 
              thread context switches. This operating system's kernel object
              allocation policy is totally exported, so the user assume full
              control over this.
              The white-box coverage of 100% of all kernel branches have been
              reached. Formal verification in progress.
              Use of 'volatile': we make every data structure volatile so all 
              memory reads and writes to potentially shared structures are 
              strongly ordered from a compiler perspective. This DOES decrease
              performance, but can survive smart LTOs without manually adding
              barriers. The kernel is written such that all frequently read data
              is a copy of the global data, so the performance loss is mild.
              Function naming conventions: user-program callable functions have
              no "_", while internal kernel-only functions begin with "_".
******************************************************************************/

/* Include *******************************************************************/
#define __HDR_DEF__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_DEF__

#define __HDR_STRUCT__
#include "rmp_platform.h"
#include "Kernel/rmp_kernel.h"
#undef __HDR_STRUCT__

/* Private include */
#include "Kernel/rmp_kernel.h"

#define __HDR_PUBLIC__
#include "rmp_platform.h"
#undef __HDR_PUBLIC__
/* End Include ***************************************************************/

/* Function:RMP_MSB_Generic ***************************************************
Description : Find the MSB's position. This is a portable solution for all
              processors; if your processor does not have fast built-in bit
              manipulation support, you can resort to this when porting.
Input       : rmp_ptr_t Value - The value to compute for.
Output      : None.
Return      : rmp_ptr_t - The result. 0 will be returned for 0.
******************************************************************************/
rmp_ptr_t RMP_MSB_Generic(rmp_ptr_t Value)
{
    rmp_ptr_t Bit;
    static const rmp_u8_t Table[256U]=
    {
        0U,0U,1U,1U,2U,2U,2U,2U,3U,3U,3U,3U,3U,3U,3U,3U,
        4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,4U,
        5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,
        5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,5U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,6U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,
        7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U,7U
    };

#if(RMP_WORD_ORDER==4U)
    /* 15-8 */
    if(Value>=RMP_POW2(8U))
    {
        RMP_COVERAGE_MARKER();
        Bit=8U;
    }
    /* 7-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        Bit=0U;
    }
#elif(RMP_WORD_ORDER==5U)
    /* 31-16 */
    if(Value>=RMP_POW2(16U))
    {
        RMP_COVERAGE_MARKER();
        /* 31-24 */
        if(Value>=RMP_POW2(24U))
        {
            RMP_COVERAGE_MARKER();
            Bit=24U;
        }
        /* 24-16 */
        else
        {
            RMP_COVERAGE_MARKER();
            Bit=16U;
        }
    }
    /* 15-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        /* 15-8 */
        if(Value>=RMP_POW2(8U))
        {
            RMP_COVERAGE_MARKER();
            Bit=8U;
        }
        /* 7-0 */
        else
        {
            RMP_COVERAGE_MARKER();
            Bit=0U;
        }
    }
#elif(RMP_WORD_ORDER==6U)
    /* 63-32 */
    if(Value>=RMP_POW2(32U))
    {
        RMP_COVERAGE_MARKER();
        /* 63-48 */
        if(Value>=RMP_POW2(48U))
        {
            RMP_COVERAGE_MARKER();
            /* 63-56 */
            if(Value>=RMP_POW2(56U))
            {
                RMP_COVERAGE_MARKER();
                Bit=56U;
            }
            /* 56-48 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=48U;
            }
        }
        /* 47-32 */
        else
        {
            RMP_COVERAGE_MARKER();
            /* 47-40 */
            if(Value>=RMP_POW2(40U))
            {
                RMP_COVERAGE_MARKER();
                Bit=40U;
            }
            /* 39-32 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=32U;
            }
        }
    }
    /* 31-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        /* 31-16 */
        if(Value>=RMP_POW2(16U))
        {
            RMP_COVERAGE_MARKER();
            /* 31-24 */
            if(Value>=RMP_POW2(24U))
            {
                RMP_COVERAGE_MARKER();
                Bit=24U;
            }
            /* 24-16 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=16U;
            }
        }
        /* 15-0 */
        else
        {
            RMP_COVERAGE_MARKER();
            /* 15-8 */
            if(Value>=RMP_POW2(8U))
            {
                RMP_COVERAGE_MARKER();
                Bit=8U;
            }
            /* 7-0 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=0U;
            }
        }
    }
#else
#error Generic MSB for 128-bits & above are not implemented.
#endif

    return Table[Value>>Bit]+Bit;
}
/* End Function:RMP_MSB_Generic **********************************************/

/* Function:RMP_LSB_Generic ***************************************************
Description : Find the LSB's position. This is a portable solution for all
              processors; if your processor does not have fast built-in bit
              manipulation support, you can resort to this when porting.
Input       : rmp_ptr_t Value - The value to count.
Output      : None.
Return      : rmp_ptr_t - The result. 0 will be returned for 0.
******************************************************************************/
rmp_ptr_t RMP_LSB_Generic(rmp_ptr_t Value)
{
    rmp_ptr_t Bit;
    static const rmp_u8_t Table[256U]=
    {
        0U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        6U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        7U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        6U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        5U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U,
        4U,0U,1U,0U,2U,0U,1U,0U,3U,0U,1U,0U,2U,0U,1U,0U
    };
    
#if(RMP_WORD_ORDER==4U)
    /* 16-8 */
    if((Value<<8U)==0U)
    {
        RMP_COVERAGE_MARKER();
        Bit=8U;
    }
    /* 7-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        Bit=0U;
    }
#elif(RMP_WORD_ORDER==5U)
    /* 31-16 */
    if((Value<<16U)==0U)
    {
        RMP_COVERAGE_MARKER();
        /* 31-24 */
        if((Value<<8U)==0U)
        {
            RMP_COVERAGE_MARKER();
            Bit=24U;
        }
        /* 24-16 */
        else
        {
            RMP_COVERAGE_MARKER();
            Bit=16U;
        }
    }
    /* 15-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        /* 15-8 */
        if((Value<<24U)==0U)
        {
            RMP_COVERAGE_MARKER();
            Bit=8U;
        }
        /* 7-0 */
        else
        {
            RMP_COVERAGE_MARKER();
            Bit=0U;
        }
    }
#elif(RMP_WORD_ORDER==6U)
    /* 63-32 */
    if((Value<<32U)==0U)
    {
        RMP_COVERAGE_MARKER();
        /* 63-48 */
        if((Value<<16U)==0U)
        {
            RMP_COVERAGE_MARKER();
            /* 63-56 */
            if((Value<<8U)==0U)
            {
                RMP_COVERAGE_MARKER();
                Bit=56U;
            }
            /* 56-48 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=48U;
            }
        }
        /* 47-32 */
        else
        {
            RMP_COVERAGE_MARKER();
            /* 47-40 */
            if((Value<<24U)==0U)
            {
                RMP_COVERAGE_MARKER();
                Bit=40U;
            }
            /* 39-32 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=32U;
            }
        }
    }
    /* 31-0 */
    else
    {
        RMP_COVERAGE_MARKER();
        /* 31-16 */
        if((Value<<48U)==0U)
        {
            RMP_COVERAGE_MARKER();
            /* 31-24 */
            if((Value<<40U)==0U)
            {
                RMP_COVERAGE_MARKER();
                Bit=24U;
            }
            /* 24-16 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=16U;
            }
        }
        /* 15-0 */
        else
        {
            RMP_COVERAGE_MARKER();
            /* 15-8 */
            if((Value<<56U)==0U)
            {
                RMP_COVERAGE_MARKER();
                Bit=8U;
            }
            /* 7-0 */
            else
            {
                RMP_COVERAGE_MARKER();
                Bit=0U;
            }
        }
    }
#else
#error Generic LSB for 128-bits & above are not implemented.
#endif

    return Table[(rmp_u8_t)(Value>>Bit)]+Bit;
}
/* End Function:RMP_LSB_Generic **********************************************/

/* Function:RMP_RBT_Generic ***************************************************
Description : Reverse bit order. This is a portable solution meant for all
              processors; if your processor does not have fast built-in RBT,
              you can resort to this instead.
              This is implemented to be as fast as possible.
Input       : rmp_ptr_t Value - The value to count.
Output      : None.
Return      : rmp_ptr_t - The result.
******************************************************************************/
rmp_ptr_t RMP_RBT_Generic(rmp_ptr_t Value)
{
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;

    static const rmp_u8_t Table[256U]=
    {
        0x00U,0x80U,0x40U,0xC0U,0x20U,0xA0U,0x60U,0xE0U,
        0x10U,0x90U,0x50U,0xD0U,0x30U,0xB0U,0x70U,0xF0U,
        0x08U,0x88U,0x48U,0xC8U,0x28U,0xA8U,0x68U,0xE8U,
        0x18U,0x98U,0x58U,0xD8U,0x38U,0xB8U,0x78U,0xF8U,
        0x04U,0x84U,0x44U,0xC4U,0x24U,0xA4U,0x64U,0xE4U,
        0x14U,0x94U,0x54U,0xD4U,0x34U,0xB4U,0x74U,0xF4U,
        0x0CU,0x8CU,0x4CU,0xCCU,0x2CU,0xACU,0x6CU,0xECU,
        0x1CU,0x9CU,0x5CU,0xDCU,0x3CU,0xBCU,0x7CU,0xFCU,
        0x02U,0x82U,0x42U,0xC2U,0x22U,0xA2U,0x62U,0xE2U,
        0x12U,0x92U,0x52U,0xD2U,0x32U,0xB2U,0x72U,0xF2U,
        0x0AU,0x8AU,0x4AU,0xCAU,0x2AU,0xAAU,0x6AU,0xEAU,
        0x1AU,0x9AU,0x5AU,0xDAU,0x3AU,0xBAU,0x7AU,0xFAU,
        0x06U,0x86U,0x46U,0xC6U,0x26U,0xA6U,0x66U,0xE6U,
        0x16U,0x96U,0x56U,0xD6U,0x36U,0xB6U,0x76U,0xF6U,
        0x0EU,0x8EU,0x4EU,0xCEU,0x2EU,0xAEU,0x6EU,0xEEU,
        0x1EU,0x9EU,0x5EU,0xDEU,0x3EU,0xBEU,0x7EU,0xFEU,
        0x01U,0x81U,0x41U,0xC1U,0x21U,0xA1U,0x61U,0xE1U,
        0x11U,0x91U,0x51U,0xD1U,0x31U,0xB1U,0x71U,0xF1U,
        0x09U,0x89U,0x49U,0xC9U,0x29U,0xA9U,0x69U,0xE9U,
        0x19U,0x99U,0x59U,0xD9U,0x39U,0xB9U,0x79U,0xF9U,
        0x05U,0x85U,0x45U,0xC5U,0x25U,0xA5U,0x65U,0xE5U,
        0x15U,0x95U,0x55U,0xD5U,0x35U,0xB5U,0x75U,0xF5U,
        0x0DU,0x8DU,0x4DU,0xCDU,0x2DU,0xADU,0x6DU,0xEDU,
        0x1DU,0x9DU,0x5DU,0xDDU,0x3DU,0xBDU,0x7DU,0xFDU,
        0x03U,0x83U,0x43U,0xC3U,0x23U,0xA3U,0x63U,0xE3U,
        0x13U,0x93U,0x53U,0xD3U,0x33U,0xB3U,0x73U,0xF3U,
        0x0BU,0x8BU,0x4BU,0xCBU,0x2BU,0xABU,0x6BU,0xEBU,
        0x1BU,0x9BU,0x5BU,0xDBU,0x3BU,0xBBU,0x7BU,0xFBU,
        0x07U,0x87U,0x47U,0xC7U,0x27U,0xA7U,0x67U,0xE7U,
        0x17U,0x97U,0x57U,0xD7U,0x37U,0xB7U,0x77U,0xF7U,
        0x0FU,0x8FU,0x4FU,0xCFU,0x2FU,0xAFU,0x6FU,0xEFU,
        0x1FU,0x9FU,0x5FU,0xDFU,0x3FU,0xBFU,0x7FU,0xFFU
    };

    Src=Value;
    To=(rmp_u8_t*)(&Ret);
    From=(rmp_u8_t*)(&Src);

#if(RMP_WORD_ORDER==4U)
    To[0]=Table[From[1]];
    To[1]=Table[From[0]];
#elif(RMP_WORD_ORDER==5U)
    To[0]=Table[From[3]];
    To[1]=Table[From[2]];
    To[2]=Table[From[1]];
    To[3]=Table[From[0]];
#elif(RMP_WORD_ORDER==6U)
    To[0]=Table[From[7]];
    To[1]=Table[From[6]];
    To[2]=Table[From[5]];
    To[3]=Table[From[4]];
    To[4]=Table[From[3]];
    To[5]=Table[From[2]];
    To[6]=Table[From[1]];
    To[7]=Table[From[0]];
#else
#error Generic RBT for 128-bits & above are not implemented.
#endif

    return Ret;
}
/* End Function:RMP_RBT_Generic **********************************************/

/* Function:RMP_Coverage_Print ************************************************
Description : The coverage data printer. Should always be disabled for all cases
              except where a kernel coverage test is needed. This should never
              be called any any user application; for coverage testing only.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_COVERAGE_LINE_NUM
void RMP_Coverage_Print(void)
{
    rmp_ptr_t Count;
    rmp_ptr_t Next;
    
    Next=0U;
    for(Count=0U;Count<RMP_COVERAGE_LINE_NUM;Count++)
    {
        if((RMP_Coverage[Count>>RMP_WORD_ORDER]&RMP_POW2(Count&RMP_WORD_MASK))!=0U)
        {
            RMP_COVERAGE_MARKER();
            RMP_DBG_I(Count);
            RMP_DBG_S(",");
            /* We put 12 markers on a single line */
            Next++;
            if(Next>11U)
            {
                RMP_COVERAGE_MARKER();
                Next=0U;
                RMP_DBG_S("\r\n");
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action needed */
            }
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action needed */
        }
    }
}
#endif
/* End Function:RMP_Coverage_Print *******************************************/

/* Function:RMP_Clear *********************************************************
Description : Memset a memory area to zero.
Input       : volatile void* Addr - The address to clear.
              rmp_ptr_t Size - The size to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Clear(volatile void* Addr,
               rmp_ptr_t Size)
{
    rmp_ptr_t Count;
   
    for(Count=0U;Count<Size;Count++)
    {
        ((volatile rmp_u8_t*)Addr)[Count]=0U;
    }
}
/* End Function:RMP_Clear ****************************************************/

/* Function:RMP_Int_Print *****************************************************
Description : Print a signed integer on the debugging console. This integer is
              printed as decimal with sign.
Input       : rmp_cnt_t Int - The integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
rmp_cnt_t RMP_Int_Print(rmp_cnt_t Int)
{
    rmp_cnt_t Num;
    rmp_cnt_t Abs;
    rmp_cnt_t Iter;
    rmp_cnt_t Count;
    rmp_cnt_t Div;
    
    /* Exit on zero */
    if(Int==0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Putchar('0');
        return 1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action needed */
    }

    /* Correct all negatives into positives */
    if(Int<0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Putchar('-');
        Abs=-Int;
        Num=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Abs=Int;
        Num=0;
    }

    /* How many digits are there? */
    Count=0;
    Div=1;
    Iter=Abs;
    while(1)
    {
        Iter/=10;
        Count++;
        if(Iter!=0)
        {
            RMP_COVERAGE_MARKER();
            Div*=10;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            break;
        }
    }
    Num+=Count;

    /* Print the integer */
    Iter=Abs;
    while(Count>0)
    {
        Count--;
        RMP_Putchar((rmp_s8_t)(Iter/Div)+'0');
        Iter=Iter%Div;
        Div/=10;
    }
    
    return Num;
}
/* End Function:RMP_Int_Print ************************************************/

/* Function:RMP_Hex_Print *****************************************************
Description : Print a unsigned integer on the debugging console. This integer is
              printed as hexadecimal.
Input       : rmp_ptr_t Uint - The unsigned integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
rmp_cnt_t RMP_Hex_Print(rmp_ptr_t Uint)
{
    rmp_ptr_t Iter;
    rmp_ptr_t Count;
    rmp_ptr_t Num;

    /* Exit on zero */
    if(Uint==0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Putchar('0');
        return 1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action needed */
    }

    /* Filter out all the zeroes */
    Count=0U;
    Iter=Uint;
    while((Iter>>((sizeof(rmp_ptr_t)*8U)-4U))==0U)
    {
        Iter<<=4U;
        Count++;
    }

    /* Count is the number of pts to print */
    Count=(sizeof(rmp_ptr_t)<<1U)-Count;
    Num=Count;
    while(Count>0U)
    {
        Count--;
        Iter=(Uint>>(Count<<2U))&0x0FU;
        if(Iter<10U)
        {
            RMP_COVERAGE_MARKER();
            RMP_Putchar((rmp_s8_t)Iter+'0');
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_Putchar((rmp_s8_t)Iter+'A'-10);
        }
    }
    
    return (rmp_cnt_t)Num;
}
/* End Function:RMP_Hex_Print ************************************************/

/* Function:RMP_Str_Print *****************************************************
Description : Print a string on the debugging console.
              This is only used for user-level debugging.
Input       : const rmp_s8_t* String - The string to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed, the '\0' is not included.
******************************************************************************/
rmp_cnt_t RMP_Str_Print(const rmp_s8_t* String)
{
    rmp_ptr_t Count;
    
    for(Count=0U;Count<RMP_DEBUG_PRINT_MAX;Count++)
    {
        if(String[Count]==(rmp_s8_t)'\0')
        {
            RMP_COVERAGE_MARKER();
            break;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action needed */
        }
        RMP_Putchar(String[Count]);
    }
    
    return (rmp_cnt_t)Count;
}
/* End Function:RMP_Str_Print ************************************************/

/* Function:RMP_List_Crt ******************************************************
Description : Create a doubly linkled list.
Input       : volatile struct RMP_List* Head - The pointer to the list head.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=Head;
    Head->Next=Head;
}
/* End Function:RMP_List_Crt *************************************************/

/* Function:RMP_List_Del ******************************************************
Description : Delete a node from the doubly-linked list.
Input       : volatile struct RMP_List* Prev - The prevoius node of the target node.
              volatile struct RMP_List* Next - The next node of the target node.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=Prev;
    Prev->Next=Next;
}
/* End Function:RMP_List_Del *************************************************/

/* Function:RMP_List_Ins ******************************************************
Description : Insert a node to the doubly-linked list.
Input       : volatile struct RMP_List* New - The new node to insert.
              volatile struct RMP_List* Prev - The previous node.
              volatile struct RMP_List* Next - The next node.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=New;
    New->Next=Next;
    New->Prev=Prev;
    Prev->Next=New;
}
/* End Function:RMP_List_Ins *************************************************/

/* Function:RMP_CRC16 *********************************************************
Description    : CRC16 checksum calculation. Polynomial=0xA001.
Input          : const rmp_u8_t* Data - The pointer to the dataset.
                 rmp_ptr_t Length - The length of the data in bytes.
Output         : None.
Return         : rmp_ptr_t - The CRC16 value calculated.
******************************************************************************/
#ifdef __RMP_U16_T__
rmp_ptr_t RMP_CRC16(const rmp_u8_t* Data,
                    rmp_ptr_t Length)
{
    rmp_ptr_t Data_Cnt;
    rmp_u8_t Index;
    rmp_u8_t Temp_High;
    rmp_u8_t Temp_Low;
    
    /* CRC16 constants with X^16+X^15+X^2+1 */
    static const rmp_u8_t CRC16_High[256]=
    {
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x00U, 0xC1U, 0x81U, 0x40U, 0x01U, 0xC0U, 0x80U, 0x41U,
        0x01U, 0xC0U, 0x80U, 0x41U, 0x00U, 0xC1U, 0x81U, 0x40U
    };

    const rmp_u8_t CRC16_Low[256]=
    {
        0x00U, 0xC0U, 0xC1U, 0x01U, 0xC3U, 0x03U, 0x02U, 0xC2U,
        0xC6U, 0x06U, 0x07U, 0xC7U, 0x05U, 0xC5U, 0xC4U, 0x04U,
        0xCCU, 0x0CU, 0x0DU, 0xCDU, 0x0FU, 0xCFU, 0xCEU, 0x0EU,
        0x0AU, 0xCAU, 0xCBU, 0x0BU, 0xC9U, 0x09U, 0x08U, 0xC8U,
        0xD8U, 0x18U, 0x19U, 0xD9U, 0x1BU, 0xDBU, 0xDAU, 0x1AU,
        0x1EU, 0xDEU, 0xDFU, 0x1FU, 0xDDU, 0x1DU, 0x1CU, 0xDCU,
        0x14U, 0xD4U, 0xD5U, 0x15U, 0xD7U, 0x17U, 0x16U, 0xD6U,
        0xD2U, 0x12U, 0x13U, 0xD3U, 0x11U, 0xD1U, 0xD0U, 0x10U,
        0xF0U, 0x30U, 0x31U, 0xF1U, 0x33U, 0xF3U, 0xF2U, 0x32U,
        0x36U, 0xF6U, 0xF7U, 0x37U, 0xF5U, 0x35U, 0x34U, 0xF4U,
        0x3CU, 0xFCU, 0xFDU, 0x3DU, 0xFFU, 0x3FU, 0x3EU, 0xFEU,
        0xFAU, 0x3AU, 0x3BU, 0xFBU, 0x39U, 0xF9U, 0xF8U, 0x38U,
        0x28U, 0xE8U, 0xE9U, 0x29U, 0xEBU, 0x2BU, 0x2AU, 0xEAU,
        0xEEU, 0x2EU, 0x2FU, 0xEFU, 0x2DU, 0xEDU, 0xECU, 0x2CU,
        0xE4U, 0x24U, 0x25U, 0xE5U, 0x27U, 0xE7U, 0xE6U, 0x26U,
        0x22U, 0xE2U, 0xE3U, 0x23U, 0xE1U, 0x21U, 0x20U, 0xE0U,
        0xA0U, 0x60U, 0x61U, 0xA1U, 0x63U, 0xA3U, 0xA2U, 0x62U,
        0x66U, 0xA6U, 0xA7U, 0x67U, 0xA5U, 0x65U, 0x64U, 0xA4U,
        0x6CU, 0xACU, 0xADU, 0x6DU, 0xAFU, 0x6FU, 0x6EU, 0xAEU,
        0xAAU, 0x6AU, 0x6BU, 0xABU, 0x69U, 0xA9U, 0xA8U, 0x68U,
        0x78U, 0xB8U, 0xB9U, 0x79U, 0xBBU, 0x7BU, 0x7AU, 0xBAU,
        0xBEU, 0x7EU, 0x7FU, 0xBFU, 0x7DU, 0xBDU, 0xBCU, 0x7CU,
        0xB4U, 0x74U, 0x75U, 0xB5U, 0x77U, 0xB7U, 0xB6U, 0x76U,
        0x72U, 0xB2U, 0xB3U, 0x73U, 0xB1U, 0x71U, 0x70U, 0xB0U,
        0x50U, 0x90U, 0x91U, 0x51U, 0x93U, 0x53U, 0x52U, 0x92U,
        0x96U, 0x56U, 0x57U, 0x97U, 0x55U, 0x95U, 0x94U, 0x54U,
        0x9CU, 0x5CU, 0x5DU, 0x9DU, 0x5FU, 0x9FU, 0x9EU, 0x5EU,
        0x5AU, 0x9AU, 0x9BU, 0x5BU, 0x99U, 0x59U, 0x58U, 0x98U,
        0x88U, 0x48U, 0x49U, 0x89U, 0x4BU, 0x8BU, 0x8AU, 0x4AU,
        0x4EU, 0x8EU, 0x8FU, 0x4FU, 0x8DU, 0x4DU, 0x4CU, 0x8CU,
        0x44U, 0x84U, 0x85U, 0x45U, 0x87U, 0x47U, 0x46U, 0x86U,
        0x82U, 0x42U, 0x43U, 0x83U, 0x41U, 0x81U, 0x80U, 0x40U
    };

    Temp_High=0xFFU;
    Temp_Low=0xFFU;
    for(Data_Cnt=0U;Data_Cnt<Length;Data_Cnt++)
    {
        Index=(Temp_Low^Data[Data_Cnt])&0xFFU;
        Temp_Low=(rmp_u8_t)(Temp_High^CRC16_High[Index])&0xFFU;
        Temp_High=CRC16_Low[Index];
    }

    return (rmp_ptr_t)((((rmp_u16_t)Temp_High)<<8U)|Temp_Low);
}
#endif
/* End Function:RMP_CRC16 ****************************************************/

/* Function:RMP_Sched_Lock ****************************************************
Description : The function locks the scheduler. The locking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Sched_Lock(void)
{
    RMP_INT_MASK();
    RMP_Sched_Lock_Cnt++;
}
/* End Function:RMP_Sched_Lock ***********************************************/

/* Function:RMP_Sched_Unlock **************************************************
Description : The function unlocks the scheduler. The unlocking can be stacked.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Sched_Unlock(void)
{
    if(RMP_Sched_Lock_Cnt==1U)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0U;
        /* Deal with pending scheduler activations */
        if(RMP_Sched_Pend!=0U)
        {
            RMP_COVERAGE_MARKER();
             /* Context switch will clear the pend flag */
            _RMP_Yield();
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        /* Calling _RMP_Yield before truly unmasking interrupts is allowed:
         * (1) We're using a dedicated interrupt handler to switch context.
         *     In this case, the software interrupt will be pending until we 
         *     unmask the kernel-aware interrupts here.
         * (2) We're using a routine to do context switch.
         *     In this case, the routine will unmask interrupts at its end 
         *     anyway, and will not leave the interrupt masked when it is 
         *     exiting, and unmasking it again here does no harm. 
         * We don't want to absorb RMP_INT_UNMASK into _RMP_Yield due to 
         * possible complications of the programming mental model, and
         * we want to keep all invariants as simple as possible. */
        RMP_INT_UNMASK();
    }
    else if(RMP_Sched_Lock_Cnt>1U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
    {
        RMP_ASSERT(0);
        /* No action required */
    }
}
/* End Function:RMP_Sched_Unlock *********************************************/

/* Function:RMP_Yield *********************************************************
Description : Yield to another thread.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Yield(void)
{
    /* Scheduler not locked, perform a schedule immediately */
    if(RMP_Sched_Lock_Cnt==0U)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Yield();
    }
    /* Scheduler locked, have to pend the flag to schedule later */
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1U;
    }
}
/* End Function:RMP_Yield ****************************************************/

/* Function:_RMP_Timer_Proc ***************************************************
Description : Process RMP timer events.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Timer_Proc(void)
{
    rmp_ptr_t State;
    rmp_ptr_t Diff;
    volatile struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        
        /* This thread is overflown */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COVERAGE_MARKER();
            
            RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
            State=RMP_THD_STATE(Thread->State);
            if((State==RMP_THD_SNDDLY)||(State==RMP_THD_SEMDLY))
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
                Thread->Retval=RMP_ERR_OPER;
            }
            else if(State==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
            }
            else if(State==RMP_THD_DELAYED)
            {
                RMP_COVERAGE_MARKER();
            }
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_ASSERT(0);
            }

            /* Set to ready if not suspended */
            RMP_THD_STATE_SET(Thread->State, RMP_THD_RUNNING);
            _RMP_Run_Ins(Thread);
        }
        /* Stop until we find a timer that is not overflown */
        else
        {
            RMP_COVERAGE_MARKER();
            break;
        }
    }
}
/* Function:_RMP_Timer_Proc **************************************************/

/* Function:_RMP_Run_High *****************************************************
Description : Get the highest priority thread that is ready. The return value
              will be written into the global variables. Note that providing a
              fast path where no context operations happen when the new thread
              equal the old thread has little value in RMP, because (1) context
              switches are rarely idempotent because threads prefer to block
              themselves rather than to exhaust slices, (2) such design trades
              worst-case latency for average performance, (3) such design 
              complicates the programmer mental model for using hooks. If such
              saving/restoring of coprocessor context is deemed to inefficient,
              the implementer is free to implement lazy switching.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Run_High(void)
{
    rmp_cnt_t Word;
    rmp_ptr_t Prio;
    
    /* Save potential extra context */
#if(RMP_HOOK_EXTRA!=0U)
    RMP_Ctx_Save();
#endif
    
    /* Write the SP value to thread structure */
    RMP_Thd_Cur->Stack=RMP_SP_Cur;
    
    /* No need to detect scheduler locks - if this function can be called, the
     * scheduler can't be locked, and after we choose the highest priority
     * thread the scheduler pending flag should be cleared */
    RMP_Sched_Pend=0U;
    /* See which one is ready, and pick it */
    Prio=RMP_PRIO_WORD_NUM-1U;
    for(Word=(rmp_cnt_t)Prio;Word>=0;Word--)
    {
        if(RMP_Bitmap[Word]!=0U)
        {
            RMP_COVERAGE_MARKER();
            break;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }

    /* There must be one, at least the initial thread must be ready at all moments! */
    RMP_ASSERT(Word>=0);
    Prio=(rmp_ptr_t)Word;
    Prio=RMP_MSB_GET(RMP_Bitmap[Prio])+(Prio<<RMP_WORD_ORDER);

    /* See if the current thread is the highest priority one.
     * If yes, place the current at the end of the queue. */
    if(RMP_Thd_Cur==(volatile struct RMP_Thd*)(RMP_Run[Prio].Next))
    {
        RMP_COVERAGE_MARKER();
        RMP_ASSERT(RMP_Thd_Cur->Prio==Prio);
        RMP_List_Del(RMP_Thd_Cur->Run_Head.Prev, RMP_Thd_Cur->Run_Head.Next);
        RMP_List_Ins(&(RMP_Thd_Cur->Run_Head),
                     RMP_Run[Prio].Prev,
                     &(RMP_Run[Prio]));
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Replenish timeslices for the old thread and switch to the new one */
    RMP_Thd_Cur->Slice_Left=RMP_Thd_Cur->Slice;
    RMP_Thd_Cur=(volatile struct RMP_Thd*)(RMP_Run[Prio].Next);

    /* Load the SP value from thread structure */
    RMP_SP_Cur=RMP_Thd_Cur->Stack;
    
    /* Load potential extra context and do scheduler hook */
#if(RMP_HOOK_EXTRA!=0U)
    RMP_Ctx_Load();
    RMP_Sched_Hook();
#endif
}
/* End Function:_RMP_Run_High ************************************************/

/* Function:_RMP_Tim_Handler **************************************************
Description : The system tick timer interrupt routine.
Input       : rmp_ptr_t Slice - How many timeslices have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tim_Handler(rmp_ptr_t Slice)
{
    volatile struct RMP_Thd* Thread;
    rmp_ptr_t Diff;
    
    /* Increase the timestamp as always */
    RMP_Timestamp+=Slice;
    
    /* See if the current thread expired; if yes, trigger a scheduler event */
    if(Slice>=RMP_Thd_Cur->Slice_Left)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1U;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Thd_Cur->Slice_Left-=Slice;
    }
    
    /* Check if there are any timer events */
    if((&RMP_Delay)!=RMP_Delay.Next)
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        /* If there are overflows, process all pending timers */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COVERAGE_MARKER();
            /* No need to care about scheduler locks if this interrupt can be entered
             * - we have disabled timer and scheduler interrupts in scheduler lock */
            _RMP_Timer_Proc();
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Trigger a context switch if required */
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COVERAGE_MARKER();
        /* Context switch will clear the pend flag */
        _RMP_Yield();
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
#if(RMP_HOOK_EXTRA!=0U)
    RMP_Tim_Hook(Slice);
#endif
}
/* End Function:_RMP_Tim_Handler *********************************************/

/* Function:_RMP_Tim_Elapse ***************************************************
Description : Honor the elapse of time from the last timer firing. This is to be
              called before all potential context switch points to correctly 
              account for the time elapsed before a context switch.
              If a tickless kernel is not desired, this function can be ignored.
Input       : rmp_ptr_t Slice - Number of slices passed since last call of
                                _RMP_Tim_Elapse or _RMP_Tim_Handler.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tim_Elapse(rmp_ptr_t Slice)
{
    /* Increase the timestamp as always */
    RMP_Timestamp+=Slice;

    /* When calculating the next timer interrupt with _RMP_Tim_Future
     * in context switches triggered by means other than timer interrupts, and 
     * when inserting threads in to the delay queue in _RMP_Dly_Ins, we need to
     * update RMP_Timestamp to the latest value manually. This is where
     * _RMP_Tim_Elapse kicks in, and why it must be called before these uses. 
     * It is not required to call this immediately on context switch entry 
     * (which is the required method for RME), but doing so is acceptable. */

    /* When a context switch is pending, there's no need to account for the 
     * timeslice of the current thread anymore, because its timeslice is always
     * replenished immediately. Note that this is VERY different from RME which 
     * needs user-level budget replenishments.
     * That said, between context switches, if we can guarantee that the elapsed
     * time is always tracked exclusively by _RMP_Tim_Handler which accounts for
     * all timeslice losses, and we can load whatever is left into the hardware
     * timer without losing track of timeslices. 
     * We note that there are two usecases for _RMP_Tim_Elapse: (1) in context
     * switch hooks and (2) in delay hooks, and both will always follow a 
     * context switch. Thus the optimization is valid. */
}
/* End Function:_RMP_Tim_Elapse **********************************************/

/* Function:_RMP_Tim_Future ***************************************************
Description : Get the nearest timer interrupt arrival time. This is used to set
              the timer after a context switch or a timer interrupt. If a 
              tickless kernel is not desired, this function can be ignored.
Input       : None.
Output      : None.
Return      : rmp_ptr_t - How many slices to program until the next timeout.
******************************************************************************/
rmp_ptr_t _RMP_Tim_Future(void)
{
    rmp_ptr_t Diff;
    rmp_ptr_t Value;
    volatile struct RMP_Thd* Thread;
    
    /* What is the current thread's timeout value? */
    Value=RMP_Thd_Cur->Slice_Left;
    
    /* What is the nearest timer timeout value? */
    if(RMP_Delay.Next!=&RMP_Delay)
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
        
        /* Detect possible overflows - trigger timer interrupt ASAP */
        Diff=RMP_DLY_DIFF(Thread->Timeout);
        if(RMP_DIFF_OVF(Diff))
        {
            RMP_COVERAGE_MARKER();
            Value=1U;
        }
        else if(Diff<Value)
        {
            RMP_COVERAGE_MARKER();
            Value=Diff;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    return Value;
}
/* End Function:_RMP_Tim_Future **********************************************/

/* Function:_RMP_Tim_Idle *****************************************************
Description : See if the timer could be idle. When both conditions below are
              met, we could turn off all kernel clock sources altogether to
              achieve the lowest possible power:
              1. The only running thread is the initial idle thread.
              2. There are no outstanding delay timers.
Input       : None.
Output      : None.
Return      : rmp_ret_t - If yes, 0; else -1.
******************************************************************************/
rmp_ret_t _RMP_Tim_Idle(void)
{
    /* Are there any threads other than the initial idle thread? */
    if(RMP_Run[0].Next!=((volatile struct RMP_List*)&RMP_Init_Thd))
    {
        RMP_COVERAGE_MARKER();
        return -1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    if(RMP_Run[0].Next!=RMP_Run[0].Prev)
    {
        RMP_COVERAGE_MARKER();
        return -1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Are there any pending timers? */
    if(RMP_Delay.Next!=&RMP_Delay)
    {
        RMP_COVERAGE_MARKER();
        return -1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    return 0;
}
/* End Function:_RMP_Tim_Idle ************************************************/

/* Function:_RMP_Run_Ins ******************************************************
Description : Set the thread as ready to schedule. That means, put the thread
              into the runqueue. When this is called, please make sure that
              the scheduler is locked.
              This function will also try to identify if this thread is currently 
              suspended. If yes, it will not be placed into the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Run_Ins(volatile struct RMP_Thd* Thread)
{
    /* No need to operate on suspended threads */
    if((Thread->State&RMP_THD_SUSPENDED)==0U)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue's back */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this priority level as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=RMP_POW2(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Thd_Cur->Prio)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1U;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Run_Ins *************************************************/

/* Function:_RMP_Run_Del ******************************************************
Description : Clear the thread from the runqueue. When this is called, please 
              make sure that the scheduler is locked. This function also checks whether
              the thread is suspended. If yes, it will not remove it from the queue.
Input       : volatile struct RMP_Thd* Thread - The thread to clear from the runqueue.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Run_Del(volatile struct RMP_Thd* Thread)
{
    /* No need to operate on suspended threads */
    if((Thread->State&RMP_THD_SUSPENDED)==0U)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        {
            RMP_COVERAGE_MARKER();
            /* If yes, set the priority level as inactive */
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~RMP_POW2(Thread->Prio&RMP_WORD_MASK);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Thd_Cur)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1U;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Run_Del *************************************************/

/* Function:_RMP_Dly_Ins ******************************************************
Description : Insert the thread into the delay queue, given some timeslices into
              the future. The thread must not be in the run queue any more.
Input       : volatile struct RMP_Thd* Thread - The thread to put into the delay
                                                queue.
              rmp_ptr_t - The timeslices to delay.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Dly_Ins(volatile struct RMP_Thd* Thread,
                  rmp_ptr_t Slice)
{
    rmp_ptr_t Diff;
    volatile struct RMP_List* Trav_Ptr;
    volatile struct RMP_Thd* Trav_Thd;

#if(RMP_HOOK_EXTRA!=0U)
    /* Potentially update the timestamp if we're running tickless */
    RMP_Dly_Hook(Slice);
#endif
    
    /* Find a place to insert this timer */
    Trav_Ptr=RMP_Delay.Next;
    while(Trav_Ptr!=&RMP_Delay)
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        
        /* Overflow possible due to bumpy timestamp updates in tickless kernel - 
         * we need to find one that is greater than us yet is not overflown */
        Diff=RMP_DLY_DIFF(Trav_Thd->Timeout);
        if(RMP_DIFF_OVF(Diff)||(Diff<Slice))
        {
            RMP_COVERAGE_MARKER();
            Trav_Ptr=Trav_Ptr->Next;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            break;
        }
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Timestamp+Slice;
    RMP_List_Ins(&(Thread->Dly_Head), Trav_Ptr->Prev,Trav_Ptr);
}
/* End Function:_RMP_Dly_Ins *************************************************/

/* Function:RMP_Thd_Crt *******************************************************
Description : Create a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The thread structure provided. 
                                                The user should make this allocation
                                                as needed.
              void* Entry - The entry of the thread.
              void* Param - The argument to pass to the thread.
              void* Stack - The stack base of this thread.
              rmp_ptr_t Size - The stack size of this thread.
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slice - The number of timeslices to assign to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread,
                      void* Entry,
                      void* Param,
                      void* Stack,
                      rmp_ptr_t Size,
                      rmp_ptr_t Prio,
                      rmp_ptr_t Slice)
{
    /* Check priority validity */
    if(Prio>=RMP_PREEMPT_PRIO_NUM)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_PRIO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check slice validity */
    if((Slice==0U)||(Slice>=RMP_SLICE_MAX))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Create the thread and insert it into the list */
    Thread->Prio=Prio;
    Thread->Slice=Slice;
    Thread->Slice_Left=Slice;
    
    /* Initialize its stack and sending list */
    Thread->Stack=_RMP_Stack_Init((rmp_ptr_t)Stack, Size, 
                                  (rmp_ptr_t)Entry, (rmp_ptr_t)Param);
    RMP_List_Crt(&(Thread->Snd_List));
    
    /* Notify the scheduler that we have created something new, also check locks */
    Thread->State=RMP_THD_RUNNING;
    _RMP_Run_Ins(Thread);
    
    RMP_Sched_Unlock();

    return 0;
}
/* End Function:RMP_Thd_Crt **************************************************/

/* Function:RMP_Thd_Del *******************************************************
Description : Delete a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Del(volatile struct RMP_Thd* Thread)
{
    rmp_ptr_t State;
    volatile struct RMP_Thd* Release;
    
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if anyone waiting to send to this thread. If there is, release all these threads */
    while(&(Thread->Snd_List)!=Thread->Snd_List.Next)
    {
        Release=(volatile struct RMP_Thd*)(Thread->Snd_List.Next);
        RMP_List_Del(Release->Run_Head.Prev, Release->Run_Head.Next);
        if(RMP_THD_STATE(Release->State)==RMP_THD_SNDDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Release->Dly_Head.Prev, Release->Dly_Head.Next);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        RMP_THD_STATE_SET(Release->State, RMP_THD_RUNNING);
        /* Set ready if not suspended */
        _RMP_Run_Ins(Release);
        Release->Retval=RMP_ERR_OPER;
    }
    
    /* See what is its state */
    State=RMP_THD_STATE(Thread->State);
    /* Clear ready if not suspended */
    if(State==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Run_Del(Thread);
    }
    /* Do nothing if it is just blocked on receive */
    else if(State==RMP_THD_RCVBLK)
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    /* Unblock it if it was blocked on other stuff */
    else if((State==RMP_THD_SNDBLK)||(State==RMP_THD_SEMBLK))
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
    }
    else if((State==RMP_THD_RCVDLY)||(State==RMP_THD_DELAYED))
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
    }
    else if((State==RMP_THD_SNDDLY)||(State==RMP_THD_SEMDLY))
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
        RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
    }
    /* Can't be in this state */
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_ASSERT(0);
    }

    /* Set return value to failure anyway */
    Thread->Retval=RMP_ERR_OPER;
    Thread->State=RMP_THD_FREE;
    
    /* If we are deleting ourself, a schedule must be pending at this point */
    RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
    
    RMP_Sched_Unlock();
    
    /* Can't reach here if we're deleting ourself */
    if(Thread==RMP_Thd_Cur)
    {
        RMP_COVERAGE_MARKER();
        RMP_ASSERT(0);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    return 0;
}
/* End Function:RMP_Thd_Del **************************************************/

/* Function:RMP_Thd_Set *******************************************************
Description : Change the priority or timeslice of a real-time thread. If one of
              the changes is not desired, just leave it to RMP_PREEMPT_PRIO_NUM
              or RMP_SLICE_MAX.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
              rmp_ptr_t Prio - The priority of the thread.
              rmp_ptr_t Slice - The new timeslice value for this thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread,
                      rmp_ptr_t Prio,
                      rmp_ptr_t Slice)
{
    /* Check slice validity */
    if(Slice==0U)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the thread is in running state */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        
        /* See if we are gonna change one of it or both */
        if(Prio<RMP_PREEMPT_PRIO_NUM)
        {
            RMP_COVERAGE_MARKER();
            if(Thread->Prio!=Prio)
            {
                RMP_COVERAGE_MARKER();
                /* It doesn't matter whether this is suspended or not. 
                 * If suspended, the operations will not be conducted. */
                _RMP_Run_Del(Thread);
                Thread->Prio=Prio;
                _RMP_Run_Ins(Thread);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COVERAGE_MARKER();
            Thread->Slice=Slice;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        if(Prio<RMP_PREEMPT_PRIO_NUM)
        {
            RMP_COVERAGE_MARKER();
            Thread->Prio=Prio;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COVERAGE_MARKER();
            Thread->Slice=Slice;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Set **************************************************/

/* Function:RMP_Thd_Suspend ***************************************************
Description : Suspend the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Suspend(volatile struct RMP_Thd* Thread)
{
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Suspend it */
    if((Thread->State&RMP_THD_SUSPENDED)!=0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Only when it is running do we clear this. If we are clearing this, it is
     * not suspended, thus the running queue removal is guaranteed to succeed */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Run_Del(Thread);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Mark this as suspended */
    Thread->State|=RMP_THD_SUSPENDED;
    
    /* If we are suspending ourself, a schedule must be pending at this point */
    RMP_ASSERT((Thread!=RMP_Thd_Cur)||(RMP_Sched_Pend!=0U));
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Suspend **********************************************/

/* Function:RMP_Thd_Resume ****************************************************
Description : Resume the execution of a real-time thread.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Resume(volatile struct RMP_Thd* Thread)
{
    rmp_ret_t Retval;
    
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the thread is suspended, if not, then throw an error */
    if((Thread->State&RMP_THD_SUSPENDED)!=0U)
    {
        RMP_COVERAGE_MARKER();
        
        /* Suspended */
        Thread->State&=~RMP_THD_SUSPENDED;
        /* Only when it is running will we put it back. It can't be suspended here, 
         * so the set ready operation will surely put it back */
        if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Run_Ins(Thread);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        Retval=0;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Retval=RMP_ERR_STATE;
    }
    
    RMP_Sched_Unlock();

    return Retval;
}
/* End Function:RMP_Thd_Resume ***********************************************/

/* Function:RMP_Thd_Snd *******************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation can potentially block.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
              rmp_ptr_t Slice - The timeslice to wait, if the mailbox is already full.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread,
                      rmp_ptr_t Data,
                      rmp_ptr_t Slice)
{
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Are we sending to ourself? This is not allowed */
    if(RMP_Thd_Cur==Thread)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        /* No action required */
        RMP_COVERAGE_MARKER();
    }
    
    RMP_Thd_Cur->Retval=0;

    /* See if there is already a value in the mailbox, if yes, we block */
    if((Thread->State&RMP_THD_MBOXFUL)!=0U)
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox full, we block, and put ourself into the queue */
        if(Slice==0U)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Unlock();
            return RMP_ERR_OPER;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }

        /* We must be running */
        _RMP_Run_Del(RMP_Thd_Cur);
        RMP_List_Ins(&(RMP_Thd_Cur->Run_Head), Thread->Snd_List.Prev, &(Thread->Snd_List));

        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Thd_Cur, Slice);
            RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_SNDDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_SNDBLK);
        }

        RMP_Thd_Cur->Data=Data;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();
            
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            
            RMP_THD_STATE_SET(Thread->State, RMP_THD_RUNNING);
            /* Set to running if not suspended */
            _RMP_Run_Ins(Thread);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }
    
    RMP_Sched_Unlock();

    return RMP_Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Snd **************************************************/

/* Function:RMP_Thd_Snd_ISR ***************************************************
Description : Send to a real-time thread's mailbox. If the mailbox is full, then
              this operation will just fail. This function can only be called from
              an ISR whose priority is below or equal to the context switch handler's.
              We do not check whether the scheduler is locked; if we are calling this
              function, we're pretty sure that it's not.
Input       : volatile struct RMP_Thd* Thread - The thread structure of the 
                                                thread to send to.
              rmp_ptr_t Data - The data to send to that thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread,
                          rmp_ptr_t Data)
{
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* See if there is already a value in the mailbox, if yes, we abort */
    if((Thread->State&RMP_THD_MBOXFUL)!=0U)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();

            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            
            RMP_THD_STATE_SET(Thread->State, RMP_THD_RUNNING);

            /* Set to running if not suspended */
            _RMP_Run_Ins(Thread);

            /* Trigger a context switch if required */
            if(RMP_Sched_Pend!=0U)
            {
                RMP_COVERAGE_MARKER();
                /* Context switch will clear the pend flag */
                _RMP_Yield();
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
}
/* End Function:RMP_Thd_Snd_ISR **********************************************/

/* Function:RMP_Thd_Rcv *******************************************************
Description : Receive a message from our own mailbox, and this is blocking.
Input       : rmp_ptr_t Slice - The timeslices to wait, if the mailbox is empty.
Output      : rmp_ptr_t* Data - The pointer to put the data to.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data,
                      rmp_ptr_t Slice)
{
    volatile struct RMP_Thd* Sender;
    
    /* Check data pointer */
    if(Data==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Is there any other guy waiting on us? If there is, unblock it and set it running */
    Sender=RMP_NULL;
    if(&(RMP_Thd_Cur->Snd_List)!=RMP_Thd_Cur->Snd_List.Next)
    {
        RMP_COVERAGE_MARKER();
        /* Read the data */
        Sender=(volatile struct RMP_Thd*)(RMP_Thd_Cur->Snd_List.Next);
        RMP_List_Del(Sender->Run_Head.Prev, Sender->Run_Head.Next);
        *Data=Sender->Data;
        /* Now we unblock it - what state is it in? */
        if((RMP_THD_STATE(Sender->State)==RMP_THD_SNDDLY))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Sender->Dly_Head.Prev,Sender->Dly_Head.Next);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        RMP_THD_STATE_SET(Sender->State, RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Run_Ins(Sender);
    }

    /* Check if there is a value in our mailbox. If yes, we return with that value */
    if((RMP_Thd_Cur->State&RMP_THD_MBOXFUL)!=0U)
    {
        RMP_COVERAGE_MARKER();
        /* Get the value from mailbox */
        *Data=RMP_Thd_Cur->Mailbox;
        /* See if we unblocked a sender. If yes, we place the new value into 
         * our mailbox and it is still full */
        if(Sender!=RMP_NULL)
        {
            RMP_COVERAGE_MARKER();
            RMP_Thd_Cur->Mailbox=Sender->Data;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_Thd_Cur->State&=~RMP_THD_MBOXFUL;
        }
        
        RMP_Sched_Unlock();
        return 0;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Box empty. Do we have somebody waiting? */
        if(Sender!=RMP_NULL)
        {
            RMP_COVERAGE_MARKER();
            RMP_Thd_Cur->Mailbox=Sender->Data;
            RMP_Sched_Unlock();
            return 0;
        }
        /* No sender waiting on us and box empty, we need to block */
        else
        {
            RMP_COVERAGE_MARKER();
            
            if(Slice==0U)
            {
                RMP_COVERAGE_MARKER();
                RMP_Sched_Unlock();
                return RMP_ERR_OPER;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }

            /* We must be running and not suspended so we will surely be deleted from queue */
            _RMP_Run_Del(RMP_Thd_Cur);

            if(Slice<RMP_SLICE_MAX)
            {
                RMP_COVERAGE_MARKER();
                _RMP_Dly_Ins(RMP_Thd_Cur, Slice);
                RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_RCVDLY);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_RCVBLK);
            }

            RMP_Sched_Unlock();

            /* Dummy read - to separate the lock & unlock. If the compiler optimizes these two
             * functions(inline them) on some architectures sometimes we never block. */
            *Data=RMP_Thd_Cur->Mailbox;
            
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Sched_Lock();
            *Data=RMP_Thd_Cur->Mailbox;
            RMP_Thd_Cur->State&=~RMP_THD_MBOXFUL;
        }
    }
    
    RMP_Sched_Unlock();
    return RMP_Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Rcv **************************************************/

/* Function:RMP_Thd_Delay *****************************************************
Description : Delay the execution of a real-time thread.
Input       : rmp_ptr_t Slice - The number of timeslices to delay.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Delay(rmp_ptr_t Slice)
{
    /* Check slice validity */
    if((Slice==0U)||(Slice>=RMP_SLICE_MAX))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();

    /* We must be running and not suspended so we will be out of running queue */
    _RMP_Run_Del(RMP_Thd_Cur);
    RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_DELAYED);
    _RMP_Dly_Ins(RMP_Thd_Cur, Slice);

    RMP_Thd_Cur->Retval=0;
    RMP_Sched_Unlock();
    /* Need to return if successful or not */
    return RMP_Thd_Cur->Retval;
}
/* End Function:RMP_Thd_Delay ************************************************/

/* Function:RMP_Thd_Cancel ****************************************************
Description : Cancel the real-time thread from a previous delay.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Cancel(volatile struct RMP_Thd* Thread)
{
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the thread is in use, and whether it is delayed */
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_DELAYED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Delete it from the delay list */
    RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
    RMP_THD_STATE_SET(Thread->State, RMP_THD_RUNNING);
    /* Set to running if not suspended */
    _RMP_Run_Ins(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Thd_Cancel ***********************************************/

/* Function:RMP_Thd_Loop ******************************************************
Description : Enter a useless loop to waste some time. Can be used when the
              scheduler is locked. The delay caused by each loop is chip and
			  toolchain specific.
Input       : rmp_ptr_t Loop - The number of useless loops to run.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Thd_Loop(rmp_ptr_t Loop)
{
	volatile rmp_ptr_t Current;
	
	/* Just waste some time */
	for(Current=0U;Current<Loop;Current++)
	{
        RMP_COVERAGE_MARKER();
	}
}
/* End Function:RMP_Thd_Loop *************************************************/

/* Function:RMP_Sem_Crt *******************************************************
Description : Create a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The initial number of this semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore,
                      rmp_ptr_t Number)
{
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_CNT_MAX)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Initialize contents */
    Semaphore->Cur_Num=Number;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    Semaphore->State=RMP_SEM_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Crt **************************************************/

/* Function:RMP_Sem_Del *******************************************************
Description : Delete a semaphore in the system.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Del(volatile struct RMP_Sem* Semaphore)
{
    volatile struct RMP_Thd* Thread;
    
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Get rid of all guys waiting on it */
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        Thread=(volatile struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
        
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        
        /* Set to running if not suspended */
        _RMP_Run_Ins(Thread);
        Thread->Retval=RMP_ERR_OPER;
    }
    
    Semaphore->State=RMP_SEM_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Del **************************************************/

/* Function:_RMP_Sem_Pend_Core ************************************************
Description : Pend on the semaphore, trying to get one. This is the core logic.
              When this is entered, the scheduler shall be locked.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t _RMP_Sem_Pend_Core(volatile struct RMP_Sem* Semaphore,
                             rmp_ptr_t Slice)
{
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if we can get one immediately */
    if(Semaphore->Cur_Num!=0U)
    {
        RMP_COVERAGE_MARKER();
        Semaphore->Cur_Num--;
        RMP_Sched_Unlock();
        return (rmp_ret_t)Semaphore->Cur_Num;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* Cannot get one, we need to block */
        if(Slice==0U)
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Unlock();
            return RMP_ERR_OPER;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }

        /* We must be running - place into waitlist now */
        _RMP_Run_Del(RMP_Thd_Cur);
        RMP_List_Ins(&(RMP_Thd_Cur->Run_Head), Semaphore->Wait_List.Prev, &(Semaphore->Wait_List));
        
        if(Slice<RMP_SLICE_MAX)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Thd_Cur, Slice);
            RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_SEMDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Thd_Cur->State, RMP_THD_SEMBLK);
        }
        
        RMP_Thd_Cur->Retval=0;
    }
    
    RMP_Sched_Unlock();

    /* Context switch will happen before this when the scheduler unlocks */
    return RMP_Thd_Cur->Retval;
}
/* End Function:_RMP_Sem_Pend_Core *******************************************/

/* Function:RMP_Sem_Pend ******************************************************
Description : Pend on the semaphore, trying to get one.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore,
                       rmp_ptr_t Slice)
{
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    return _RMP_Sem_Pend_Core(Semaphore, Slice);
}
/* End Function:RMP_Sem_Pend *************************************************/

/* Function:RMP_Sem_Pend_Unlock ***********************************************
Description : Pend on the semaphore, trying to get one. When we enter this
              function, the scheduler shall be locked, and it would be auto-
              unlocked when we exit.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend_Unlock(volatile struct RMP_Sem* Semaphore,
                              rmp_ptr_t Slice)
{
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the scheduler is really locked only once */
    if(RMP_Sched_Lock_Cnt!=1U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        /* No action required */
        RMP_COVERAGE_MARKER();
    }
    
    return _RMP_Sem_Pend_Core(Semaphore, Slice);
}
/* End Function:RMP_Sem_Pend_Unlock ******************************************/

/* Function:RMP_Sem_Abort *****************************************************
Description : Abort the waiting of one thread on a semaphore.
Input       : volatile struct RMP_Thd* Thread - The pointer to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Abort(volatile struct RMP_Thd* Thread)
{
    /* Check thread pointer */
    if(Thread==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    RMP_Sched_Lock();
    
    /* Check if the thread is in use */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_THD;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Is it waiting on a semaphore? If no, we abort and return an error code */
    if((RMP_THD_STATE(Thread->State)!=RMP_THD_SEMBLK)&&
       (RMP_THD_STATE(Thread->State)!=RMP_THD_SEMDLY))
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_STATE;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Waiting for a semaphore. We abort it and return */
    RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
    if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
    /* Set to running if not suspended */
    _RMP_Run_Ins(Thread);
    
    Thread->Retval=RMP_ERR_OPER;
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Abort ************************************************/

/* Function:_RMP_Sem_Unblock **************************************************
Description : Unblock a thread from the semaphore's waitlist.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Sem_Unblock(volatile struct RMP_Sem* Semaphore)
{
    volatile struct RMP_Thd* Thread;
    
    /* Get the next thread in the list */
    Thread=(volatile struct RMP_Thd*)(Semaphore->Wait_List.Next);
    RMP_List_Del(Thread->Run_Head.Prev, Thread->Run_Head.Next);
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
    {
        RMP_COVERAGE_MARKER();
        RMP_List_Del(Thread->Dly_Head.Prev, Thread->Dly_Head.Next);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Set to running if not suspended */
    RMP_THD_STATE_SET(Thread->State, RMP_THD_RUNNING);
    _RMP_Run_Ins(Thread);

    /* Finally, return success on unblock */
    Thread->Retval=0;
}
/* End Function:_RMP_Sem_Unblock *********************************************/

/* Function:RMP_Sem_Post ******************************************************
Description : Post a number of semaphores to the list.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore,
                       rmp_ptr_t Number)
{
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check number validity */
    if(Number==0U)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_CNT_MAX)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Semaphore->Cur_Num+=Number;
    
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0U))
    {
        _RMP_Sem_Unblock(Semaphore);
        Semaphore->Cur_Num--;
    }

    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Sem_Post *************************************************/

/* Function:RMP_Sem_Post_ISR **************************************************
Description : Post a number of semaphores to the list. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore,
                           rmp_ptr_t Number)
{   
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check number validity */
    if(Number==0U)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_CNT_MAX)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Semaphore->Cur_Num+=Number;
    
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0U))
    {
        _RMP_Sem_Unblock(Semaphore);
        Semaphore->Cur_Num--;
    }
    
    /* Trigger a context switch if required */
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COVERAGE_MARKER();
        /* Context switch will clear the pend flag */
        _RMP_Yield();   
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    return 0;
}
/* End Function:RMP_Sem_Post_ISR *********************************************/

/* Function:RMP_Sem_Bcst ******************************************************
Description : Unblock all threads waiting on the semaphore.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of threads unblocked; 
                          or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Bcst(volatile struct RMP_Sem* Semaphore)
{
    rmp_ret_t Number;
    
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Is there any thread waiting on it? If there are, clean them up */
    Number=0;
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        _RMP_Sem_Unblock(Semaphore);
        Number++;
    }

    RMP_Sched_Unlock();
    return Number;
}
/* End Function:RMP_Sem_Bcst *************************************************/

/* Function:RMP_Sem_Bcst_ISR **************************************************
Description : Unblock all threads on the semaphore. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of threads unblocked; 
                          or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Bcst_ISR(volatile struct RMP_Sem* Semaphore)
{   
    rmp_ret_t Number;
    
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the semaphore is in use */
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Is there any thread waiting on it? If there are, clean them up */
    Number=0;
    while(&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)
    {
        _RMP_Sem_Unblock(Semaphore);
        Number++;
    }
    
    /* Trigger a context switch if required */
    if(RMP_Sched_Pend!=0U)
    {
        RMP_COVERAGE_MARKER();
        /* Context switch will clear the pend flag */
        _RMP_Yield();
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    return Number;
}
/* End Function:RMP_Sem_Bcst_ISR *********************************************/

/* Function:RMP_Sem_Cnt *******************************************************
Description : Get the number of semaphores.
Input       : volatile struct RMP_Sem* Semaphore - The pointer to the semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, the number of semaphores; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Cnt(volatile struct RMP_Sem* Semaphore)
{
    rmp_ret_t Count;
    
    /* Check semaphore pointer */
    if(Semaphore==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    RMP_Sched_Lock();
    
    /* Check if the semaphore is in use */
	if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_SEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Count=(rmp_ret_t)(Semaphore->Cur_Num);
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Sem_Cnt **************************************************/

/* Function:RMP_Init **********************************************************
Description : The entry of the user thread. This is the first user thread that
              will be created.
              The user threads should never return.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init(void)
{
    RMP_Sched_Lock();
    /* Perform per-platform initialization */
    _RMP_Plat_Hook();
    /* Create all other kernel objects here */
    RMP_Init_Hook();
    RMP_Sched_Unlock();
    
    while(1)
    {
        RMP_Init_Idle();
    }
}
/* End Function:RMP_Init *****************************************************/

/* Function:main **************************************************************
Description : The entry of the operating system. This function is for 
              compatibility with the toolchains.
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
    rmp_ptr_t Count;

#ifdef RMP_COVERAGE_LINE_NUM
    /* Initialize coverage markers if coverage enabled */
    for(Count=0U;Count<RMP_COVERAGE_WORD_NUM;Count++)
    {
        RMP_Coverage[Count]=0U;
    }
#endif
    
    /* Initialize the kernel data structures first */
    _RMP_Lowlvl_Init();
    
#if(RMP_HOOK_EXTRA!=0U)
    RMP_Start_Hook();
#endif

    RMP_Timestamp=0U;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0U;
    RMP_Sched_Pend=0U;
    RMP_Timer_Pend=0U;

    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0U;Count<RMP_PREEMPT_PRIO_NUM;Count++)
    {
        RMP_List_Crt(&RMP_Run[Count]);
    }
    for(Count=0U;Count<RMP_PRIO_WORD_NUM;Count++)
    {
        RMP_Bitmap[Count]=0U;
    }

    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0U;
    RMP_Init_Thd.Slice=65500U;
    RMP_Init_Thd.Slice_Left=65500U;
    RMP_Init_Thd.State=RMP_THD_RUNNING;
    RMP_Init_Thd.Stack=RMP_INIT_STACK;

    /* Initialize sending list */
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));

    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1U;
    
    /* Set current thread and stack */
    RMP_Thd_Cur=(volatile struct RMP_Thd*)&RMP_Init_Thd;
    RMP_SP_Cur=RMP_Init_Thd.Stack;
    
    /* Now jump to the init thread and will never return; stack
     * initialization is unnecessary for the first thread */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
    
    return 0;
}
/* End Function:main *********************************************************/

/* Function:RMP_Mem_Init ******************************************************
Description : Initialize a trunk of memory as the memory pool. The TLSF allocator's
              FLI will be decided upon the memory block size. Memory allocation does
              not lock the scheduler by itself; it is up to you to decide whether a
              scheduler lock is needed.
              The TLSF memory allocator consists of FLI, SLI and allocatable memory. 
              The FLI is grouped by 2^n, and the SLI segregates the FLI section by a
              power of 2, i.e. 8 or 16. Thus, when we need a memory block, we will 
              try to find it in the corresponding FLI, and then the SLI. (You can 
              consider the FLI-SLI segregation as a two-dimensional matrix.) Then 
              (1) If the SLI has no allocatable blocks, we will allocate some
                  from the nearest bigger block.
              (2) If there is some block from the SLI block, allocate the memory
                  size and put the residue memory into the corresponding FLI and
                  SLI area.
              When freeing memory, the adjacent free memory blocks will automatically
              merge.
              In the system, the FLI is variable and the SLI is fixed to 8.
              The FLI has a miniumum block size of 64 Byte(If the allocated size
              is always smaller than 64 bits, then there's no need to use DSA.)
              To make sure that it is like this, we set the smallest allocatable
              size to 64B. In addition, we set the alignment to 8.
              [FLI]:
              .....    6       5      4       3         2        1         0
                     8K-4K   4K-2K  2K-1K  1K-512B  511-256B  255-128B  127-64B
              For example, when a memory block is 720 byte, then it should be in
              FLI=3,SLI=3.
              When a lower FLI has no blocks for allocation, it will "borrow"
              a block from the nearest FLI block that is big enough.
Input       : volatile void* Pool - The start address of the memory pool, word-aligned.
              rmp_ptr_t Size - The size of the memory pool, word-aligned. Must be 
                               more than 1024 machine words, or pool creation will not
                               be successful.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else an error code.
******************************************************************************/
rmp_ret_t RMP_Mem_Init(volatile void* Pool,
                       rmp_ptr_t Size)
{
    rmp_ptr_t FLI_Cnt;
    rmp_ptr_t Offset;
    rmp_ptr_t Bitmap_Size;
    volatile struct RMP_Mem* Mem;
    
    /* See if the memory pool is large enough to enable dynamic allocation - at
     * least 1024 machine words or pool initialization will be refused */
    if((Pool==RMP_NULL)||(Size<(1024U*sizeof(rmp_ptr_t)))||((((rmp_ptr_t)Pool)+Size)<Size))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the address and size is word-aligned - divisions will be optimized out */
    if(((((rmp_ptr_t)Pool)%sizeof(rmp_ptr_t))!=0U)||((Size%sizeof(rmp_ptr_t))!=0U))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MEM;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_GET(Size-sizeof(struct RMP_Mem))-6U+1U;
    
    /* Decide the location of the bitmap */
    Offset=sizeof(struct RMP_Mem);
    /* Initialize the bitmap - how many words are needed for this bitmap? */
    Bitmap_Size=RMP_MEM_WORD_NUM(Mem->FLI_Num);
    for(FLI_Cnt=0U;FLI_Cnt<Bitmap_Size;FLI_Cnt++)
    {
        Mem->Bitmap[FLI_Cnt]=0U;
    }
    Bitmap_Size*=sizeof(rmp_ptr_t);
    
    /* Decide the location of the allocation table - "-sizeof(rmp_ptr_t)" is
     * because we defined the length=1 in our struct already */
    Offset+=Bitmap_Size-sizeof(rmp_ptr_t);
    Mem->Table=(struct RMP_List*)(((rmp_ptr_t)Mem)+Offset);
    /* Initialize the allocation table */
    for(FLI_Cnt=0U;FLI_Cnt<Mem->FLI_Num;FLI_Cnt++)
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 0U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 1U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 2U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 3U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 4U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 5U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 6U)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt, 7U)]));
    }
    
    /* Calculate the offset of the actual allocatable memory - each FLI have
     * 8 SLIs, and each SLI has a corresponding table header */
    Offset+=sizeof(struct RMP_List)*8U*Mem->FLI_Num;
    Mem->Base=((rmp_ptr_t)Mem)+Offset;
    
    /* Initialize the first big block */
    _RMP_Mem_Block((struct RMP_Mem_Head*)(Mem->Base), Size-Offset);
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool,(struct RMP_Mem_Head*)(Mem->Base));
    
    return 0;
}
/* End Function:RMP_Mem_Init *************************************************/

/* Function:_RMP_Mem_Block ****************************************************
Description : Make a memory block from the memory trunk. The memory block is
              always free when created. No parameter check performed here.
Input       : volatile struct RMP_Mem_Head* Addr - The start address of the
                                                   memory block, word-aligned.
              rmp_ptr_t Size - The size of the memory block, word-aligned.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Block(volatile struct RMP_Mem_Head* Addr,
                    rmp_ptr_t Size)
{
    volatile struct RMP_Mem_Head* Mem_Head;
    
    /* Get the big memory block's size and position */
    Mem_Head=Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
    Mem_Head->Tail=(volatile struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
    Mem_Head->Tail->Head=Mem_Head;
}
/* End Function:_RMP_Mem_Block ***********************************************/

/* Function:_RMP_Mem_Ins ******************************************************
Description : The memory insertion function, to insert a certain memory block
              into the corresponding FLI and SLI slot.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Mem_Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Ins(volatile void* Pool,
                  volatile struct RMP_Mem_Head* Mem_Head)
{
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_GET(Size)-6U;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3U))&0x07U;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=RMP_POW2(Level&RMP_WORD_MASK);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
}
/* End Function:_RMP_Mem_Ins *************************************************/

/* Function:_RMP_Mem_Del ******************************************************
Description : The memory deletion function, to delete a certain memory block
              from the corresponding FLI and SLI class.
Input       : volatile void* Pool - The memory pool.
              volatile struct RMP_Mem_Head* Mem_Head - The pointer to the memory block.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Mem_Del(volatile void* Pool,
                  volatile struct RMP_Mem_Head* Mem_Head)
{
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    rmp_ptr_t Level;
    rmp_ptr_t Size;
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_GET(Size)-6U;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3U))&0x07U;
    /* Calculate the bit position */
    Level=RMP_MEM_POS(FLI_Level,SLI_Level);
    /* Get the slot */
    Slot=&(Mem->Table[Level]);

    /* Delete the node now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last blockm need to clear the flag */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]&=~RMP_POW2(Level&RMP_WORD_MASK);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:_RMP_Mem_Del *************************************************/

/* Function:_RMP_Mem_Search ***************************************************
Description : The TLSF memory searcher.
Input       : volatile void* Pool - The memory pool.
              rmp_ptr_t Size - The memory size, must be bigger than 64. This must be
                               guaranteed before calling this function or an error
                               will unavoidably occur.
Output      : rmp_ptr_t* FLI_Level - The FLI level found.
              rmp_ptr_t* SLI_Level - The SLI level found.
Return      : rmp_ret_t - If successful, 0; else -1 for failure.
******************************************************************************/
rmp_ret_t _RMP_Mem_Search(volatile void* Pool,
                          rmp_ptr_t Size,
                          rmp_ptr_t* FLI_Level,
                          rmp_ptr_t* SLI_Level)
{
    rmp_ptr_t Level;
    rmp_ptr_t Word;
    rmp_ptr_t Limit;
    rmp_ptr_t LSB;
    rmp_ptr_t FLI_Search;
    rmp_ptr_t SLI_Search;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64=2^6 */
    FLI_Search=RMP_MSB_GET(Size)-6U;
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * one of the levels, then we don't need to jump to the next level and can fit directly */
    SLI_Search=(Size>>(FLI_Search+3U))&0x07U;
    if(Size!=(RMP_POW2(FLI_Search+3U)*(SLI_Search+8U)))
    {
        RMP_COVERAGE_MARKER();
        SLI_Search++;
        
        /* If the SLI level is the largest of the SLI levels, then jump to the next FLI level */
        if(SLI_Search==8U)
        {
            RMP_COVERAGE_MARKER();
            FLI_Search+=1U;
            SLI_Search=0U;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the FLI level is over the boundary */
    Mem=(volatile struct RMP_Mem*)Pool;
    if(FLI_Search>=Mem->FLI_Num)
    {
        RMP_COVERAGE_MARKER();
        return -1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Try to find the word that contains this level, then right shift away the
     * lower levels to extract the ones that can satisfy this alocation request */
    Level=RMP_MEM_POS(FLI_Search,SLI_Search);
    Word=Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK);
    
    /* If there's at least one block that matches the query, return the level */
    if(Word!=0U)
    {
        RMP_COVERAGE_MARKER();
        LSB=RMP_LSB_GET(Word);
        /* Need to compensate for the lower levels that were shifted away */
        Level=(Level&(~RMP_WORD_MASK))+(LSB+(Level&RMP_WORD_MASK));
        *FLI_Level=Level>>3U;
        *SLI_Level=Level&0x07U;
        return 0;
    }
    /* No one exactly fits, compute the size of bitmap and look through it */
    else
    {
        RMP_COVERAGE_MARKER();
        Limit=RMP_MEM_WORD_NUM(Mem->FLI_Num);
        /* From the next word, query one by one */
        for(Word=(Level>>RMP_WORD_ORDER)+1U;Word<Limit;Word++)
        {
            /* If the level has blocks of one FLI level */
            if(Mem->Bitmap[Word]!=0U)
            {
                RMP_COVERAGE_MARKER();
                /* Find the actual level */ 
                LSB=RMP_LSB_GET(Mem->Bitmap[Word]);
                *FLI_Level=((Word<<RMP_WORD_ORDER)+LSB)>>3U;
                *SLI_Level=LSB&0x07U;
                return 0;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
    }

    /* Search failed */
    return -1;
}
/* End Function:_RMP_Mem_Search **********************************************/

/* Function:RMP_Malloc ********************************************************
Description : Allocate some memory from a designated memory pool.
Input       : volatile void* Pool - The pool to allocate from.
              rmp_ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available, 0 is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool,
                 rmp_ptr_t Size)
{    
    rmp_ptr_t FLI_Level;
    rmp_ptr_t SLI_Level;
    volatile struct RMP_Mem* Mem;
    rmp_ptr_t Old_Size;
    volatile struct RMP_Mem_Head* Mem_Head;
    rmp_ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    rmp_ptr_t New_Size;
    
    if((Pool==RMP_NULL)||(Size==0U))
    {
        RMP_COVERAGE_MARKER();
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Rounded_Size=RMP_ROUND_UP(Size, 3U);
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64U)?Rounded_Size:64U;

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool, Rounded_Size, &FLI_Level, &SLI_Level)!=0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
    /* There is such block. Get it and delete it from the TLSF list. */
    Mem_Head=(volatile struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level, SLI_Level)].Next);
    _RMP_Mem_Del(Pool, Mem_Head);

    /* Allocate and calculate if the space left could be big enough to be a new 
     * block. If so, we will put the block back into the TLSF table */
    New_Size=((rmp_ptr_t)(Mem_Head->Tail))-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
    if(New_Size>=(sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
        New_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

        _RMP_Mem_Block(Mem_Head, Old_Size);
        _RMP_Mem_Block(New_Mem, New_Size);

        /* Put the extra block back */
        _RMP_Mem_Ins(Pool, New_Mem);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Mark the block as in use */
    Mem_Head->State=RMP_MEM_USED;

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
}
/* End Function:RMP_Malloc ***************************************************/

/* Function:RMP_Free **********************************************************
Description : Free allocated memory, for system use mainly. It will free memory 
              in the name of a certain process, specified by the PID.
Input       : volatile void* Pool - The pool to free to.
              void* Mem_Ptr - The pointer returned by "RMP_Malloc".
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Free(volatile void* Pool,
              void* Mem_Ptr)
{
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_Mem_Head* Mem_Head;
    volatile struct RMP_Mem_Head* Left_Head;
    volatile struct RMP_Mem_Head* Right_Head;
    rmp_ptr_t Merge_Left;

    /* Check if pointer is null */
    if((Pool==RMP_NULL)||(Mem_Ptr==RMP_NULL))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    Mem_Head=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
    /* See if the block can really be freed */
    if(Mem_Head->State==RMP_MEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Mark it as free */
    Mem_Head->State=RMP_MEM_FREE;
    
    /* Now check if we can merge it with the higher blocks */
    Right_Head=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
    if(((rmp_ptr_t)Right_Head)!=(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COVERAGE_MARKER();
        /* If this one is unoccupied */
        if((Right_Head->State)==RMP_MEM_FREE)
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Right_Head);
            _RMP_Mem_Block(Mem_Head,
                           ((rmp_ptr_t)(Right_Head->Tail))+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Mem_Head);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Now check if we can merge it with the lower blocks */
    Merge_Left=0U;
    Left_Head=((volatile struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Tail)))->Head;
    if((rmp_ptr_t)Mem_Head!=Mem->Base)
    {
        RMP_COVERAGE_MARKER();

        /* If this one is unoccupied */
        if(Left_Head->State==RMP_MEM_FREE)
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool, Left_Head);
            _RMP_Mem_Block(Left_Head,
                           (rmp_ptr_t)((rmp_ptr_t)(Mem_Head->Tail)+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Left_Head));

            /* We have completed the merge here and the original block has destroyed.
             * Thus there's no need to insert it into the list again */
            Merge_Left=1U;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* If we did not merge it with the left-side blocks, insert the original pointer's block 
     * into the TLSF table(Merging with the right-side one won't disturb this) */
    if(Merge_Left==0U)
    {
        RMP_COVERAGE_MARKER();
        _RMP_Mem_Ins(Pool, Mem_Head);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        _RMP_Mem_Ins(Pool, Left_Head);
    }
}
/* End Function:RMP_Free *****************************************************/

/* Function:RMP_Realloc *******************************************************
Description : Expand or shrink an allocation to the desired size. The behavior
              of this function equals RMP_Malloc if the Mem_Ptr passed in is 0,
              or RMP_Free if the Size passed in is 0.
Input       : volatile void* Pool - The pool to reallocate from.
              void* Mem_Ptr - The old memory block to expand.
              rmp_ptr_t Size - The size of the RAM needed to resize to.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available or an
                      error occurred, 0 is returned.
******************************************************************************/
void* RMP_Realloc(volatile void* Pool,
                  void* Mem_Ptr,
                  rmp_ptr_t Size)
{
    /* The size of the original memory block */
    rmp_ptr_t Mem_Size;
    /* The rounded size of the new memory request */
    rmp_ptr_t Rounded_Size;
    rmp_ptr_t Count;
    /* The pointer to the pool */
    volatile struct RMP_Mem* Mem;
    /* The head of the old memory */
    volatile struct RMP_Mem_Head* Mem_Head;
    /* The right-side block head */
    volatile struct RMP_Mem_Head* Right_Head;
    /* The pointer to the residue memory head */
    volatile struct RMP_Mem_Head* Res_Mem;
    /* The new memory block */
    void* New_Mem;
    /* The size of the memory block including the header sizes */
    rmp_ptr_t Old_Size;
    /* The size of the residue memory block including the header sizes */
    rmp_ptr_t Res_Size;
    
    /* Check if no pool present */
    if(Pool==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Are we passing in a NULL pointer? */
    if(Mem_Ptr==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_Malloc(Pool,Size);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Is the size passed in zero? If yes, we free directly - this is a little different
     * than standard realloc where you get a "0"-sized realloc-able memory trunk. */
    if(Size==0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Free(Pool, Mem_Ptr);
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
    {
        RMP_COVERAGE_MARKER();
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Yes, get the location of the header of the memory */
    Mem_Head=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
    /* See if the block can really be realloced */
    if(Mem_Head->State==RMP_MEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        return RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Rounded_Size=RMP_ROUND_UP(Size, 3U);
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64U)?Rounded_Size:64U;
    
    Mem_Size=((rmp_ptr_t)Mem_Head->Tail)-((rmp_ptr_t)Mem_Ptr);
    /* Does the right-side head exist at all? */
    Right_Head=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
    if(((rmp_ptr_t)Right_Head)==(((rmp_ptr_t)Mem)+Mem->Size))
    {
        RMP_COVERAGE_MARKER();
        Right_Head=RMP_NULL;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Are we gonna expand it? */
    if(Mem_Size<Rounded_Size)
    {
        /* Expanding */
        RMP_COVERAGE_MARKER();
        /* Does the right side exist at all? */
        if(Right_Head!=RMP_NULL)
        {
            RMP_COVERAGE_MARKER();
            /* Is it allocated? */
            if(Right_Head->State==RMP_MEM_FREE)
            {
                RMP_COVERAGE_MARKER();
                /* Right-side exists and is free. How big is its usable size? Is it sufficient for our realloc? */
                if((((rmp_ptr_t)Right_Head->Tail)-((rmp_ptr_t)Mem_Ptr))>=Rounded_Size)
                {
                    RMP_COVERAGE_MARKER();
                    /* Remove the right-side from the free list so we can operate on it */
                    _RMP_Mem_Del(Pool, Right_Head);   
                    /* Allocate and calculate if the space left could be big enough to be a new 
                     * block. If so, we will put the block back into the TLSF table */
                    Res_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Ptr)-Rounded_Size;
                    /* Is the residue big enough to be a block? */
                    if(Res_Size>=(sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail)))
                    {
                        RMP_COVERAGE_MARKER();
                        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
                        Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

                        _RMP_Mem_Block(Mem_Head, Old_Size);
                        _RMP_Mem_Block(Res_Mem, Res_Size);

                        /* Put the extra block back */
                        _RMP_Mem_Ins(Pool, Res_Mem);
                    }
                    else
                    {
                        /* Residue too small. Merging the whole thing in is the only option */
                        RMP_COVERAGE_MARKER();
                        Old_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Tail);
                        _RMP_Mem_Block(Mem_Head, Old_Size);
                    }
                    
                    /* Mark the block as in use (making new block clears this flag) */
                    Mem_Head->State=RMP_MEM_USED;
                    /* Return the old pointer because we expanded it */
                    return Mem_Ptr;
                }
                /* Right-side not large enough, have to go malloc then memcpy */
                else
                {
                    RMP_COVERAGE_MARKER();
                    /* No action required */
                }
            }
            /* It is allocated, have to go malloc then memcpy */
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
        /* Right-side doesn't exist, have to go malloc then memcpy */
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        New_Mem=RMP_Malloc(Pool,Rounded_Size);
        /* See if we can allocate this much, if we can't at all, exit */
        if(New_Mem==RMP_NULL)
        {
            RMP_COVERAGE_MARKER();
            return RMP_NULL;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
        
        /* Copy old memory to new memory - we know that it must be aligned to word boundary */
        Mem_Size/=sizeof(rmp_ptr_t);
        for(Count=0U;Count<Mem_Size;Count++)
        {
            ((rmp_ptr_t*)New_Mem)[Count]=((rmp_ptr_t*)Mem_Ptr)[Count];
        }
        
        /* Free old memory then return */
        RMP_Free(Pool,Mem_Ptr);
        return New_Mem;
    }
    /* Shrinking or keeping */
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Are we keeping the size? */
    if(Mem_Size==Rounded_Size)
    {
        RMP_COVERAGE_MARKER();
        return Mem_Ptr;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Does the right side exist at all? */
    if(Right_Head!=RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        /* Is it allocated? */
        if(Right_Head->State==RMP_MEM_FREE)
        {
            /* Right-side not allocated. Need to merge the block */
            RMP_COVERAGE_MARKER();
            /* Remove the right-side from the allocation list so we can operate on it */
            _RMP_Mem_Del(Pool, Right_Head);
            Res_Size=((rmp_ptr_t)(Right_Head->Tail))-((rmp_ptr_t)Mem_Ptr)-Rounded_Size;
            Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
            Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

            _RMP_Mem_Block(Mem_Head, Old_Size);
            _RMP_Mem_Block(Res_Mem, Res_Size);

            /* Put the extra block back */
            _RMP_Mem_Ins(Pool, Res_Mem);
            
            /* Mark the block as in use (making new block clears this flag) */
            Mem_Head->State=RMP_MEM_USED;
            /* Return the old pointer because we shrinked it */
            return Mem_Ptr;
        }
        /* Allocated. Need to see if the residue block itself is large enough to be inserted back */
        else
        {
            RMP_COVERAGE_MARKER();
            /* No action required */
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* The right-side head either does not exist or is allocated. Calculate the resulting residue size */
    Res_Size=Mem_Size-Rounded_Size;
    if(Res_Size<(sizeof(struct RMP_Mem_Head)+64U+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        /* The residue block wouldn't even count as a small one. Do nothing and quit */
        return Mem_Ptr;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* The residue will be big enough to become a standalone block. We need to place it back */ 
    Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
    Res_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);

    _RMP_Mem_Block(Mem_Head, Old_Size);
    _RMP_Mem_Block(Res_Mem, Res_Size);

    /* Put the extra block back */
    _RMP_Mem_Ins(Pool, Res_Mem);
            
    /* Mark the block as in use (making new block clears this flag) */
    Mem_Head->State=RMP_MEM_USED;
    /* Return the old pointer because we shrinked it */
    return Mem_Ptr;
}
/* End Function:RMP_Realloc **************************************************/

/* Function:RMP_Fifo_Crt ******************************************************
Description : Create a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Crt(volatile struct RMP_Fifo* Fifo)
{
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Create linked list */
    RMP_List_Crt(&(Fifo->Head));
    Fifo->Cur_Num=0U;
    Fifo->State=RMP_FIFO_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Crt *************************************************/

/* Function:RMP_Fifo_Del ******************************************************
Description : Delete a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Del(volatile struct RMP_Fifo* Fifo)
{
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* See if the FIFO have any elements */
    if(Fifo->Cur_Num!=0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Mark as free */
    Fifo->State=RMP_FIFO_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Del *************************************************/

/* Function:RMP_Fifo_Read *****************************************************
Description : Read an element from a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : struct RMP_List** Node - The node read.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Read(volatile struct RMP_Fifo* Fifo,
                        volatile struct RMP_List** Node)
{
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the FIFO is empty */
    if(Fifo->Head.Next==&(Fifo->Head))
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* If not, grab one */
    *Node=Fifo->Head.Next;
    RMP_List_Del((*Node)->Prev, (*Node)->Next);
    
    /* The count should not be zero, decrease it */
    RMP_ASSERT(Fifo->Cur_Num!=0U);
    Fifo->Cur_Num--;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Read ************************************************/

/* Function:RMP_Fifo_Write ****************************************************
Description : Write an element to a FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
              volatile struct RMP_List* Node - The node to put into the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Write(volatile struct RMP_Fifo* Fifo,
                         volatile struct RMP_List* Node)
{
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    RMP_Sched_Lock();
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Write to list and increase count */
    RMP_List_Ins(Node, Fifo->Head.Prev, &(Fifo->Head));
    Fifo->Cur_Num++;

    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Fifo_Write ***********************************************/

/* Function:RMP_Fifo_Write_ISR ************************************************
Description : Write an element to a FIFO, from the ISR. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
              volatile struct RMP_List* Node - The node to put into the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Write_ISR(volatile struct RMP_Fifo* Fifo,
                             volatile struct RMP_List* Node)
{
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Write to list and increase count */
    RMP_List_Ins(Node, Fifo->Head.Prev, &(Fifo->Head));
    Fifo->Cur_Num++;

    return 0;
}
/* End Function:RMP_Fifo_Write_ISR *******************************************/

/* Function:RMP_Fifo_Cnt ******************************************************
Description : Get the number of elements in the FIFO.
Input       : volatile struct RMP_Fifo* Fifo - The pointer to the FIFO.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Fifo_Cnt(volatile struct RMP_Fifo* Fifo)
{
    rmp_ret_t Count;
    
    /* Check the FIFO pointer */
    if(Fifo==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the FIFO is in use */
    if(Fifo->State!=RMP_FIFO_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_FIFO;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Count=(rmp_ret_t)(Fifo->Cur_Num);
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Fifo_Cnt *************************************************/

/* Function:RMP_Msgq_Crt ******************************************************
Description : Create a message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Crt(volatile struct RMP_Msgq* Queue)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
		
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* A queue is just a FIFO paired with a counting semaphore */
    RMP_ASSERT(RMP_Sem_Crt(&(Queue->Sem), 0U)==0);
    RMP_ASSERT(RMP_Fifo_Crt(&(Queue->Fifo))==0);
    Queue->State=RMP_MSGQ_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Crt *************************************************/

/* Function:RMP_Msgq_Del ******************************************************
Description : Delete a message queue. Only message queues that are empty may be
              deleted.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Del(volatile struct RMP_Msgq* Queue)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* See if the FIFO could be deleted */
    if(RMP_Fifo_Del(&(Queue->Fifo))<0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Proceed to delete the semaphore */
    RMP_ASSERT(RMP_Sem_Del(&(Queue->Sem))==0);
    Queue->State=RMP_MSGQ_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Del *************************************************/

/* Function:RMP_Msgq_Snd ******************************************************
Description : Send a message to the message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Snd(volatile struct RMP_Msgq* Queue,
                       volatile struct RMP_List* Node)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Note the trick here: we have locked the scheduler, so we're safe to 
     * post the semaphore first. We do this lest the semaphore post may fail
     * due to maximum number limit. Semaphore post is in fact safe to use
     * when inside a critical section. */
    if(RMP_Sem_Post(&(Queue->Sem), 1U)<0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Insert the node */
    RMP_ASSERT(RMP_Fifo_Write(&(Queue->Fifo), Node)==0);
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Msgq_Snd *************************************************/

/* Function:RMP_Msgq_Snd_ISR **************************************************
Description : Send a message to the message queue. This function can only be
              called from an ISR whose priority is below or equal to the context
              switch handler's. We do not check whether the scheduler is locked;
              if we are calling this function, we're pretty sure that it's not.
              We do not check whether the scheduler is locked; if we are calling
              this function, we're pretty sure that it's not.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Snd_ISR(volatile struct RMP_Msgq* Queue,
                           volatile struct RMP_List* Node)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Notify the receiver(s) first */
    if(RMP_Sem_Post_ISR(&(Queue->Sem), 1U)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Then insert node - must be successful */
    RMP_ASSERT(RMP_Fifo_Write_ISR(&(Queue->Fifo), Node)==0);

    return 0;
}
/* End Function:RMP_Msgq_Snd_ISR *********************************************/

/* Function:RMP_Msgq_Rcv ******************************************************
Description : Receive a message from a message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : volatile struct RMP_List** Node - The node received.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Rcv(volatile struct RMP_Msgq* Queue,
                       volatile struct RMP_List** Node,
                       rmp_ptr_t Slice)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Try to grab a semaphore, and only when we succeed do we proceed - 
     * there is the possibility that the whole queue gets deleted, so
     * we need to take that into account. Note that the potential 
     * semaphore-FIFO race here is impossible: unlike condition variables,
     * semaphore is counting, so if a thread grabs a semaphore it is 
     * guaranteed that something is in the FIFO. */
    if(RMP_Sem_Pend_Unlock(&(Queue->Sem), Slice)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Grab the data then - we could have a delete race here */
    if(RMP_Fifo_Read(&(Queue->Fifo), Node)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    return 0;
}
/* End Function:RMP_Msgq_Rcv *************************************************/

/* Function:RMP_Msgq_Cnt ******************************************************
Description : Get the number of nodes in the message queue.
Input       : volatile struct RMP_Msgq* Queue - The pointer to the message queue.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Msgq_Cnt(volatile struct RMP_Msgq* Queue)
{
    rmp_ret_t Count;
    
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Count=RMP_Fifo_Cnt(&(Queue->Fifo));
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Msgq_Cnt *************************************************/

/* Function:RMP_Bmq_Crt *******************************************************
Description : Create a blocking message queue. A blocking message queue is a
              queue that may block senders in addition to receivers.
              It must be pointed out that providing a core-level semaphore
              variant that will block senders if it reaches limit does NOT help
              implementation of this type of queue. A race between (1) mounting
              the actual info block and (2) notifying the receiver will race no
              matter how they are arranged relative to each other. If (1) is 
              before (2), the actual number of messages on the queue may exceed
              limit when multiple senders are present. If (2) is before (1),
              the receiver may be notified before the actual block is mounted.
              Using normal semaphores do not have this issue because a mounting
              permission is always granted before the actual mounting, and the
              notification of the receiver will follow the actual mounting.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Limit - The message number limit.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Crt(volatile struct RMP_Bmq* Queue,
                      rmp_ptr_t Limit)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_BMQ_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* A blocking queue is just a normal queue paired with a message
     * number limiting semaphore */
    if((Limit==0U)||(RMP_Sem_Crt(&(Queue->Sem), Limit)<0))
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    RMP_ASSERT(RMP_Msgq_Crt(&(Queue->Msgq))==0);
    Queue->State=RMP_BMQ_USED;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Bmq_Crt **************************************************/

/* Function:RMP_Bmq_Del *******************************************************
Description : Delete a blocking message queue. Only blocking message queues that
              are empty may be deleted.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Del(volatile struct RMP_Bmq* Queue)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Attempt to delete the message queue first */
    if(RMP_Msgq_Del(&(Queue->Msgq))<0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Proceed to delete the semaphore */
    RMP_ASSERT(RMP_Sem_Del(&(Queue->Sem))==0);
    Queue->State=RMP_BMQ_FREE;
    
    RMP_Sched_Unlock();
    return 0;
}
/* End Function:RMP_Bmq_Del **************************************************/

/* Function:RMP_Bmq_Snd *******************************************************
Description : Send to a blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Snd(volatile struct RMP_Bmq* Queue,
                      volatile struct RMP_List* Node,
                      rmp_ptr_t Slice)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the queue is in use - no lock needed because we have 
     * deletion race protection below: assuming the operations can fail. */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Grab a slot first. If we're unable to do this, we need to exit */
    if(RMP_Sem_Pend(&(Queue->Sem), Slice)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    /* Do the message queue send. This can't fail due to semaphore limit
     * because if it was the case the creation of the blocking message queue
     * won't succeed at first. However, if the message queue is deleted, then
     * this could fail in the middle. */
    if(RMP_Msgq_Snd(&(Queue->Msgq), Node)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    return 0;
}
/* End Function:RMP_Bmq_Snd **************************************************/

/* Function:RMP_Bmq_Snd_ISR ***************************************************
Description : Send to a blocking message queue. Different from the normal 
              version, This function does not block (and instead return failure
              when the queue is full) and can only be called from an ISR whose
              priority is below or equal to the context switch handler's. We do
              not check whether the scheduler is locked; if we are calling this
              function, we're pretty sure that it's not.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Snd_ISR(volatile struct RMP_Bmq* Queue,
                          volatile struct RMP_List* Node)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if we have used the queue up */
    if(Queue->Sem.Cur_Num==0U)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Manually operate the semaphore, then send to message queue */
    Queue->Sem.Cur_Num--;
    RMP_Msgq_Snd_ISR(&(Queue->Msgq), Node);

    return 0;
}
/* End Function:RMP_Bmq_Snd_ISR **********************************************/

/* Function:RMP_Bmq_Rcv *******************************************************
Description : Receive from a blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the queue.
              rmp_ptr_t Slice - The number of slices to wait.
Output      : volatile struct RMP_List** Node - The node received.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Rcv(volatile struct RMP_Bmq* Queue,
                      volatile struct RMP_List** Node,
                      rmp_ptr_t Slice)
{
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check the data pointer */
    if(Node==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Check if the queue is in use - no lock needed because we have 
     * deletion race protection below: assuming the operations can fail */
    if(Queue->State!=RMP_BMQ_USED)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    /* Attempt a message queue receive */
    if(RMP_Msgq_Rcv(&(Queue->Msgq), Node, Slice)<0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
                          
    /* If we're successful, wake up one guy on the send wait list. We don't care 
     * if this fails: if this fails due to queue deletion, that's totally fine.
     * Also there is no possibility that the FIFO will (even transiently) contain
     * more elements than the limit: this only gets posted when the FIFO dequeue
     * is actually performed. */
    RMP_Sem_Post(&(Queue->Sem), 1U);
    
    return 0;
}
/* End Function:RMP_Bmq_Rcv **************************************************/

/* Function:RMP_Bmq_Cnt *******************************************************
Description : Get the number of nodes in the blocking message queue.
Input       : volatile struct RMP_Bmq* Queue - The pointer to the message queue.
Output      : None.
Return      : rmp_ret_t - If successful, the number of nodes; or an error code.
******************************************************************************/
rmp_ret_t RMP_Bmq_Cnt(volatile struct RMP_Bmq* Queue)
{
    rmp_ret_t Count;
    
    /* Check the queue pointer */
    if(Queue==RMP_NULL)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_BMQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    RMP_Sched_Lock();
    
    /* Check if the queue is in use */
    if(Queue->State!=RMP_MSGQ_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Unlock();
        return RMP_ERR_MSGQ;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Count=RMP_Msgq_Cnt(&(Queue->Msgq));
    
    RMP_Sched_Unlock();
    return Count;
}
/* End Function:RMP_Bmq_Cnt **************************************************/

/* Function:RMP_Line **********************************************************
Description : Draw a line given the start and end coordinates.
Input       : rmp_cnt_t Begin_X - The begin point X coordinate.
              rmp_cnt_t Begin_Y - The begin point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Color - The color of the line.
Output      : None.
Return      : None.
******************************************************************************/
#ifdef RMP_POINT
void RMP_Line(rmp_cnt_t Begin_X,
              rmp_cnt_t Begin_Y,
              rmp_cnt_t End_X,
              rmp_cnt_t End_Y,
              rmp_ptr_t Color)
{
    rmp_cnt_t Trav_X;
    rmp_cnt_t Trav_Y;
    rmp_cnt_t Dir_X;
    rmp_cnt_t Dir_Y;
    rmp_cnt_t Error;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    
    /* See if this line is horizontal or vertical. If so we speed it up */
    if(Begin_X==End_X)
    {
        RMP_COVERAGE_MARKER();

        /* Vertical */
        if(Begin_Y>End_Y)
        {
            RMP_COVERAGE_MARKER();
            Dir_Y=End_Y;
            Trav_Y=Begin_Y;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            Dir_Y=Begin_Y;
            Trav_Y=End_Y;
        }
        
        for(Cur_Y=Dir_Y;Cur_Y<=Trav_Y;Cur_Y++)
        {
            RMP_POINT(Begin_X,Cur_Y,Color);
        }
        return;
    }
    else if(Begin_Y==End_Y)
    {
        RMP_COVERAGE_MARKER();

        /* Horizontal */
        if(Begin_X>End_X)
        {
            RMP_COVERAGE_MARKER();
            Dir_X=End_X;
            Trav_X=Begin_X;
        }
        else
        {
            RMP_COVERAGE_MARKER();
            Dir_X=Begin_X;
            Trav_X=End_X;
        }
        
        for(Cur_X=Dir_X;Cur_X<=Trav_X;Cur_X++)
        {
            RMP_POINT(Cur_X,Begin_Y,Color);
        }
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    Error=0;
    /* Get their absolute value, and then draw the line */
    Trav_X=RMP_ABS(Begin_X,End_X);
    Trav_Y=RMP_ABS(Begin_Y,End_Y);

    /* Decide the increment direction */
    if((End_X-Begin_X)>0)
    {
        RMP_COVERAGE_MARKER();
        Dir_X=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Dir_X=-1;
    }

    if((End_Y-Begin_Y)>0)
    {
        RMP_COVERAGE_MARKER();
        Dir_Y=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Dir_Y=-1;
    }

    if(Trav_X>Trav_Y)
    {
        RMP_COVERAGE_MARKER();

        Cur_Y=Begin_Y;
        for(Cur_X=Begin_X;Cur_X!=(End_X+Dir_X);Cur_X+=Dir_X)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_Y;
            if(RMP_SAL(Error,1)>=Trav_X)
            {
                RMP_COVERAGE_MARKER();
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();

        Cur_X=Begin_X;
        for(Cur_Y=Begin_Y;Cur_Y!=(End_Y+Dir_Y);Cur_Y+=Dir_Y)
        {
            RMP_POINT(Cur_X,Cur_Y,Color);
            Error+=Trav_X;
            if(RMP_SAL(Error,1)>=Trav_Y)
            {
                RMP_COVERAGE_MARKER();
                Cur_X+=Dir_X;
                Error-=Trav_Y;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
    }
}
/* End Function:RMP_Line *****************************************************/

/* Function:RMP_Dot_Line ******************************************************
Description : Draw a dotted line given the start and end coordinates.
Input       : rmp_cnt_t Begin_X - The begin point X coordinate.
              rmp_cnt_t Begin_Y - The begin point Y coordinate.
              rmp_cnt_t End_X - The end point X coordinate.
              rmp_cnt_t End_Y - The end point Y coordinate.
              rmp_ptr_t Dot - The color of the dotted line.
              rmp_ptr_t Space - The color of the white space. "RMP_TRANS" for nothing.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Dot_Line(rmp_cnt_t Begin_X,
                  rmp_cnt_t Begin_Y,
                  rmp_cnt_t End_X,
                  rmp_cnt_t End_Y,
                  rmp_ptr_t Dot,
                  rmp_ptr_t Space)
{
    rmp_cnt_t Trav_X;
    rmp_cnt_t Trav_Y;
    rmp_cnt_t Dir_X;
    rmp_cnt_t Dir_Y;
    rmp_cnt_t Error;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;

    Error=0;
    /* Get their absolute value, and then draw the line */
    Trav_X=RMP_ABS(Begin_X,End_X);
    Trav_Y=RMP_ABS(Begin_Y,End_Y);

    /* Decide the increment direction */
    if((End_X-Begin_X)>0)
    {
        RMP_COVERAGE_MARKER();
        Dir_X=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Dir_X=-1;
    }

    if((End_Y-Begin_Y)>0)
    {
        RMP_COVERAGE_MARKER();
        Dir_Y=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        Dir_Y=-1;
    }

    if(Trav_X>Trav_Y)
    {
        RMP_COVERAGE_MARKER();

        Cur_Y=Begin_Y;
        for(Cur_X=Begin_X;Cur_X!=(End_X+Dir_X);Cur_X+=Dir_X)
        {
            /* Draw the dot and the white space alternatively */
            if((Cur_X&0x01)!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_POINT(Cur_X,Cur_Y,Dot);
            }
            else
            {
                RMP_COVERAGE_MARKER();

                if(Space!=RMP_TRANS)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_POINT(Cur_X,Cur_Y,Space);
                }
                else
                {
                    RMP_COVERAGE_MARKER();
                    /* No action required */
                }
            }

            Error+=Trav_Y;
            if(RMP_SAL(Error,1)>=Trav_X)
            {
                RMP_COVERAGE_MARKER();
                Cur_Y+=Dir_Y;
                Error-=Trav_X;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();

        Cur_X=Begin_X;
        for(Cur_Y=Begin_Y;Cur_Y!=(End_Y+Dir_Y);Cur_Y+=Dir_Y)
        {
            /* Draw the dot and the white space alternatively */
            if((((rmp_ptr_t)Cur_Y)&0x01U)!=0U)
            {
                RMP_COVERAGE_MARKER();
                RMP_POINT(Cur_X,Cur_Y,Dot);
            }
            else
            {
                RMP_COVERAGE_MARKER();

                if(Space!=RMP_TRANS)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_POINT(Cur_X,Cur_Y,Space);
                }
                else
                {
                    RMP_COVERAGE_MARKER();
                    /* No action required */
                }
            }

            Error+=Trav_X;
            if(RMP_SAL(Error,1)>=Trav_Y)
            {
                RMP_COVERAGE_MARKER();
                Cur_X+=Dir_X;
                Error-=Trav_Y;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
        }
    }
}
/* End Function:RMP_Dot_Line *************************************************/

/*Begin Function:RMP_Rectangle ************************************************
Description : Draw a rectangle on the screen. You can choose whether the rectangle 
              is filled or not. By setting the fill color to "RMP_TRANS", the 
              rectangle is not filled.
              The border can also be transparent.
Input       : rmp_cnt_t Coord_X - The X position of The top-left corner.
              rmp_cnt_t Coord_Y - The Y position of The top-left corner.
               rmp_cnt_t Length - The length of the rectangle.
              rmp_cnt_t Width - The width of the rectangle. 
              rmp_ptr_t Border - The color of its boundary.
              rmp_ptr_t Fill - The color filled within its boundary.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Rectangle(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width,
                   rmp_ptr_t Border,
                   rmp_ptr_t Fill)
{
    rmp_cnt_t Line_Cnt;
    
    if(Fill!=RMP_TRANS)
    {
        RMP_COVERAGE_MARKER();
        for(Line_Cnt=0;Line_Cnt<Width;Line_Cnt++)
        {
            RMP_Line(Coord_X, Coord_Y+Line_Cnt, Coord_X+Length-1, Coord_Y+Line_Cnt, Fill);
        }
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }

    if(Border!=RMP_TRANS)
    {
        RMP_COVERAGE_MARKER();
        RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, Border);
        RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, Border);
        RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, Border);
        RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, Border);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Rectangle ************************************************/

/* Function:RMP_Round_Rect ****************************************************
Description : Draw a rectangle with rounded corners.
Input       : rmp_cnt_t Coord_X - The X position of The top-left corner.
              rmp_cnt_t Coord_Y - The Y position of The top-left corner.
              rmp_cnt_t Length - The length of the rectangle.
              rmp_cnt_t Width - The width of the rectangle. 
              rmp_cnt_t Round - The radius of the round corner. 
              rmp_ptr_t Color - The color of the rectangle.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Round_Rect(rmp_cnt_t Coord_X,
                    rmp_cnt_t Coord_Y,
                    rmp_cnt_t Length,
                    rmp_cnt_t Width,
                    rmp_cnt_t Round,
                    rmp_ptr_t Color)
{
    rmp_cnt_t Cir_X_0;
    rmp_cnt_t Cir_X_1;
    rmp_cnt_t Cir_Y_0;
    rmp_cnt_t Cir_Y_1;
    
    Cir_X_0=Coord_X+Round+1;
    Cir_X_1=Coord_X+Length-Round-1;
    Cir_Y_0=Coord_Y+Round+1;
    Cir_Y_1=Coord_Y+Width-Round-1;
    
    /* Draw the innermost one rectangle */
    RMP_Rectangle(Coord_X+Round+1, Coord_Y+Round+1, Length-Round-Round-2, Width-Round-Round-2, Color, Color);

    /* Draw 4 small side rectangles */
    RMP_Rectangle(Coord_X, Cir_Y_0, Round+1, Width-Round-Round-2, Color, Color);
    RMP_Rectangle(Cir_X_1, Cir_Y_0, Round+1, Width-Round-Round-2, Color, Color);
    RMP_Rectangle(Cir_X_0, Cir_Y_1, Length-Round-Round-2, Round+1, Color, Color);
    RMP_Rectangle(Cir_X_0, Coord_Y, Length-Round-Round-2, Round+1, Color, Color);
    
    /* Draw 4 circles */
    RMP_Circle(Cir_X_0, Cir_Y_0, Round, Color, Color);
    RMP_Circle(Cir_X_1, Cir_Y_0, Round, Color, Color);
    RMP_Circle(Cir_X_0, Cir_Y_1, Round, Color, Color);
    RMP_Circle(Cir_X_1, Cir_Y_1, Round, Color, Color);
}
/* End Function:RMP_Round_Rect ***********************************************/

/* Function:RMP_Circle ********************************************************
Description : Draw a circle on the screen. You can choose whether the circle 
              is filled or not. By setting the fill color to "RMP_TRANS", the 
              circle is not filled. Here, the arc function is not supported.
              Here we utilize the Bresenham algorithm to draw the circle.
              In this algorithm, we draw a 1/8 circle first, then we make use of
              the symmetry of the circle to get the rest of the circle.
Input       : rmp_cnt_t Center_X - The circle center's X coordinate.
              rmp_cnt_t Center_Y - The circle center's Y coordinate.
              rmp_cnt_t Radius - The radius of the circle.
              rmp_ptr_t Border -The color of its boundary.
              rmp_ptr_t Fill - The color filled within its boundary.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Circle(rmp_cnt_t Center_X,
                rmp_cnt_t Center_Y,
                rmp_cnt_t Radius,
                rmp_ptr_t Border,
                rmp_ptr_t Fill)
{
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Fill_Y;
    rmp_cnt_t Error;  
    rmp_cnt_t Quick;

    Cur_X=0;
    Cur_Y=Radius;
    Error=3-RMP_SAL(Radius,1);

    if(Fill!=RMP_TRANS)
    {
        RMP_COVERAGE_MARKER();

        /* When we are filling the circle, we can try not to fill the areas that have been filled
         * using the rectangle fill method to increase fill speed.
         * Square 2 is 1.414. Here we let the condition to be 1.5. */
        Quick=RMP_SAL(Radius,1)/3;
        /* First, fill the square hole using the fast direct-fill method */
        RMP_Rectangle(Center_X-Quick, Center_Y-Quick, RMP_SAL(Quick,1)+1, RMP_SAL(Quick,1)+1, Fill, Fill);

        while(Cur_X<=Cur_Y) 
        {  
            if(Cur_X<Quick)
            {
                RMP_COVERAGE_MARKER();

                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {
                    if(Fill_Y<Quick)
                    {
                        RMP_COVERAGE_MARKER();
                        continue;
                    }
                    else
                    {
                        RMP_COVERAGE_MARKER();
                        /* No action required */
                    }
                    
                    RMP_POINT(Center_X+Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X-Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X+Cur_X, Center_Y-Fill_Y, Fill); 
                    RMP_POINT(Center_X-Cur_X, Center_Y-Fill_Y, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y+Cur_X, Fill); 
                    RMP_POINT(Center_X-Fill_Y, Center_Y+Cur_X, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y-Cur_X, Fill);  
                    RMP_POINT(Center_X-Fill_Y, Center_Y-Cur_X, Fill);
                }
            }
            /* Here the "Cur_X" is already out of range. We do not check the conditions anymore */
            else
            {
                RMP_COVERAGE_MARKER();

                for(Fill_Y=Cur_X;Fill_Y<=Cur_Y;Fill_Y++)  
                {                   
                    RMP_POINT(Center_X+Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X-Cur_X, Center_Y+Fill_Y, Fill);  
                    RMP_POINT(Center_X+Cur_X, Center_Y-Fill_Y, Fill); 
                    RMP_POINT(Center_X-Cur_X, Center_Y-Fill_Y, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y+Cur_X, Fill); 
                    RMP_POINT(Center_X-Fill_Y, Center_Y+Cur_X, Fill);  
                    RMP_POINT(Center_X+Fill_Y, Center_Y-Cur_X, Fill);  
                    RMP_POINT(Center_X-Fill_Y, Center_Y-Cur_X, Fill);
                }
            }
            RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, Border); 
            RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, Border); 
            RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, Border);  
            RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, Border);

            if(Error<0) 
            {
                RMP_COVERAGE_MARKER();
                Error+=RMP_SAL(Cur_X,2)+6;
            }
            else 
            {  
                RMP_COVERAGE_MARKER();
                Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2)+10;
                Cur_Y--;  
            }

            Cur_X++;  
        }
    }
    else 
    {
        RMP_COVERAGE_MARKER();

        /* Border only */ 
        while(Cur_X<=Cur_Y) 
        {
            RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, Border); 
            RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, Border); 
            RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, Border);  
            RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, Border);  
            RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, Border);

            if(Error<0)
            {
                RMP_COVERAGE_MARKER();
                Error+=RMP_SAL(Cur_X,2)+6;
            }
            else 
            {
                RMP_COVERAGE_MARKER();
                Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
                Cur_Y--;
            }

            Cur_X++;  
        }  
    }
}
/* End Function:RMP_Circle ***************************************************/

/* Function:RMP_Matrix ********************************************************
Description : Display a monochrome bit map in the given color.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_ptr_t Bit_Order - The bit ordering.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Matrix(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                const rmp_u8_t* Matrix,
                rmp_ptr_t Bit_Order,
                rmp_cnt_t Length,
                rmp_cnt_t Width,
                rmp_ptr_t Color)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    
    if((Matrix==RMP_NULL)||(Length==0)||(Width==0)||((((rmp_u8_t)Length)&0x07U)!=0U))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Mat_Pos=0;
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        RMP_COVERAGE_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix,Mat_Pos)!=0U)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                }
                else
                {
                    RMP_COVERAGE_MARKER();
                    /* No action required */
                }

                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        RMP_COVERAGE_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix,Mat_Pos)!=0U)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_Point(Len_Cnt,Wid_Cnt,Color);
                }
                else
                {
                    RMP_COVERAGE_MARKER();
                    /* No action required */
                }

                Mat_Pos++;
            }
        }
    }
}
/* End Function:RMP_Matrix ***************************************************/

/* Function:RMP_Matrix_AA *****************************************************
Description : Display a monochrome bit map in the given color, with simple 
              anti-aliasing (FXAA 1X algorithm).
              When using this, three color mixing macros must be provided.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              const rmp_u8_t* Matrix - The data matrix.
              rmp_ptr_t Bit_Order - The bit ordering.
              rmp_cnt_t Length - The length of the picture.
              rmp_cnt_t Width - The width of the picture.
              rmp_ptr_t Color - The color to display this with.
              rmp_ptr_t Back - The background color, for anti-aliasing.
Output      : None.
Return      : None.
******************************************************************************/
#if((defined RMP_COLOR_25P)&&(defined RMP_COLOR_50P)&&(defined RMP_COLOR_75P))
void RMP_Matrix_AA(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   const rmp_u8_t* Matrix,
                   rmp_ptr_t Bit_Order,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width,
                   rmp_ptr_t Color,
                   rmp_ptr_t Back)
{
    rmp_cnt_t Len_Cnt;
    rmp_cnt_t Wid_Cnt;
    rmp_cnt_t Mat_Pos;
    rmp_cnt_t AA_Pos;
    rmp_cnt_t AA_Val;
    rmp_ptr_t Color_25;
    rmp_ptr_t Color_50;
    rmp_ptr_t Color_75;
    rmp_cnt_t Total;
    
    if((Matrix==RMP_NULL)||(Length==0)||(Width==0)||((((rmp_u8_t)Length)&0x07U)!=0U))
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    Mat_Pos=0;
    Total=Length*Width;
    Color_25=RMP_COLOR_25P(Color,Back);
    Color_50=RMP_COLOR_50P(Color,Back);
    Color_75=RMP_COLOR_75P(Color,Back);
    
    /* Natural order */
    if(Bit_Order==RMP_MAT_BIG)
    {
        RMP_COVERAGE_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_BPOS(Matrix, Mat_Pos)!=0U)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_Point(Len_Cnt, Wid_Cnt, Color);
                }
                else
                {
                    RMP_COVERAGE_MARKER();

                    /* Anti-aliasing */
                    AA_Val=0;
                    
                    AA_Pos=Mat_Pos-1;
                    if(AA_Pos>=0)
                    {
                        RMP_COVERAGE_MARKER();

                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos-Length;
                        if((AA_Pos>=0)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COVERAGE_MARKER();
                        /* No action required */
                    }
                    
                    AA_Pos=Mat_Pos+1;
                    if(AA_Pos<Total)
                    {
                        RMP_COVERAGE_MARKER();

                        if((Len_Cnt!=(Coord_X+Length-1))&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos+Length;
                        if((AA_Pos<Total)&&(RMP_MAT_BPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COVERAGE_MARKER();
                        /* No action required */
                    }
                    
                    switch(AA_Val)
                    {
                        case 1:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_25);
                            break;
                        }
                        case 2:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_50);
                            break;
                        }
                        case 3:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_75);
                            break;
                        }
                        default:
                        {
                            RMP_COVERAGE_MARKER();
                            break;
                        }
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
    /* Small order */
    else
    {
        RMP_COVERAGE_MARKER();

        for(Wid_Cnt=Coord_Y;Wid_Cnt<(Width+Coord_Y);Wid_Cnt++)
        {
            for(Len_Cnt=Coord_X;Len_Cnt<(Length+Coord_X);Len_Cnt++)
            {
                if(RMP_MAT_SPOS(Matrix, Mat_Pos)!=0U)
                {
                    RMP_COVERAGE_MARKER();
                    RMP_Point(Len_Cnt, Wid_Cnt, Color);
                }
                else
                {
                    RMP_COVERAGE_MARKER();

                    /* Anti-aliasing */
                    AA_Val=0;

                    AA_Pos=Mat_Pos-1;
                    if(AA_Pos>=0)
                    {
                        RMP_COVERAGE_MARKER();

                        if((Len_Cnt!=Coord_X)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos-Length;
                        if((AA_Pos>=0)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COVERAGE_MARKER();
                        /* No action required */
                    }

                    AA_Pos=Mat_Pos+1;
                    if(AA_Pos<Total)
                    {
                        RMP_COVERAGE_MARKER();

                        if((Len_Cnt!=(Coord_X+Length-1))&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }

                        AA_Pos=Mat_Pos+Length;
                        if((AA_Pos<Total)&&(RMP_MAT_SPOS(Matrix, AA_Pos)!=0U))
                        {
                            RMP_COVERAGE_MARKER();
                            AA_Val++;
                        }
                        else
                        {
                            RMP_COVERAGE_MARKER();
                            /* No action required */
                        }
                    }
                    else
                    {
                        RMP_COVERAGE_MARKER();
                        /* No action required */
                    }
                    
                    switch(AA_Val)
                    {
                        case 1:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_25);
                            break;
                        }
                        case 2:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_50);
                            break;
                        }
                        case 3:
                        {
                            RMP_COVERAGE_MARKER();
                            RMP_Point(Len_Cnt, Wid_Cnt, Color_75);
                            break;
                        }
                        default:
                        {
                            RMP_COVERAGE_MARKER();
                            break;
                        }
                    }
                }
                
                Mat_Pos++;
            }
        }
    }
}
#endif
/* End Function:RMP_Matrix_AA ************************************************/

/* Function:RMP_Cursor ********************************************************
Description : Draw a cursor (size 16x16) on the screen.
Input       : rmp_cnt_t Coord_X - The X coordinate of the cursor.
              rmp_cnt_t Coord_Y - The Y coordinate of the cursor.
              rmp_ptr_t Style - The style of the mouse. Can be one of the following:
              RMP_CUR_NORM         [0x00]    The normal arrow cursor.
              RMP_CUR_BUSY         [0x01]    The cursor with a busy mark.
              RMP_CUR_QUESTION     [0x02]    The cursor with a question mark.
              RMP_CUR_HAND         [0x03]    The cursor for hyperlinks.
              RMP_CUR_TEXT         [0x04]    The text editing cursor.
              RMP_CUR_STOP         [0x05]    The "not available" cursor.
              RMP_CUR_MOVE         [0x06]    The cursor for moving the window.
              RMP_CUR_LR           [0x07]    Left-to-right double arrow.
              RMP_CUR_UD           [0x08]    Up-to-down double arrow.
              RMP_CUR_ULBR         [0x09]    Upper-left-to-bottom-right double arrow.
              RMP_CUR_URBL         [0x0A]    Upper-right-to-bottom-left double arrow.
              RMP_CUR_CROSS        [0x0B]    Crosshair cursor.
Output      : None.
Return      : None.
******************************************************************************/
#if((defined RMP_CTL_WHITE)&&(defined RMP_CTL_LGREY)&& \
    (defined RMP_CTL_GREY)&&(defined RMP_CTL_DGREY)&& \
    (defined RMP_CTL_DARK)&&(defined RMP_CTL_DDARK)&& \
    (defined RMP_CTL_BLACK))
void RMP_Cursor(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                rmp_ptr_t Style)
{
    rmp_cnt_t Count;
    rmp_cnt_t Size_B;
    rmp_cnt_t Size_W;
    const rmp_u8_t* Black;
    const rmp_u8_t* White;

    /* Normal Mouse - all matrix encoded in (Y,X) */
    static const rmp_u8_t Arrow_B[]=
    {
        0x00U, 0x10U, 0x11U, 0x20U, 0x22U, 0x30U, 0x33U, 0x40U,
        0x44U, 0x50U, 0x55U, 0x60U, 0x66U, 0x70U, 0x77U, 0x80U,
        0x88U, 0x90U, 0x95U, 0x96U, 0x97U, 0x98U, 0x99U, 0xA0U,
        0xA2U, 0xA3U, 0xA5U, 0xB0U, 0xB1U, 0xB3U, 0xB6U, 0xC0U,
        0xC1U, 0xC4U, 0xC6U, 0xD0U, 0xD4U, 0xD7U, 0xE5U, 0xE7U,
        0xF5U, 0xF6U
    };
    static const rmp_u8_t Arrow_W[]=
    {
        0x21U, 0x31U, 0x32U, 0x41U, 0x42U, 0x43U, 0x51U, 0x52U,
        0x53U, 0x54U, 0x61U, 0x62U, 0x63U, 0x64U, 0x65U, 0x71U,
        0x72U, 0x73U, 0x74U, 0x75U, 0x76U, 0x81U, 0x82U, 0x83U,
        0x84U, 0x85U, 0x86U, 0x87U, 0x91U, 0x92U, 0x93U, 0x94U,
        0xA1U, 0xA4U, 0xB4U, 0xB5U, 0xC5U, 0xD5U, 0xD6U, 0xE6U
    };
    /* Busy mark - no white part */
    static const rmp_u8_t Busy_B[]=
    {
        0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x1AU, 0x1BU, 0x1DU,
        0x1EU, 0x2AU, 0x2CU, 0x2EU, 0x3BU, 0x3DU, 0x4CU, 0x5BU,
        0x5DU, 0x6AU, 0x6EU, 0x7AU, 0x7CU, 0x7EU, 0x8AU, 0x8BU,
        0x8CU, 0x8DU, 0x8EU
    };
    static const rmp_u8_t Busy_W[]=
    {
        0x1CU, 0x2BU, 0x2DU, 0x3CU, 0x5CU, 0x6BU, 0x6CU, 0x6DU,
        0x7BU, 0x7DU
    };
    /* Question mark - no white part */
    static const rmp_u8_t Question[]=
    {
        0x0BU, 0x0CU, 0x0DU, 0x1AU, 0x1BU, 0x1CU, 0x1DU, 0x1EU,
        0x29U, 0x2AU, 0x2BU, 0x2DU, 0x2EU, 0x2FU, 0x39U, 0x3AU,
        0x3EU, 0x3FU, 0x4AU, 0x4DU, 0x4EU, 0x4FU, 0x5CU, 0x5DU,
        0x5EU, 0x6CU, 0x6DU, 0x8DU, 0x8EU, 0x9DU, 0x9EU
    };
    /* Hand-shaped cursor */
    static const rmp_u8_t Hand_B[]=
    {
        0x05U, 0x06U, 0x14U, 0x17U, 0x24U, 0x27U, 0x34U, 0x37U,
        0x39U, 0x44U, 0x47U, 0x48U, 0x4AU, 0x4CU, 0x51U, 0x52U,
        0x54U, 0x57U, 0x5AU, 0x5BU, 0x5DU, 0x5EU, 0x60U, 0x63U,
        0x64U, 0x67U, 0x6AU, 0x6DU, 0x6FU, 0x70U, 0x74U, 0x77U,
        0x7AU, 0x7DU, 0x7FU, 0x80U, 0x8FU, 0x90U, 0x9FU, 0xA0U,
        0xAFU, 0xB0U, 0xBFU, 0xC1U, 0xCEU, 0xD1U, 0xD2U, 0xD3U,
        0xD4U, 0xD5U, 0xD6U, 0xD7U, 0xD8U, 0xD9U, 0xDAU, 0xDBU,
        0xDCU, 0xDDU, 0xDEU, 0xE1U, 0xEEU, 0xF2U, 0xF3U, 0xF4U,
        0xF5U, 0xF6U, 0xF7U, 0xF8U, 0xF9U, 0xFAU, 0xFBU, 0xFCU,
        0xFDU
    };
    static const rmp_u8_t Hand_W[]=
    {
        0x15U, 0x16U, 0x25U, 0x26U, 0x35U, 0x36U, 0x45U, 0x46U,
        0x49U, 0x55U, 0x56U, 0x58U, 0x59U, 0x5CU, 0x61U, 0x62U,
        0x65U, 0x66U, 0x68U, 0x69U, 0x6BU, 0x6CU, 0x6EU, 0x71U,
        0x72U, 0x73U, 0x75U, 0x76U, 0x78U, 0x79U, 0x7BU, 0x7CU,
        0x7EU, 0x81U, 0x82U, 0x83U, 0x84U, 0x85U, 0x86U, 0x87U,
        0x88U, 0x89U, 0x8AU, 0x8BU, 0x8CU, 0x8DU, 0x8EU, 0x91U,
        0x92U, 0x93U, 0x94U, 0x95U, 0x96U, 0x97U, 0x98U, 0x99U,
        0x9AU, 0x9BU, 0x9CU, 0x9DU, 0x9EU, 0xA1U, 0xA2U, 0xA3U,
        0xA4U, 0xA5U, 0xA6U, 0xA7U, 0xA8U, 0xA9U, 0xAAU, 0xABU,
        0xACU, 0xADU, 0xAEU, 0xB1U, 0xB2U, 0xB3U, 0xB4U, 0xB5U,
        0xB6U, 0xB7U, 0xB8U, 0xB9U, 0xBAU, 0xBBU, 0xBCU, 0xBDU,
        0xBEU, 0xC2U, 0xC3U, 0xC4U, 0xC5U, 0xC6U, 0xC7U, 0xC8U,
        0xC9U, 0xCAU, 0xCBU, 0xCCU, 0xCDU, 0xE2U, 0xE3U, 0xE4U,
        0xE5U, 0xE6U, 0xE7U, 0xE8U, 0xE9U, 0xEAU, 0xEBU, 0xECU,
        0xEDU
    };
    /* The cursor for text editing - no white part */
    static const rmp_u8_t Text[]=
    {
        0x00U, 0x01U, 0x02U, 0x04U, 0x05U, 0x06U, 0x13U, 0x23U,
        0x33U, 0x43U, 0x53U, 0x63U, 0x73U, 0x83U, 0x93U, 0xA3U,
        0xB3U, 0xC3U, 0xD3U, 0xE3U, 0xF0U, 0xF1U, 0xF2U, 0xF4U,
        0xF5U, 0xF6U
    };
    /* The stop cursor */
    static const rmp_u8_t Stop_B[]=
    {
        0x05U, 0x06U, 0x07U, 0x08U, 0x13U, 0x14U, 0x15U, 0x16U,
        0x17U, 0x18U, 0x19U, 0x1AU, 0x22U, 0x23U, 0x24U, 0x29U,
        0x2AU, 0x2BU, 0x31U, 0x32U, 0x33U, 0x34U, 0x3AU, 0x3BU,
        0x3CU, 0x41U, 0x42U, 0x43U, 0x44U, 0x45U, 0x4BU, 0x4CU,
        0x50U, 0x51U, 0x54U, 0x55U, 0x56U, 0x5CU, 0x5DU, 0x60U,
        0x61U, 0x65U, 0x66U, 0x67U, 0x6CU, 0x6DU, 0x70U, 0x71U,
        0x76U, 0x77U, 0x78U, 0x7CU, 0x7DU, 0x80U, 0x81U, 0x87U,
        0x88U, 0x89U, 0x8CU, 0x8DU, 0x91U, 0x92U, 0x98U, 0x99U,
        0x9AU, 0x9BU, 0x9CU, 0xA1U, 0xA2U, 0xA3U, 0xA9U, 0xAAU,
        0xABU, 0xACU, 0xB2U, 0xB3U, 0xB4U, 0xB9U, 0xBAU, 0xBBU,
        0xC3U, 0xC4U, 0xC5U, 0xC6U, 0xC7U, 0xC8U, 0xC9U, 0xCAU,
        0xD5U, 0xD6U, 0xD7U, 0xD8U
    };
    static const rmp_u8_t Stop_W[]=
    {
        0x25U, 0x26U, 0x27U, 0x28U, 0x35U, 0x36U, 0x37U, 0x38U,
        0x39U, 0x46U, 0x47U, 0x48U, 0x49U, 0x4AU, 0x52U, 0x53U,
        0x57U, 0x58U, 0x59U, 0x5AU, 0x5BU, 0x62U, 0x63U, 0x64U,
        0x68U, 0x69U, 0x6AU, 0x6BU, 0x72U, 0x73U, 0x74U, 0x75U,
        0x79U, 0x7AU, 0x7BU, 0x82U, 0x83U, 0x84U, 0x85U, 0x86U,
        0x8AU, 0x8BU, 0x93U, 0x94U, 0x95U, 0x96U, 0x97U, 0xA4U,
        0xA5U, 0xA6U, 0xA7U, 0xA8U, 0xB5U, 0xB6U, 0xB7U, 0xB8U
    };
       /* Cross arrow - no white part  */
    static const rmp_u8_t Adj_ALL[]=
    {
        0x07U, 0x16U, 0x17U, 0x18U, 0x25U, 0x26U, 0x27U, 0x28U,
        0x29U, 0x37U, 0x47U, 0x52U, 0x57U, 0x5CU, 0x61U, 0x62U,
        0x67U, 0x6CU, 0x6DU, 0x70U, 0x71U, 0x72U, 0x73U, 0x74U,
        0x75U, 0x76U, 0x77U, 0x78U, 0x79U, 0x7AU, 0x7BU, 0x7CU,
        0x7DU, 0x7EU, 0x81U, 0x82U, 0x87U, 0x8CU, 0x8DU, 0x92U,
        0x97U, 0x9CU, 0xA7U, 0xB7U, 0xC5U, 0xC6U, 0xC7U, 0xC8U,
        0xC9U, 0xD6U, 0xD7U, 0xD8U, 0xE7U
    };    
    /* Left to right arrow - no white part */
    static const rmp_u8_t Adj_LR[]=
    {
        0x52U, 0x5DU, 0x61U, 0x62U, 0x6DU, 0x6EU, 0x70U, 0x71U,
        0x72U, 0x73U, 0x74U, 0x75U, 0x76U, 0x77U, 0x78U, 0x79U,
        0x7AU, 0x7BU, 0x7CU, 0x7DU, 0x7EU, 0x7FU, 0x81U, 0x82U,
        0x8DU, 0x8EU, 0x92U, 0x9DU
    };
    /* Up to down arrow - no white part */
    static const rmp_u8_t Adj_UD[]=
    {
        0x07U, 0x16U, 0x17U, 0x18U, 0x25U, 0x26U, 0x27U, 0x28U,
        0x29U, 0x37U, 0x47U, 0x57U, 0x67U, 0x77U, 0x87U, 0x97U,
        0xA7U, 0xB7U, 0xC7U, 0xD5U, 0xD6U, 0xD7U, 0xD8U, 0xD9U,
        0xE6U, 0xE7U, 0xE8U, 0xF7U

    };
    /* Bottom-right to top-left arrow - no white part */
    static const rmp_u8_t Adj_ULBR[]=
    {
        0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x10U, 0x11U, 0x12U,
        0x13U, 0x20U, 0x21U, 0x22U, 0x30U, 0x31U, 0x33U, 0x40U,
        0x44U, 0x55U, 0x66U, 0x77U, 0x88U, 0x99U, 0xAAU, 0xBBU,
        0xBFU, 0xCCU, 0xCEU, 0xCFU, 0xDDU, 0xDEU, 0xDFU, 0xECU,
        0xEDU, 0xEEU, 0xEFU, 0xFBU, 0xFCU, 0xFDU, 0xFEU, 0xFFU
    };
    /* Bottom-left to top-right arrow - no white part */
    static const rmp_u8_t Adj_URBL[]=
    {
        0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU, 0x1CU, 0x1DU, 0x1EU,
        0x1FU, 0x2DU, 0x2EU, 0x2FU, 0x3CU, 0x3EU, 0x3FU, 0x4BU,
        0x4FU, 0x5AU, 0x69U, 0x78U, 0x87U, 0x96U, 0xA5U, 0xB0U,
        0xB4U, 0xC0U, 0xC1U, 0xC3U, 0xD0U, 0xD1U, 0xD2U, 0xE0U,
        0xE1U, 0xE2U, 0xE3U, 0xF0U, 0xF1U, 0xF2U, 0xF3U, 0xF4U
    };
    /* The crosshair cursor - no white part */
    static const rmp_u8_t Cross[]=
    {
        0x07U, 0x17U, 0x27U, 0x37U, 0x46U, 0x47U, 0x48U, 0x55U,
        0x57U, 0x59U, 0x64U, 0x6AU, 0x70U, 0x71U, 0x72U, 0x73U,
        0x74U, 0x75U, 0x77U, 0x79U, 0x7AU, 0x7BU, 0x7CU, 0x7DU,
        0x7EU, 0x84U, 0x8AU, 0x95U, 0x97U, 0x99U, 0xA6U, 0xA7U,
        0xA8U, 0xB7U, 0xC7U, 0xD7U, 0xE7U
    };
    
    /* Draw them */
    switch(Style)
    {
        case RMP_CUR_NORM:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Arrow_B);
            Size_W=sizeof(Arrow_W);
            Black=Arrow_B;
            White=Arrow_W;
            break;
        }
        case RMP_CUR_BUSY:
        case RMP_CUR_QUESTION:
        {
            RMP_COVERAGE_MARKER();

            for(Count=0;Count<(rmp_cnt_t)sizeof(Arrow_B);Count++)
            {
                RMP_POINT(Coord_X+RMP_CUR_XPOS(Arrow_B,Count),
                          Coord_Y+RMP_CUR_YPOS(Arrow_B,Count),RMP_CTL_BLACK);
            }
            for(Count=0;Count<(rmp_cnt_t)sizeof(Arrow_W);Count++)
            {
                RMP_POINT(Coord_X+RMP_CUR_XPOS(Arrow_W,Count),
                          Coord_Y+RMP_CUR_YPOS(Arrow_W,Count),RMP_CTL_WHITE);
            }
            
            if(Style==RMP_CUR_BUSY)
            {
                RMP_COVERAGE_MARKER();
                Size_B=sizeof(Busy_B);
                Size_W=sizeof(Busy_W);
                Black=Busy_B;
                White=Busy_W;
            }
            else
            {
                RMP_COVERAGE_MARKER();
                Size_B=sizeof(Question);
                Size_W=0;
                Black=Question;
                White=RMP_NULL;
            }
            break;
        }       
        case RMP_CUR_HAND:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Hand_B);
            Size_W=sizeof(Hand_W);
            Black=Hand_B;
            White=Hand_W;
            break;
        }
        case RMP_CUR_TEXT:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Text);
            Size_W=0;
            Black=Text;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_STOP:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Stop_B);
            Size_W=sizeof(Stop_W);
            Black=Stop_B;
            White=Stop_W;
            break;
        }
        case RMP_CUR_MOVE:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Adj_ALL);
            Size_W=0;
            Black=Adj_ALL;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_LR:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Adj_LR);
            Size_W=0;
            Black=Adj_LR;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_UD:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Adj_UD);
            Size_W=0;
            Black=Adj_UD;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_ULBR:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Adj_ULBR);
            Size_W=0;
            Black=Adj_ULBR;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_URBL:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Adj_URBL);
            Size_W=0;
            Black=Adj_URBL;
            White=RMP_NULL;
            break;
        }
        case RMP_CUR_CROSS:
        {
            RMP_COVERAGE_MARKER();
            Size_B=sizeof(Cross);
            Size_W=0;
            Black=Cross;
            White=RMP_NULL;
            break;
        }
        default:
        {
            RMP_COVERAGE_MARKER();
            return;
        }
    }
    
    for(Count=0;Count<Size_B;Count++)
    {
        RMP_POINT(Coord_X+RMP_CUR_XPOS(Black,Count),
                  Coord_Y+RMP_CUR_YPOS(Black,Count),RMP_CTL_BLACK);
    }
    for(Count=0;Count<Size_W;Count++)
    {
        RMP_POINT(Coord_X+RMP_CUR_XPOS(White,Count),
                  Coord_Y+RMP_CUR_YPOS(White,Count),RMP_CTL_WHITE);
    }
}
/* End Function: RMP_Cursor **************************************************/

/* Function:RMP_Checkbox_Set **************************************************
Description : Set the checbox (tick it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Set(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Draw a tick inside - not using shifts for accuracy */
    for(Count=0;Count<=(2*Length/13);Count++)
    {
        RMP_Line(Coord_X+(4*Length/13)+Count, Coord_Y+(6*Length/13)+Count,
                 Coord_X+(4*Length/13)+Count, Coord_Y+(8*Length/13)+Count, RMP_CTL_BLACK);
    }
    for(Count=0;Count<=(4*Length/13);Count++)
    {
        RMP_Line(Coord_X+(6*Length/13)+Count, Coord_Y+(8*Length/13)-Count,
                 Coord_X+(6*Length/13)+Count, Coord_Y+(10*Length/13)-Count, RMP_CTL_BLACK);
    }
}
/* End Function:RMP_Checkbox_Set *********************************************/

/* Function:RMP_Checkbox_Clr **************************************************
Description : Clear the checbox (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox_Clr(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    rmp_cnt_t Count;
    
    /* Erase the tick inside - not using shifts for accuracy */
    for(Count=0;Count<=(2*Length/13);Count++)
    {
        RMP_Line(Coord_X+(4*Length/13)+Count, Coord_Y+(6*Length/13)+Count,
                 Coord_X+(4*Length/13)+Count, Coord_Y+(8*Length/13)+Count, RMP_CTL_WHITE);
    }
    for(Count=0;Count<=(4*Length/13);Count++)
    {
        RMP_Line(Coord_X+(6*Length/13)+Count, Coord_Y+(8*Length/13)-Count,
                 Coord_X+(6*Length/13)+Count, Coord_Y+(10*Length/13)-Count, RMP_CTL_WHITE);
    }
}
/* End Function:RMP_Checkbox_Clr *********************************************/

/* Function:RMP_Checkbox ******************************************************
Description : Draw the checkbox according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the checkbox(also the width).
              rmp_ptr_t Status - The checkbox status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Checkbox(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_ptr_t Status)    
{   
    /* Clear the area */
    RMP_Rectangle(Coord_X, Coord_Y, Length, Length, RMP_CTL_WHITE, RMP_CTL_WHITE);
                                                                     
    /* Draw checkbox outline */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_CTL_DARK);                 
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Length-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Length-1, Coord_X, Coord_Y+Length-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X, Coord_Y+Length-1, Coord_X, Coord_Y, RMP_CTL_DARK);

    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Length-2, RMP_CTL_DARK);
    RMP_Line(Coord_X+Length-2, Coord_Y+Length-2, Coord_X, Coord_Y+Length-2, RMP_CTL_DARK);
    RMP_Line(Coord_X+1, Coord_Y+Length-2, Coord_X+1, Coord_Y+1, RMP_CTL_BLACK);
    
    if(Status!=0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Checkbox_Set(Coord_X, Coord_Y, Length);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Checkbox *************************************************/

/* Function:RMP_Cmdbtn_Down ***************************************************
Description : Turn the command button to a pushed-down version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Down(rmp_cnt_t Coord_X,
                     rmp_cnt_t Coord_Y,
                     rmp_cnt_t Length,
                     rmp_cnt_t Width)
{
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_CTL_BLACK);
    /* Clear the old shadow */
    RMP_Line(Coord_X+Length-2, Coord_Y+Width-2, Coord_X+1, Coord_Y+Width-2, RMP_CTL_GREY);    
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Width-2, RMP_CTL_GREY);    
    /* The shadow */                            
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_CTL_DARK);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_CTL_DARK);    
}
/* End Function:RMP_Cmdbtn_Down **********************************************/

/* Function:RMP_Cmdbtn_Up *****************************************************
Description : Turn the command button to a popped-up version.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn_Up(rmp_cnt_t Coord_X,
                   rmp_cnt_t Coord_Y,
                   rmp_cnt_t Length,
                   rmp_cnt_t Width)
{
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_CTL_BLACK);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_CTL_WHITE);
    /* Clear the old shadow */
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_CTL_GREY);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_CTL_GREY);    
    /* The shadow */
    RMP_Line(Coord_X+Length-2, Coord_Y+Width-2, Coord_X+1, Coord_Y+Width-2, RMP_CTL_DARK);    
    RMP_Line(Coord_X+Length-2, Coord_Y+1, Coord_X+Length-2, Coord_Y+Width-2, RMP_CTL_DARK);    
}
/* End Function:RMP_Cmdbtn_Up ************************************************/

/* Function:RMP_Cmdbtn ********************************************************
Description : Draw a command button.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_ptr_t Status - The status of the command button.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Cmdbtn(rmp_cnt_t Coord_X,
                rmp_cnt_t Coord_Y,
                rmp_cnt_t Length,
                rmp_cnt_t Width,
                rmp_ptr_t Status)
{
    RMP_Rectangle(Coord_X, Coord_Y, Length, Width, RMP_CTL_GREY, RMP_CTL_GREY);

    if(Status!=0U)
    {
        RMP_COVERAGE_MARKER();
        RMP_Cmdbtn_Down(Coord_X, Coord_Y, Length, Width);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Cmdbtn_Up(Coord_X, Coord_Y, Length, Width);
    }
}
/* End Function:RMP_Cmdbtn ***************************************************/

/* Function:RMP_Lineedit_Clr **************************************************
Description : Clear a portion of the line edit box.
Input       : rmp_cnt_t Clr_X - The X coordinate to start clearing.
              rmp_cnt_t Clr_Len - The length to clear.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Width - The width of the line edit box.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit_Clr(rmp_cnt_t Clr_X,
                      rmp_cnt_t Clr_Len,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Width)
{
    RMP_Rectangle(Clr_X, Coord_Y+1, Clr_Len, Width-2, RMP_CTL_WHITE, RMP_CTL_WHITE);
}
/* End Function:RMP_Lineedit_Clr *********************************************/

/* Function:RMP_Lineedit ******************************************************
Description : Draw a line edit box with string on it.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the line edit box.
              rmp_cnt_t Width - The width of the line edit box.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lineedit(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_cnt_t Width)
{
    RMP_Rectangle(Coord_X, Coord_Y, Length, Width, RMP_CTL_WHITE, RMP_CTL_WHITE);
    
    /* Now draw the border */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_CTL_DARK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_CTL_WHITE);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_CTL_DARK);
    /* The shadow */
    RMP_Line(Coord_X+1, Coord_Y+Width-2, Coord_X+1, Coord_Y+1, RMP_CTL_BLACK);    
    RMP_Line(Coord_X+1, Coord_Y+1, Coord_X+Length-2, Coord_Y+1, RMP_CTL_BLACK);
}
/* End Function:RMP_Lineedit *************************************************/

/* Function:RMP_Radiobtn_Circle ***********************************************
Description : Draw the radio button outside circle according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).         
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Circle(rmp_cnt_t Coord_X,
                         rmp_cnt_t Coord_Y,
                         rmp_cnt_t Length)
{   
    rmp_cnt_t Radius;
    rmp_cnt_t Center_X;
    rmp_cnt_t Center_Y;
    rmp_cnt_t Cur_X;
    rmp_cnt_t Cur_Y;
    rmp_cnt_t Error;
    
    /* The radius is the length/2 */
    Radius=RMP_SAR(Length,1);
    Center_X=Coord_X+Radius;
    Center_Y=Coord_Y+Radius;
    
    /* Now we begin to draw the inner halfcircle */
    Cur_X=0;
    Cur_Y=Radius-1;
    Error=3-RMP_SAL(Cur_Y,1);
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, RMP_CTL_DDARK);
        RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, RMP_CTL_DDARK);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, RMP_CTL_LGREY);
        RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, RMP_CTL_LGREY);  
        RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, RMP_CTL_LGREY); 
        RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, RMP_CTL_LGREY);  
        
        if(Error<0) 
        {
            RMP_COVERAGE_MARKER();
            Error+=RMP_SAL(Cur_X,2)+6;
        }
        else 
        {
            RMP_COVERAGE_MARKER();
            Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
            Cur_Y--;
        }
        Cur_X++;
    } 
    
    /* Now we begin to draw the outer halfcircle */
    Cur_X=0;
    Cur_Y=Radius;
    Error=3-RMP_SAL(Radius,1);
    
    while(Cur_X<=Cur_Y) 
    {  
        /* This is the upper-left part */
        RMP_POINT(Center_X+Cur_X, Center_Y-Cur_Y, RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_X, Center_Y-Cur_Y, RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_Y, Center_Y-Cur_X, RMP_CTL_DGREY);
        RMP_POINT(Center_X-Cur_Y, Center_Y+Cur_X, RMP_CTL_DGREY);
        
        /* This is the lower-right part */
        RMP_POINT(Center_X+Cur_X, Center_Y+Cur_Y, RMP_CTL_WHITE);
        RMP_POINT(Center_X-Cur_X, Center_Y+Cur_Y, RMP_CTL_WHITE);  
        RMP_POINT(Center_X+Cur_Y, Center_Y+Cur_X, RMP_CTL_WHITE); 
        RMP_POINT(Center_X+Cur_Y, Center_Y-Cur_X, RMP_CTL_WHITE);  
        
        if(Error<0) 
        {
            RMP_COVERAGE_MARKER();
            Error+=RMP_SAL(Cur_X,2)+6;
        } 
        else 
        {
            RMP_COVERAGE_MARKER();
            Error+=RMP_SAL((rmp_ptr_t)Cur_X-(rmp_ptr_t)Cur_Y,2);
            Cur_Y--;  
        }  
        Cur_X++;  
    } 
}
/* End Function:RMP_Radiobtn_Circle ******************************************/

/* Function:RMP_Radiobtn_Set **************************************************
Description : Set the radio button (select it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Set(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+RMP_SAR(Length,1), Coord_Y+RMP_SAR(Length,1),
               Length/6, RMP_CTL_BLACK, RMP_CTL_BLACK);
}
/* End Function:RMP_Radiobtn_Set *********************************************/

/* Function:RMP_Radiobtn_Clr **************************************************
Description : Set the radio button (reset it).
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn_Clr(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length)
{
    RMP_Circle(Coord_X+RMP_SAR(Length,1), Coord_Y+RMP_SAR(Length,1),
               Length/6, RMP_CTL_GREY, RMP_CTL_GREY);
}
/* End Function:RMP_Radiobtn_Clr *********************************************/

/* Function:RMP_Radiobtn ******************************************************
Description : Draw the radio button according to the parameters.
Input       : rmp_cnt_t Coord_X - The X coordinate.
              rmp_cnt_t Coord_Y - The Y coordinate.
              rmp_cnt_t Length - The length of the radio button(also the width).
              rmp_ptr_t Status - The radio button status.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Radiobtn(rmp_cnt_t Coord_X,
                  rmp_cnt_t Coord_Y,
                  rmp_cnt_t Length,
                  rmp_ptr_t Status)
{   
    /* Fill the area with rect */
    RMP_Rectangle(Coord_X, Coord_Y, Length, Length, RMP_CTL_GREY, RMP_CTL_GREY);
    
    /* Draw the circle first */
    RMP_Radiobtn_Circle(Coord_X, Coord_Y, Length);
    
    /* See if the radio button is selected */
    if(Status==RMP_RBTN_SEL)
    {
        RMP_COVERAGE_MARKER();
        RMP_Radiobtn_Set(Coord_X, Coord_Y, Length);
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
}
/* End Function:RMP_Radiobtn *************************************************/

/* Function:RMP_Progbar_Set **************************************************
Description : Set the progress of a progress bar.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Old_Prog - The old progress of this bar. 
              rmp_cnt_t New_Prog - The new progress of this bar. 
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.      
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar_Set(rmp_cnt_t Coord_X,
                     rmp_cnt_t Coord_Y,
                     rmp_cnt_t Length,
                     rmp_cnt_t Width,
                     rmp_cnt_t Style,
                     rmp_cnt_t Old_Prog,
                     rmp_cnt_t New_Prog,
                     rmp_ptr_t Fore,
                     rmp_ptr_t Back)
{
    /* The critical values for drawing the bar */
    rmp_cnt_t Old_Pivot;
    rmp_cnt_t New_Pivot;
    
    /* If things does not change, return to save time */
    if(Old_Prog==New_Prog)
    {
        RMP_COVERAGE_MARKER();
        return;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* No action required */
    }
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            RMP_COVERAGE_MARKER();

            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1+New_Pivot, Coord_Y+1, Old_Pivot-New_Pivot, Width-2, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1+Old_Pivot, Coord_Y+1, New_Pivot-Old_Pivot, Width-2, Fore, Fore);
            }
            break;
        }
        case RMP_PBAR_D2U:
        {
            RMP_COVERAGE_MARKER();

            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1, Coord_Y-1+Width-Old_Pivot, Length-2, Old_Pivot-New_Pivot, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1, Coord_Y-1+Width-New_Pivot, Length-2, New_Pivot-Old_Pivot, Fore, Fore);
            }
            break;
        }
        case RMP_PBAR_R2L:
        {
            RMP_COVERAGE_MARKER();

            Old_Pivot=(Length-2)*Old_Prog/100;
            New_Pivot=(Length-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X-1+Length-Old_Pivot, Coord_Y+1, Old_Pivot-New_Pivot, Width-2, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X-1+Length-New_Pivot, Coord_Y+1, New_Pivot-Old_Pivot, Width-2, Fore, Fore);
            }
            break;
        }
        case RMP_PBAR_U2D:
        {
            RMP_COVERAGE_MARKER();

            Old_Pivot=(Width-2)*Old_Prog/100;
            New_Pivot=(Width-2)*New_Prog/100;
            /* Progress decreased */
            if(Old_Pivot>New_Pivot)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1, Coord_Y+1+New_Pivot, Length-2, Old_Pivot-New_Pivot, Back, Back);
            }
            /* Progress increased */
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+1, Coord_Y+1+Old_Pivot, Length-2, New_Pivot-Old_Pivot, Fore, Fore);
            }
            break;
        }
        default: break;
    }
}
/* End Function:RMP_Progbar_Set **********************************************/

/* Function:RMP_Progbar_Prog **************************************************
Description : Draw a progress bar's progress part.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Prog - The progress of the progress bar.    
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.   
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar_Prog(rmp_cnt_t Coord_X,
                      rmp_cnt_t Coord_Y,
                      rmp_cnt_t Length,
                      rmp_cnt_t Width,
                      rmp_cnt_t Style,
                      rmp_cnt_t Prog,
                      rmp_ptr_t Fore,
                      rmp_ptr_t Back)
{
    /* The critical value for drawing the bar */
    rmp_cnt_t Pivot;
    
    switch(Style)
    {
        case RMP_PBAR_L2R:
        {
            RMP_COVERAGE_MARKER();

            Pivot=Length*Prog/100;
            if(Prog!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y, Pivot, Width, Fore, Fore);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+Pivot, Coord_Y, Length-Pivot, Width, Back, Back);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            break;
        }
        case RMP_PBAR_D2U:
        {
            RMP_COVERAGE_MARKER();

            Pivot=Width*Prog/100;
            if(Prog!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y+Width-Pivot, Length, Pivot, Fore, Fore);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y, Length, Width-Pivot, Back, Back);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            break;
        }
        case RMP_PBAR_R2L:
        {
            RMP_COVERAGE_MARKER();

            Pivot=Length*Prog/100;
            if(Prog!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X+Length-Pivot, Coord_Y, Pivot, Width, Fore, Fore);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y, Length-Pivot, Width, Back, Back);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            break;
        }
        case RMP_PBAR_U2D:
        {
            RMP_COVERAGE_MARKER();

            Pivot=Width*Prog/100;
            if(Prog!=0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y, Length, Pivot, Fore, Fore);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }

            if(Prog!=100)
            {
                RMP_COVERAGE_MARKER();
                RMP_Rectangle(Coord_X, Coord_Y+Pivot, Length, Width-Pivot, Back, Back);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                /* No action required */
            }
            break;
        }
        default: break;
    }
}
/* End Function:RMP_Progbar_Prog *********************************************/

/* Function:RMP_Progbar *******************************************************
Description : Draw a progress bar with string on it.
Input       : rmp_cnt_t Coord_X - The X coordinate of its top-left corner.
              rmp_cnt_t Coord_Y - The Y coordinate of its top-left corner.
              rmp_cnt_t Length - The length of the button.
              rmp_cnt_t Width - The width of the button.
              rmp_cnt_t Style - The style of the progress.
              rmp_cnt_t Prog - The progress of the progress bar. 
              rmp_ptr_t Fore - The foreground color.
              rmp_ptr_t Back - The background color.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Progbar(rmp_cnt_t Coord_X,
                 rmp_cnt_t Coord_Y,
                 rmp_cnt_t Length,
                 rmp_cnt_t Width,
                 rmp_cnt_t Style,
                 rmp_cnt_t Prog,
                 rmp_ptr_t Fore,
                 rmp_ptr_t Back)
{
    /* Draw the progress bar according to the style of the progress bar */
    RMP_Progbar_Prog(Coord_X, Coord_Y, Length, Width, Style, Prog, Fore, Back);
    /* Now draw the border */
    RMP_Line(Coord_X, Coord_Y, Coord_X+Length-1, Coord_Y, RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y, Coord_X+Length-1, Coord_Y+Width-1, RMP_CTL_BLACK);
    RMP_Line(Coord_X+Length-1, Coord_Y+Width-1, Coord_X, Coord_Y+Width-1, RMP_CTL_BLACK);
    RMP_Line(Coord_X, Coord_Y+Width-1, Coord_X, Coord_Y, RMP_CTL_BLACK);
}
#endif
#endif
/* End Function:RMP_Progbar **************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
