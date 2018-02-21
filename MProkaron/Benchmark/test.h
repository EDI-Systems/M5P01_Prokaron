
#include "RMP.h"

//const struct RVM_Hdr_Pgtbl RMP_Pgtbl[1]=
//{
//    /* The first page table */
//    {
//        0                                                                      /* ptr_t Parent */,
//        0x00000000                                                             /* ptr_t Addr */,
//        RME_PGTBL_ORDER(RME_PGTBL_SIZE_512M,RME_PGTBL_NUM_8)                   /* ptr_t Order */,
//        {                                                                      /* u8 Flags[8] */
//            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
//            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
//            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM,
//            RME_PGTBL_ALL_PERM,RME_PGTBL_ALL_PERM
//        }
//    }
//};

//const struct RVM_Image RMP_Image=
//{
//    RVM_MAGIC                                                                  /* const ptr_t Magic */,
//    "VM Domain 01"                                                             /* s8 Name[16] */,
//    _RMP_Entry                                                                 /* void* User_Entry */,
//    &RMP_User_Stack                                                            /* void* User_Stack */,
//    0x100                                                                      /* ptr_t User_Size */,
//    _RMP_Int                                                                   /* void* Int_Entry */,
//    &RMP_Int_Stack                                                             /* void* Int_Stack */,
//    0x400                                                                      /* ptr_t Int_Size */,
//    
//    &RMP_Param                                                                 /* struct RVM_Param* Param */,
//    &RMP_Regs                                                                  /* struct RVM_Regs* Regs */,
//    &RMP_Flag                                                                  /* struct RVM_Int_Flag* Int_Flags */,
//    
//    RMP_Console                                                                /* void* Console_Buf */,
//    RMP_KERNEL_DEBUG_MAX_STR                                                   /* ptr_t Console_Size */,
//    
//    1                                                                          /* ptr_t Prio */,
//    10                                                                         /* ptr_t Slices */,
//    
//    1                                                                          /* ptr_t Pgtbl_Num */,
//    RMP_Pgtbl                                                                  /* const struct RVM_Hdr_Pgtbl* Pgtbl */,
//    0                                                                          /* const struct RVM_Image* const * const Next_Image; */
//};

ptr_t Stack_1[256];
struct RMP_Thd Thd_1={0};

ptr_t Stack_2[256];
struct RMP_Thd Thd_2={0};

ptr_t Time;



