;/*****************************************************************************
;Filename    : platform_cmx_RVM.s
;Author      : pry
;Date        : 09/02/2018
;Description : The assembly part of the RMP RTOS for the RVM virtual machine.
;*****************************************************************************/

;/* The ARM Cortex-M Structure ************************************************
;R0-R7:General purpose registers that are accessible. 
;R8-R12:general purpose registers that can only be reached by 32-bit instructions.
;R13:SP/SP_process/SP_main    Stack pointer
;R14:LR                       Link Register(used for returning from a subfunction)
;R15:PC                       Program counter.
;IPSR                         Interrupt Program Status Register.
;APSR                         Application Program Status Register.
;EPSR                         Execute Program Status Register.
;The above 3 registers are saved into the stack in combination(xPSR).
;
;The ARM Cortex-M4 also include a single-accuracy FPU.
;*****************************************************************************/
            
;/* Begin Header *************************************************************/
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
RMP_User_Stack
User_Stack      SPACE   0x00000100
__initial_sp

RMP_Int_Stack
Int_Stack       SPACE   0x00000400

                ;The align is "(2^3)/8=1(Byte)." In fact it does not take effect.            
                AREA            RESET,CODE,READONLY,ALIGN=3
                
                THUMB
                REQUIRE8
                PRESERVE8
;/* End Header ***************************************************************/

;/* Begin Exports ************************************************************/
                ;The initial stack pointer position
                EXPORT          __initial_sp
                ;The user stack pointer position
                EXPORT          RMP_User_Stack
                ;The interrupt stack pointer position
                EXPORT          RMP_Int_Stack
                ;The entry stub
                EXPORT          _RMP_Entry
                ;Get the MSB                              
                EXPORT          RMP_MSB_Get
                ;Start the first thread
                EXPORT          _RMP_Start
                ;The hypercall maker
                EXPORT          _RMP_Hypercall
                ;The PendSV trigger
                EXPORT          _RMP_Yield  
                ;The interrupt handling routine receive endpoint 
                EXPORT          _RMP_Int_Rcv
                ;The atomic AND operation
                EXPORT          _RMP_Fetch_And                
;/* End Exports **************************************************************/

;/* Begin Imports ************************************************************/
                ;The real task switch handling function
                IMPORT          _RMP_Get_High_Rdy 
                ;The real systick handler function
                IMPORT          _RMP_Tick_Handler
                ;The PID of the current thread                     
                IMPORT          RMP_Cur_Thd
                ;The stack address of current thread
                IMPORT          RMP_Cur_SP        
                ;Save and load extra contexts, such as FPU, peripherals and MPU
                IMPORT          RMP_Save_Ctx
                IMPORT          RMP_Load_Ctx
                ;The RVM image header
                IMPORT          RMP_Image
                ;The real entry
                IMPORT          __main
                ;The context switch interrupt flag
                IMPORT          RMP_Ctxsw
;/* End Imports **************************************************************/

;/* Begin Global Variable ****************************************************/
                DCD             RMP_Image
;/* End Global Variable ******************************************************/

;/* Begin Function:_RMP_Entry *************************************************
;Description    : The entry of the OS, when running in an virtualized environment.
;Input          : None.
;Output         : None.
;Return         : None.   
;Register Usage : None. 
;*****************************************************************************/
_RMP_Entry
                 LDR     R0, =__main
                 BX      R0
;/* End Function:_RMP_Entry **************************************************/

;/* Begin Function:_RMP_Fetch_Or **********************************************
;Description : The fetch-and-logic-or atomic instruction. Logic OR the pointer
;              value with the operand, and return the value before logic OR.
;Input       : ptr_t* Ptr - The pointer to the data.
;              ptr_t Operand - The number to logic OR with the destination.
;Output      : ptr_t* Ptr - The pointer to the data.
;Return      : ptr_t - The value before the OR operation.
;*****************************************************************************/
_RMP_Fetch_And
                PUSH    {R4}
_RMP_Fetch_And_Fail
                LDREX   R2,[R0]             ; Load exclusive
                AND     R3,R2,R1            ; OR operation
                STREX   R4,R3,[R0]          ; Store exclusive
                CMP     R4,#0x00            ; See if we succeeded
                BNE     _RMP_Fetch_And_Fail ; Go back if we fail
                MOV     R0,R2
                POP     {R4}
                BX      LR
;/* End Function:_RMP_Fetch_Or ***********************************************/

;/* Begin Function:RMP_MSB_Get ************************************************
;Description    : Get the MSB of the word.
;Input          : ptr_t Val - The value.
;Output         : None.
;Return         : ptr_t - The MSB position.   
;Register Usage : None. 
;*****************************************************************************/
RMP_MSB_Get
                CLZ      R1,R0
                MOV      R0,#31
                SUB      R0,R1
                BX       LR
;/* End Function:RMP_MSB_Get *************************************************/

;/* Begin Function:_RMP_Hypercall *********************************************
;Description    : The stub to make a hypercall. This is actually a RME system call(asnd).
;Input          : None.
;Output         : None.
;Return         : None.   
;Register Usage : None. 
;*****************************************************************************/
_RMP_Hypercall
                PUSH       {R4-R5}     ; Manual clobbering
                MOV        R4,#0x20000 ; Asynchronous send
                MOV        R5,#0       ; To capability number 0.
                
                SVC        #0x00       ; System call
                ISB                    ; Instruction barrier - wait for instruction to complete.
                
                POP        {R4-R5}     ; Manual recovering
                BX         LR          ; Return from the call
                
                B          .           ; Shouldn't reach here.
;/* End Function:_RMP_Hypercall **********************************************/

;/* Begin Function:_RMP_Yield *************************************************
;Description : Trigger a yield to another thread. This will actually send a interrupt
;              to the interrupt thread.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Yield
                PUSH       {R4-R5}       ; Manual clobbering
                LDR        R4,=RMP_Ctxsw ; Set the context switch flag 
                MOV        R5,#0x01      ; Flag & capability number
                STR        R5,[R4]
                MOV        R4,#0x20000   ; Asynchronous send
                
                SVC        #0x00         ; System call
                ISB                      ; Instruction barrier - wait for instruction to complete.
                
                POP        {R4-R5}       ; Manual recovering
                BX         LR            ; Return from the call                                             
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Int_Rcv ***********************************************
;Description : Cause the interrupt handler routine to wait on the interrupt endpoint.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Int_Rcv
                PUSH       {R4-R5}       ; Manual clobbering
                MOV        R4,#0x30000   ; Asynchronous receive
                MOV        R5,#1         ; From capability number 1.
                
                SVC        #0x00         ; System call
                ISB                      ; Instruction barrier - wait for instruction to complete.
                
                POP        {R4-R5}       ; Manual recovering
                BX         LR            ; Return from the call                                             
;/* End Function:_RMP_Yield **************************************************/

;/* Begin Function:_RMP_Start *************************************************
;Description : Jump to the user function and will never return from it.
;Input       : None.
;Output      : None.                                      
;*****************************************************************************/
_RMP_Start
                ;Should never reach here
                SUB       R1,#16                       ; This is how we push our registers so move forward
                MOV       SP,R1                        ; Set the stack pointer
                
                DSB                                    ; Data and instruction barrier
                ISB
                
                BLX       R0                           ; Branch to our target
;/* End Function:_RMP_Start **************************************************/

                END
;/* End Of File **************************************************************/

;/* Copyright (C) 2011-2013 Evo-Devo Instrum. All rights reserved ************/
