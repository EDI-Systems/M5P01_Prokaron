
RMP:     file format elf32-littlearm


Disassembly of section .text:

08020000 <_RVM_Entry-0x4>:
 8020000:	080207e8 	.word	0x080207e8

08020004 <_RVM_Entry>:
 8020004:	f8df d0cc 	ldr.w	sp, [pc, #204]	; 80200d4 <_RVM_Kern+0x1c>
 8020008:	4833      	ldr	r0, [pc, #204]	; (80200d8 <_RVM_Kern+0x20>)
 802000a:	4934      	ldr	r1, [pc, #208]	; (80200dc <_RVM_Kern+0x24>)
 802000c:	4a34      	ldr	r2, [pc, #208]	; (80200e0 <_RVM_Kern+0x28>)

0802000e <load_data>:
 802000e:	4288      	cmp	r0, r1
 8020010:	d006      	beq.n	8020020 <load_done>
 8020012:	6813      	ldr	r3, [r2, #0]
 8020014:	6003      	str	r3, [r0, #0]
 8020016:	f100 0004 	add.w	r0, r0, #4
 802001a:	f102 0204 	add.w	r2, r2, #4
 802001e:	e7f6      	b.n	802000e <load_data>

08020020 <load_done>:
 8020020:	4830      	ldr	r0, [pc, #192]	; (80200e4 <_RVM_Kern+0x2c>)
 8020022:	4931      	ldr	r1, [pc, #196]	; (80200e8 <_RVM_Kern+0x30>)
 8020024:	2200      	movs	r2, #0

08020026 <clear_bss>:
 8020026:	4288      	cmp	r0, r1
 8020028:	d003      	beq.n	8020032 <clear_done>
 802002a:	6002      	str	r2, [r0, #0]
 802002c:	f100 0004 	add.w	r0, r0, #4
 8020030:	e7f9      	b.n	8020026 <clear_bss>

08020032 <clear_done>:
 8020032:	f000 fb3b 	bl	80206ac <__libc_init_array>
 8020036:	482d      	ldr	r0, [pc, #180]	; (80200ec <_RVM_Kern+0x34>)
 8020038:	4700      	bx	r0

0802003a <RVM_MSB_Get>:
 802003a:	fab0 f180 	clz	r1, r0
 802003e:	f04f 001f 	mov.w	r0, #31
 8020042:	eba0 0001 	sub.w	r0, r0, r1
 8020046:	4770      	bx	lr

08020048 <RVM_Fetch_And>:
 8020048:	b410      	push	{r4}

0802004a <RVM_Fetch_And_Fail>:
 802004a:	e850 2f00 	ldrex	r2, [r0]
 802004e:	ea02 0301 	and.w	r3, r2, r1
 8020052:	e840 3400 	strex	r4, r3, [r0]
 8020056:	2c00      	cmp	r4, #0
 8020058:	d1f7      	bne.n	802004a <RVM_Fetch_And_Fail>
 802005a:	4610      	mov	r0, r2
 802005c:	bc10      	pop	{r4}
 802005e:	4770      	bx	lr

08020060 <RVM_Fetch_Or>:
 8020060:	b410      	push	{r4}

08020062 <RVM_Fetch_Or_Fail>:
 8020062:	e850 2f00 	ldrex	r2, [r0]
 8020066:	ea42 0301 	orr.w	r3, r2, r1
 802006a:	e840 3400 	strex	r4, r3, [r0]
 802006e:	2c00      	cmp	r4, #0
 8020070:	d1f7      	bne.n	8020062 <RVM_Fetch_Or_Fail>
 8020072:	4610      	mov	r0, r2
 8020074:	bc10      	pop	{r4}
 8020076:	4770      	bx	lr

08020078 <_RVM_Hypercall>:
 8020078:	b430      	push	{r4, r5}
 802007a:	f44f 3400 	mov.w	r4, #131072	; 0x20000
 802007e:	f04f 0500 	mov.w	r5, #0
 8020082:	df00      	svc	0
 8020084:	f3bf 8f6f 	isb	sy
 8020088:	bc30      	pop	{r4, r5}
 802008a:	4770      	bx	lr

0802008c <RVM_Int_Rcv>:
 802008c:	b470      	push	{r4, r5, r6}
 802008e:	f44f 3440 	mov.w	r4, #196608	; 0x30000
 8020092:	f04f 0501 	mov.w	r5, #1
 8020096:	f04f 0601 	mov.w	r6, #1
 802009a:	df00      	svc	0
 802009c:	f3bf 8f6f 	isb	sy
 80200a0:	bc70      	pop	{r4, r5, r6}
 80200a2:	4770      	bx	lr

080200a4 <_RVM_Yield>:
 80200a4:	b430      	push	{r4, r5}
 80200a6:	f44f 3400 	mov.w	r4, #131072	; 0x20000
 80200aa:	f04f 0501 	mov.w	r5, #1
 80200ae:	df00      	svc	0
 80200b0:	f3bf 8f6f 	isb	sy
 80200b4:	bc30      	pop	{r4, r5}
 80200b6:	4770      	bx	lr

080200b8 <_RVM_Kern>:
 80200b8:	b4f0      	push	{r4, r5, r6, r7}
 80200ba:	f100 0402 	add.w	r4, r0, #2
 80200be:	f2c0 0404 	movt	r4, #4
 80200c2:	460d      	mov	r5, r1
 80200c4:	4616      	mov	r6, r2
 80200c6:	461f      	mov	r7, r3
 80200c8:	df00      	svc	0
 80200ca:	f3bf 8f6f 	isb	sy
 80200ce:	bcf0      	pop	{r4, r5, r6, r7}
 80200d0:	4770      	bx	lr
 80200d2:	0000      	.short	0x0000
 80200d4:	20080000 	.word	0x20080000
 80200d8:	20020000 	.word	0x20020000
 80200dc:	20020428 	.word	0x20020428
 80200e0:	08022000 	.word	0x08022000
 80200e4:	20020428 	.word	0x20020428
 80200e8:	20020838 	.word	0x20020838
 80200ec:	080205e9 	.word	0x080205e9

080200f0 <__do_global_dtors_aux>:
 80200f0:	b510      	push	{r4, lr}
 80200f2:	4c05      	ldr	r4, [pc, #20]	; (8020108 <__do_global_dtors_aux+0x18>)
 80200f4:	7823      	ldrb	r3, [r4, #0]
 80200f6:	b933      	cbnz	r3, 8020106 <__do_global_dtors_aux+0x16>
 80200f8:	4b04      	ldr	r3, [pc, #16]	; (802010c <__do_global_dtors_aux+0x1c>)
 80200fa:	b113      	cbz	r3, 8020102 <__do_global_dtors_aux+0x12>
 80200fc:	4804      	ldr	r0, [pc, #16]	; (8020110 <__do_global_dtors_aux+0x20>)
 80200fe:	f3af 8000 	nop.w
 8020102:	2301      	movs	r3, #1
 8020104:	7023      	strb	r3, [r4, #0]
 8020106:	bd10      	pop	{r4, pc}
 8020108:	20020428 	.word	0x20020428
 802010c:	00000000 	.word	0x00000000
 8020110:	08020864 	.word	0x08020864

08020114 <frame_dummy>:
 8020114:	4b08      	ldr	r3, [pc, #32]	; (8020138 <frame_dummy+0x24>)
 8020116:	b510      	push	{r4, lr}
 8020118:	b11b      	cbz	r3, 8020122 <frame_dummy+0xe>
 802011a:	4908      	ldr	r1, [pc, #32]	; (802013c <frame_dummy+0x28>)
 802011c:	4808      	ldr	r0, [pc, #32]	; (8020140 <frame_dummy+0x2c>)
 802011e:	f3af 8000 	nop.w
 8020122:	4808      	ldr	r0, [pc, #32]	; (8020144 <frame_dummy+0x30>)
 8020124:	6803      	ldr	r3, [r0, #0]
 8020126:	b903      	cbnz	r3, 802012a <frame_dummy+0x16>
 8020128:	bd10      	pop	{r4, pc}
 802012a:	4b07      	ldr	r3, [pc, #28]	; (8020148 <frame_dummy+0x34>)
 802012c:	2b00      	cmp	r3, #0
 802012e:	d0fb      	beq.n	8020128 <frame_dummy+0x14>
 8020130:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8020134:	4718      	bx	r3
 8020136:	bf00      	nop
 8020138:	00000000 	.word	0x00000000
 802013c:	2002042c 	.word	0x2002042c
 8020140:	08020864 	.word	0x08020864
 8020144:	20020428 	.word	0x20020428
 8020148:	00000000 	.word	0x00000000

0802014c <RMP_Init_Hook>:
 802014c:	4770      	bx	lr
 802014e:	bf00      	nop

08020150 <RMP_Init_Idle>:
    RMP_Thd_Crt(&Thd_2, Func_2, THD2_STACK, (void*)0x4321, 1, 1000);
#endif
}

void RMP_Init_Idle(void)
{
 8020150:	4770      	bx	lr
 8020152:	bf00      	nop

08020154 <RMP_PendSV_Handler>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_PendSV_Handler(void)
{
 8020154:	b570      	push	{r4, r5, r6, lr}
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
 8020156:	4c1e      	ldr	r4, [pc, #120]	; (80201d0 <RMP_PendSV_Handler+0x7c>)
    RMP_Save_Ctx();
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(rmp_ptr_t)SP;
 8020158:	4e1e      	ldr	r6, [pc, #120]	; (80201d4 <RMP_PendSV_Handler+0x80>)
{
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
 802015a:	6823      	ldr	r3, [r4, #0]
    *(--SP)=RVM_Regs.Reg.LR;
 802015c:	6a62      	ldr	r2, [r4, #36]	; 0x24
    *(--SP)=RVM_Regs.Reg.R9;
    *(--SP)=RVM_Regs.Reg.R8;
    *(--SP)=RVM_Regs.Reg.R7;
    *(--SP)=RVM_Regs.Reg.R6;
    *(--SP)=RVM_Regs.Reg.R5;
    *(--SP)=RVM_Regs.Reg.R4;
 802015e:	461d      	mov	r5, r3
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
    *(--SP)=RVM_Regs.Reg.LR;
 8020160:	f843 2c04 	str.w	r2, [r3, #-4]
    *(--SP)=RVM_Regs.Reg.R11;
 8020164:	6a22      	ldr	r2, [r4, #32]
 8020166:	f843 2c08 	str.w	r2, [r3, #-8]
    *(--SP)=RVM_Regs.Reg.R10;
 802016a:	69e2      	ldr	r2, [r4, #28]
 802016c:	f843 2c0c 	str.w	r2, [r3, #-12]
    *(--SP)=RVM_Regs.Reg.R9;
 8020170:	69a2      	ldr	r2, [r4, #24]
 8020172:	f843 2c10 	str.w	r2, [r3, #-16]
    *(--SP)=RVM_Regs.Reg.R8;
 8020176:	6962      	ldr	r2, [r4, #20]
 8020178:	f843 2c14 	str.w	r2, [r3, #-20]
    *(--SP)=RVM_Regs.Reg.R7;
 802017c:	6922      	ldr	r2, [r4, #16]
 802017e:	f843 2c18 	str.w	r2, [r3, #-24]
    *(--SP)=RVM_Regs.Reg.R6;
 8020182:	68e2      	ldr	r2, [r4, #12]
 8020184:	f843 2c1c 	str.w	r2, [r3, #-28]
    *(--SP)=RVM_Regs.Reg.R5;
 8020188:	68a2      	ldr	r2, [r4, #8]
 802018a:	f843 2c20 	str.w	r2, [r3, #-32]
    *(--SP)=RVM_Regs.Reg.R4;
 802018e:	6863      	ldr	r3, [r4, #4]
 8020190:	f845 3d24 	str.w	r3, [r5, #-36]!
    
    /* Save extra context
     * BL       RMP_Save_Ctx */
    RMP_Save_Ctx();
 8020194:	f000 fa24 	bl	80205e0 <RMP_Save_Ctx>
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(rmp_ptr_t)SP;
 8020198:	6035      	str	r5, [r6, #0]
                
    /* Get the highest ready task
     * BL        _RMP_Get_High_Rdy */
    _RMP_Get_High_Rdy();
 802019a:	f000 f945 	bl	8020428 <_RMP_Get_High_Rdy>
    
    /* Load the SP
     * LDR       R1,=RMP_Cur_SP
     * LDR       R0,[R1] */
    SP=(rmp_ptr_t*)RMP_Cur_SP;
 802019e:	6835      	ldr	r5, [r6, #0]
    
    /* Load extra context
     * BL        RMP_Load_Ctx */
    RMP_Load_Ctx();
 80201a0:	f000 fa20 	bl	80205e4 <RMP_Load_Ctx>
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
 80201a4:	682b      	ldr	r3, [r5, #0]
    RVM_Regs.Reg.R7=*(SP++);
    RVM_Regs.Reg.R8=*(SP++);
    RVM_Regs.Reg.R9=*(SP++);
    RVM_Regs.Reg.R10=*(SP++);
    RVM_Regs.Reg.R11=*(SP++);
    RVM_Regs.Reg.LR=*(SP++);
 80201a6:	f105 0224 	add.w	r2, r5, #36	; 0x24
    RMP_Load_Ctx();
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
 80201aa:	6063      	str	r3, [r4, #4]
    RVM_Regs.Reg.R5=*(SP++);
 80201ac:	686b      	ldr	r3, [r5, #4]
    RVM_Regs.Reg.R8=*(SP++);
    RVM_Regs.Reg.R9=*(SP++);
    RVM_Regs.Reg.R10=*(SP++);
    RVM_Regs.Reg.R11=*(SP++);
    RVM_Regs.Reg.LR=*(SP++);
    RVM_Regs.Reg.SP=(rmp_ptr_t)SP;
 80201ae:	6022      	str	r2, [r4, #0]
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
    RVM_Regs.Reg.R5=*(SP++);
 80201b0:	60a3      	str	r3, [r4, #8]
    RVM_Regs.Reg.R6=*(SP++);
 80201b2:	68ab      	ldr	r3, [r5, #8]
 80201b4:	60e3      	str	r3, [r4, #12]
    RVM_Regs.Reg.R7=*(SP++);
 80201b6:	68eb      	ldr	r3, [r5, #12]
 80201b8:	6123      	str	r3, [r4, #16]
    RVM_Regs.Reg.R8=*(SP++);
 80201ba:	692b      	ldr	r3, [r5, #16]
 80201bc:	6163      	str	r3, [r4, #20]
    RVM_Regs.Reg.R9=*(SP++);
 80201be:	696b      	ldr	r3, [r5, #20]
 80201c0:	61a3      	str	r3, [r4, #24]
    RVM_Regs.Reg.R10=*(SP++);
 80201c2:	69ab      	ldr	r3, [r5, #24]
 80201c4:	61e3      	str	r3, [r4, #28]
    RVM_Regs.Reg.R11=*(SP++);
 80201c6:	69eb      	ldr	r3, [r5, #28]
 80201c8:	6223      	str	r3, [r4, #32]
    RVM_Regs.Reg.LR=*(SP++);
 80201ca:	6a2b      	ldr	r3, [r5, #32]
 80201cc:	6263      	str	r3, [r4, #36]	; 0x24
 80201ce:	bd70      	pop	{r4, r5, r6, pc}
 80201d0:	200206ac 	.word	0x200206ac
 80201d4:	2002082c 	.word	0x2002082c

080201d8 <RMP_SysTick_Handler>:
{
    /* PUSH      {LR} */
    /* Note the system that we have entered an interrupt. We are not using tickless here */
    /* MOV       R0,#0x01 */
    /* BL        _RMP_Tick_Handler */
    _RMP_Tick_Handler(1);
 80201d8:	2001      	movs	r0, #1
 80201da:	f000 b969 	b.w	80204b0 <_RMP_Tick_Handler>
 80201de:	bf00      	nop

080201e0 <_RMP_Low_Level_Init>:
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Low_Level_Init(void)
{
 80201e0:	b508      	push	{r3, lr}
    RVM_Init();
 80201e2:	f000 f871 	bl	80202c8 <RVM_Init>
    RVM_Vect_Init(0, RMP_SysTick_Handler);
 80201e6:	4906      	ldr	r1, [pc, #24]	; (8020200 <_RMP_Low_Level_Init+0x20>)
 80201e8:	2000      	movs	r0, #0
 80201ea:	f000 f88b 	bl	8020304 <RVM_Vect_Init>
    RVM_Vect_Init(1, RMP_PendSV_Handler);
 80201ee:	4905      	ldr	r1, [pc, #20]	; (8020204 <_RMP_Low_Level_Init+0x24>)
 80201f0:	2001      	movs	r0, #1
 80201f2:	f000 f887 	bl	8020304 <RVM_Vect_Init>
    RMP_Console_Ptr=0;
 80201f6:	4b04      	ldr	r3, [pc, #16]	; (8020208 <_RMP_Low_Level_Init+0x28>)
 80201f8:	2200      	movs	r2, #0
 80201fa:	601a      	str	r2, [r3, #0]
 80201fc:	bd08      	pop	{r3, pc}
 80201fe:	bf00      	nop
 8020200:	080201d9 	.word	0x080201d9
 8020204:	08020155 	.word	0x08020155
 8020208:	20020444 	.word	0x20020444

0802020c <_RMP_Plat_Hook>:
 802020c:	f000 b888 	b.w	8020320 <RVM_Enable_Int>

08020210 <_RMP_Yield>:
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    RVM_Yield();
 8020210:	f000 b8b0 	b.w	8020374 <RVM_Yield>
 8020214:	0000      	movs	r0, r0
	...

08020218 <RMP_MSB_Get>:
 8020218:	fab0 f180 	clz	r1, r0
 802021c:	f04f 001f 	mov.w	r0, #31
 8020220:	eba0 0001 	sub.w	r0, r0, r1
 8020224:	4770      	bx	lr

08020226 <_RMP_Start>:
 8020226:	f1a1 0110 	sub.w	r1, r1, #16
 802022a:	468d      	mov	sp, r1
 802022c:	f3bf 8f6f 	isb	sy
 8020230:	4780      	blx	r0
 8020232:	bf00      	nop

08020234 <RVM_Int>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Int(void)
{
 8020234:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8020238:	4c1e      	ldr	r4, [pc, #120]	; (80202b4 <RVM_Int+0x80>)
                }
                Int_Num=RVM_Get_Int();
            }
        }
        else
            RVM_Int_Pend=1;
 802023a:	2701      	movs	r7, #1
 802023c:	f8df a080 	ldr.w	sl, [pc, #128]	; 80202c0 <RVM_Int+0x8c>
 8020240:	4d1d      	ldr	r5, [pc, #116]	; (80202b8 <RVM_Int+0x84>)
 8020242:	4e1e      	ldr	r6, [pc, #120]	; (80202bc <RVM_Int+0x88>)
 8020244:	f8df 807c 	ldr.w	r8, [pc, #124]	; 80202c4 <RVM_Int+0x90>
{
    rvm_cnt_t Int_Num;
    
    while(1)
    {
        RVM_Int_Rcv();
 8020248:	f7ff ff20 	bl	802008c <RVM_Int_Rcv>
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
 802024c:	6823      	ldr	r3, [r4, #0]
 802024e:	b36b      	cbz	r3, 80202ac <RVM_Int+0x78>
        {
            RVM_Int_Pend=0;
 8020250:	f04f 0900 	mov.w	r9, #0
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 8020254:	6828      	ldr	r0, [r5, #0]
    {
        RVM_Int_Rcv();
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
        {
            RVM_Int_Pend=0;
 8020256:	f8ca 9000 	str.w	r9, [sl]
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 802025a:	2800      	cmp	r0, #0
 802025c:	d0f4      	beq.n	8020248 <RVM_Int+0x14>
            continue;
        
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
 802025e:	f7ff feec 	bl	802003a <RVM_MSB_Get>
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 8020262:	f1b0 0b00 	subs.w	fp, r0, #0
 8020266:	dbef      	blt.n	8020248 <RVM_Int+0x14>
        RVM_Fetch_And(&RVM_Flag[Count],~(((rvm_ptr_t)1)<<Pos));
 8020268:	fa07 f10b 	lsl.w	r1, r7, fp
 802026c:	4812      	ldr	r0, [pc, #72]	; (80202b8 <RVM_Int+0x84>)
 802026e:	43c9      	mvns	r1, r1
 8020270:	f7ff feea 	bl	8020048 <RVM_Fetch_And>
            /* Look for interrupts to handle from the first */
            Int_Num=RVM_Get_Int();
            /* Handle the interrupt here - the interrupt is tail-chained */
            while(Int_Num>=0)
            {
                if(RVM_Vect[Int_Num]!=0)
 8020274:	f856 302b 	ldr.w	r3, [r6, fp, lsl #2]
 8020278:	b12b      	cbz	r3, 8020286 <RVM_Int+0x52>
                {
                    RVM_Int_Active=1;
                    ((void(*)(rvm_ptr_t))RVM_Vect[Int_Num])(Int_Num);
 802027a:	4658      	mov	r0, fp
            /* Handle the interrupt here - the interrupt is tail-chained */
            while(Int_Num>=0)
            {
                if(RVM_Vect[Int_Num]!=0)
                {
                    RVM_Int_Active=1;
 802027c:	f8c8 7000 	str.w	r7, [r8]
                    ((void(*)(rvm_ptr_t))RVM_Vect[Int_Num])(Int_Num);
 8020280:	4798      	blx	r3
                    RVM_Int_Active=0;
 8020282:	f8c8 9000 	str.w	r9, [r8]
{
    rvm_cnt_t Count;
    rvm_cnt_t Pos;
    
    /* See if interrupt enabled */
    if(RVM_Int_Enable==0)
 8020286:	6823      	ldr	r3, [r4, #0]
 8020288:	2b00      	cmp	r3, #0
 802028a:	d0dd      	beq.n	8020248 <RVM_Int+0x14>
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 802028c:	6828      	ldr	r0, [r5, #0]
 802028e:	2800      	cmp	r0, #0
 8020290:	d0da      	beq.n	8020248 <RVM_Int+0x14>
            continue;
        
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
 8020292:	f7ff fed2 	bl	802003a <RVM_MSB_Get>
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 8020296:	f1b0 0b00 	subs.w	fp, r0, #0
        RVM_Fetch_And(&RVM_Flag[Count],~(((rvm_ptr_t)1)<<Pos));
 802029a:	4807      	ldr	r0, [pc, #28]	; (80202b8 <RVM_Int+0x84>)
 802029c:	fa07 f10b 	lsl.w	r1, r7, fp
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 80202a0:	dae5      	bge.n	802026e <RVM_Int+0x3a>
{
    rvm_cnt_t Int_Num;
    
    while(1)
    {
        RVM_Int_Rcv();
 80202a2:	f7ff fef3 	bl	802008c <RVM_Int_Rcv>
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
 80202a6:	6823      	ldr	r3, [r4, #0]
 80202a8:	2b00      	cmp	r3, #0
 80202aa:	d1d1      	bne.n	8020250 <RVM_Int+0x1c>
                }
                Int_Num=RVM_Get_Int();
            }
        }
        else
            RVM_Int_Pend=1;
 80202ac:	f8ca 7000 	str.w	r7, [sl]
 80202b0:	e7ca      	b.n	8020248 <RVM_Int+0x14>
 80202b2:	bf00      	nop
 80202b4:	2002044c 	.word	0x2002044c
 80202b8:	20020714 	.word	0x20020714
 80202bc:	2002072c 	.word	0x2002072c
 80202c0:	20020448 	.word	0x20020448
 80202c4:	20020450 	.word	0x20020450

080202c8 <RVM_Init>:
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80202c8:	2000      	movs	r0, #0
 80202ca:	4b09      	ldr	r3, [pc, #36]	; (80202f0 <RVM_Init+0x28>)
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Init(void)
{
 80202cc:	b4f0      	push	{r4, r5, r6, r7}
    RVM_Int_Active=0;
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
        RVM_Vect[Count]=0;
 80202ce:	4601      	mov	r1, r0
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80202d0:	4f08      	ldr	r7, [pc, #32]	; (80202f4 <RVM_Init+0x2c>)
    RVM_Int_Pend=0;
 80202d2:	4e09      	ldr	r6, [pc, #36]	; (80202f8 <RVM_Init+0x30>)
 80202d4:	f103 0280 	add.w	r2, r3, #128	; 0x80
    RVM_Int_Active=0;
 80202d8:	4d08      	ldr	r5, [pc, #32]	; (80202fc <RVM_Init+0x34>)
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
 80202da:	4c09      	ldr	r4, [pc, #36]	; (8020300 <RVM_Init+0x38>)
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80202dc:	6038      	str	r0, [r7, #0]
    RVM_Int_Pend=0;
 80202de:	6030      	str	r0, [r6, #0]
    RVM_Int_Active=0;
 80202e0:	6028      	str	r0, [r5, #0]
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
 80202e2:	6020      	str	r0, [r4, #0]
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
        RVM_Vect[Count]=0;
 80202e4:	f843 1f04 	str.w	r1, [r3, #4]!
    RVM_Int_Pend=0;
    RVM_Int_Active=0;
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
 80202e8:	4293      	cmp	r3, r2
 80202ea:	d1fb      	bne.n	80202e4 <RVM_Init+0x1c>
        RVM_Vect[Count]=0;
}
 80202ec:	bcf0      	pop	{r4, r5, r6, r7}
 80202ee:	4770      	bx	lr
 80202f0:	20020728 	.word	0x20020728
 80202f4:	2002044c 	.word	0x2002044c
 80202f8:	20020448 	.word	0x20020448
 80202fc:	20020450 	.word	0x20020450
 8020300:	20020714 	.word	0x20020714

08020304 <RVM_Vect_Init>:
Output      : None.
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
 8020304:	281f      	cmp	r0, #31
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
 8020306:	4603      	mov	r3, r0
    if(Num>=RVM_MAX_INTVECT)
 8020308:	d804      	bhi.n	8020314 <RVM_Vect_Init+0x10>
        return -1;
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
 802030a:	4a04      	ldr	r2, [pc, #16]	; (802031c <RVM_Vect_Init+0x18>)
    return 0;
 802030c:	2000      	movs	r0, #0
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
        return -1;
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
 802030e:	f842 1023 	str.w	r1, [r2, r3, lsl #2]
    return 0;
 8020312:	4770      	bx	lr
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
        return -1;
 8020314:	f04f 30ff 	mov.w	r0, #4294967295
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
    return 0;
}
 8020318:	4770      	bx	lr
 802031a:	bf00      	nop
 802031c:	2002072c 	.word	0x2002072c

08020320 <RVM_Enable_Int>:
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8020320:	4b05      	ldr	r3, [pc, #20]	; (8020338 <RVM_Enable_Int+0x18>)
 8020322:	2200      	movs	r2, #0
Return      : None.
******************************************************************************/
void RVM_Enable_Int(void)
{
    /* Must be successful */
    RVM_Int_Enable=1;
 8020324:	4905      	ldr	r1, [pc, #20]	; (802033c <RVM_Enable_Int+0x1c>)
 8020326:	2001      	movs	r0, #1
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8020328:	601a      	str	r2, [r3, #0]
Return      : None.
******************************************************************************/
void RVM_Enable_Int(void)
{
    /* Must be successful */
    RVM_Int_Enable=1;
 802032a:	6008      	str	r0, [r1, #0]
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
 802032c:	605a      	str	r2, [r3, #4]
    RVM_Param.Param[1]=Param2;
 802032e:	609a      	str	r2, [r3, #8]
    RVM_Param.Param[2]=Param3;
 8020330:	60da      	str	r2, [r3, #12]
    RVM_Param.Param[3]=Param4;
 8020332:	611a      	str	r2, [r3, #16]
    
    /* Do the hypercall */
    _RVM_Hypercall();
 8020334:	f7ff bea0 	b.w	8020078 <_RVM_Hypercall>
 8020338:	20020718 	.word	0x20020718
 802033c:	2002044c 	.word	0x2002044c

08020340 <RVM_Mask_Int>:
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Mask_Int(void)
{
    RVM_Int_Pend=0;
 8020340:	2300      	movs	r3, #0
 8020342:	4902      	ldr	r1, [pc, #8]	; (802034c <RVM_Mask_Int+0xc>)
    RVM_Int_Enable=0;
 8020344:	4a02      	ldr	r2, [pc, #8]	; (8020350 <RVM_Mask_Int+0x10>)
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Mask_Int(void)
{
    RVM_Int_Pend=0;
 8020346:	600b      	str	r3, [r1, #0]
    RVM_Int_Enable=0;
 8020348:	6013      	str	r3, [r2, #0]
 802034a:	4770      	bx	lr
 802034c:	20020448 	.word	0x20020448
 8020350:	2002044c 	.word	0x2002044c

08020354 <RVM_Unmask_Int>:
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 8020354:	4b05      	ldr	r3, [pc, #20]	; (802036c <RVM_Unmask_Int+0x18>)
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
 8020356:	2001      	movs	r0, #1
 8020358:	4905      	ldr	r1, [pc, #20]	; (8020370 <RVM_Unmask_Int+0x1c>)
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 802035a:	681a      	ldr	r2, [r3, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
 802035c:	6008      	str	r0, [r1, #0]
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 802035e:	b902      	cbnz	r2, 8020362 <RVM_Unmask_Int+0xe>
 8020360:	4770      	bx	lr
    {
        RVM_Int_Pend=0;
 8020362:	2200      	movs	r2, #0
 8020364:	601a      	str	r2, [r3, #0]
        _RVM_Yield();
 8020366:	f7ff be9d 	b.w	80200a4 <_RVM_Yield>
 802036a:	bf00      	nop
 802036c:	20020448 	.word	0x20020448
 8020370:	2002044c 	.word	0x2002044c

08020374 <RVM_Yield>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Yield(void)
{
 8020374:	b508      	push	{r3, lr}
    RVM_Fetch_Or(RVM_Flag,0x02);
 8020376:	2102      	movs	r1, #2
 8020378:	4808      	ldr	r0, [pc, #32]	; (802039c <RVM_Yield+0x28>)
 802037a:	f7ff fe71 	bl	8020060 <RVM_Fetch_Or>
    
    /* We send a new trigger if the interrupt is not masked and we are not in interrupt */
    if(RVM_Int_Active==0)
 802037e:	4b08      	ldr	r3, [pc, #32]	; (80203a0 <RVM_Yield+0x2c>)
 8020380:	681b      	ldr	r3, [r3, #0]
 8020382:	b92b      	cbnz	r3, 8020390 <RVM_Yield+0x1c>
    {
        if(RVM_Int_Enable!=0)
 8020384:	4b07      	ldr	r3, [pc, #28]	; (80203a4 <RVM_Yield+0x30>)
 8020386:	681b      	ldr	r3, [r3, #0]
 8020388:	b91b      	cbnz	r3, 8020392 <RVM_Yield+0x1e>
            _RVM_Yield();
        else
            RVM_Int_Pend=1;
 802038a:	4b07      	ldr	r3, [pc, #28]	; (80203a8 <RVM_Yield+0x34>)
 802038c:	2201      	movs	r2, #1
 802038e:	601a      	str	r2, [r3, #0]
 8020390:	bd08      	pop	{r3, pc}
    }
}
 8020392:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
    
    /* We send a new trigger if the interrupt is not masked and we are not in interrupt */
    if(RVM_Int_Active==0)
    {
        if(RVM_Int_Enable!=0)
            _RVM_Yield();
 8020396:	f7ff be85 	b.w	80200a4 <_RVM_Yield>
 802039a:	bf00      	nop
 802039c:	20020714 	.word	0x20020714
 80203a0:	20020450 	.word	0x20020450
 80203a4:	2002044c 	.word	0x2002044c
 80203a8:	20020448 	.word	0x20020448

080203ac <RMP_Unlock_Sched>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
 80203ac:	b508      	push	{r3, lr}
    if(RMP_Sched_Lock_Cnt==1)
 80203ae:	4b0f      	ldr	r3, [pc, #60]	; (80203ec <RMP_Unlock_Sched+0x40>)
 80203b0:	681a      	ldr	r2, [r3, #0]
 80203b2:	2a01      	cmp	r2, #1
 80203b4:	d006      	beq.n	80203c4 <RMP_Unlock_Sched+0x18>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80203b6:	681a      	ldr	r2, [r3, #0]
 80203b8:	2a01      	cmp	r2, #1
 80203ba:	d90e      	bls.n	80203da <RMP_Unlock_Sched+0x2e>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80203bc:	681a      	ldr	r2, [r3, #0]
 80203be:	3a01      	subs	r2, #1
 80203c0:	601a      	str	r2, [r3, #0]
 80203c2:	bd08      	pop	{r3, pc}
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80203c4:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
 80203c6:	480a      	ldr	r0, [pc, #40]	; (80203f0 <RMP_Unlock_Sched+0x44>)
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80203c8:	490a      	ldr	r1, [pc, #40]	; (80203f4 <RMP_Unlock_Sched+0x48>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80203ca:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80203cc:	6002      	str	r2, [r0, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80203ce:	680b      	ldr	r3, [r1, #0]
 80203d0:	b923      	cbnz	r3, 80203dc <RMP_Unlock_Sched+0x30>
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
        while(1);
}
 80203d2:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80203d6:	f7ff bfbd 	b.w	8020354 <RVM_Unmask_Int>
 80203da:	e7fe      	b.n	80203da <RMP_Unlock_Sched+0x2e>
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 80203dc:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 80203de:	f7ff ff17 	bl	8020210 <_RMP_Yield>
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
        while(1);
}
 80203e2:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80203e6:	f7ff bfb5 	b.w	8020354 <RVM_Unmask_Int>
 80203ea:	bf00      	nop
 80203ec:	2002065c 	.word	0x2002065c
 80203f0:	200206a8 	.word	0x200206a8
 80203f4:	20020558 	.word	0x20020558

080203f8 <RMP_Init>:
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init(void)
{
 80203f8:	b508      	push	{r3, lr}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 80203fa:	f7ff ffa1 	bl	8020340 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 80203fe:	2101      	movs	r1, #1
    RMP_Sched_Lock_Cnt++;
 8020400:	4a07      	ldr	r2, [pc, #28]	; (8020420 <RMP_Init+0x28>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8020402:	4b08      	ldr	r3, [pc, #32]	; (8020424 <RMP_Init+0x2c>)
 8020404:	6019      	str	r1, [r3, #0]
    RMP_Sched_Lock_Cnt++;
 8020406:	6813      	ldr	r3, [r2, #0]
 8020408:	440b      	add	r3, r1
 802040a:	6013      	str	r3, [r2, #0]
void RMP_Init(void)
{
    RMP_Lock_Sched();
    
    /* Platform will use this hook to do something */
    _RMP_Plat_Hook();
 802040c:	f7ff fefe 	bl	802020c <_RMP_Plat_Hook>
    /* Start the second thread here */
    RMP_Init_Hook();
 8020410:	f7ff fe9c 	bl	802014c <RMP_Init_Hook>
    
    RMP_Unlock_Sched();
 8020414:	f7ff ffca 	bl	80203ac <RMP_Unlock_Sched>
    
    while(1)
        RMP_Init_Idle();
 8020418:	f7ff fe9a 	bl	8020150 <RMP_Init_Idle>
 802041c:	e7fc      	b.n	8020418 <RMP_Init+0x20>
 802041e:	bf00      	nop
 8020420:	2002065c 	.word	0x2002065c
 8020424:	200206a8 	.word	0x200206a8

08020428 <_RMP_Get_High_Rdy>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Get_High_Rdy(void)
{
 8020428:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
    
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
 802042a:	4a1c      	ldr	r2, [pc, #112]	; (802049c <_RMP_Get_High_Rdy+0x74>)
 802042c:	2600      	movs	r6, #0
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 802042e:	4c1c      	ldr	r4, [pc, #112]	; (80204a0 <_RMP_Get_High_Rdy+0x78>)
 8020430:	4d1c      	ldr	r5, [pc, #112]	; (80204a4 <_RMP_Get_High_Rdy+0x7c>)
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 8020432:	4b1d      	ldr	r3, [pc, #116]	; (80204a8 <_RMP_Get_High_Rdy+0x80>)
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 8020434:	6821      	ldr	r1, [r4, #0]
 8020436:	6828      	ldr	r0, [r5, #0]
    
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
 8020438:	6016      	str	r6, [r2, #0]
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 802043a:	681a      	ldr	r2, [r3, #0]
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 802043c:	6348      	str	r0, [r1, #52]	; 0x34
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 802043e:	b91a      	cbnz	r2, 8020448 <_RMP_Get_High_Rdy+0x20>
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
 8020440:	6823      	ldr	r3, [r4, #0]
 8020442:	6b5b      	ldr	r3, [r3, #52]	; 0x34
 8020444:	602b      	str	r3, [r5, #0]
 8020446:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
            continue;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Count=RMP_MSB_Get(RMP_Bitmap[Count])+(Count<<RMP_WORD_ORDER);
 8020448:	6818      	ldr	r0, [r3, #0]
 802044a:	f7ff fee5 	bl	8020218 <RMP_MSB_Get>
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
 802044e:	4b17      	ldr	r3, [pc, #92]	; (80204ac <_RMP_Get_High_Rdy+0x84>)
 8020450:	00c0      	lsls	r0, r0, #3
 8020452:	181a      	adds	r2, r3, r0
 8020454:	6851      	ldr	r1, [r2, #4]
 8020456:	6822      	ldr	r2, [r4, #0]
 8020458:	4291      	cmp	r1, r2
 802045a:	d00a      	beq.n	8020472 <_RMP_Get_High_Rdy+0x4a>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
 802045c:	6822      	ldr	r2, [r4, #0]
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
 802045e:	4418      	add	r0, r3
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
 8020460:	6823      	ldr	r3, [r4, #0]
 8020462:	69db      	ldr	r3, [r3, #28]
 8020464:	6213      	str	r3, [r2, #32]
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
 8020466:	6843      	ldr	r3, [r0, #4]
 8020468:	6023      	str	r3, [r4, #0]
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
 802046a:	6823      	ldr	r3, [r4, #0]
 802046c:	6b5b      	ldr	r3, [r3, #52]	; 0x34
 802046e:	602b      	str	r3, [r5, #0]
 8020470:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
 8020472:	6822      	ldr	r2, [r4, #0]
 8020474:	6821      	ldr	r1, [r4, #0]
 8020476:	6812      	ldr	r2, [r2, #0]
 8020478:	6849      	ldr	r1, [r1, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802047a:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 802047c:	6051      	str	r1, [r2, #4]
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 802047e:	6822      	ldr	r2, [r4, #0]
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
 8020480:	6821      	ldr	r1, [r4, #0]
 8020482:	6a49      	ldr	r1, [r1, #36]	; 0x24
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 8020484:	f853 1031 	ldr.w	r1, [r3, r1, lsl #3]
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
                         &(RMP_Run[RMP_Cur_Thd->Prio]));
 8020488:	6826      	ldr	r6, [r4, #0]
 802048a:	6a76      	ldr	r6, [r6, #36]	; 0x24
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 802048c:	eb03 07c6 	add.w	r7, r3, r6, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8020490:	f843 2036 	str.w	r2, [r3, r6, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8020494:	6057      	str	r7, [r2, #4]
    New->Prev=(struct RMP_List*)Prev;
 8020496:	6011      	str	r1, [r2, #0]
    Prev->Next=(struct RMP_List*)New;
 8020498:	604a      	str	r2, [r1, #4]
 802049a:	e7df      	b.n	802045c <_RMP_Get_High_Rdy+0x34>
 802049c:	20020558 	.word	0x20020558
 80204a0:	20020830 	.word	0x20020830
 80204a4:	2002082c 	.word	0x2002082c
 80204a8:	20020554 	.word	0x20020554
 80204ac:	2002055c 	.word	0x2002055c

080204b0 <_RMP_Tick_Handler>:
Input       : rmp_ptr_t Ticks - How many ticks have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tick_Handler(rmp_ptr_t Ticks)
{
 80204b0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
 80204b4:	4d44      	ldr	r5, [pc, #272]	; (80205c8 <_RMP_Tick_Handler+0x118>)
    
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>RMP_Cur_Thd->Slices_Left)
 80204b6:	4e45      	ldr	r6, [pc, #276]	; (80205cc <_RMP_Tick_Handler+0x11c>)
******************************************************************************/
void _RMP_Tick_Handler(rmp_ptr_t Ticks)
{
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
 80204b8:	682b      	ldr	r3, [r5, #0]
 80204ba:	4403      	add	r3, r0
 80204bc:	602b      	str	r3, [r5, #0]
    
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>RMP_Cur_Thd->Slices_Left)
 80204be:	6833      	ldr	r3, [r6, #0]
 80204c0:	6a1b      	ldr	r3, [r3, #32]
 80204c2:	4298      	cmp	r0, r3
 80204c4:	d973      	bls.n	80205ae <_RMP_Tick_Handler+0xfe>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
 80204c6:	4c42      	ldr	r4, [pc, #264]	; (80205d0 <_RMP_Tick_Handler+0x120>)
 80204c8:	2301      	movs	r3, #1
 80204ca:	6023      	str	r3, [r4, #0]
        RMP_COVERAGE_MARKER();
        RMP_Cur_Thd->Slices_Left-=Ticks;
    }
    
    /* Check if there are any timer events */
    if((&RMP_Delay)!=RMP_Delay.Next)
 80204cc:	4a41      	ldr	r2, [pc, #260]	; (80205d4 <_RMP_Tick_Handler+0x124>)
 80204ce:	6853      	ldr	r3, [r2, #4]
 80204d0:	4293      	cmp	r3, r2
 80204d2:	d067      	beq.n	80205a4 <_RMP_Tick_Handler+0xf4>
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 80204d4:	6851      	ldr	r1, [r2, #4]
        /* If the value is less than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)<=(RMP_ALLBITS>>1))
 80204d6:	682b      	ldr	r3, [r5, #0]
 80204d8:	6a09      	ldr	r1, [r1, #32]
 80204da:	1a5b      	subs	r3, r3, r1
 80204dc:	2b00      	cmp	r3, #0
 80204de:	db61      	blt.n	80205a4 <_RMP_Tick_Handler+0xf4>
void _RMP_Timer_Proc(void)
{
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
 80204e0:	4692      	mov	sl, r2
            }
            
            case RMP_THD_RCVDLY:
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
 80204e2:	f06f 0c04 	mvn.w	ip, #4
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 80204e6:	f8df e0f4 	ldr.w	lr, [pc, #244]	; 80205dc <_RMP_Tick_Handler+0x12c>
 80204ea:	f04f 0901 	mov.w	r9, #1
void _RMP_Timer_Proc(void)
{
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
 80204ee:	6853      	ldr	r3, [r2, #4]
 80204f0:	4293      	cmp	r3, r2
 80204f2:	d057      	beq.n	80205a4 <_RMP_Tick_Handler+0xf4>
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 80204f4:	f8da 7004 	ldr.w	r7, [sl, #4]
        /* If the value is more than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)>(RMP_ALLBITS>>1))
 80204f8:	6829      	ldr	r1, [r5, #0]
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 80204fa:	f1a7 0308 	sub.w	r3, r7, #8
        /* If the value is more than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)>(RMP_ALLBITS>>1))
 80204fe:	6a98      	ldr	r0, [r3, #40]	; 0x28
 8020500:	1a09      	subs	r1, r1, r0
 8020502:	2900      	cmp	r1, #0
 8020504:	db4e      	blt.n	80205a4 <_RMP_Tick_Handler+0xf4>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 8020506:	6899      	ldr	r1, [r3, #8]
 8020508:	f8d3 800c 	ldr.w	r8, [r3, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802050c:	f8c8 1000 	str.w	r1, [r8]
    Prev->Next=(struct RMP_List*)Next;
 8020510:	f8c1 8004 	str.w	r8, [r1, #4]
        else
            RMP_COVERAGE_MARKER();
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        switch(RMP_THD_STATE(Thread->State))
 8020514:	6999      	ldr	r1, [r3, #24]
 8020516:	fa5f f881 	uxtb.w	r8, r1
 802051a:	f1a8 0803 	sub.w	r8, r8, #3
 802051e:	f1b8 0f05 	cmp.w	r8, #5
 8020522:	d83e      	bhi.n	80205a2 <_RMP_Tick_Handler+0xf2>
 8020524:	e8df f008 	tbb	[pc, r8]
 8020528:	0e3a3d03 	.word	0x0e3a3d03
 802052c:	033d      	.short	0x033d
        {
            case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
            case RMP_THD_SEMDLY:
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 802052e:	f857 1c08 	ldr.w	r1, [r7, #-8]
 8020532:	f8d3 8004 	ldr.w	r8, [r3, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8020536:	f8c8 1000 	str.w	r1, [r8]
    Prev->Next=(struct RMP_List*)Next;
 802053a:	f8c1 8004 	str.w	r8, [r1, #4]
 802053e:	6999      	ldr	r1, [r3, #24]
            case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
            case RMP_THD_SEMDLY:
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
                Thread->Retval=RMP_ERR_OPER;
 8020540:	f8c3 c038 	str.w	ip, [r3, #56]	; 0x38
            case RMP_THD_DELAYED:RMP_COVERAGE_MARKER();break;
            /* Should not get here */
            default:while(1);
        }

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 8020544:	f021 01ff 	bic.w	r1, r1, #255	; 0xff
 8020548:	f041 0101 	orr.w	r1, r1, #1
 802054c:	6199      	str	r1, [r3, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 802054e:	6999      	ldr	r1, [r3, #24]
 8020550:	05c9      	lsls	r1, r1, #23
 8020552:	d4cc      	bmi.n	80204ee <_RMP_Tick_Handler+0x3e>
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8020554:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8020556:	4820      	ldr	r0, [pc, #128]	; (80205d8 <_RMP_Tick_Handler+0x128>)
 8020558:	f850 1031 	ldr.w	r1, [r0, r1, lsl #3]
 802055c:	f8d3 8024 	ldr.w	r8, [r3, #36]	; 0x24
 8020560:	eb00 0bc8 	add.w	fp, r0, r8, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8020564:	f840 3038 	str.w	r3, [r0, r8, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8020568:	f8c3 b004 	str.w	fp, [r3, #4]
    New->Prev=(struct RMP_List*)Prev;
 802056c:	f847 1c08 	str.w	r1, [r7, #-8]
    Prev->Next=(struct RMP_List*)New;
 8020570:	604b      	str	r3, [r1, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8020572:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8020574:	6a5f      	ldr	r7, [r3, #36]	; 0x24
 8020576:	f001 011f 	and.w	r1, r1, #31
 802057a:	097f      	lsrs	r7, r7, #5
 802057c:	fa09 f101 	lsl.w	r1, r9, r1
 8020580:	f85e 8027 	ldr.w	r8, [lr, r7, lsl #2]
 8020584:	ea41 0108 	orr.w	r1, r1, r8
 8020588:	f84e 1027 	str.w	r1, [lr, r7, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 802058c:	6a59      	ldr	r1, [r3, #36]	; 0x24
 802058e:	6833      	ldr	r3, [r6, #0]
 8020590:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 8020592:	4299      	cmp	r1, r3
 8020594:	d9ab      	bls.n	80204ee <_RMP_Tick_Handler+0x3e>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8020596:	2301      	movs	r3, #1
 8020598:	6023      	str	r3, [r4, #0]
 802059a:	e7a6      	b.n	80204ea <_RMP_Tick_Handler+0x3a>
            }
            
            case RMP_THD_RCVDLY:
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
 802059c:	f8c3 c038 	str.w	ip, [r3, #56]	; 0x38
 80205a0:	e7d0      	b.n	8020544 <_RMP_Tick_Handler+0x94>
 80205a2:	e7fe      	b.n	80205a2 <_RMP_Tick_Handler+0xf2>
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(RMP_Sched_Pend!=0)
 80205a4:	6822      	ldr	r2, [r4, #0]
 80205a6:	4b0a      	ldr	r3, [pc, #40]	; (80205d0 <_RMP_Tick_Handler+0x120>)
 80205a8:	b93a      	cbnz	r2, 80205ba <_RMP_Tick_Handler+0x10a>
 80205aa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
        RMP_Sched_Pend=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Cur_Thd->Slices_Left-=Ticks;
 80205ae:	6832      	ldr	r2, [r6, #0]
 80205b0:	4c07      	ldr	r4, [pc, #28]	; (80205d0 <_RMP_Tick_Handler+0x120>)
 80205b2:	6a13      	ldr	r3, [r2, #32]
 80205b4:	1a1b      	subs	r3, r3, r0
 80205b6:	6213      	str	r3, [r2, #32]
 80205b8:	e788      	b.n	80204cc <_RMP_Tick_Handler+0x1c>
        RMP_COVERAGE_MARKER();
    
    if(RMP_Sched_Pend!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=0;
 80205ba:	2200      	movs	r2, #0
 80205bc:	601a      	str	r2, [r3, #0]
        RMP_COVERAGE_MARKER();
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Tick_Hook(Ticks);
#endif
}
 80205be:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    
    if(RMP_Sched_Pend!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=0;
        _RMP_Yield();
 80205c2:	f7ff be25 	b.w	8020210 <_RMP_Yield>
 80205c6:	bf00      	nop
 80205c8:	20020834 	.word	0x20020834
 80205cc:	20020830 	.word	0x20020830
 80205d0:	20020558 	.word	0x20020558
 80205d4:	200206a0 	.word	0x200206a0
 80205d8:	2002055c 	.word	0x2002055c
 80205dc:	20020554 	.word	0x20020554

080205e0 <RMP_Save_Ctx>:
 80205e0:	4770      	bx	lr
 80205e2:	bf00      	nop

080205e4 <RMP_Load_Ctx>:
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Load_Ctx(void)
{
 80205e4:	4770      	bx	lr
 80205e6:	bf00      	nop

080205e8 <main>:
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
 80205e8:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
    for(Count=0;Count<RMP_COVERAGE_LINES;Count++)
        RMP_Coverage[Count]=0;
#endif
    
    /* Initialize the kernel data structures first */
    _RMP_Low_Level_Init();
 80205ea:	f7ff fdf9 	bl	80201e0 <_RMP_Low_Level_Init>
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
 80205ee:	4d22      	ldr	r5, [pc, #136]	; (8020678 <main+0x90>)
 80205f0:	2000      	movs	r0, #0
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
 80205f2:	4c22      	ldr	r4, [pc, #136]	; (802067c <main+0x94>)
 80205f4:	4922      	ldr	r1, [pc, #136]	; (8020680 <main+0x98>)
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
 80205f6:	6028      	str	r0, [r5, #0]
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 80205f8:	4603      	mov	r3, r0
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
 80205fa:	6020      	str	r0, [r4, #0]
 80205fc:	460a      	mov	r2, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80205fe:	4c21      	ldr	r4, [pc, #132]	; (8020684 <main+0x9c>)
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
    RMP_Sched_Locked=0;
 8020600:	4f21      	ldr	r7, [pc, #132]	; (8020688 <main+0xa0>)
    RMP_Sched_Pend=0;
 8020602:	4e22      	ldr	r6, [pc, #136]	; (802068c <main+0xa4>)
    RMP_Timer_Pend=0;
 8020604:	4d22      	ldr	r5, [pc, #136]	; (8020690 <main+0xa8>)
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
    RMP_Sched_Locked=0;
 8020606:	6038      	str	r0, [r7, #0]
    RMP_Sched_Pend=0;
 8020608:	6030      	str	r0, [r6, #0]
    RMP_Timer_Pend=0;
 802060a:	6028      	str	r0, [r5, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802060c:	6024      	str	r4, [r4, #0]
    Head->Next=(struct RMP_List*)Head;
 802060e:	6064      	str	r4, [r4, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 8020610:	f841 2033 	str.w	r2, [r1, r3, lsl #3]
    Head->Next=(struct RMP_List*)Head;
 8020614:	eb01 00c3 	add.w	r0, r1, r3, lsl #3
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 8020618:	3301      	adds	r3, #1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802061a:	4c19      	ldr	r4, [pc, #100]	; (8020680 <main+0x98>)
    Head->Next=(struct RMP_List*)Head;
 802061c:	6042      	str	r2, [r0, #4]
 802061e:	3208      	adds	r2, #8
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 8020620:	2b20      	cmp	r3, #32
 8020622:	d1f5      	bne.n	8020610 <main+0x28>
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 8020624:	4b1b      	ldr	r3, [pc, #108]	; (8020694 <main+0xac>)
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
        RMP_List_Crt(&RMP_Run[Count]);
    for(Count=0;Count<RMP_BITMAP_SIZE;Count++)
        RMP_Bitmap[Count]=0;
 8020626:	2500      	movs	r5, #0
 8020628:	4a1b      	ldr	r2, [pc, #108]	; (8020698 <main+0xb0>)
        
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
    RMP_Init_Thd.Slices=10;
 802062a:	200a      	movs	r0, #10
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802062c:	f103 0110 	add.w	r1, r3, #16
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
    RMP_Init_Thd.Slices=10;
    RMP_Init_Thd.Slices_Left=10;
    RMP_Init_Thd.State=RMP_THD_RUNNING;
 8020630:	2701      	movs	r7, #1
    RMP_Init_Thd.Stack=RMP_INIT_STACK;
 8020632:	4e1a      	ldr	r6, [pc, #104]	; (802069c <main+0xb4>)
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
        RMP_List_Crt(&RMP_Run[Count]);
    for(Count=0;Count<RMP_BITMAP_SIZE;Count++)
        RMP_Bitmap[Count]=0;
 8020634:	6015      	str	r5, [r2, #0]
        
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
 8020636:	625d      	str	r5, [r3, #36]	; 0x24
    RMP_Init_Thd.Slices=10;
 8020638:	61d8      	str	r0, [r3, #28]
    RMP_Init_Thd.Slices_Left=10;
 802063a:	6218      	str	r0, [r3, #32]
    RMP_Init_Thd.State=RMP_THD_RUNNING;
 802063c:	619f      	str	r7, [r3, #24]
    RMP_Init_Thd.Stack=RMP_INIT_STACK;
 802063e:	635e      	str	r6, [r3, #52]	; 0x34
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 8020640:	6119      	str	r1, [r3, #16]
    Head->Next=(struct RMP_List*)Head;
 8020642:	6159      	str	r1, [r3, #20]
    
    /* Initialize sending list */
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
 8020644:	6821      	ldr	r1, [r4, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 8020646:	609d      	str	r5, [r3, #8]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8020648:	6023      	str	r3, [r4, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 802064a:	60dd      	str	r5, [r3, #12]
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
 802064c:	605c      	str	r4, [r3, #4]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 802064e:	629d      	str	r5, [r3, #40]	; 0x28
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
 8020650:	6019      	str	r1, [r3, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 8020652:	62dd      	str	r5, [r3, #44]	; 0x2c
 8020654:	631d      	str	r5, [r3, #48]	; 0x30
 8020656:	639d      	str	r5, [r3, #56]	; 0x38
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
    Prev->Next=(struct RMP_List*)New;
 8020658:	604b      	str	r3, [r1, #4]
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 802065a:	6811      	ldr	r1, [r2, #0]
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
 802065c:	4e10      	ldr	r6, [pc, #64]	; (80206a0 <main+0xb8>)
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 802065e:	4339      	orrs	r1, r7
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
    RMP_Cur_SP=RMP_Init_Thd.Stack;
 8020660:	4c10      	ldr	r4, [pc, #64]	; (80206a4 <main+0xbc>)
    
    /* Now jump to the user function and will never return. Initialization of stack is not needed */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
 8020662:	4811      	ldr	r0, [pc, #68]	; (80206a8 <main+0xc0>)
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 8020664:	6011      	str	r1, [r2, #0]
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
 8020666:	6033      	str	r3, [r6, #0]
    RMP_Cur_SP=RMP_Init_Thd.Stack;
 8020668:	6b5a      	ldr	r2, [r3, #52]	; 0x34
 802066a:	6022      	str	r2, [r4, #0]
    
    /* Now jump to the user function and will never return. Initialization of stack is not needed */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
 802066c:	6b59      	ldr	r1, [r3, #52]	; 0x34
 802066e:	f7ff fdda 	bl	8020226 <_RMP_Start>
    
    return 0;
}
 8020672:	4628      	mov	r0, r5
 8020674:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 8020676:	bf00      	nop
 8020678:	20020834 	.word	0x20020834
 802067c:	2002065c 	.word	0x2002065c
 8020680:	2002055c 	.word	0x2002055c
 8020684:	200206a0 	.word	0x200206a0
 8020688:	200206a8 	.word	0x200206a8
 802068c:	20020558 	.word	0x20020558
 8020690:	20020660 	.word	0x20020660
 8020694:	20020664 	.word	0x20020664
 8020698:	20020554 	.word	0x20020554
 802069c:	20020510 	.word	0x20020510
 80206a0:	20020830 	.word	0x20020830
 80206a4:	2002082c 	.word	0x2002082c
 80206a8:	080203f9 	.word	0x080203f9

080206ac <__libc_init_array>:
 80206ac:	b570      	push	{r4, r5, r6, lr}
 80206ae:	4e0f      	ldr	r6, [pc, #60]	; (80206ec <__libc_init_array+0x40>)
 80206b0:	4d0f      	ldr	r5, [pc, #60]	; (80206f0 <__libc_init_array+0x44>)
 80206b2:	1b76      	subs	r6, r6, r5
 80206b4:	10b6      	asrs	r6, r6, #2
 80206b6:	bf18      	it	ne
 80206b8:	2400      	movne	r4, #0
 80206ba:	d005      	beq.n	80206c8 <__libc_init_array+0x1c>
 80206bc:	3401      	adds	r4, #1
 80206be:	f855 3b04 	ldr.w	r3, [r5], #4
 80206c2:	4798      	blx	r3
 80206c4:	42a6      	cmp	r6, r4
 80206c6:	d1f9      	bne.n	80206bc <__libc_init_array+0x10>
 80206c8:	4e0a      	ldr	r6, [pc, #40]	; (80206f4 <__libc_init_array+0x48>)
 80206ca:	4d0b      	ldr	r5, [pc, #44]	; (80206f8 <__libc_init_array+0x4c>)
 80206cc:	1b76      	subs	r6, r6, r5
 80206ce:	f000 f8d3 	bl	8020878 <_init>
 80206d2:	10b6      	asrs	r6, r6, #2
 80206d4:	bf18      	it	ne
 80206d6:	2400      	movne	r4, #0
 80206d8:	d006      	beq.n	80206e8 <__libc_init_array+0x3c>
 80206da:	3401      	adds	r4, #1
 80206dc:	f855 3b04 	ldr.w	r3, [r5], #4
 80206e0:	4798      	blx	r3
 80206e2:	42a6      	cmp	r6, r4
 80206e4:	d1f9      	bne.n	80206da <__libc_init_array+0x2e>
 80206e6:	bd70      	pop	{r4, r5, r6, pc}
 80206e8:	bd70      	pop	{r4, r5, r6, pc}
 80206ea:	bf00      	nop
 80206ec:	08020890 	.word	0x08020890
 80206f0:	08020890 	.word	0x08020890
 80206f4:	08020898 	.word	0x08020898
 80206f8:	08020890 	.word	0x08020890

080206fc <register_fini>:
 80206fc:	4b02      	ldr	r3, [pc, #8]	; (8020708 <register_fini+0xc>)
 80206fe:	b113      	cbz	r3, 8020706 <register_fini+0xa>
 8020700:	4802      	ldr	r0, [pc, #8]	; (802070c <register_fini+0x10>)
 8020702:	f000 b805 	b.w	8020710 <atexit>
 8020706:	4770      	bx	lr
 8020708:	00000000 	.word	0x00000000
 802070c:	0802071d 	.word	0x0802071d

08020710 <atexit>:
 8020710:	2300      	movs	r3, #0
 8020712:	4601      	mov	r1, r0
 8020714:	461a      	mov	r2, r3
 8020716:	4618      	mov	r0, r3
 8020718:	f000 b814 	b.w	8020744 <__register_exitproc>

0802071c <__libc_fini_array>:
 802071c:	b538      	push	{r3, r4, r5, lr}
 802071e:	4d07      	ldr	r5, [pc, #28]	; (802073c <__libc_fini_array+0x20>)
 8020720:	4c07      	ldr	r4, [pc, #28]	; (8020740 <__libc_fini_array+0x24>)
 8020722:	1b2c      	subs	r4, r5, r4
 8020724:	10a4      	asrs	r4, r4, #2
 8020726:	d005      	beq.n	8020734 <__libc_fini_array+0x18>
 8020728:	3c01      	subs	r4, #1
 802072a:	f855 3d04 	ldr.w	r3, [r5, #-4]!
 802072e:	4798      	blx	r3
 8020730:	2c00      	cmp	r4, #0
 8020732:	d1f9      	bne.n	8020728 <__libc_fini_array+0xc>
 8020734:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8020738:	f000 b8a4 	b.w	8020884 <_fini>
 802073c:	0802089c 	.word	0x0802089c
 8020740:	08020898 	.word	0x08020898

08020744 <__register_exitproc>:
 8020744:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8020748:	4c25      	ldr	r4, [pc, #148]	; (80207e0 <__register_exitproc+0x9c>)
 802074a:	6825      	ldr	r5, [r4, #0]
 802074c:	f8d5 4148 	ldr.w	r4, [r5, #328]	; 0x148
 8020750:	4606      	mov	r6, r0
 8020752:	4688      	mov	r8, r1
 8020754:	4692      	mov	sl, r2
 8020756:	4699      	mov	r9, r3
 8020758:	b3c4      	cbz	r4, 80207cc <__register_exitproc+0x88>
 802075a:	6860      	ldr	r0, [r4, #4]
 802075c:	281f      	cmp	r0, #31
 802075e:	dc17      	bgt.n	8020790 <__register_exitproc+0x4c>
 8020760:	1c43      	adds	r3, r0, #1
 8020762:	b176      	cbz	r6, 8020782 <__register_exitproc+0x3e>
 8020764:	eb04 0580 	add.w	r5, r4, r0, lsl #2
 8020768:	2201      	movs	r2, #1
 802076a:	f8c5 a088 	str.w	sl, [r5, #136]	; 0x88
 802076e:	f8d4 1188 	ldr.w	r1, [r4, #392]	; 0x188
 8020772:	4082      	lsls	r2, r0
 8020774:	4311      	orrs	r1, r2
 8020776:	2e02      	cmp	r6, #2
 8020778:	f8c4 1188 	str.w	r1, [r4, #392]	; 0x188
 802077c:	f8c5 9108 	str.w	r9, [r5, #264]	; 0x108
 8020780:	d01e      	beq.n	80207c0 <__register_exitproc+0x7c>
 8020782:	3002      	adds	r0, #2
 8020784:	6063      	str	r3, [r4, #4]
 8020786:	f844 8020 	str.w	r8, [r4, r0, lsl #2]
 802078a:	2000      	movs	r0, #0
 802078c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8020790:	4b14      	ldr	r3, [pc, #80]	; (80207e4 <__register_exitproc+0xa0>)
 8020792:	b303      	cbz	r3, 80207d6 <__register_exitproc+0x92>
 8020794:	f44f 70c8 	mov.w	r0, #400	; 0x190
 8020798:	f3af 8000 	nop.w
 802079c:	4604      	mov	r4, r0
 802079e:	b1d0      	cbz	r0, 80207d6 <__register_exitproc+0x92>
 80207a0:	f8d5 3148 	ldr.w	r3, [r5, #328]	; 0x148
 80207a4:	2700      	movs	r7, #0
 80207a6:	e880 0088 	stmia.w	r0, {r3, r7}
 80207aa:	f8c5 4148 	str.w	r4, [r5, #328]	; 0x148
 80207ae:	4638      	mov	r0, r7
 80207b0:	2301      	movs	r3, #1
 80207b2:	f8c4 7188 	str.w	r7, [r4, #392]	; 0x188
 80207b6:	f8c4 718c 	str.w	r7, [r4, #396]	; 0x18c
 80207ba:	2e00      	cmp	r6, #0
 80207bc:	d0e1      	beq.n	8020782 <__register_exitproc+0x3e>
 80207be:	e7d1      	b.n	8020764 <__register_exitproc+0x20>
 80207c0:	f8d4 118c 	ldr.w	r1, [r4, #396]	; 0x18c
 80207c4:	430a      	orrs	r2, r1
 80207c6:	f8c4 218c 	str.w	r2, [r4, #396]	; 0x18c
 80207ca:	e7da      	b.n	8020782 <__register_exitproc+0x3e>
 80207cc:	f505 74a6 	add.w	r4, r5, #332	; 0x14c
 80207d0:	f8c5 4148 	str.w	r4, [r5, #328]	; 0x148
 80207d4:	e7c1      	b.n	802075a <__register_exitproc+0x16>
 80207d6:	f04f 30ff 	mov.w	r0, #4294967295
 80207da:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80207de:	bf00      	nop
 80207e0:	08020860 	.word	0x08020860
 80207e4:	00000000 	.word	0x00000000

080207e8 <RVM_Img>:
 80207e8:	56495254 316d6f44 00000000 00000000     TRIVDom1........
 80207f8:	00000000 08020005 20020838 00000100     ........8.. ....
 8020808:	08020235 20020938 00000400 20020718     5...8.. ....... 
 8020818:	200206ac 00000001 0000000a 00000080     ... ............
 8020828:	200207ac 00000020 20020714 00000001     ...  ...... ....
 8020838:	08020848 00000001 0802085c 00000000     H.......\.......

08020848 <RVM_Pgtbl>:
	...
 8020850:	001d0003 3f3f3f3f 3f3f3f3f              ....????????

0802085c <RVM_Kcap>:
 802085c:	0000001e                                ....

08020860 <_global_impure_ptr>:
 8020860:	20020000                                ... 

Disassembly of section .inits:

08020864 <__data_regions_array_start>:
 8020864:	08022000 	.word	0x08022000
 8020868:	20020000 	.word	0x20020000
 802086c:	20020428 	.word	0x20020428

08020870 <__bss_regions_array_start>:
 8020870:	20020428 	.word	0x20020428
 8020874:	20020838 	.word	0x20020838

08020878 <_init>:
 8020878:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 802087a:	bf00      	nop
 802087c:	bcf8      	pop	{r3, r4, r5, r6, r7}
 802087e:	bc08      	pop	{r3}
 8020880:	469e      	mov	lr, r3
 8020882:	4770      	bx	lr

08020884 <_fini>:
 8020884:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8020886:	bf00      	nop
 8020888:	bcf8      	pop	{r3, r4, r5, r6, r7}
 802088a:	bc08      	pop	{r3}
 802088c:	469e      	mov	lr, r3
 802088e:	4770      	bx	lr

08020890 <__init_array_start>:
 8020890:	080206fd 	.word	0x080206fd

08020894 <__frame_dummy_init_array_entry>:
 8020894:	08020115                                ....

08020898 <__do_global_dtors_aux_fini_array_entry>:
 8020898:	080200f1                                ....
