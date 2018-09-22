
RMP:     file format elf32-littlearm


Disassembly of section .text:

08020000 <_RVM_Entry-0x4>:
 8020000:	08022fd8 	.word	0x08022fd8

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
 8020032:	f002 fe69 	bl	8022d08 <__libc_init_array>
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
 80200dc:	20020430 	.word	0x20020430
 80200e0:	08024000 	.word	0x08024000
 80200e4:	20020430 	.word	0x20020430
 80200e8:	20029214 	.word	0x20029214
 80200ec:	08022699 	.word	0x08022699

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
 8020108:	20020430 	.word	0x20020430
 802010c:	00000000 	.word	0x00000000
 8020110:	08023154 	.word	0x08023154

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
 802013c:	20020434 	.word	0x20020434
 8020140:	08023154 	.word	0x08023154
 8020144:	20020430 	.word	0x20020430
 8020148:	00000000 	.word	0x00000000

0802014c <Int_Handler>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Handler(void)
{
 802014c:	b510      	push	{r4, lr}
    static rmp_cnt_t Count=0;
    
    if(Count<10000)
 802014e:	4c30      	ldr	r4, [pc, #192]	; (8020210 <Int_Handler+0xc4>)
 8020150:	f242 720f 	movw	r2, #9999	; 0x270f
 8020154:	6823      	ldr	r3, [r4, #0]
 8020156:	4293      	cmp	r3, r2
 8020158:	dd38      	ble.n	80201cc <Int_Handler+0x80>
            RMP_PRINTK_I(Count);
            RMP_PRINTK_S(" sends.\r\n");
            while(1);
        }
    }
    else if(Count<20000)
 802015a:	f644 621f 	movw	r2, #19999	; 0x4e1f
 802015e:	4293      	cmp	r3, r2
 8020160:	dc0e      	bgt.n	8020180 <Int_Handler+0x34>
    {
        Count++;
 8020162:	3301      	adds	r3, #1
        Start=COUNTER_READ();
 8020164:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
 8020168:	2101      	movs	r1, #1
        }
    }
    else if(Count<20000)
    {
        Count++;
        Start=COUNTER_READ();
 802016a:	4a2a      	ldr	r2, [pc, #168]	; (8020214 <Int_Handler+0xc8>)
            while(1);
        }
    }
    else if(Count<20000)
    {
        Count++;
 802016c:	6023      	str	r3, [r4, #0]
        Start=COUNTER_READ();
 802016e:	6a43      	ldr	r3, [r0, #36]	; 0x24
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
 8020170:	4829      	ldr	r0, [pc, #164]	; (8020218 <Int_Handler+0xcc>)
        }
    }
    else if(Count<20000)
    {
        Count++;
        Start=COUNTER_READ();
 8020172:	408b      	lsls	r3, r1
 8020174:	6013      	str	r3, [r2, #0]
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
 8020176:	f002 fa07 	bl	8022588 <RMP_Sem_Post_ISR>
 802017a:	2800      	cmp	r0, #0
 802017c:	db3e      	blt.n	80201fc <Int_Handler+0xb0>
 802017e:	bd10      	pop	{r4, pc}
Return      : None.
******************************************************************************/
void Int_Disable(void)
{
    /* Disable interrupt */
    RMP_ASSERT(RVM_HW_Int_Disable(RVM_TIM4_INT_IRQ30)==0);
 8020180:	2000      	movs	r0, #0
 8020182:	f001 f8b1 	bl	80212e8 <RVM_HW_Int_Disable>
 8020186:	4601      	mov	r1, r0
 8020188:	b1d8      	cbz	r0, 80201c2 <Int_Handler+0x76>
 802018a:	4824      	ldr	r0, [pc, #144]	; (802021c <Int_Handler+0xd0>)
 802018c:	f001 f972 	bl	8021474 <RMP_Print_String>
 8020190:	4823      	ldr	r0, [pc, #140]	; (8020220 <Int_Handler+0xd4>)
 8020192:	f001 f96f 	bl	8021474 <RMP_Print_String>
 8020196:	4823      	ldr	r0, [pc, #140]	; (8020224 <Int_Handler+0xd8>)
 8020198:	f001 f96c 	bl	8021474 <RMP_Print_String>
 802019c:	2094      	movs	r0, #148	; 0x94
 802019e:	f001 f903 	bl	80213a8 <RMP_Print_Int>
 80201a2:	4821      	ldr	r0, [pc, #132]	; (8020228 <Int_Handler+0xdc>)
 80201a4:	f001 f966 	bl	8021474 <RMP_Print_String>
 80201a8:	4820      	ldr	r0, [pc, #128]	; (802022c <Int_Handler+0xe0>)
 80201aa:	f001 f963 	bl	8021474 <RMP_Print_String>
 80201ae:	4820      	ldr	r0, [pc, #128]	; (8020230 <Int_Handler+0xe4>)
 80201b0:	f001 f960 	bl	8021474 <RMP_Print_String>
 80201b4:	481f      	ldr	r0, [pc, #124]	; (8020234 <Int_Handler+0xe8>)
 80201b6:	f001 f95d 	bl	8021474 <RMP_Print_String>
 80201ba:	481b      	ldr	r0, [pc, #108]	; (8020228 <Int_Handler+0xdc>)
 80201bc:	f001 f95a 	bl	8021474 <RMP_Print_String>
 80201c0:	e7fe      	b.n	80201c0 <Int_Handler+0x74>
    /* Reverse registration */
    RVM_Vect_Init(2,0);
 80201c2:	2002      	movs	r0, #2
            while(1);
        }
    }
    else
        Int_Disable();
}
 80201c4:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80201c8:	f001 b80c 	b.w	80211e4 <RVM_Vect_Init>
{
    static rmp_cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
 80201cc:	3301      	adds	r3, #1
        Start=COUNTER_READ();
 80201ce:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
 80201d2:	2101      	movs	r1, #1
    static rmp_cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
        Start=COUNTER_READ();
 80201d4:	4a0f      	ldr	r2, [pc, #60]	; (8020214 <Int_Handler+0xc8>)
{
    static rmp_cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
 80201d6:	6023      	str	r3, [r4, #0]
        Start=COUNTER_READ();
 80201d8:	6a43      	ldr	r3, [r0, #36]	; 0x24
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
 80201da:	4817      	ldr	r0, [pc, #92]	; (8020238 <Int_Handler+0xec>)
    static rmp_cnt_t Count=0;
    
    if(Count<10000)
    {
        Count++;
        Start=COUNTER_READ();
 80201dc:	408b      	lsls	r3, r1
 80201de:	6013      	str	r3, [r2, #0]
        if(RMP_Thd_Snd_ISR(&Thd_2, 1)<0)
 80201e0:	f001 fd64 	bl	8021cac <RMP_Thd_Snd_ISR>
 80201e4:	2800      	cmp	r0, #0
 80201e6:	daca      	bge.n	802017e <Int_Handler+0x32>
        {
            RMP_PRINTK_S("ISR Mailbox send failure: ");
 80201e8:	4814      	ldr	r0, [pc, #80]	; (802023c <Int_Handler+0xf0>)
 80201ea:	f001 f943 	bl	8021474 <RMP_Print_String>
            RMP_PRINTK_I(Count);
 80201ee:	6820      	ldr	r0, [r4, #0]
 80201f0:	f001 f8da 	bl	80213a8 <RMP_Print_Int>
            RMP_PRINTK_S(" sends.\r\n");
 80201f4:	4812      	ldr	r0, [pc, #72]	; (8020240 <Int_Handler+0xf4>)
 80201f6:	f001 f93d 	bl	8021474 <RMP_Print_String>
 80201fa:	e7fe      	b.n	80201fa <Int_Handler+0xae>
    {
        Count++;
        Start=COUNTER_READ();
        if(RMP_Sem_Post_ISR(&Sem_1, 1)<0)
        {
            RMP_PRINTK_S("ISR semaphore post failure: ");
 80201fc:	4811      	ldr	r0, [pc, #68]	; (8020244 <Int_Handler+0xf8>)
 80201fe:	f001 f939 	bl	8021474 <RMP_Print_String>
            RMP_PRINTK_I(Count);
 8020202:	6820      	ldr	r0, [r4, #0]
 8020204:	f001 f8d0 	bl	80213a8 <RMP_Print_Int>
            RMP_PRINTK_S(" posts.\r\n");
 8020208:	480f      	ldr	r0, [pc, #60]	; (8020248 <Int_Handler+0xfc>)
 802020a:	f001 f933 	bl	8021474 <RMP_Print_String>
 802020e:	e7fe      	b.n	802020e <Int_Handler+0xc2>
 8020210:	2002848c 	.word	0x2002848c
 8020214:	20028490 	.word	0x20028490
 8020218:	20028bc4 	.word	0x20028bc4
 802021c:	08022e98 	.word	0x08022e98
 8020220:	08022ec0 	.word	0x08022ec0
 8020224:	08022f34 	.word	0x08022f34
 8020228:	08022f94 	.word	0x08022f94
 802022c:	08022f40 	.word	0x08022f40
 8020230:	08022f4c 	.word	0x08022f4c
 8020234:	08022f50 	.word	0x08022f50
 8020238:	20028bd4 	.word	0x20028bd4
 802023c:	08022e44 	.word	0x08022e44
 8020240:	08022e60 	.word	0x08022e60
 8020244:	08022e6c 	.word	0x08022e6c
 8020248:	08022e8c 	.word	0x08022e8c

0802024c <TIM_Base_SetConfig>:
{
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
  
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
 802024c:	4a40      	ldr	r2, [pc, #256]	; (8020350 <TIM_Base_SetConfig+0x104>)
 802024e:	f1b0 4f80 	cmp.w	r0, #1073741824	; 0x40000000
Return      : None.
******************************************************************************/
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
 8020252:	6803      	ldr	r3, [r0, #0]
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
 8020254:	b470      	push	{r4, r5, r6}
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
  
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
 8020256:	eba0 0602 	sub.w	r6, r0, r2
 802025a:	fab6 f686 	clz	r6, r6
 802025e:	ea4f 1656 	mov.w	r6, r6, lsr #5
 8020262:	d051      	beq.n	8020308 <TIM_Base_SetConfig+0xbc>
 8020264:	2e00      	cmp	r6, #0
 8020266:	d14f      	bne.n	8020308 <TIM_Base_SetConfig+0xbc>
 8020268:	f5a2 427c 	sub.w	r2, r2, #64512	; 0xfc00
 802026c:	4290      	cmp	r0, r2
 802026e:	d00d      	beq.n	802028c <TIM_Base_SetConfig+0x40>
 8020270:	f502 6280 	add.w	r2, r2, #1024	; 0x400
 8020274:	4290      	cmp	r0, r2
 8020276:	d009      	beq.n	802028c <TIM_Base_SetConfig+0x40>
 8020278:	4c36      	ldr	r4, [pc, #216]	; (8020354 <TIM_Base_SetConfig+0x108>)
 802027a:	f502 427c 	add.w	r2, r2, #64512	; 0xfc00
 802027e:	4290      	cmp	r0, r2
 8020280:	bf18      	it	ne
 8020282:	42a0      	cmpne	r0, r4
 8020284:	bf0c      	ite	eq
 8020286:	2401      	moveq	r4, #1
 8020288:	2400      	movne	r4, #0
 802028a:	d111      	bne.n	80202b0 <TIM_Base_SetConfig+0x64>
 802028c:	4c32      	ldr	r4, [pc, #200]	; (8020358 <TIM_Base_SetConfig+0x10c>)
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
 802028e:	f023 0370 	bic.w	r3, r3, #112	; 0x70
    tmpcr1 |= Structure->CounterMode;
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
 8020292:	4a32      	ldr	r2, [pc, #200]	; (802035c <TIM_Base_SetConfig+0x110>)
 8020294:	1b04      	subs	r4, r0, r4
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
 8020296:	684d      	ldr	r5, [r1, #4]
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
 8020298:	4290      	cmp	r0, r2
 802029a:	fab4 f484 	clz	r4, r4
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
 802029e:	ea43 0305 	orr.w	r3, r3, r5
 80202a2:	ea4f 1454 	mov.w	r4, r4, lsr #5
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
 80202a6:	d038      	beq.n	802031a <TIM_Base_SetConfig+0xce>
 80202a8:	f5a2 6280 	sub.w	r2, r2, #1024	; 0x400
 80202ac:	4290      	cmp	r0, r2
 80202ae:	d034      	beq.n	802031a <TIM_Base_SetConfig+0xce>
 80202b0:	4a28      	ldr	r2, [pc, #160]	; (8020354 <TIM_Base_SetConfig+0x108>)
 80202b2:	4290      	cmp	r0, r2
 80202b4:	bf14      	ite	ne
 80202b6:	4622      	movne	r2, r4
 80202b8:	f044 0201 	orreq.w	r2, r4, #1
 80202bc:	bb6a      	cbnz	r2, 802031a <TIM_Base_SetConfig+0xce>
 80202be:	4c28      	ldr	r4, [pc, #160]	; (8020360 <TIM_Base_SetConfig+0x114>)
 80202c0:	4d28      	ldr	r5, [pc, #160]	; (8020364 <TIM_Base_SetConfig+0x118>)
 80202c2:	42a0      	cmp	r0, r4
 80202c4:	bf18      	it	ne
 80202c6:	42a8      	cmpne	r0, r5
 80202c8:	bf0c      	ite	eq
 80202ca:	2401      	moveq	r4, #1
 80202cc:	2400      	movne	r4, #0
 80202ce:	d03a      	beq.n	8020346 <TIM_Base_SetConfig+0xfa>
 80202d0:	f505 6500 	add.w	r5, r5, #2048	; 0x800
 80202d4:	4a24      	ldr	r2, [pc, #144]	; (8020368 <TIM_Base_SetConfig+0x11c>)
 80202d6:	4290      	cmp	r0, r2
 80202d8:	bf18      	it	ne
 80202da:	42a8      	cmpne	r0, r5
 80202dc:	d01d      	beq.n	802031a <TIM_Base_SetConfig+0xce>
 80202de:	f502 6280 	add.w	r2, r2, #1024	; 0x400
 80202e2:	4290      	cmp	r0, r2
 80202e4:	d031      	beq.n	802034a <TIM_Base_SetConfig+0xfe>
 80202e6:	f502 6280 	add.w	r2, r2, #1024	; 0x400
 80202ea:	4290      	cmp	r0, r2
 80202ec:	d02d      	beq.n	802034a <TIM_Base_SetConfig+0xfe>
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 80202ee:	694a      	ldr	r2, [r1, #20]
 80202f0:	f023 0380 	bic.w	r3, r3, #128	; 0x80
 80202f4:	4313      	orrs	r3, r2

  TIMx->CR1 = tmpcr1;
 80202f6:	6003      	str	r3, [r0, #0]

  /* Set the Auto-reload value */
  TIMx->ARR = (uint32_t)Structure->Period ;
 80202f8:	688b      	ldr	r3, [r1, #8]
 80202fa:	62c3      	str	r3, [r0, #44]	; 0x2c
 
  /* Set the Prescaler value */
  TIMx->PSC = (uint32_t)Structure->Prescaler;
 80202fc:	680b      	ldr	r3, [r1, #0]
 80202fe:	6283      	str	r3, [r0, #40]	; 0x28
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 8020300:	2301      	movs	r3, #1
}
 8020302:	bc70      	pop	{r4, r5, r6}
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 8020304:	6143      	str	r3, [r0, #20]
}
 8020306:	4770      	bx	lr
 8020308:	4c13      	ldr	r4, [pc, #76]	; (8020358 <TIM_Base_SetConfig+0x10c>)
  
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC3_INSTANCE(TIMx) != RESET)   
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
 802030a:	f023 0370 	bic.w	r3, r3, #112	; 0x70
    tmpcr1 |= Structure->CounterMode;
 802030e:	684a      	ldr	r2, [r1, #4]
 8020310:	1b04      	subs	r4, r0, r4
 8020312:	4313      	orrs	r3, r2
 8020314:	fab4 f484 	clz	r4, r4
 8020318:	0964      	lsrs	r4, r4, #5
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 802031a:	68ca      	ldr	r2, [r1, #12]
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
  {
    /* Set the clock division */
    tmpcr1 &= ~TIM_CR1_CKD;
 802031c:	f423 7340 	bic.w	r3, r3, #768	; 0x300
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 8020320:	694d      	ldr	r5, [r1, #20]
 8020322:	431a      	orrs	r2, r3
 8020324:	f022 0280 	bic.w	r2, r2, #128	; 0x80
 8020328:	432a      	orrs	r2, r5

  TIMx->CR1 = tmpcr1;
 802032a:	6002      	str	r2, [r0, #0]

  /* Set the Auto-reload value */
  TIMx->ARR = (uint32_t)Structure->Period ;
 802032c:	688b      	ldr	r3, [r1, #8]
 802032e:	62c3      	str	r3, [r0, #44]	; 0x2c
 
  /* Set the Prescaler value */
  TIMx->PSC = (uint32_t)Structure->Prescaler;
 8020330:	680b      	ldr	r3, [r1, #0]
 8020332:	6283      	str	r3, [r0, #40]	; 0x28
    
  if(IS_TIM_ADVANCED_INSTANCE(TIMx) != RESET)  
 8020334:	b90e      	cbnz	r6, 802033a <TIM_Base_SetConfig+0xee>
 8020336:	2c00      	cmp	r4, #0
 8020338:	d0e2      	beq.n	8020300 <TIM_Base_SetConfig+0xb4>
  {
    /* Set the Repetition Counter value */
    TIMx->RCR = Structure->RepetitionCounter;
 802033a:	690b      	ldr	r3, [r1, #16]
 802033c:	6303      	str	r3, [r0, #48]	; 0x30
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 802033e:	2301      	movs	r3, #1
}
 8020340:	bc70      	pop	{r4, r5, r6}
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 8020342:	6143      	str	r3, [r0, #20]
}
 8020344:	4770      	bx	lr
 8020346:	4614      	mov	r4, r2
 8020348:	e7e7      	b.n	802031a <TIM_Base_SetConfig+0xce>
 802034a:	2400      	movs	r4, #0
 802034c:	e7e5      	b.n	802031a <TIM_Base_SetConfig+0xce>
 802034e:	bf00      	nop
 8020350:	40010000 	.word	0x40010000
 8020354:	40000c00 	.word	0x40000c00
 8020358:	40010400 	.word	0x40010400
 802035c:	40000800 	.word	0x40000800
 8020360:	40014400 	.word	0x40014400
 8020364:	40014000 	.word	0x40014000
 8020368:	40001800 	.word	0x40001800

0802036c <Int_Init>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
 802036c:	b570      	push	{r4, r5, r6, lr}
    /* Connect the physical interrupt to our machine */
    RMP_ASSERT(RVM_Reg_Int(RVM_TIM4_INT_IRQ30, 2)==0);
 802036e:	2102      	movs	r1, #2
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Int_Init(void)
{
 8020370:	b082      	sub	sp, #8
    /* Connect the physical interrupt to our machine */
    RMP_ASSERT(RVM_Reg_Int(RVM_TIM4_INT_IRQ30, 2)==0);
 8020372:	2000      	movs	r0, #0
 8020374:	f000 ff6e 	bl	8021254 <RVM_Reg_Int>
 8020378:	b1d8      	cbz	r0, 80203b2 <Int_Init+0x46>
 802037a:	4845      	ldr	r0, [pc, #276]	; (8020490 <Int_Init+0x124>)
 802037c:	f001 f87a 	bl	8021474 <RMP_Print_String>
 8020380:	4844      	ldr	r0, [pc, #272]	; (8020494 <Int_Init+0x128>)
 8020382:	f001 f877 	bl	8021474 <RMP_Print_String>
 8020386:	4844      	ldr	r0, [pc, #272]	; (8020498 <Int_Init+0x12c>)
 8020388:	f001 f874 	bl	8021474 <RMP_Print_String>
 802038c:	2070      	movs	r0, #112	; 0x70
 802038e:	f001 f80b 	bl	80213a8 <RMP_Print_Int>
 8020392:	4842      	ldr	r0, [pc, #264]	; (802049c <Int_Init+0x130>)
 8020394:	f001 f86e 	bl	8021474 <RMP_Print_String>
 8020398:	4841      	ldr	r0, [pc, #260]	; (80204a0 <Int_Init+0x134>)
 802039a:	f001 f86b 	bl	8021474 <RMP_Print_String>
 802039e:	4841      	ldr	r0, [pc, #260]	; (80204a4 <Int_Init+0x138>)
 80203a0:	f001 f868 	bl	8021474 <RMP_Print_String>
 80203a4:	4840      	ldr	r0, [pc, #256]	; (80204a8 <Int_Init+0x13c>)
 80203a6:	f001 f865 	bl	8021474 <RMP_Print_String>
 80203aa:	483c      	ldr	r0, [pc, #240]	; (802049c <Int_Init+0x130>)
 80203ac:	f001 f862 	bl	8021474 <RMP_Print_String>
 80203b0:	e7fe      	b.n	80203b0 <Int_Init+0x44>
    /* Set the priority of the physical interrupt and enable it */
    RMP_ASSERT(RVM_HW_Int_Enable(RVM_TIM4_INT_IRQ30)==0);
 80203b2:	f000 ff8f 	bl	80212d4 <RVM_HW_Int_Enable>
 80203b6:	b1d8      	cbz	r0, 80203f0 <Int_Init+0x84>
 80203b8:	4835      	ldr	r0, [pc, #212]	; (8020490 <Int_Init+0x124>)
 80203ba:	f001 f85b 	bl	8021474 <RMP_Print_String>
 80203be:	4835      	ldr	r0, [pc, #212]	; (8020494 <Int_Init+0x128>)
 80203c0:	f001 f858 	bl	8021474 <RMP_Print_String>
 80203c4:	4834      	ldr	r0, [pc, #208]	; (8020498 <Int_Init+0x12c>)
 80203c6:	f001 f855 	bl	8021474 <RMP_Print_String>
 80203ca:	2072      	movs	r0, #114	; 0x72
 80203cc:	f000 ffec 	bl	80213a8 <RMP_Print_Int>
 80203d0:	4832      	ldr	r0, [pc, #200]	; (802049c <Int_Init+0x130>)
 80203d2:	f001 f84f 	bl	8021474 <RMP_Print_String>
 80203d6:	4832      	ldr	r0, [pc, #200]	; (80204a0 <Int_Init+0x134>)
 80203d8:	f001 f84c 	bl	8021474 <RMP_Print_String>
 80203dc:	4831      	ldr	r0, [pc, #196]	; (80204a4 <Int_Init+0x138>)
 80203de:	f001 f849 	bl	8021474 <RMP_Print_String>
 80203e2:	4831      	ldr	r0, [pc, #196]	; (80204a8 <Int_Init+0x13c>)
 80203e4:	f001 f846 	bl	8021474 <RMP_Print_String>
 80203e8:	482c      	ldr	r0, [pc, #176]	; (802049c <Int_Init+0x130>)
 80203ea:	f001 f843 	bl	8021474 <RMP_Print_String>
 80203ee:	e7fe      	b.n	80203ee <Int_Init+0x82>
    RMP_ASSERT(RVM_HW_Int_Prio(RVM_TIM4_INT_IRQ30,0xFF)==0);
 80203f0:	21ff      	movs	r1, #255	; 0xff
 80203f2:	f000 ff83 	bl	80212fc <RVM_HW_Int_Prio>
 80203f6:	4605      	mov	r5, r0
 80203f8:	b1d8      	cbz	r0, 8020432 <Int_Init+0xc6>
 80203fa:	4825      	ldr	r0, [pc, #148]	; (8020490 <Int_Init+0x124>)
 80203fc:	f001 f83a 	bl	8021474 <RMP_Print_String>
 8020400:	4824      	ldr	r0, [pc, #144]	; (8020494 <Int_Init+0x128>)
 8020402:	f001 f837 	bl	8021474 <RMP_Print_String>
 8020406:	4824      	ldr	r0, [pc, #144]	; (8020498 <Int_Init+0x12c>)
 8020408:	f001 f834 	bl	8021474 <RMP_Print_String>
 802040c:	2073      	movs	r0, #115	; 0x73
 802040e:	f000 ffcb 	bl	80213a8 <RMP_Print_Int>
 8020412:	4822      	ldr	r0, [pc, #136]	; (802049c <Int_Init+0x130>)
 8020414:	f001 f82e 	bl	8021474 <RMP_Print_String>
 8020418:	4821      	ldr	r0, [pc, #132]	; (80204a0 <Int_Init+0x134>)
 802041a:	f001 f82b 	bl	8021474 <RMP_Print_String>
 802041e:	4821      	ldr	r0, [pc, #132]	; (80204a4 <Int_Init+0x138>)
 8020420:	f001 f828 	bl	8021474 <RMP_Print_String>
 8020424:	4820      	ldr	r0, [pc, #128]	; (80204a8 <Int_Init+0x13c>)
 8020426:	f001 f825 	bl	8021474 <RMP_Print_String>
 802042a:	481c      	ldr	r0, [pc, #112]	; (802049c <Int_Init+0x130>)
 802042c:	f001 f822 	bl	8021474 <RMP_Print_String>
 8020430:	e7fe      	b.n	8020430 <Int_Init+0xc4>
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Vect_Init(2,Int_Handler);
 8020432:	491e      	ldr	r1, [pc, #120]	; (80204ac <Int_Init+0x140>)
 8020434:	2002      	movs	r0, #2
 8020436:	f000 fed5 	bl	80211e4 <RVM_Vect_Init>
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
 802043a:	4c1d      	ldr	r4, [pc, #116]	; (80204b0 <Int_Init+0x144>)
 802043c:	4b1d      	ldr	r3, [pc, #116]	; (80204b4 <Int_Init+0x148>)
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
 802043e:	f245 4260 	movw	r2, #21600	; 0x5460
    RVM_Vect_Init(2,Int_Handler);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
 8020442:	2610      	movs	r6, #16
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
 8020444:	1d21      	adds	r1, r4, #4
 8020446:	4618      	mov	r0, r3
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
 8020448:	60e2      	str	r2, [r4, #12]
    RMP_ASSERT(RVM_HW_Int_Prio(RVM_TIM4_INT_IRQ30,0xFF)==0);
    /* Interrupt generation is initialized too, here we only register our handler */
    RVM_Vect_Init(2,Int_Handler);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
 802044a:	6023      	str	r3, [r4, #0]
    TIM4_Handle.Init.Prescaler=0;
 802044c:	6065      	str	r5, [r4, #4]
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
 802044e:	6125      	str	r5, [r4, #16]
	TIM4_Handle.Init.RepetitionCounter=0;
 8020450:	6165      	str	r5, [r4, #20]
    RVM_Vect_Init(2,Int_Handler);
    
    /* TIM4 clock = 1/2 CPU clock */
    TIM4_Handle.Instance=TIM4;
    TIM4_Handle.Init.Prescaler=0;
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
 8020452:	60a6      	str	r6, [r4, #8]
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
 8020454:	f7ff fefa 	bl	802024c <TIM_Base_SetConfig>
    TIM4_Handle.State=HAL_TIM_STATE_READY;
    __HAL_RCC_TIM4_CLK_ENABLE();
 8020458:	4b17      	ldr	r3, [pc, #92]	; (80204b8 <Int_Init+0x14c>)
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
    TIM4_Handle.State=HAL_TIM_STATE_READY;
 802045a:	2201      	movs	r2, #1
    __HAL_RCC_TIM4_CLK_ENABLE();
	/* Clear interrupt pending bit, because we used EGR to update the registers */
	__HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
 802045c:	f06f 0101 	mvn.w	r1, #1
    TIM4_Handle.Init.CounterMode=TIM_COUNTERMODE_DOWN;
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
    TIM4_Handle.State=HAL_TIM_STATE_READY;
 8020460:	f884 203d 	strb.w	r2, [r4, #61]	; 0x3d
    __HAL_RCC_TIM4_CLK_ENABLE();
 8020464:	6c1a      	ldr	r2, [r3, #64]	; 0x40
 8020466:	f042 0204 	orr.w	r2, r2, #4
 802046a:	641a      	str	r2, [r3, #64]	; 0x40
 802046c:	6c1a      	ldr	r2, [r3, #64]	; 0x40
	/* Clear interrupt pending bit, because we used EGR to update the registers */
	__HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
 802046e:	6823      	ldr	r3, [r4, #0]
    TIM4_Handle.Init.Period=21600;
    TIM4_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	TIM4_Handle.Init.RepetitionCounter=0;
    TIM_Base_SetConfig(TIM4_Handle.Instance, &(TIM4_Handle.Init)); 
    TIM4_Handle.State=HAL_TIM_STATE_READY;
    __HAL_RCC_TIM4_CLK_ENABLE();
 8020470:	f002 0204 	and.w	r2, r2, #4
 8020474:	9201      	str	r2, [sp, #4]
 8020476:	9a01      	ldr	r2, [sp, #4]
	/* Clear interrupt pending bit, because we used EGR to update the registers */
	__HAL_TIM_CLEAR_IT(&TIM4_Handle, TIM_IT_UPDATE);
 8020478:	6119      	str	r1, [r3, #16]
      /* Enable the TIM Update interrupt */
    __HAL_TIM_ENABLE_IT(&TIM4_Handle, TIM_IT_UPDATE);
 802047a:	68da      	ldr	r2, [r3, #12]
 802047c:	f042 0201 	orr.w	r2, r2, #1
 8020480:	60da      	str	r2, [r3, #12]
    /* Enable the Peripheral */
    __HAL_TIM_ENABLE(&TIM4_Handle);
 8020482:	681a      	ldr	r2, [r3, #0]
 8020484:	f042 0201 	orr.w	r2, r2, #1
 8020488:	601a      	str	r2, [r3, #0]
}
 802048a:	b002      	add	sp, #8
 802048c:	bd70      	pop	{r4, r5, r6, pc}
 802048e:	bf00      	nop
 8020490:	08022e98 	.word	0x08022e98
 8020494:	08022ec0 	.word	0x08022ec0
 8020498:	08022f34 	.word	0x08022f34
 802049c:	08022f94 	.word	0x08022f94
 80204a0:	08022f40 	.word	0x08022f40
 80204a4:	08022f4c 	.word	0x08022f4c
 80204a8:	08022f50 	.word	0x08022f50
 80204ac:	0802014d 	.word	0x0802014d
 80204b0:	200284c0 	.word	0x200284c0
 80204b4:	40000800 	.word	0x40000800
 80204b8:	40023800 	.word	0x40023800

080204bc <Test_Mail_1>:
        RMP_Yield();
    }
}

void Test_Mail_1(void)
{
 80204bc:	b570      	push	{r4, r5, r6, lr}
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80204be:	2300      	movs	r3, #0
 80204c0:	4c0a      	ldr	r4, [pc, #40]	; (80204ec <Test_Mail_1+0x30>)
 80204c2:	4e0b      	ldr	r6, [pc, #44]	; (80204f0 <Test_Mail_1+0x34>)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 80204c4:	f04f 4580 	mov.w	r5, #1073741824	; 0x40000000
}

void Test_Mail_1(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80204c8:	6023      	str	r3, [r4, #0]
    {
        /* Read counter here */
        Start=COUNTER_READ();
 80204ca:	6a6b      	ldr	r3, [r5, #36]	; 0x24
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
 80204cc:	2101      	movs	r1, #1
 80204ce:	4a09      	ldr	r2, [pc, #36]	; (80204f4 <Test_Mail_1+0x38>)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 80204d0:	408b      	lsls	r3, r1
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
 80204d2:	4809      	ldr	r0, [pc, #36]	; (80204f8 <Test_Mail_1+0x3c>)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 80204d4:	6033      	str	r3, [r6, #0]
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
 80204d6:	f001 fa8d 	bl	80219f4 <RMP_Thd_Snd>
}

void Test_Mail_1(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80204da:	6823      	ldr	r3, [r4, #0]
 80204dc:	f242 720f 	movw	r2, #9999	; 0x270f
 80204e0:	3301      	adds	r3, #1
 80204e2:	4293      	cmp	r3, r2
 80204e4:	6023      	str	r3, [r4, #0]
 80204e6:	ddf0      	ble.n	80204ca <Test_Mail_1+0xe>
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Thd_Snd(&Thd_2, 1, RMP_MAX_SLICES);
    }
}
 80204e8:	bd70      	pop	{r4, r5, r6, pc}
 80204ea:	bf00      	nop
 80204ec:	20028558 	.word	0x20028558
 80204f0:	20028490 	.word	0x20028490
 80204f4:	000186a0 	.word	0x000186a0
 80204f8:	20028bd4 	.word	0x20028bd4

080204fc <Test_Sem_1>:

void Test_Sem_1(void)
{
 80204fc:	b570      	push	{r4, r5, r6, lr}
 80204fe:	f242 7410 	movw	r4, #10000	; 0x2710
 8020502:	4e06      	ldr	r6, [pc, #24]	; (802051c <Test_Sem_1+0x20>)
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 8020504:	f04f 4580 	mov.w	r5, #1073741824	; 0x40000000
 8020508:	6a6b      	ldr	r3, [r5, #36]	; 0x24
        RMP_Sem_Post(&Sem_1, 1);
 802050a:	2101      	movs	r1, #1
 802050c:	4804      	ldr	r0, [pc, #16]	; (8020520 <Test_Sem_1+0x24>)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 802050e:	408b      	lsls	r3, r1
 8020510:	6033      	str	r3, [r6, #0]
        RMP_Sem_Post(&Sem_1, 1);
 8020512:	f001 ff4d 	bl	80223b0 <RMP_Sem_Post>
}

void Test_Sem_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020516:	3c01      	subs	r4, #1
 8020518:	d1f6      	bne.n	8020508 <Test_Sem_1+0xc>
    {
        /* Read counter here */
        Start=COUNTER_READ();
        RMP_Sem_Post(&Sem_1, 1);
    }
}
 802051a:	bd70      	pop	{r4, r5, r6, pc}
 802051c:	20028490 	.word	0x20028490
 8020520:	20028bc4 	.word	0x20028bc4

08020524 <Func_1>:

void Func_1(void)
{
 8020524:	f242 7410 	movw	r4, #10000	; 0x2710
 8020528:	4e0a      	ldr	r6, [pc, #40]	; (8020554 <Func_1+0x30>)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 802052a:	f04f 4580 	mov.w	r5, #1073741824	; 0x40000000
        RMP_Sem_Post(&Sem_1, 1);
    }
}

void Func_1(void)
{
 802052e:	b508      	push	{r3, lr}
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        /* Read counter here */
        Start=COUNTER_READ();
 8020530:	6a6b      	ldr	r3, [r5, #36]	; 0x24
 8020532:	005b      	lsls	r3, r3, #1
 8020534:	6033      	str	r3, [r6, #0]
        RMP_Yield();
 8020536:	f000 fffb 	bl	8021530 <RMP_Yield>
Return      : None.
******************************************************************************/
void Test_Yield_1(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802053a:	3c01      	subs	r4, #1
 802053c:	d1f8      	bne.n	8020530 <Func_1+0xc>

void Func_1(void)
{
    Test_Yield_1();
    /* Change priority of thread 2 */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
 802053e:	4a06      	ldr	r2, [pc, #24]	; (8020558 <Func_1+0x34>)
 8020540:	2102      	movs	r1, #2
 8020542:	4806      	ldr	r0, [pc, #24]	; (802055c <Func_1+0x38>)
 8020544:	f001 f992 	bl	802186c <RMP_Thd_Set>
    Test_Mail_1();
 8020548:	f7ff ffb8 	bl	80204bc <Test_Mail_1>
    Test_Sem_1();
 802054c:	f7ff ffd6 	bl	80204fc <Test_Sem_1>
 8020550:	e7fe      	b.n	8020550 <Func_1+0x2c>
 8020552:	bf00      	nop
 8020554:	20028490 	.word	0x20028490
 8020558:	000186a0 	.word	0x000186a0
 802055c:	20028bd4 	.word	0x20028bd4

08020560 <Test_Yield_2>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
 8020560:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8020564:	f242 7410 	movw	r4, #10000	; 0x2710
 8020568:	f8df 8038 	ldr.w	r8, [pc, #56]	; 80205a4 <Test_Yield_2+0x44>
 802056c:	4e0b      	ldr	r6, [pc, #44]	; (802059c <Test_Yield_2+0x3c>)
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
 802056e:	f04f 4580 	mov.w	r5, #1073741824	; 0x40000000
 8020572:	4f0b      	ldr	r7, [pc, #44]	; (80205a0 <Test_Yield_2+0x40>)
void Test_Yield_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Yield();
 8020574:	f000 ffdc 	bl	8021530 <RMP_Yield>
        /* Read counter here */
        End=COUNTER_READ();
 8020578:	6a6b      	ldr	r3, [r5, #36]	; 0x24
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802057a:	3c01      	subs	r4, #1
    {
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
 802057c:	ea4f 0343 	mov.w	r3, r3, lsl #1
 8020580:	f8c8 3000 	str.w	r3, [r8]
        Total+=(rmp_tim_t)(End-Start);
 8020584:	f8d8 3000 	ldr.w	r3, [r8]
 8020588:	6831      	ldr	r1, [r6, #0]
 802058a:	683a      	ldr	r2, [r7, #0]
 802058c:	4413      	add	r3, r2
 802058e:	eba3 0301 	sub.w	r3, r3, r1
 8020592:	603b      	str	r3, [r7, #0]
Return      : None.
******************************************************************************/
void Test_Yield_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020594:	d1ee      	bne.n	8020574 <Test_Yield_2+0x14>
        RMP_Yield();
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
    };
}
 8020596:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 802059a:	bf00      	nop
 802059c:	20028490 	.word	0x20028490
 80205a0:	20028510 	.word	0x20028510
 80205a4:	2002845c 	.word	0x2002845c

080205a8 <Test_Mail_2>:
void Test_Mail_2(void)
{
 80205a8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 80205ac:	f242 7410 	movw	r4, #10000	; 0x2710
 80205b0:	b082      	sub	sp, #8
 80205b2:	f8df 8044 	ldr.w	r8, [pc, #68]	; 80205f8 <Test_Mail_2+0x50>
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 80205b6:	f04f 4680 	mov.w	r6, #1073741824	; 0x40000000
 80205ba:	4d0c      	ldr	r5, [pc, #48]	; (80205ec <Test_Mail_2+0x44>)
 80205bc:	4f0c      	ldr	r7, [pc, #48]	; (80205f0 <Test_Mail_2+0x48>)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
 80205be:	490d      	ldr	r1, [pc, #52]	; (80205f4 <Test_Mail_2+0x4c>)
 80205c0:	a801      	add	r0, sp, #4
 80205c2:	f001 fbdf 	bl	8021d84 <RMP_Thd_Rcv>
        /* Read counter here */
        End=COUNTER_READ();
 80205c6:	6a73      	ldr	r3, [r6, #36]	; 0x24
}
void Test_Mail_2(void)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80205c8:	3c01      	subs	r4, #1
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 80205ca:	ea4f 0343 	mov.w	r3, r3, lsl #1
 80205ce:	f8c8 3000 	str.w	r3, [r8]
        Total+=(rmp_tim_t)(End-Start);
 80205d2:	f8d8 3000 	ldr.w	r3, [r8]
 80205d6:	6829      	ldr	r1, [r5, #0]
 80205d8:	683a      	ldr	r2, [r7, #0]
 80205da:	4413      	add	r3, r2
 80205dc:	eba3 0301 	sub.w	r3, r3, r1
 80205e0:	603b      	str	r3, [r7, #0]
}
void Test_Mail_2(void)
{
    rmp_ptr_t Data;
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80205e2:	d1ec      	bne.n	80205be <Test_Mail_2+0x16>
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}
 80205e4:	b002      	add	sp, #8
 80205e6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80205ea:	bf00      	nop
 80205ec:	20028490 	.word	0x20028490
 80205f0:	20028510 	.word	0x20028510
 80205f4:	000186a0 	.word	0x000186a0
 80205f8:	2002845c 	.word	0x2002845c

080205fc <Test_Sem_2>:

void Test_Sem_2(void)
{
 80205fc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8020600:	f242 7410 	movw	r4, #10000	; 0x2710
 8020604:	f8df 8044 	ldr.w	r8, [pc, #68]	; 802064c <Test_Sem_2+0x50>
 8020608:	4e0c      	ldr	r6, [pc, #48]	; (802063c <Test_Sem_2+0x40>)
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 802060a:	f04f 4580 	mov.w	r5, #1073741824	; 0x40000000
 802060e:	4f0c      	ldr	r7, [pc, #48]	; (8020640 <Test_Sem_2+0x44>)
void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
 8020610:	490c      	ldr	r1, [pc, #48]	; (8020644 <Test_Sem_2+0x48>)
 8020612:	480d      	ldr	r0, [pc, #52]	; (8020648 <Test_Sem_2+0x4c>)
 8020614:	f001 fdc2 	bl	802219c <RMP_Sem_Pend>
        /* Read counter here */
        End=COUNTER_READ();
 8020618:	6a6b      	ldr	r3, [r5, #36]	; 0x24
}

void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802061a:	3c01      	subs	r4, #1
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 802061c:	ea4f 0343 	mov.w	r3, r3, lsl #1
 8020620:	f8c8 3000 	str.w	r3, [r8]
        Total+=(rmp_tim_t)(End-Start);
 8020624:	f8d8 3000 	ldr.w	r3, [r8]
 8020628:	6831      	ldr	r1, [r6, #0]
 802062a:	683a      	ldr	r2, [r7, #0]
 802062c:	4413      	add	r3, r2
 802062e:	eba3 0301 	sub.w	r3, r3, r1
 8020632:	603b      	str	r3, [r7, #0]
}

void Test_Sem_2(void)
{
    rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020634:	d1ec      	bne.n	8020610 <Test_Sem_2+0x14>
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}
 8020636:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 802063a:	bf00      	nop
 802063c:	20028490 	.word	0x20028490
 8020640:	20028510 	.word	0x20028510
 8020644:	000186a0 	.word	0x000186a0
 8020648:	20028bc4 	.word	0x20028bc4
 802064c:	2002845c 	.word	0x2002845c

08020650 <Test_Mail_ISR>:

void Test_Mail_ISR(void)
{
 8020650:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020654:	2300      	movs	r3, #0
 8020656:	4c13      	ldr	r4, [pc, #76]	; (80206a4 <Test_Mail_ISR+0x54>)
        Total+=(rmp_tim_t)(End-Start);
    }
}

void Test_Mail_ISR(void)
{
 8020658:	b083      	sub	sp, #12
 802065a:	f8df 9054 	ldr.w	r9, [pc, #84]	; 80206b0 <Test_Mail_ISR+0x60>
 802065e:	4f12      	ldr	r7, [pc, #72]	; (80206a8 <Test_Mail_ISR+0x58>)
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 8020660:	f04f 4680 	mov.w	r6, #1073741824	; 0x40000000
 8020664:	f8df 804c 	ldr.w	r8, [pc, #76]	; 80206b4 <Test_Mail_ISR+0x64>

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020668:	f242 750f 	movw	r5, #9999	; 0x270f
 802066c:	6023      	str	r3, [r4, #0]
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
 802066e:	490f      	ldr	r1, [pc, #60]	; (80206ac <Test_Mail_ISR+0x5c>)
 8020670:	a801      	add	r0, sp, #4
 8020672:	f001 fb87 	bl	8021d84 <RMP_Thd_Rcv>
        /* Read counter here */
        End=COUNTER_READ();
 8020676:	6a73      	ldr	r3, [r6, #36]	; 0x24

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020678:	6821      	ldr	r1, [r4, #0]
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 802067a:	005b      	lsls	r3, r3, #1

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802067c:	3101      	adds	r1, #1
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 802067e:	f8c9 3000 	str.w	r3, [r9]
        Total+=(rmp_tim_t)(End-Start);
 8020682:	f8d9 3000 	ldr.w	r3, [r9]

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 8020686:	42a9      	cmp	r1, r5
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
 8020688:	6838      	ldr	r0, [r7, #0]
 802068a:	f8d8 2000 	ldr.w	r2, [r8]

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802068e:	6021      	str	r1, [r4, #0]
    {
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
 8020690:	4413      	add	r3, r2
 8020692:	eba3 0300 	sub.w	r3, r3, r0
 8020696:	f8c8 3000 	str.w	r3, [r8]

void Test_Mail_ISR(void)
{
    rmp_ptr_t Data;
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 802069a:	dde8      	ble.n	802066e <Test_Mail_ISR+0x1e>
        RMP_Thd_Rcv(&Data, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}
 802069c:	b003      	add	sp, #12
 802069e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 80206a2:	bf00      	nop
 80206a4:	20028484 	.word	0x20028484
 80206a8:	20028490 	.word	0x20028490
 80206ac:	000186a0 	.word	0x000186a0
 80206b0:	2002845c 	.word	0x2002845c
 80206b4:	20028510 	.word	0x20028510

080206b8 <Test_Sem_ISR>:

void Test_Sem_ISR(void)
{
 80206b8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206bc:	2300      	movs	r3, #0
 80206be:	4c11      	ldr	r4, [pc, #68]	; (8020704 <Test_Sem_ISR+0x4c>)
 80206c0:	f8df 8054 	ldr.w	r8, [pc, #84]	; 8020718 <Test_Sem_ISR+0x60>
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 80206c4:	f04f 4680 	mov.w	r6, #1073741824	; 0x40000000
 80206c8:	4d0f      	ldr	r5, [pc, #60]	; (8020708 <Test_Sem_ISR+0x50>)
 80206ca:	4f10      	ldr	r7, [pc, #64]	; (802070c <Test_Sem_ISR+0x54>)
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206cc:	6023      	str	r3, [r4, #0]
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
 80206ce:	4910      	ldr	r1, [pc, #64]	; (8020710 <Test_Sem_ISR+0x58>)
 80206d0:	4810      	ldr	r0, [pc, #64]	; (8020714 <Test_Sem_ISR+0x5c>)
 80206d2:	f001 fd63 	bl	802219c <RMP_Sem_Pend>
        /* Read counter here */
        End=COUNTER_READ();
 80206d6:	6a73      	ldr	r3, [r6, #36]	; 0x24
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206d8:	6821      	ldr	r1, [r4, #0]
 80206da:	f242 700f 	movw	r0, #9999	; 0x270f
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 80206de:	005b      	lsls	r3, r3, #1
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206e0:	3101      	adds	r1, #1
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
 80206e2:	f8c8 3000 	str.w	r3, [r8]
        Total+=(rmp_tim_t)(End-Start);
 80206e6:	f8d8 3000 	ldr.w	r3, [r8]
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206ea:	4281      	cmp	r1, r0
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
 80206ec:	f8d5 e000 	ldr.w	lr, [r5]
 80206f0:	683a      	ldr	r2, [r7, #0]
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206f2:	6021      	str	r1, [r4, #0]
    {
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
 80206f4:	4413      	add	r3, r2
 80206f6:	ebce 0303 	rsb	r3, lr, r3
 80206fa:	603b      	str	r3, [r7, #0]
}

void Test_Sem_ISR(void)
{
    static rmp_cnt_t Count;
    for(Count=0;Count<10000;Count++)
 80206fc:	dde7      	ble.n	80206ce <Test_Sem_ISR+0x16>
        RMP_Sem_Pend(&Sem_1, RMP_MAX_SLICES);
        /* Read counter here */
        End=COUNTER_READ();
        Total+=(rmp_tim_t)(End-Start);
    }
}
 80206fe:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8020702:	bf00      	nop
 8020704:	20020450 	.word	0x20020450
 8020708:	20028490 	.word	0x20028490
 802070c:	20028510 	.word	0x20028510
 8020710:	000186a0 	.word	0x000186a0
 8020714:	20028bc4 	.word	0x20028bc4
 8020718:	2002845c 	.word	0x2002845c

0802071c <Test_Mem_Pool>:
    *Arg1=*Arg2;
    *Arg2=Temp;
}

void Test_Mem_Pool(void)
{
 802071c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    static rmp_u8_t Size[8];
    static rmp_ptr_t Amount[8];
    rmp_cnt_t Count;
    rmp_cnt_t Test_Count;
    
    Amount[0]=TEST_MEM_POOL/32;
 8020720:	4a15      	ldr	r2, [pc, #84]	; (8020778 <Test_Mem_Pool+0x5c>)
 8020722:	f44f 7880 	mov.w	r8, #256	; 0x100
    Amount[1]=TEST_MEM_POOL/64+16;
 8020726:	f04f 0c90 	mov.w	ip, #144	; 0x90
    Amount[2]=TEST_MEM_POOL/4;
 802072a:	f44f 6e00 	mov.w	lr, #2048	; 0x800
    Amount[3]=TEST_MEM_POOL/128+32;
 802072e:	2760      	movs	r7, #96	; 0x60
    Amount[4]=TEST_MEM_POOL/16;
 8020730:	f44f 7600 	mov.w	r6, #512	; 0x200
    Amount[5]=TEST_MEM_POOL/8+16;
 8020734:	f44f 6582 	mov.w	r5, #1040	; 0x410
    Amount[6]=TEST_MEM_POOL/128+64;
 8020738:	2480      	movs	r4, #128	; 0x80
    Amount[7]=TEST_MEM_POOL/2-64;
 802073a:	f44f 697c 	mov.w	r9, #4032	; 0xfc0
    static rmp_u8_t Size[8];
    static rmp_ptr_t Amount[8];
    rmp_cnt_t Count;
    rmp_cnt_t Test_Count;
    
    Amount[0]=TEST_MEM_POOL/32;
 802073e:	f8c2 8000 	str.w	r8, [r2]
    Amount[5]=TEST_MEM_POOL/8+16;
    Amount[6]=TEST_MEM_POOL/128+64;
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
 8020742:	2300      	movs	r3, #0
    *Arg1=*Arg2;
    *Arg2=Temp;
}

void Test_Mem_Pool(void)
{
 8020744:	b08d      	sub	sp, #52	; 0x34
    static rmp_ptr_t Amount[8];
    rmp_cnt_t Count;
    rmp_cnt_t Test_Count;
    
    Amount[0]=TEST_MEM_POOL/32;
    Amount[1]=TEST_MEM_POOL/64+16;
 8020746:	f8c2 c004 	str.w	ip, [r2, #4]
    Amount[6]=TEST_MEM_POOL/128+64;
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
 802074a:	f44f 4100 	mov.w	r1, #32768	; 0x8000
    rmp_cnt_t Count;
    rmp_cnt_t Test_Count;
    
    Amount[0]=TEST_MEM_POOL/32;
    Amount[1]=TEST_MEM_POOL/64+16;
    Amount[2]=TEST_MEM_POOL/4;
 802074e:	f8c2 e008 	str.w	lr, [r2, #8]
    Amount[3]=TEST_MEM_POOL/128+32;
 8020752:	60d7      	str	r7, [r2, #12]
    Amount[4]=TEST_MEM_POOL/16;
 8020754:	6116      	str	r6, [r2, #16]
    Amount[5]=TEST_MEM_POOL/8+16;
 8020756:	6155      	str	r5, [r2, #20]
    Amount[6]=TEST_MEM_POOL/128+64;
 8020758:	6194      	str	r4, [r2, #24]
    Amount[7]=TEST_MEM_POOL/2-64;
 802075a:	f8c2 901c 	str.w	r9, [r2, #28]
    
    /* Initialize the pool */
    Total=0;
 802075e:	4a07      	ldr	r2, [pc, #28]	; (802077c <Test_Mem_Pool+0x60>)
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
 8020760:	4807      	ldr	r0, [pc, #28]	; (8020780 <Test_Mem_Pool+0x64>)
    for(Test_Count=0;Test_Count<10000;Test_Count++)
 8020762:	9304      	str	r3, [sp, #16]
    Amount[5]=TEST_MEM_POOL/8+16;
    Amount[6]=TEST_MEM_POOL/128+64;
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
 8020764:	6013      	str	r3, [r2, #0]
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
 8020766:	f001 fff9 	bl	802275c <RMP_Mem_Init>
 802076a:	f8df a018 	ldr.w	sl, [pc, #24]	; 8020784 <Test_Mem_Pool+0x68>
 802076e:	f8df b018 	ldr.w	fp, [pc, #24]	; 8020788 <Test_Mem_Pool+0x6c>
 8020772:	f8df 8018 	ldr.w	r8, [pc, #24]	; 802078c <Test_Mem_Pool+0x70>
 8020776:	e017      	b.n	80207a8 <Test_Mem_Pool+0x8c>
 8020778:	2002849c 	.word	0x2002849c
 802077c:	20028510 	.word	0x20028510
 8020780:	20020454 	.word	0x20020454
 8020784:	20028454 	.word	0x20028454
 8020788:	20028494 	.word	0x20028494
 802078c:	20028504 	.word	0x20028504
    for(Test_Count=0;Test_Count<10000;Test_Count++)
 8020790:	9b04      	ldr	r3, [sp, #16]
            RMP_PRINTK_S("Memory test failure: ");
            RMP_PRINTK_I(Test_Count);
            RMP_PRINTK_S(" runs.\r\n");
            while(1);
        }
        RMP_Free(Pool, Mem[0]); 
 8020792:	48e1      	ldr	r0, [pc, #900]	; (8020b18 <Test_Mem_Pool+0x3fc>)
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    for(Test_Count=0;Test_Count<10000;Test_Count++)
 8020794:	3301      	adds	r3, #1
 8020796:	461c      	mov	r4, r3
 8020798:	9304      	str	r3, [sp, #16]
            RMP_PRINTK_S("Memory test failure: ");
            RMP_PRINTK_I(Test_Count);
            RMP_PRINTK_S(" runs.\r\n");
            while(1);
        }
        RMP_Free(Pool, Mem[0]); 
 802079a:	f002 f9d9 	bl	8022b50 <RMP_Free>
    Amount[7]=TEST_MEM_POOL/2-64;
    
    /* Initialize the pool */
    Total=0;
    RMP_Mem_Init(Pool, TEST_MEM_POOL*sizeof(rmp_ptr_t));
    for(Test_Count=0;Test_Count<10000;Test_Count++)
 802079e:	f242 7310 	movw	r3, #10000	; 0x2710
 80207a2:	429c      	cmp	r4, r3
 80207a4:	f000 82ef 	beq.w	8020d86 <Test_Mem_Pool+0x66a>
 80207a8:	4bdc      	ldr	r3, [pc, #880]	; (8020b1c <Test_Mem_Pool+0x400>)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
            Free[Count]=Count;
 80207aa:	4ddd      	ldr	r5, [pc, #884]	; (8020b20 <Test_Mem_Pool+0x404>)
 80207ac:	681a      	ldr	r2, [r3, #0]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 80207ae:	4bdd      	ldr	r3, [pc, #884]	; (8020b24 <Test_Mem_Pool+0x408>)
 80207b0:	f012 0f01 	tst.w	r2, #1
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
            Free[Count]=Count;
 80207b4:	f8cb 5000 	str.w	r5, [fp]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 80207b8:	ea4f 6413 	mov.w	r4, r3, lsr #24
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
 80207bc:	f1a3 3304 	sub.w	r3, r3, #67372036	; 0x4040404
            Free[Count]=Count;
 80207c0:	f105 3504 	add.w	r5, r5, #67372036	; 0x4040404
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 80207c4:	4fd8      	ldr	r7, [pc, #864]	; (8020b28 <Test_Mem_Pool+0x40c>)
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
 80207c6:	f8ca 3000 	str.w	r3, [sl]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 80207ca:	ea4f 0352 	mov.w	r3, r2, lsr #1
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
 80207ce:	4ad5      	ldr	r2, [pc, #852]	; (8020b24 <Test_Mem_Pool+0x408>)
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 80207d0:	4620      	mov	r0, r4
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
            Free[Count]=Count;
 80207d2:	f8cb 5004 	str.w	r5, [fp, #4]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 80207d6:	4621      	mov	r1, r4
    for(Test_Count=0;Test_Count<10000;Test_Count++)
    {
        /* Random sequence and number generation */
        for(Count=0;Count<8;Count++)
        {
            Alloc[Count]=Count;
 80207d8:	f8ca 2004 	str.w	r2, [sl, #4]
 80207dc:	f483 4234 	eor.w	r2, r3, #46080	; 0xb400
 80207e0:	bf08      	it	eq
 80207e2:	461a      	moveq	r2, r3
            Free[Count]=Count;
            Size[Count]=Count;
 80207e4:	4bce      	ldr	r3, [pc, #824]	; (8020b20 <Test_Mem_Pool+0x404>)
 80207e6:	f8c8 5004 	str.w	r5, [r8, #4]
 80207ea:	f8c8 3000 	str.w	r3, [r8]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 80207ee:	0853      	lsrs	r3, r2, #1
 80207f0:	f012 0f01 	tst.w	r2, #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 80207f4:	f002 0207 	and.w	r2, r2, #7
 80207f8:	f483 4634 	eor.w	r6, r3, #46080	; 0xb400

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 80207fc:	f81a 5002 	ldrb.w	r5, [sl, r2]
 8020800:	bf18      	it	ne
 8020802:	4633      	movne	r3, r6
 8020804:	f88a 5007 	strb.w	r5, [sl, #7]
 8020808:	f013 0f01 	tst.w	r3, #1
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 802080c:	ea4f 0553 	mov.w	r5, r3, lsr #1
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020810:	f003 0307 	and.w	r3, r3, #7
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020814:	f80a 4002 	strb.w	r4, [sl, r2]
 8020818:	f485 4234 	eor.w	r2, r5, #46080	; 0xb400
 802081c:	bf08      	it	eq
 802081e:	462a      	moveq	r2, r5

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020820:	f81b 4003 	ldrb.w	r4, [fp, r3]
 8020824:	f012 0f01 	tst.w	r2, #1
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020828:	f89a 9006 	ldrb.w	r9, [sl, #6]
    *Arg1=*Arg2;
 802082c:	f88b 4007 	strb.w	r4, [fp, #7]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020830:	ea4f 0452 	mov.w	r4, r2, lsr #1
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020834:	f002 0207 	and.w	r2, r2, #7
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020838:	f80b 0003 	strb.w	r0, [fp, r3]
 802083c:	f484 4334 	eor.w	r3, r4, #46080	; 0xb400
 8020840:	bf08      	it	eq
 8020842:	4623      	moveq	r3, r4

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020844:	f818 0002 	ldrb.w	r0, [r8, r2]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020848:	4cb8      	ldr	r4, [pc, #736]	; (8020b2c <Test_Mem_Pool+0x410>)
 802084a:	f013 0f01 	tst.w	r3, #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 802084e:	f888 0007 	strb.w	r0, [r8, #7]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020852:	ea4f 0053 	mov.w	r0, r3, lsr #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020856:	fba3 4504 	umull	r4, r5, r3, r4
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 802085a:	f89b 6006 	ldrb.w	r6, [fp, #6]
 802085e:	f480 4434 	eor.w	r4, r0, #46080	; 0xb400
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020862:	f808 1002 	strb.w	r1, [r8, r2]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020866:	960a      	str	r6, [sp, #40]	; 0x28
 8020868:	bf18      	it	ne
 802086a:	4620      	movne	r0, r4
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 802086c:	1b5c      	subs	r4, r3, r5
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 802086e:	0842      	lsrs	r2, r0, #1
 8020870:	f010 0f01 	tst.w	r0, #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020874:	eb05 0454 	add.w	r4, r5, r4, lsr #1
 8020878:	f482 4134 	eor.w	r1, r2, #46080	; 0xb400
 802087c:	ea4f 0494 	mov.w	r4, r4, lsr #2
 8020880:	bf18      	it	ne
 8020882:	460a      	movne	r2, r1
 8020884:	ebc4 04c4 	rsb	r4, r4, r4, lsl #3
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020888:	0851      	lsrs	r1, r2, #1
 802088a:	f012 0f01 	tst.w	r2, #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 802088e:	eba3 0404 	sub.w	r4, r3, r4
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020892:	f898 3006 	ldrb.w	r3, [r8, #6]
 8020896:	f481 4534 	eor.w	r5, r1, #46080	; 0xb400
 802089a:	bf08      	it	eq
 802089c:	460d      	moveq	r5, r1
 802089e:	9309      	str	r3, [sp, #36]	; 0x24
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 80208a0:	086b      	lsrs	r3, r5, #1
 80208a2:	f015 0f01 	tst.w	r5, #1
 80208a6:	f483 4634 	eor.w	r6, r3, #46080	; 0xb400
 80208aa:	bf0c      	ite	eq
 80208ac:	4619      	moveq	r1, r3
 80208ae:	4631      	movne	r1, r6
 80208b0:	084b      	lsrs	r3, r1, #1
 80208b2:	f011 0f01 	tst.w	r1, #1
 80208b6:	9105      	str	r1, [sp, #20]
 80208b8:	f483 4134 	eor.w	r1, r3, #46080	; 0xb400
 80208bc:	bf0c      	ite	eq
 80208be:	469c      	moveq	ip, r3
 80208c0:	468c      	movne	ip, r1
 80208c2:	ea4f 035c 	mov.w	r3, ip, lsr #1
 80208c6:	f01c 0f01 	tst.w	ip, #1
 80208ca:	f483 4134 	eor.w	r1, r3, #46080	; 0xb400
 80208ce:	bf18      	it	ne
 80208d0:	460b      	movne	r3, r1
 80208d2:	0859      	lsrs	r1, r3, #1
 80208d4:	f013 0f01 	tst.w	r3, #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 80208d8:	fba7 e703 	umull	lr, r7, r7, r3
 80208dc:	f481 4e34 	eor.w	lr, r1, #46080	; 0xb400
 80208e0:	ea4f 0797 	mov.w	r7, r7, lsr #2
 80208e4:	bf18      	it	ne
 80208e6:	4671      	movne	r1, lr
 80208e8:	eb07 0787 	add.w	r7, r7, r7, lsl #2
 80208ec:	460e      	mov	r6, r1
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 80208ee:	0849      	lsrs	r1, r1, #1
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 80208f0:	1bdf      	subs	r7, r3, r7
 80208f2:	4b8f      	ldr	r3, [pc, #572]	; (8020b30 <Test_Mem_Pool+0x414>)
 80208f4:	f016 0f01 	tst.w	r6, #1
 80208f8:	9607      	str	r6, [sp, #28]
 80208fa:	fba3 e305 	umull	lr, r3, r3, r5
 80208fe:	f481 4e34 	eor.w	lr, r1, #46080	; 0xb400
 8020902:	bf08      	it	eq
 8020904:	468e      	moveq	lr, r1
 8020906:	089b      	lsrs	r3, r3, #2
 8020908:	4671      	mov	r1, lr
 802090a:	f01e 0f01 	tst.w	lr, #1
 802090e:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8020912:	f8cd e020 	str.w	lr, [sp, #32]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020916:	ea4f 0151 	mov.w	r1, r1, lsr #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 802091a:	f81a e004 	ldrb.w	lr, [sl, r4]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 802091e:	eba5 0343 	sub.w	r3, r5, r3, lsl #1
 8020922:	f481 4534 	eor.w	r5, r1, #46080	; 0xb400

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020926:	f88a e006 	strb.w	lr, [sl, #6]
    *Arg2=Temp;
 802092a:	f80a 9004 	strb.w	r9, [sl, r4]
 802092e:	bf18      	it	ne
 8020930:	4629      	movne	r1, r5

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020932:	f81a 4003 	ldrb.w	r4, [sl, r3]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020936:	f89a 9005 	ldrb.w	r9, [sl, #5]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 802093a:	ea4f 0e51 	mov.w	lr, r1, lsr #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 802093e:	4626      	mov	r6, r4
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020940:	4c7a      	ldr	r4, [pc, #488]	; (8020b2c <Test_Mem_Pool+0x410>)
 8020942:	f011 0f01 	tst.w	r1, #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020946:	f88a 6005 	strb.w	r6, [sl, #5]
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 802094a:	fba2 4504 	umull	r4, r5, r2, r4
 802094e:	f48e 4434 	eor.w	r4, lr, #46080	; 0xb400
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020952:	f80a 9003 	strb.w	r9, [sl, r3]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020956:	f001 0903 	and.w	r9, r1, #3
 802095a:	bf18      	it	ne
 802095c:	46a6      	movne	lr, r4
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 802095e:	1b53      	subs	r3, r2, r5
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020960:	f8cd 9004 	str.w	r9, [sp, #4]
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020964:	4c72      	ldr	r4, [pc, #456]	; (8020b30 <Test_Mem_Pool+0x414>)
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020966:	eb05 0353 	add.w	r3, r5, r3, lsr #1
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 802096a:	9905      	ldr	r1, [sp, #20]
 802096c:	f01e 0f01 	tst.w	lr, #1
 8020970:	4d6e      	ldr	r5, [pc, #440]	; (8020b2c <Test_Mem_Pool+0x410>)
 8020972:	fba4 4901 	umull	r4, r9, r4, r1
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020976:	ea4f 045e 	mov.w	r4, lr, lsr #1
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 802097a:	ea4f 0193 	mov.w	r1, r3, lsr #2
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 802097e:	fba0 5605 	umull	r5, r6, r0, r5
 8020982:	f484 4334 	eor.w	r3, r4, #46080	; 0xb400
 8020986:	ea4f 0999 	mov.w	r9, r9, lsr #2
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 802098a:	ebc1 01c1 	rsb	r1, r1, r1, lsl #3
 802098e:	bf18      	it	ne
 8020990:	461c      	movne	r4, r3
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020992:	eb09 0949 	add.w	r9, r9, r9, lsl #1
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020996:	1a51      	subs	r1, r2, r1
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020998:	0863      	lsrs	r3, r4, #1
 802099a:	f014 0f01 	tst.w	r4, #1
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 802099e:	f004 0403 	and.w	r4, r4, #3
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209a2:	e9cd 5602 	strd	r5, r6, [sp, #8]
 80209a6:	9a03      	ldr	r2, [sp, #12]
 80209a8:	f00e 0503 	and.w	r5, lr, #3
 80209ac:	9e05      	ldr	r6, [sp, #20]
 80209ae:	eba0 0202 	sub.w	r2, r0, r2
 80209b2:	9506      	str	r5, [sp, #24]
 80209b4:	eba6 0949 	sub.w	r9, r6, r9, lsl #1
 80209b8:	f483 4534 	eor.w	r5, r3, #46080	; 0xb400
 80209bc:	9e03      	ldr	r6, [sp, #12]

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 80209be:	f818 e001 	ldrb.w	lr, [r8, r1]
 80209c2:	bf18      	it	ne
 80209c4:	462b      	movne	r3, r5
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209c6:	eb06 0252 	add.w	r2, r6, r2, lsr #1
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 80209ca:	f89d 5024 	ldrb.w	r5, [sp, #36]	; 0x24

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 80209ce:	f888 e006 	strb.w	lr, [r8, #6]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 80209d2:	085e      	lsrs	r6, r3, #1
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 80209d4:	f808 5001 	strb.w	r5, [r8, r1]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209d8:	0892      	lsrs	r2, r2, #2
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 80209da:	4d55      	ldr	r5, [pc, #340]	; (8020b30 <Test_Mem_Pool+0x414>)
 80209dc:	f013 0f01 	tst.w	r3, #1
 80209e0:	f486 4134 	eor.w	r1, r6, #46080	; 0xb400
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209e4:	ebc2 02c2 	rsb	r2, r2, r2, lsl #3
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 80209e8:	fba5 e50c 	umull	lr, r5, r5, ip
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209ec:	eba0 0202 	sub.w	r2, r0, r2
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 80209f0:	940b      	str	r4, [sp, #44]	; 0x2c
 80209f2:	bf0c      	ite	eq
 80209f4:	4630      	moveq	r0, r6
 80209f6:	4608      	movne	r0, r1
 80209f8:	08ac      	lsrs	r4, r5, #2
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 80209fa:	4e4b      	ldr	r6, [pc, #300]	; (8020b28 <Test_Mem_Pool+0x40c>)
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 80209fc:	4625      	mov	r5, r4
 80209fe:	4604      	mov	r4, r0

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020a00:	f81b 0002 	ldrb.w	r0, [fp, r2]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020a04:	0861      	lsrs	r1, r4, #1
 8020a06:	f014 0f01 	tst.w	r4, #1
 8020a0a:	9409      	str	r4, [sp, #36]	; 0x24
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a0c:	eb05 0545 	add.w	r5, r5, r5, lsl #1
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020a10:	9c07      	ldr	r4, [sp, #28]
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a12:	ebac 0545 	sub.w	r5, ip, r5, lsl #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020a16:	f88b 0006 	strb.w	r0, [fp, #6]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020a1a:	46a6      	mov	lr, r4

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020a1c:	f81a c007 	ldrb.w	ip, [sl, r7]
    *Arg2=Temp;
 8020a20:	f89d 0028 	ldrb.w	r0, [sp, #40]	; 0x28
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020a24:	fba6 e40e 	umull	lr, r4, r6, lr
 8020a28:	f481 4634 	eor.w	r6, r1, #46080	; 0xb400
 8020a2c:	bf08      	it	eq
 8020a2e:	460e      	moveq	r6, r1
 8020a30:	9907      	ldr	r1, [sp, #28]
 8020a32:	08a4      	lsrs	r4, r4, #2
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020a34:	f80b 0002 	strb.w	r0, [fp, r2]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020a38:	f89b 2005 	ldrb.w	r2, [fp, #5]
 8020a3c:	f016 0f01 	tst.w	r6, #1
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020a40:	eb04 0484 	add.w	r4, r4, r4, lsl #2
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a44:	9505      	str	r5, [sp, #20]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020a46:	920a      	str	r2, [sp, #40]	; 0x28
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020a48:	ea4f 0256 	mov.w	r2, r6, lsr #1
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020a4c:	eba1 0404 	sub.w	r4, r1, r4
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020a50:	f89a 1004 	ldrb.w	r1, [sl, #4]
    *Arg1=*Arg2;
 8020a54:	f88a c004 	strb.w	ip, [sl, #4]
    *Arg2=Temp;
 8020a58:	f80a 1007 	strb.w	r1, [sl, r7]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a5c:	4f34      	ldr	r7, [pc, #208]	; (8020b30 <Test_Mem_Pool+0x414>)
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a5e:	9808      	ldr	r0, [sp, #32]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a60:	fba7 7103 	umull	r7, r1, r7, r3
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a64:	4d30      	ldr	r5, [pc, #192]	; (8020b28 <Test_Mem_Pool+0x40c>)
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a66:	ea4f 0151 	mov.w	r1, r1, lsr #1
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a6a:	fba5 e000 	umull	lr, r0, r5, r0
 8020a6e:	f482 4e34 	eor.w	lr, r2, #46080	; 0xb400
 8020a72:	bf08      	it	eq
 8020a74:	4696      	moveq	lr, r2
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a76:	9102      	str	r1, [sp, #8]
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a78:	0880      	lsrs	r0, r0, #2
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a7a:	f00e 0701 	and.w	r7, lr, #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020a7e:	9901      	ldr	r1, [sp, #4]
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020a80:	ea4f 025e 	mov.w	r2, lr, lsr #1
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020a84:	f89a e003 	ldrb.w	lr, [sl, #3]
    *Arg1=*Arg2;
 8020a88:	f81a c001 	ldrb.w	ip, [sl, r1]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a8c:	463d      	mov	r5, r7
 8020a8e:	9902      	ldr	r1, [sp, #8]
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a90:	eb00 0080 	add.w	r0, r0, r0, lsl #2
 8020a94:	9f08      	ldr	r7, [sp, #32]
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020a96:	eb01 0141 	add.w	r1, r1, r1, lsl #1
 8020a9a:	9507      	str	r5, [sp, #28]
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020a9c:	1a38      	subs	r0, r7, r0
 8020a9e:	f482 4734 	eor.w	r7, r2, #46080	; 0xb400
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020aa2:	1a59      	subs	r1, r3, r1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020aa4:	f88a c003 	strb.w	ip, [sl, #3]
 8020aa8:	2d00      	cmp	r5, #0
 8020aaa:	bf08      	it	eq
 8020aac:	4617      	moveq	r7, r2
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020aae:	9a09      	ldr	r2, [sp, #36]	; 0x24
            Size[Count]=Count;
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
 8020ab0:	9102      	str	r1, [sp, #8]
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020ab2:	4615      	mov	r5, r2
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020ab4:	9901      	ldr	r1, [sp, #4]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020ab6:	4a1e      	ldr	r2, [pc, #120]	; (8020b30 <Test_Mem_Pool+0x414>)
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020ab8:	f80a e001 	strb.w	lr, [sl, r1]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020abc:	fba2 5205 	umull	r5, r2, r2, r5

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020ac0:	f81b e009 	ldrb.w	lr, [fp, r9]
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020ac4:	4b1a      	ldr	r3, [pc, #104]	; (8020b30 <Test_Mem_Pool+0x414>)

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020ac6:	f88b e005 	strb.w	lr, [fp, #5]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020aca:	0852      	lsrs	r2, r2, #1
{
    static rmp_ptr_t LFSR=0xACE1;
    
    if((LFSR&0x01)!=0)
    {
        LFSR>>=1;
 8020acc:	ea4f 0e57 	mov.w	lr, r7, lsr #1
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020ad0:	fba3 c306 	umull	ip, r3, r3, r6
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020ad4:	f007 0701 	and.w	r7, r7, #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020ad8:	9d05      	ldr	r5, [sp, #20]
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020ada:	085b      	lsrs	r3, r3, #1
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020adc:	f89d 1028 	ldrb.w	r1, [sp, #40]	; 0x28
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020ae0:	9701      	str	r7, [sp, #4]
 8020ae2:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 8020ae6:	9f09      	ldr	r7, [sp, #36]	; 0x24
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020ae8:	eb03 0343 	add.w	r3, r3, r3, lsl #1

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020aec:	f818 c005 	ldrb.w	ip, [r8, r5]
        }
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
 8020af0:	1aba      	subs	r2, r7, r2
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020af2:	f898 7005 	ldrb.w	r7, [r8, #5]
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020af6:	1af3      	subs	r3, r6, r3

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020af8:	f888 c005 	strb.w	ip, [r8, #5]
 8020afc:	f48e 4634 	eor.w	r6, lr, #46080	; 0xb400
    *Arg2=Temp;
 8020b00:	f808 7005 	strb.w	r7, [r8, r5]
 8020b04:	9f01      	ldr	r7, [sp, #4]
 8020b06:	f80b 1009 	strb.w	r1, [fp, r9]
 8020b0a:	2f00      	cmp	r7, #0
 8020b0c:	bf0c      	ite	eq
 8020b0e:	46f4      	moveq	ip, lr
 8020b10:	46b4      	movne	ip, r6

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020b12:	f81b 9004 	ldrb.w	r9, [fp, r4]
 8020b16:	e00d      	b.n	8020b34 <Test_Mem_Pool+0x418>
 8020b18:	20020454 	.word	0x20020454
 8020b1c:	20020000 	.word	0x20020000
 8020b20:	03020100 	.word	0x03020100
 8020b24:	07060504 	.word	0x07060504
 8020b28:	cccccccd 	.word	0xcccccccd
 8020b2c:	24924925 	.word	0x24924925
 8020b30:	aaaaaaab 	.word	0xaaaaaaab
 8020b34:	f818 5000 	ldrb.w	r5, [r8, r0]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020b38:	f89b 1004 	ldrb.w	r1, [fp, #4]
 8020b3c:	f898 6004 	ldrb.w	r6, [r8, #4]
    *Arg1=*Arg2;
 8020b40:	f88b 9004 	strb.w	r9, [fp, #4]
        
        for(Count=7;Count>0;Count--)
        {
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
 8020b44:	f00c 0901 	and.w	r9, ip, #1
void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
    *Arg2=Temp;
 8020b48:	f80b 1004 	strb.w	r1, [fp, r4]

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020b4c:	f888 5004 	strb.w	r5, [r8, #4]
 8020b50:	9906      	ldr	r1, [sp, #24]
    *Arg2=Temp;
 8020b52:	f808 6000 	strb.w	r6, [r8, r0]

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020b56:	9e0b      	ldr	r6, [sp, #44]	; 0x2c
 8020b58:	f81b 7001 	ldrb.w	r7, [fp, r1]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020b5c:	f89b 4003 	ldrb.w	r4, [fp, #3]
    *Arg1=*Arg2;
 8020b60:	f818 5006 	ldrb.w	r5, [r8, r6]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020b64:	f898 0003 	ldrb.w	r0, [r8, #3]
    *Arg1=*Arg2;
 8020b68:	f88b 7003 	strb.w	r7, [fp, #3]
 8020b6c:	f888 5003 	strb.w	r5, [r8, #3]
    *Arg2=Temp;
 8020b70:	f80b 4001 	strb.w	r4, [fp, r1]

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020b74:	9902      	ldr	r1, [sp, #8]
 8020b76:	f81b 7002 	ldrb.w	r7, [fp, r2]
 8020b7a:	f81a e001 	ldrb.w	lr, [sl, r1]
    *Arg2=Temp;
 8020b7e:	f808 0006 	strb.w	r0, [r8, r6]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020b82:	f89b 5002 	ldrb.w	r5, [fp, #2]
 8020b86:	f89a 6002 	ldrb.w	r6, [sl, #2]
    *Arg1=*Arg2;
 8020b8a:	f88b 7002 	strb.w	r7, [fp, #2]
 8020b8e:	9f07      	ldr	r7, [sp, #28]
 8020b90:	f88a e002 	strb.w	lr, [sl, #2]
 8020b94:	f818 4003 	ldrb.w	r4, [r8, r3]
    *Arg2=Temp;
 8020b98:	f80b 5002 	strb.w	r5, [fp, r2]
 8020b9c:	f80a 6001 	strb.w	r6, [sl, r1]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020ba0:	f898 0002 	ldrb.w	r0, [r8, #2]
    *Arg1=*Arg2;
 8020ba4:	f81a 6007 	ldrb.w	r6, [sl, r7]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020ba8:	f89a 1001 	ldrb.w	r1, [sl, #1]
    *Arg1=*Arg2;
 8020bac:	9a01      	ldr	r2, [sp, #4]
 8020bae:	f888 4002 	strb.w	r4, [r8, #2]
 8020bb2:	f81b 5002 	ldrb.w	r5, [fp, r2]
    *Arg2=Temp;
 8020bb6:	f808 0003 	strb.w	r0, [r8, r3]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020bba:	f89b 2001 	ldrb.w	r2, [fp, #1]
    *Arg1=*Arg2;
 8020bbe:	f818 0009 	ldrb.w	r0, [r8, r9]
}

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
 8020bc2:	f898 3001 	ldrb.w	r3, [r8, #1]
    *Arg1=*Arg2;
 8020bc6:	f88a 6001 	strb.w	r6, [sl, #1]
    *Arg2=Temp;
 8020bca:	f80a 1007 	strb.w	r1, [sl, r7]
 8020bce:	9f01      	ldr	r7, [sp, #4]

void Swap(rmp_u8_t* Arg1, rmp_u8_t* Arg2)
{
    rmp_u8_t Temp;
    Temp=*Arg1;
    *Arg1=*Arg2;
 8020bd0:	f88b 5001 	strb.w	r5, [fp, #1]
 8020bd4:	f888 0001 	strb.w	r0, [r8, #1]
    *Arg2=Temp;
 8020bd8:	f80b 2007 	strb.w	r2, [fp, r7]
 8020bdc:	f808 3009 	strb.w	r3, [r8, r9]
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
 8020be0:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 8020be4:	4a71      	ldr	r2, [pc, #452]	; (8020dac <Test_Mem_Pool+0x690>)
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020be6:	4972      	ldr	r1, [pc, #456]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020be8:	f8c2 c000 	str.w	ip, [r2]
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
 8020bec:	6a5b      	ldr	r3, [r3, #36]	; 0x24
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020bee:	f898 2000 	ldrb.w	r2, [r8]
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
 8020bf2:	005b      	lsls	r3, r3, #1
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020bf4:	486f      	ldr	r0, [pc, #444]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020bf6:	f851 1022 	ldr.w	r1, [r1, r2, lsl #2]
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
 8020bfa:	4a6f      	ldr	r2, [pc, #444]	; (8020db8 <Test_Mem_Pool+0x69c>)
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020bfc:	f89a 4000 	ldrb.w	r4, [sl]
            Swap(&Alloc[Count], &Alloc[Rand()%(Count+1)]);
            Swap(&Free[Count], &Free[Rand()%(Count+1)]);
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
 8020c00:	6013      	str	r3, [r2, #0]
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020c02:	f001 fe51 	bl	80228a8 <RMP_Malloc>
 8020c06:	4a6d      	ldr	r2, [pc, #436]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
 8020c08:	f898 3001 	ldrb.w	r3, [r8, #1]
            Swap(&Size[Count], &Size[Rand()%(Count+1)]);
        }
        
        Start=COUNTER_READ();
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
 8020c0c:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
 8020c10:	4a67      	ldr	r2, [pc, #412]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c12:	4868      	ldr	r0, [pc, #416]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c14:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c18:	f89a 4001 	ldrb.w	r4, [sl, #1]
 8020c1c:	f001 fe44 	bl	80228a8 <RMP_Malloc>
 8020c20:	4a66      	ldr	r2, [pc, #408]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
 8020c22:	f898 3002 	ldrb.w	r3, [r8, #2]
        }
        
        Start=COUNTER_READ();
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
 8020c26:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
 8020c2a:	4a61      	ldr	r2, [pc, #388]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c2c:	4861      	ldr	r0, [pc, #388]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c2e:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c32:	f89a 4002 	ldrb.w	r4, [sl, #2]
 8020c36:	f001 fe37 	bl	80228a8 <RMP_Malloc>
 8020c3a:	4a60      	ldr	r2, [pc, #384]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
 8020c3c:	f898 3003 	ldrb.w	r3, [r8, #3]
        
        Start=COUNTER_READ();
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
 8020c40:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
 8020c44:	4a5a      	ldr	r2, [pc, #360]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c46:	485b      	ldr	r0, [pc, #364]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c48:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c4c:	f89a 4003 	ldrb.w	r4, [sl, #3]
 8020c50:	f001 fe2a 	bl	80228a8 <RMP_Malloc>
 8020c54:	4a59      	ldr	r2, [pc, #356]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
 8020c56:	f898 3004 	ldrb.w	r3, [r8, #4]
        Start=COUNTER_READ();
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
 8020c5a:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
 8020c5e:	4a54      	ldr	r2, [pc, #336]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c60:	4854      	ldr	r0, [pc, #336]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c62:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c66:	f89a 4004 	ldrb.w	r4, [sl, #4]
 8020c6a:	f001 fe1d 	bl	80228a8 <RMP_Malloc>
 8020c6e:	4a53      	ldr	r2, [pc, #332]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
 8020c70:	f898 3005 	ldrb.w	r3, [r8, #5]
        /* Allocation tests */
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
 8020c74:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
 8020c78:	4a4d      	ldr	r2, [pc, #308]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c7a:	484e      	ldr	r0, [pc, #312]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c7c:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c80:	f89a 4005 	ldrb.w	r4, [sl, #5]
 8020c84:	f001 fe10 	bl	80228a8 <RMP_Malloc>
 8020c88:	4a4c      	ldr	r2, [pc, #304]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[6]]=RMP_Malloc(Pool, Amount[Size[6]]);
 8020c8a:	f898 3006 	ldrb.w	r3, [r8, #6]
        Mem[Alloc[0]]=RMP_Malloc(Pool, Amount[Size[0]]);
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
 8020c8e:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[6]]=RMP_Malloc(Pool, Amount[Size[6]]);
 8020c92:	4a47      	ldr	r2, [pc, #284]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020c94:	4847      	ldr	r0, [pc, #284]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020c96:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020c9a:	f89a 4006 	ldrb.w	r4, [sl, #6]
 8020c9e:	f001 fe03 	bl	80228a8 <RMP_Malloc>
 8020ca2:	4a46      	ldr	r2, [pc, #280]	; (8020dbc <Test_Mem_Pool+0x6a0>)
        Mem[Alloc[7]]=RMP_Malloc(Pool, Amount[Size[7]]);
 8020ca4:	f898 3007 	ldrb.w	r3, [r8, #7]
        Mem[Alloc[1]]=RMP_Malloc(Pool, Amount[Size[1]]);
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
        Mem[Alloc[6]]=RMP_Malloc(Pool, Amount[Size[6]]);
 8020ca8:	f842 0024 	str.w	r0, [r2, r4, lsl #2]
        Mem[Alloc[7]]=RMP_Malloc(Pool, Amount[Size[7]]);
 8020cac:	4a40      	ldr	r2, [pc, #256]	; (8020db0 <Test_Mem_Pool+0x694>)
 8020cae:	4841      	ldr	r0, [pc, #260]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020cb0:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020cb4:	f89a 4007 	ldrb.w	r4, [sl, #7]
 8020cb8:	f001 fdf6 	bl	80228a8 <RMP_Malloc>
 8020cbc:	4a3f      	ldr	r2, [pc, #252]	; (8020dbc <Test_Mem_Pool+0x6a0>)

        /* Deallocation tests */
        RMP_Free(Pool,Mem[Free[0]]);
 8020cbe:	f89b 3000 	ldrb.w	r3, [fp]
        Mem[Alloc[2]]=RMP_Malloc(Pool, Amount[Size[2]]);
        Mem[Alloc[3]]=RMP_Malloc(Pool, Amount[Size[3]]);
        Mem[Alloc[4]]=RMP_Malloc(Pool, Amount[Size[4]]);
        Mem[Alloc[5]]=RMP_Malloc(Pool, Amount[Size[5]]);
        Mem[Alloc[6]]=RMP_Malloc(Pool, Amount[Size[6]]);
        Mem[Alloc[7]]=RMP_Malloc(Pool, Amount[Size[7]]);
 8020cc2:	f842 0024 	str.w	r0, [r2, r4, lsl #2]

        /* Deallocation tests */
        RMP_Free(Pool,Mem[Free[0]]);
 8020cc6:	483b      	ldr	r0, [pc, #236]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020cc8:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020ccc:	f001 ff40 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[1]]);
 8020cd0:	f89b 3001 	ldrb.w	r3, [fp, #1]
 8020cd4:	4a39      	ldr	r2, [pc, #228]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020cd6:	4837      	ldr	r0, [pc, #220]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020cd8:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020cdc:	f001 ff38 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[2]]);
 8020ce0:	f89b 3002 	ldrb.w	r3, [fp, #2]
 8020ce4:	4a35      	ldr	r2, [pc, #212]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020ce6:	4833      	ldr	r0, [pc, #204]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020ce8:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020cec:	f001 ff30 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[3]]);
 8020cf0:	f89b 3003 	ldrb.w	r3, [fp, #3]
 8020cf4:	4a31      	ldr	r2, [pc, #196]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020cf6:	482f      	ldr	r0, [pc, #188]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020cf8:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020cfc:	f001 ff28 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[4]]);
 8020d00:	f89b 3004 	ldrb.w	r3, [fp, #4]
 8020d04:	4a2d      	ldr	r2, [pc, #180]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020d06:	482b      	ldr	r0, [pc, #172]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020d08:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020d0c:	f001 ff20 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[5]]);
 8020d10:	f89b 3005 	ldrb.w	r3, [fp, #5]
 8020d14:	4a29      	ldr	r2, [pc, #164]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020d16:	4827      	ldr	r0, [pc, #156]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020d18:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020d1c:	f001 ff18 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[6]]);
 8020d20:	f89b 3006 	ldrb.w	r3, [fp, #6]
 8020d24:	4a25      	ldr	r2, [pc, #148]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020d26:	4823      	ldr	r0, [pc, #140]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020d28:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020d2c:	f001 ff10 	bl	8022b50 <RMP_Free>
        RMP_Free(Pool,Mem[Free[7]]);
 8020d30:	f89b 3007 	ldrb.w	r3, [fp, #7]
 8020d34:	4a21      	ldr	r2, [pc, #132]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020d36:	481f      	ldr	r0, [pc, #124]	; (8020db4 <Test_Mem_Pool+0x698>)
 8020d38:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
 8020d3c:	f001 ff08 	bl	8022b50 <RMP_Free>
        End=COUNTER_READ();
 8020d40:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 8020d44:	4a1e      	ldr	r2, [pc, #120]	; (8020dc0 <Test_Mem_Pool+0x6a4>)
        Total+=(rmp_tim_t)(End-Start);
        
        /* This should always be successful because we deallocated everything else */
        Mem[0]=RMP_Malloc(Pool, (TEST_MEM_POOL>>7)*127);
 8020d46:	f44f 51fe 	mov.w	r1, #8128	; 0x1fc0
        RMP_Free(Pool,Mem[Free[3]]);
        RMP_Free(Pool,Mem[Free[4]]);
        RMP_Free(Pool,Mem[Free[5]]);
        RMP_Free(Pool,Mem[Free[6]]);
        RMP_Free(Pool,Mem[Free[7]]);
        End=COUNTER_READ();
 8020d4a:	6a5b      	ldr	r3, [r3, #36]	; 0x24
        Total+=(rmp_tim_t)(End-Start);
 8020d4c:	4d1d      	ldr	r5, [pc, #116]	; (8020dc4 <Test_Mem_Pool+0x6a8>)
        RMP_Free(Pool,Mem[Free[3]]);
        RMP_Free(Pool,Mem[Free[4]]);
        RMP_Free(Pool,Mem[Free[5]]);
        RMP_Free(Pool,Mem[Free[6]]);
        RMP_Free(Pool,Mem[Free[7]]);
        End=COUNTER_READ();
 8020d4e:	005b      	lsls	r3, r3, #1
        Total+=(rmp_tim_t)(End-Start);
        
        /* This should always be successful because we deallocated everything else */
        Mem[0]=RMP_Malloc(Pool, (TEST_MEM_POOL>>7)*127);
 8020d50:	4818      	ldr	r0, [pc, #96]	; (8020db4 <Test_Mem_Pool+0x698>)
        RMP_Free(Pool,Mem[Free[3]]);
        RMP_Free(Pool,Mem[Free[4]]);
        RMP_Free(Pool,Mem[Free[5]]);
        RMP_Free(Pool,Mem[Free[6]]);
        RMP_Free(Pool,Mem[Free[7]]);
        End=COUNTER_READ();
 8020d52:	6013      	str	r3, [r2, #0]
        Total+=(rmp_tim_t)(End-Start);
 8020d54:	6813      	ldr	r3, [r2, #0]
 8020d56:	4a18      	ldr	r2, [pc, #96]	; (8020db8 <Test_Mem_Pool+0x69c>)
 8020d58:	6814      	ldr	r4, [r2, #0]
 8020d5a:	682a      	ldr	r2, [r5, #0]
 8020d5c:	4413      	add	r3, r2
 8020d5e:	1b1b      	subs	r3, r3, r4
 8020d60:	602b      	str	r3, [r5, #0]
        
        /* This should always be successful because we deallocated everything else */
        Mem[0]=RMP_Malloc(Pool, (TEST_MEM_POOL>>7)*127);
 8020d62:	f001 fda1 	bl	80228a8 <RMP_Malloc>
 8020d66:	4b15      	ldr	r3, [pc, #84]	; (8020dbc <Test_Mem_Pool+0x6a0>)
 8020d68:	4601      	mov	r1, r0
 8020d6a:	6018      	str	r0, [r3, #0]
        if(Mem[0]==0)
 8020d6c:	2800      	cmp	r0, #0
 8020d6e:	f47f ad0f 	bne.w	8020790 <Test_Mem_Pool+0x74>
        {
            RMP_PRINTK_S("Memory test failure: ");
 8020d72:	4815      	ldr	r0, [pc, #84]	; (8020dc8 <Test_Mem_Pool+0x6ac>)
 8020d74:	f000 fb7e 	bl	8021474 <RMP_Print_String>
            RMP_PRINTK_I(Test_Count);
 8020d78:	9804      	ldr	r0, [sp, #16]
 8020d7a:	f000 fb15 	bl	80213a8 <RMP_Print_Int>
            RMP_PRINTK_S(" runs.\r\n");
 8020d7e:	4813      	ldr	r0, [pc, #76]	; (8020dcc <Test_Mem_Pool+0x6b0>)
 8020d80:	f000 fb78 	bl	8021474 <RMP_Print_String>
 8020d84:	e7fe      	b.n	8020d84 <Test_Mem_Pool+0x668>
            while(1);
        }
        RMP_Free(Pool, Mem[0]); 
    }
    
    RMP_PRINTK_S("Memory: ");
 8020d86:	4812      	ldr	r0, [pc, #72]	; (8020dd0 <Test_Mem_Pool+0x6b4>)
 8020d88:	f000 fb74 	bl	8021474 <RMP_Print_String>
    Memory_Time=Total/160000;
 8020d8c:	682b      	ldr	r3, [r5, #0]
 8020d8e:	4911      	ldr	r1, [pc, #68]	; (8020dd4 <Test_Mem_Pool+0x6b8>)
 8020d90:	4a11      	ldr	r2, [pc, #68]	; (8020dd8 <Test_Mem_Pool+0x6bc>)
 8020d92:	fba1 1303 	umull	r1, r3, r1, r3
 8020d96:	0c5b      	lsrs	r3, r3, #17
 8020d98:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Memory_Time);
 8020d9a:	6810      	ldr	r0, [r2, #0]
 8020d9c:	f000 fb04 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020da0:	480e      	ldr	r0, [pc, #56]	; (8020ddc <Test_Mem_Pool+0x6c0>)
}
 8020da2:	b00d      	add	sp, #52	; 0x34
 8020da4:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    }
    
    RMP_PRINTK_S("Memory: ");
    Memory_Time=Total/160000;
    RMP_PRINTK_I(Memory_Time);
    RMP_PRINTK_S(" cycles.\r\n");
 8020da8:	f000 bb64 	b.w	8021474 <RMP_Print_String>
 8020dac:	20020000 	.word	0x20020000
 8020db0:	2002849c 	.word	0x2002849c
 8020db4:	20020454 	.word	0x20020454
 8020db8:	20028490 	.word	0x20028490
 8020dbc:	20028460 	.word	0x20028460
 8020dc0:	2002845c 	.word	0x2002845c
 8020dc4:	20028510 	.word	0x20028510
 8020dc8:	08022f5c 	.word	0x08022f5c
 8020dcc:	08022f74 	.word	0x08022f74
 8020dd0:	08022f80 	.word	0x08022f80
 8020dd4:	d1b71759 	.word	0xd1b71759
 8020dd8:	20028514 	.word	0x20028514
 8020ddc:	08022f8c 	.word	0x08022f8c

08020de0 <Func_2>:
#endif

void Func_2(void)
{
    /* Yield tests */
    Total=0;
 8020de0:	4c35      	ldr	r4, [pc, #212]	; (8020eb8 <Func_2+0xd8>)
 8020de2:	2600      	movs	r6, #0
    Test_Yield_2();
    RMP_PRINTK_S("Yield: ");

    Yield_Time=Total/10000;
 8020de4:	4d35      	ldr	r5, [pc, #212]	; (8020ebc <Func_2+0xdc>)
#endif

void Func_2(void)
{
    /* Yield tests */
    Total=0;
 8020de6:	6026      	str	r6, [r4, #0]
    RMP_Thd_Del(&Thd_1);
}
#endif

void Func_2(void)
{
 8020de8:	b580      	push	{r7, lr}
    /* Yield tests */
    Total=0;
    Test_Yield_2();
 8020dea:	f7ff fbb9 	bl	8020560 <Test_Yield_2>
    RMP_PRINTK_S("Yield: ");
 8020dee:	4834      	ldr	r0, [pc, #208]	; (8020ec0 <Func_2+0xe0>)
 8020df0:	f000 fb40 	bl	8021474 <RMP_Print_String>

    Yield_Time=Total/10000;
 8020df4:	6823      	ldr	r3, [r4, #0]
 8020df6:	4a33      	ldr	r2, [pc, #204]	; (8020ec4 <Func_2+0xe4>)
 8020df8:	fba5 1303 	umull	r1, r3, r5, r3
    Total=0;
    Int_Init();
    Test_Mail_ISR();
    
    /* Semaphore from interrupt tests */
    Temp=Total;
 8020dfc:	4f32      	ldr	r7, [pc, #200]	; (8020ec8 <Func_2+0xe8>)
    /* Yield tests */
    Total=0;
    Test_Yield_2();
    RMP_PRINTK_S("Yield: ");

    Yield_Time=Total/10000;
 8020dfe:	0b5b      	lsrs	r3, r3, #13
 8020e00:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Yield_Time);
 8020e02:	6810      	ldr	r0, [r2, #0]
 8020e04:	f000 fad0 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020e08:	4830      	ldr	r0, [pc, #192]	; (8020ecc <Func_2+0xec>)
 8020e0a:	f000 fb33 	bl	8021474 <RMP_Print_String>
    /* Change priority of thread 2, just in case */
    RMP_Thd_Set(&Thd_2,2,RMP_MAX_SLICES);
 8020e0e:	2102      	movs	r1, #2
 8020e10:	4a2f      	ldr	r2, [pc, #188]	; (8020ed0 <Func_2+0xf0>)
 8020e12:	4830      	ldr	r0, [pc, #192]	; (8020ed4 <Func_2+0xf4>)
 8020e14:	f000 fd2a 	bl	802186c <RMP_Thd_Set>
    
    /* Mailbox tests */
    Total=0;
 8020e18:	6026      	str	r6, [r4, #0]
    Test_Mail_2();
 8020e1a:	f7ff fbc5 	bl	80205a8 <Test_Mail_2>
    RMP_PRINTK_S("Mailbox: ");
 8020e1e:	482e      	ldr	r0, [pc, #184]	; (8020ed8 <Func_2+0xf8>)
 8020e20:	f000 fb28 	bl	8021474 <RMP_Print_String>
    Mailbox_Time=Total/10000;
 8020e24:	6823      	ldr	r3, [r4, #0]
 8020e26:	4a2d      	ldr	r2, [pc, #180]	; (8020edc <Func_2+0xfc>)
 8020e28:	fba5 1303 	umull	r1, r3, r5, r3
 8020e2c:	0b5b      	lsrs	r3, r3, #13
 8020e2e:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Mailbox_Time);
 8020e30:	6810      	ldr	r0, [r2, #0]
 8020e32:	f000 fab9 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020e36:	4825      	ldr	r0, [pc, #148]	; (8020ecc <Func_2+0xec>)
 8020e38:	f000 fb1c 	bl	8021474 <RMP_Print_String>
    
    /* Semaphore tests */
    Total=0;
 8020e3c:	6026      	str	r6, [r4, #0]
    Test_Sem_2();
 8020e3e:	f7ff fbdd 	bl	80205fc <Test_Sem_2>
    RMP_PRINTK_S("Semaphore: ");
 8020e42:	4827      	ldr	r0, [pc, #156]	; (8020ee0 <Func_2+0x100>)
 8020e44:	f000 fb16 	bl	8021474 <RMP_Print_String>
    Semaphore_Time=Total/10000;
 8020e48:	6823      	ldr	r3, [r4, #0]
 8020e4a:	4a26      	ldr	r2, [pc, #152]	; (8020ee4 <Func_2+0x104>)
 8020e4c:	fba5 1303 	umull	r1, r3, r5, r3
 8020e50:	0b5b      	lsrs	r3, r3, #13
 8020e52:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Semaphore_Time);
 8020e54:	6810      	ldr	r0, [r2, #0]
 8020e56:	f000 faa7 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020e5a:	481c      	ldr	r0, [pc, #112]	; (8020ecc <Func_2+0xec>)
 8020e5c:	f000 fb0a 	bl	8021474 <RMP_Print_String>
    
    /* Memory pool tests */
#ifdef TEST_MEM_POOL
    Test_Mem_Pool();
 8020e60:	f7ff fc5c 	bl	802071c <Test_Mem_Pool>
#endif

    /* Mailbox from interrupt tests */
    Total=0;
 8020e64:	6026      	str	r6, [r4, #0]
    Int_Init();
 8020e66:	f7ff fa81 	bl	802036c <Int_Init>
    Test_Mail_ISR();
 8020e6a:	f7ff fbf1 	bl	8020650 <Test_Mail_ISR>
    
    /* Semaphore from interrupt tests */
    Temp=Total;
 8020e6e:	6823      	ldr	r3, [r4, #0]
 8020e70:	603b      	str	r3, [r7, #0]
    Total=0;
 8020e72:	6026      	str	r6, [r4, #0]
    Test_Sem_ISR();
 8020e74:	f7ff fc20 	bl	80206b8 <Test_Sem_ISR>
    
    RMP_PRINTK_S("Mailbox-ISR: ");
 8020e78:	481b      	ldr	r0, [pc, #108]	; (8020ee8 <Func_2+0x108>)
 8020e7a:	f000 fafb 	bl	8021474 <RMP_Print_String>
    Mailbox_ISR_Time=Temp/10000;
 8020e7e:	683b      	ldr	r3, [r7, #0]
 8020e80:	4a1a      	ldr	r2, [pc, #104]	; (8020eec <Func_2+0x10c>)
 8020e82:	fba5 1303 	umull	r1, r3, r5, r3
 8020e86:	0b5b      	lsrs	r3, r3, #13
 8020e88:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Mailbox_ISR_Time);
 8020e8a:	6810      	ldr	r0, [r2, #0]
 8020e8c:	f000 fa8c 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020e90:	480e      	ldr	r0, [pc, #56]	; (8020ecc <Func_2+0xec>)
 8020e92:	f000 faef 	bl	8021474 <RMP_Print_String>
    RMP_PRINTK_S("Semaphore-ISR: ");
 8020e96:	4816      	ldr	r0, [pc, #88]	; (8020ef0 <Func_2+0x110>)
 8020e98:	f000 faec 	bl	8021474 <RMP_Print_String>
    Semaphore_ISR_Time=Total/10000;
 8020e9c:	6823      	ldr	r3, [r4, #0]
 8020e9e:	4a15      	ldr	r2, [pc, #84]	; (8020ef4 <Func_2+0x114>)
 8020ea0:	fba5 1303 	umull	r1, r3, r5, r3
 8020ea4:	0b5b      	lsrs	r3, r3, #13
 8020ea6:	6013      	str	r3, [r2, #0]
    RMP_PRINTK_I(Semaphore_ISR_Time);
 8020ea8:	6810      	ldr	r0, [r2, #0]
 8020eaa:	f000 fa7d 	bl	80213a8 <RMP_Print_Int>
    RMP_PRINTK_S(" cycles.\r\n");
 8020eae:	4807      	ldr	r0, [pc, #28]	; (8020ecc <Func_2+0xec>)
 8020eb0:	f000 fae0 	bl	8021474 <RMP_Print_String>
 8020eb4:	e7fe      	b.n	8020eb4 <Func_2+0xd4>
 8020eb6:	bf00      	nop
 8020eb8:	20028510 	.word	0x20028510
 8020ebc:	d1b71759 	.word	0xd1b71759
 8020ec0:	08022f98 	.word	0x08022f98
 8020ec4:	20028488 	.word	0x20028488
 8020ec8:	200284bc 	.word	0x200284bc
 8020ecc:	08022f8c 	.word	0x08022f8c
 8020ed0:	000186a0 	.word	0x000186a0
 8020ed4:	20028bd4 	.word	0x20028bd4
 8020ed8:	08022fa0 	.word	0x08022fa0
 8020edc:	20028480 	.word	0x20028480
 8020ee0:	08022fac 	.word	0x08022fac
 8020ee4:	2002850c 	.word	0x2002850c
 8020ee8:	08022fb8 	.word	0x08022fb8
 8020eec:	20028500 	.word	0x20028500
 8020ef0:	08022fc8 	.word	0x08022fc8
 8020ef4:	2002044c 	.word	0x2002044c

08020ef8 <RMP_Init_Hook>:
}

void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
 8020ef8:	4b2b      	ldr	r3, [pc, #172]	; (8020fa8 <RMP_Init_Hook+0xb0>)
 8020efa:	f04f 4280 	mov.w	r2, #1073741824	; 0x40000000
{
#ifndef MINIMAL_SIZE
    /* Init the timer */
    Timer_Init();
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
 8020efe:	213c      	movs	r1, #60	; 0x3c
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
 8020f00:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8020f04:	601a      	str	r2, [r3, #0]
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 8020f06:	2501      	movs	r5, #1
Return      : None.
******************************************************************************/
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
 8020f08:	6816      	ldr	r6, [r2, #0]

void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
 8020f0a:	2400      	movs	r4, #0
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 8020f0c:	6998      	ldr	r0, [r3, #24]
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
 8020f0e:	f04f 3eff 	mov.w	lr, #4294967295
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 8020f12:	f426 767c 	bic.w	r6, r6, #1008	; 0x3f0
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    /* Set the Time Base configuration */
    TIM_Base_SetConfig(TIM2_Handle.Instance, &(TIM2_Handle.Init)); 
    TIM2_Handle.State=HAL_TIM_STATE_READY;
  
    __HAL_RCC_TIM2_CLK_ENABLE();
 8020f16:	4f25      	ldr	r7, [pc, #148]	; (8020fac <RMP_Init_Hook+0xb4>)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
 8020f18:	f8c3 e00c 	str.w	lr, [r3, #12]
 8020f1c:	b084      	sub	sp, #16
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
 8020f1e:	4330      	orrs	r0, r6

void Timer_Init(void)
{
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
 8020f20:	605c      	str	r4, [r3, #4]
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
 8020f22:	609c      	str	r4, [r3, #8]
  }

  /* Set the auto-reload preload */
  MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);

  TIMx->CR1 = tmpcr1;
 8020f24:	6010      	str	r0, [r2, #0]

  /* Set the Auto-reload value */
  TIMx->ARR = (uint32_t)Structure->Period ;
 8020f26:	f8c2 e02c 	str.w	lr, [r2, #44]	; 0x2c
 
  /* Set the Prescaler value */
  TIMx->PSC = (uint32_t)Structure->Prescaler;
 8020f2a:	6294      	str	r4, [r2, #40]	; 0x28
    TIMx->RCR = Structure->RepetitionCounter;
  }

  /* Generate an update event to reload the Prescaler 
     and the repetition counter(only for TIM1 and TIM8) value immediately */
  TIMx->EGR = TIM_EGR_UG;
 8020f2c:	6155      	str	r5, [r2, #20]
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    /* Set the Time Base configuration */
    TIM_Base_SetConfig(TIM2_Handle.Instance, &(TIM2_Handle.Init)); 
    TIM2_Handle.State=HAL_TIM_STATE_READY;
 8020f2e:	f883 503d 	strb.w	r5, [r3, #61]	; 0x3d
  
    __HAL_RCC_TIM2_CLK_ENABLE();
 8020f32:	6c3a      	ldr	r2, [r7, #64]	; 0x40
    /* TIM2 clock = 1/2 CPU clock */
    TIM2_Handle.Instance=TIM2;
    TIM2_Handle.Init.Prescaler=0;
    TIM2_Handle.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM2_Handle.Init.Period=(unsigned int)(-1);
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
 8020f34:	611c      	str	r4, [r3, #16]
    /* Set the Time Base configuration */
    TIM_Base_SetConfig(TIM2_Handle.Instance, &(TIM2_Handle.Init)); 
    TIM2_Handle.State=HAL_TIM_STATE_READY;
  
    __HAL_RCC_TIM2_CLK_ENABLE();
 8020f36:	432a      	orrs	r2, r5
#ifndef MINIMAL_SIZE
    /* Init the timer */
    Timer_Init();
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
 8020f38:	f8df 808c 	ldr.w	r8, [pc, #140]	; 8020fc8 <RMP_Init_Hook+0xd0>
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
 8020f3c:	4e1c      	ldr	r6, [pc, #112]	; (8020fb0 <RMP_Init_Hook+0xb8>)
 8020f3e:	643a      	str	r2, [r7, #64]	; 0x40
{
#ifndef MINIMAL_SIZE
    /* Init the timer */
    Timer_Init();
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
 8020f40:	4640      	mov	r0, r8
 8020f42:	6c3a      	ldr	r2, [r7, #64]	; 0x40
    __HAL_TIM_ENABLE(&TIM2_Handle);
 8020f44:	f8d3 e000 	ldr.w	lr, [r3]
    TIM2_Handle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    /* Set the Time Base configuration */
    TIM_Base_SetConfig(TIM2_Handle.Instance, &(TIM2_Handle.Init)); 
    TIM2_Handle.State=HAL_TIM_STATE_READY;
  
    __HAL_RCC_TIM2_CLK_ENABLE();
 8020f48:	ea02 0305 	and.w	r3, r2, r5
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
    RMP_Clear(&Sem_1,sizeof(struct RMP_Sem));
 8020f4c:	4f19      	ldr	r7, [pc, #100]	; (8020fb4 <RMP_Init_Hook+0xbc>)
 8020f4e:	9303      	str	r3, [sp, #12]
 8020f50:	9b03      	ldr	r3, [sp, #12]
    __HAL_TIM_ENABLE(&TIM2_Handle);
 8020f52:	f8de 3000 	ldr.w	r3, [lr]
 8020f56:	432b      	orrs	r3, r5
 8020f58:	f8ce 3000 	str.w	r3, [lr]
{
#ifndef MINIMAL_SIZE
    /* Init the timer */
    Timer_Init();
    /* Clean up the structures */
    RMP_Clear(&Thd_1,sizeof(struct RMP_Thd));
 8020f5c:	f000 f9d8 	bl	8021310 <RMP_Clear>
    RMP_Clear(&Thd_2,sizeof(struct RMP_Thd));
 8020f60:	4630      	mov	r0, r6
 8020f62:	213c      	movs	r1, #60	; 0x3c
 8020f64:	f000 f9d4 	bl	8021310 <RMP_Clear>
    RMP_Clear(&Sem_1,sizeof(struct RMP_Sem));
 8020f68:	4638      	mov	r0, r7
 8020f6a:	2110      	movs	r1, #16
 8020f6c:	f000 f9d0 	bl	8021310 <RMP_Clear>
    /* Create counting semaphore */
    RMP_Sem_Crt(&Sem_1,0);
 8020f70:	4621      	mov	r1, r4
 8020f72:	4638      	mov	r0, r7
 8020f74:	f001 f8a0 	bl	80220b8 <RMP_Sem_Crt>
    /* Start threads */
    RMP_Thd_Crt(&Thd_1, Func_1, THD1_STACK, (void*)0x1234, 1, 5);
 8020f78:	2305      	movs	r3, #5
 8020f7a:	9500      	str	r5, [sp, #0]
 8020f7c:	4640      	mov	r0, r8
 8020f7e:	9301      	str	r3, [sp, #4]
 8020f80:	f241 2334 	movw	r3, #4660	; 0x1234
 8020f84:	4a0c      	ldr	r2, [pc, #48]	; (8020fb8 <RMP_Init_Hook+0xc0>)
 8020f86:	490d      	ldr	r1, [pc, #52]	; (8020fbc <RMP_Init_Hook+0xc4>)
 8020f88:	f000 fbbc 	bl	8021704 <RMP_Thd_Crt>
    RMP_Thd_Crt(&Thd_2, Func_2, THD2_STACK, (void*)0x4321, 1, 1000);
 8020f8c:	f44f 737a 	mov.w	r3, #1000	; 0x3e8
 8020f90:	4a0b      	ldr	r2, [pc, #44]	; (8020fc0 <RMP_Init_Hook+0xc8>)
 8020f92:	4630      	mov	r0, r6
 8020f94:	9301      	str	r3, [sp, #4]
 8020f96:	f244 3321 	movw	r3, #17185	; 0x4321
 8020f9a:	490a      	ldr	r1, [pc, #40]	; (8020fc4 <RMP_Init_Hook+0xcc>)
 8020f9c:	9500      	str	r5, [sp, #0]
 8020f9e:	f000 fbb1 	bl	8021704 <RMP_Thd_Crt>
#endif
}
 8020fa2:	b004      	add	sp, #16
 8020fa4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8020fa8:	20028518 	.word	0x20028518
 8020fac:	40023800 	.word	0x40023800
 8020fb0:	20028bd4 	.word	0x20028bd4
 8020fb4:	20028bc4 	.word	0x20028bc4
 8020fb8:	20029020 	.word	0x20029020
 8020fbc:	08020525 	.word	0x08020525
 8020fc0:	20028b5c 	.word	0x20028b5c
 8020fc4:	08020de1 	.word	0x08020de1
 8020fc8:	20028c10 	.word	0x20028c10

08020fcc <RMP_Init_Idle>:

void RMP_Init_Idle(void)
{
 8020fcc:	4770      	bx	lr
 8020fce:	bf00      	nop

08020fd0 <RMP_PendSV_Handler>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_PendSV_Handler(void)
{
 8020fd0:	b570      	push	{r4, r5, r6, lr}
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
 8020fd2:	4c1e      	ldr	r4, [pc, #120]	; (802104c <RMP_PendSV_Handler+0x7c>)
    RMP_Save_Ctx();
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(rmp_ptr_t)SP;
 8020fd4:	4e1e      	ldr	r6, [pc, #120]	; (8021050 <RMP_PendSV_Handler+0x80>)
{
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
 8020fd6:	6823      	ldr	r3, [r4, #0]
    *(--SP)=RVM_Regs.Reg.LR;
 8020fd8:	6a62      	ldr	r2, [r4, #36]	; 0x24
    *(--SP)=RVM_Regs.Reg.R9;
    *(--SP)=RVM_Regs.Reg.R8;
    *(--SP)=RVM_Regs.Reg.R7;
    *(--SP)=RVM_Regs.Reg.R6;
    *(--SP)=RVM_Regs.Reg.R5;
    *(--SP)=RVM_Regs.Reg.R4;
 8020fda:	461d      	mov	r5, r3
    rmp_ptr_t* SP;
    /* Spill all the registers onto the user stack
     * MRS       R0,PSP
     * STMDB     R0!,{R4-R11,LR} */
    SP=(rmp_ptr_t*)(RVM_Regs.Reg.SP);    
    *(--SP)=RVM_Regs.Reg.LR;
 8020fdc:	f843 2c04 	str.w	r2, [r3, #-4]
    *(--SP)=RVM_Regs.Reg.R11;
 8020fe0:	6a22      	ldr	r2, [r4, #32]
 8020fe2:	f843 2c08 	str.w	r2, [r3, #-8]
    *(--SP)=RVM_Regs.Reg.R10;
 8020fe6:	69e2      	ldr	r2, [r4, #28]
 8020fe8:	f843 2c0c 	str.w	r2, [r3, #-12]
    *(--SP)=RVM_Regs.Reg.R9;
 8020fec:	69a2      	ldr	r2, [r4, #24]
 8020fee:	f843 2c10 	str.w	r2, [r3, #-16]
    *(--SP)=RVM_Regs.Reg.R8;
 8020ff2:	6962      	ldr	r2, [r4, #20]
 8020ff4:	f843 2c14 	str.w	r2, [r3, #-20]
    *(--SP)=RVM_Regs.Reg.R7;
 8020ff8:	6922      	ldr	r2, [r4, #16]
 8020ffa:	f843 2c18 	str.w	r2, [r3, #-24]
    *(--SP)=RVM_Regs.Reg.R6;
 8020ffe:	68e2      	ldr	r2, [r4, #12]
 8021000:	f843 2c1c 	str.w	r2, [r3, #-28]
    *(--SP)=RVM_Regs.Reg.R5;
 8021004:	68a2      	ldr	r2, [r4, #8]
 8021006:	f843 2c20 	str.w	r2, [r3, #-32]
    *(--SP)=RVM_Regs.Reg.R4;
 802100a:	6863      	ldr	r3, [r4, #4]
 802100c:	f845 3d24 	str.w	r3, [r5, #-36]!
    
    /* Save extra context
     * BL       RMP_Save_Ctx */
    RMP_Save_Ctx();
 8021010:	f001 fb3e 	bl	8022690 <RMP_Save_Ctx>
    
    /* Save the SP to control block
     * LDR       R1,=RMP_Cur_SP
     * STR       R0,[R1] */
    RMP_Cur_SP=(rmp_ptr_t)SP;
 8021014:	6035      	str	r5, [r6, #0]
                
    /* Get the highest ready task
     * BL        _RMP_Get_High_Rdy */
    _RMP_Get_High_Rdy();
 8021016:	f000 fa99 	bl	802154c <_RMP_Get_High_Rdy>
    
    /* Load the SP
     * LDR       R1,=RMP_Cur_SP
     * LDR       R0,[R1] */
    SP=(rmp_ptr_t*)RMP_Cur_SP;
 802101a:	6835      	ldr	r5, [r6, #0]
    
    /* Load extra context
     * BL        RMP_Load_Ctx */
    RMP_Load_Ctx();
 802101c:	f001 fb3a 	bl	8022694 <RMP_Load_Ctx>
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
 8021020:	682b      	ldr	r3, [r5, #0]
    RVM_Regs.Reg.R7=*(SP++);
    RVM_Regs.Reg.R8=*(SP++);
    RVM_Regs.Reg.R9=*(SP++);
    RVM_Regs.Reg.R10=*(SP++);
    RVM_Regs.Reg.R11=*(SP++);
    RVM_Regs.Reg.LR=*(SP++);
 8021022:	f105 0224 	add.w	r2, r5, #36	; 0x24
    RMP_Load_Ctx();
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
 8021026:	6063      	str	r3, [r4, #4]
    RVM_Regs.Reg.R5=*(SP++);
 8021028:	686b      	ldr	r3, [r5, #4]
    RVM_Regs.Reg.R8=*(SP++);
    RVM_Regs.Reg.R9=*(SP++);
    RVM_Regs.Reg.R10=*(SP++);
    RVM_Regs.Reg.R11=*(SP++);
    RVM_Regs.Reg.LR=*(SP++);
    RVM_Regs.Reg.SP=(rmp_ptr_t)SP;
 802102a:	6022      	str	r2, [r4, #0]
     
    /* Load registers from user stack
     * LDMIA     R0!,{R4-R11,LR}
     * MSR       PSP,R0 */
    RVM_Regs.Reg.R4=*(SP++);
    RVM_Regs.Reg.R5=*(SP++);
 802102c:	60a3      	str	r3, [r4, #8]
    RVM_Regs.Reg.R6=*(SP++);
 802102e:	68ab      	ldr	r3, [r5, #8]
 8021030:	60e3      	str	r3, [r4, #12]
    RVM_Regs.Reg.R7=*(SP++);
 8021032:	68eb      	ldr	r3, [r5, #12]
 8021034:	6123      	str	r3, [r4, #16]
    RVM_Regs.Reg.R8=*(SP++);
 8021036:	692b      	ldr	r3, [r5, #16]
 8021038:	6163      	str	r3, [r4, #20]
    RVM_Regs.Reg.R9=*(SP++);
 802103a:	696b      	ldr	r3, [r5, #20]
 802103c:	61a3      	str	r3, [r4, #24]
    RVM_Regs.Reg.R10=*(SP++);
 802103e:	69ab      	ldr	r3, [r5, #24]
 8021040:	61e3      	str	r3, [r4, #28]
    RVM_Regs.Reg.R11=*(SP++);
 8021042:	69eb      	ldr	r3, [r5, #28]
 8021044:	6223      	str	r3, [r4, #32]
    RVM_Regs.Reg.LR=*(SP++);
 8021046:	6a2b      	ldr	r3, [r5, #32]
 8021048:	6263      	str	r3, [r4, #36]	; 0x24
 802104a:	bd70      	pop	{r4, r5, r6, pc}
 802104c:	20029088 	.word	0x20029088
 8021050:	20029208 	.word	0x20029208

08021054 <RMP_SysTick_Handler>:
{
    /* PUSH      {LR} */
    /* Note the system that we have entered an interrupt. We are not using tickless here */
    /* MOV       R0,#0x01 */
    /* BL        _RMP_Tick_Handler */
    _RMP_Tick_Handler(1);
 8021054:	2001      	movs	r0, #1
 8021056:	f000 babd 	b.w	80215d4 <_RMP_Tick_Handler>
 802105a:	bf00      	nop

0802105c <_RMP_Stack_Init>:
              to avoid INVSTATE; set PC to the pseudo-process entrance; set LR
              (1) to 0 because the process does not return to anything; set the 
              R12,R3-R0 to 0; set R11-R4 to 0.
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
 802105c:	b410      	push	{r4}
    /* CM3:Pass the parameter */                            
    ((rmp_ptr_t*)Stack)[0+9]=Arg;       
    /* CM3:for xPSR. fill the T bit,or an INVSTATE will happen */
    ((rmp_ptr_t*)Stack)[6+9]=Entry;
    /* CM3:Set the process entrance */                            
    ((rmp_ptr_t*)Stack)[7+9]=0x01000200;      
 802105e:	4b05      	ldr	r3, [pc, #20]	; (8021074 <_RMP_Stack_Init+0x18>)
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    /* The "9" here is because we also pushed other registers to PSP */
    /* This is the LR value indicating that we never used the FPU */
    ((rmp_ptr_t*)Stack)[0+8]=0xFFFFFFFD;       
 8021060:	f06f 0402 	mvn.w	r4, #2
    /* CM3:Pass the parameter */                            
    ((rmp_ptr_t*)Stack)[0+9]=Arg;       
 8021064:	624a      	str	r2, [r1, #36]	; 0x24
    /* CM3:for xPSR. fill the T bit,or an INVSTATE will happen */
    ((rmp_ptr_t*)Stack)[6+9]=Entry;
 8021066:	63c8      	str	r0, [r1, #60]	; 0x3c
******************************************************************************/
void _RMP_Stack_Init(rmp_ptr_t Entry, rmp_ptr_t Stack, rmp_ptr_t Arg)
{
    /* The "9" here is because we also pushed other registers to PSP */
    /* This is the LR value indicating that we never used the FPU */
    ((rmp_ptr_t*)Stack)[0+8]=0xFFFFFFFD;       
 8021068:	620c      	str	r4, [r1, #32]
    /* CM3:Pass the parameter */                            
    ((rmp_ptr_t*)Stack)[0+9]=Arg;       
    /* CM3:for xPSR. fill the T bit,or an INVSTATE will happen */
    ((rmp_ptr_t*)Stack)[6+9]=Entry;
    /* CM3:Set the process entrance */                            
    ((rmp_ptr_t*)Stack)[7+9]=0x01000200;      
 802106a:	640b      	str	r3, [r1, #64]	; 0x40
}
 802106c:	f85d 4b04 	ldr.w	r4, [sp], #4
 8021070:	4770      	bx	lr
 8021072:	bf00      	nop
 8021074:	01000200 	.word	0x01000200

08021078 <_RMP_Low_Level_Init>:
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Low_Level_Init(void)
{
 8021078:	b508      	push	{r3, lr}
    RVM_Init();
 802107a:	f000 f895 	bl	80211a8 <RVM_Init>
    RVM_Vect_Init(0, RMP_SysTick_Handler);
 802107e:	4906      	ldr	r1, [pc, #24]	; (8021098 <_RMP_Low_Level_Init+0x20>)
 8021080:	2000      	movs	r0, #0
 8021082:	f000 f8af 	bl	80211e4 <RVM_Vect_Init>
    RVM_Vect_Init(1, RMP_PendSV_Handler);
 8021086:	4905      	ldr	r1, [pc, #20]	; (802109c <_RMP_Low_Level_Init+0x24>)
 8021088:	2001      	movs	r0, #1
 802108a:	f000 f8ab 	bl	80211e4 <RVM_Vect_Init>
    RMP_Console_Ptr=0;
 802108e:	4b04      	ldr	r3, [pc, #16]	; (80210a0 <_RMP_Low_Level_Init+0x28>)
 8021090:	2200      	movs	r2, #0
 8021092:	601a      	str	r2, [r3, #0]
 8021094:	bd08      	pop	{r3, pc}
 8021096:	bf00      	nop
 8021098:	08021055 	.word	0x08021055
 802109c:	08020fd1 	.word	0x08020fd1
 80210a0:	2002855c 	.word	0x2002855c

080210a4 <_RMP_Plat_Hook>:
 80210a4:	f000 b8ac 	b.w	8021200 <RVM_Enable_Int>

080210a8 <RMP_Putchar>:
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
    /* If we are not changing lines, and buffer not full */
    if((Char!='\r')&&(Char!='\n')&&(Char!='\0')&&(RMP_Console_Ptr<RMP_KERNEL_DEBUG_MAX_STR-1))
 80210a8:	280d      	cmp	r0, #13
Input       : char Char - The character to print.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
 80210aa:	b510      	push	{r4, lr}
    /* If we are not changing lines, and buffer not full */
    if((Char!='\r')&&(Char!='\n')&&(Char!='\0')&&(RMP_Console_Ptr<RMP_KERNEL_DEBUG_MAX_STR-1))
 80210ac:	4c0e      	ldr	r4, [pc, #56]	; (80210e8 <RMP_Putchar+0x40>)
 80210ae:	d90f      	bls.n	80210d0 <RMP_Putchar+0x28>
 80210b0:	6823      	ldr	r3, [r4, #0]
 80210b2:	2b7e      	cmp	r3, #126	; 0x7e
 80210b4:	d912      	bls.n	80210dc <RMP_Putchar+0x34>
    {
        RVM_Console[RMP_Console_Ptr++]=Char;
    }
    else
    {
        RMP_Lock_Sched();
 80210b6:	f000 f9ed 	bl	8021494 <RMP_Lock_Sched>
        RVM_Console[RMP_Console_Ptr]='\0';
 80210ba:	6822      	ldr	r2, [r4, #0]
 80210bc:	2300      	movs	r3, #0
 80210be:	490b      	ldr	r1, [pc, #44]	; (80210ec <RMP_Putchar+0x44>)
        RMP_Console_Ptr=0;
 80210c0:	6023      	str	r3, [r4, #0]
        RVM_Console[RMP_Console_Ptr++]=Char;
    }
    else
    {
        RMP_Lock_Sched();
        RVM_Console[RMP_Console_Ptr]='\0';
 80210c2:	548b      	strb	r3, [r1, r2]
        RMP_Console_Ptr=0;
        RVM_Print();
 80210c4:	f000 f8da 	bl	802127c <RVM_Print>
        RMP_Unlock_Sched();
    }
}
 80210c8:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
    {
        RMP_Lock_Sched();
        RVM_Console[RMP_Console_Ptr]='\0';
        RMP_Console_Ptr=0;
        RVM_Print();
        RMP_Unlock_Sched();
 80210cc:	f000 b9f2 	b.w	80214b4 <RMP_Unlock_Sched>
Return      : None.
******************************************************************************/
void RMP_Putchar(char Char)
{
    /* If we are not changing lines, and buffer not full */
    if((Char!='\r')&&(Char!='\n')&&(Char!='\0')&&(RMP_Console_Ptr<RMP_KERNEL_DEBUG_MAX_STR-1))
 80210d0:	f242 4301 	movw	r3, #9217	; 0x2401
 80210d4:	40c3      	lsrs	r3, r0
 80210d6:	07db      	lsls	r3, r3, #31
 80210d8:	d4ed      	bmi.n	80210b6 <RMP_Putchar+0xe>
 80210da:	e7e9      	b.n	80210b0 <RMP_Putchar+0x8>
    {
        RVM_Console[RMP_Console_Ptr++]=Char;
 80210dc:	1c59      	adds	r1, r3, #1
 80210de:	4a03      	ldr	r2, [pc, #12]	; (80210ec <RMP_Putchar+0x44>)
 80210e0:	6021      	str	r1, [r4, #0]
 80210e2:	54d0      	strb	r0, [r2, r3]
 80210e4:	bd10      	pop	{r4, pc}
 80210e6:	bf00      	nop
 80210e8:	2002855c 	.word	0x2002855c
 80210ec:	20029188 	.word	0x20029188

080210f0 <_RMP_Yield>:
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Yield(void)
{
    RVM_Yield();
 80210f0:	f000 b8d4 	b.w	802129c <RVM_Yield>
 80210f4:	0000      	movs	r0, r0
	...

080210f8 <RMP_MSB_Get>:
 80210f8:	fab0 f180 	clz	r1, r0
 80210fc:	f04f 001f 	mov.w	r0, #31
 8021100:	eba0 0001 	sub.w	r0, r0, r1
 8021104:	4770      	bx	lr

08021106 <_RMP_Start>:
 8021106:	f1a1 0110 	sub.w	r1, r1, #16
 802110a:	468d      	mov	sp, r1
 802110c:	f3bf 8f6f 	isb	sy
 8021110:	4780      	blx	r0
 8021112:	bf00      	nop

08021114 <RVM_Int>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Int(void)
{
 8021114:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8021118:	4c1e      	ldr	r4, [pc, #120]	; (8021194 <RVM_Int+0x80>)
                }
                Int_Num=RVM_Get_Int();
            }
        }
        else
            RVM_Int_Pend=1;
 802111a:	2701      	movs	r7, #1
 802111c:	f8df a080 	ldr.w	sl, [pc, #128]	; 80211a0 <RVM_Int+0x8c>
 8021120:	4d1d      	ldr	r5, [pc, #116]	; (8021198 <RVM_Int+0x84>)
 8021122:	4e1e      	ldr	r6, [pc, #120]	; (802119c <RVM_Int+0x88>)
 8021124:	f8df 807c 	ldr.w	r8, [pc, #124]	; 80211a4 <RVM_Int+0x90>
{
    rvm_cnt_t Int_Num;
    
    while(1)
    {
        RVM_Int_Rcv();
 8021128:	f7fe ffb0 	bl	802008c <RVM_Int_Rcv>
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
 802112c:	6823      	ldr	r3, [r4, #0]
 802112e:	b36b      	cbz	r3, 802118c <RVM_Int+0x78>
        {
            RVM_Int_Pend=0;
 8021130:	f04f 0900 	mov.w	r9, #0
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 8021134:	6828      	ldr	r0, [r5, #0]
    {
        RVM_Int_Rcv();
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
        {
            RVM_Int_Pend=0;
 8021136:	f8ca 9000 	str.w	r9, [sl]
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 802113a:	2800      	cmp	r0, #0
 802113c:	d0f4      	beq.n	8021128 <RVM_Int+0x14>
            continue;
        
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
 802113e:	f7fe ff7c 	bl	802003a <RVM_MSB_Get>
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 8021142:	f1b0 0b00 	subs.w	fp, r0, #0
 8021146:	dbef      	blt.n	8021128 <RVM_Int+0x14>
        RVM_Fetch_And(&RVM_Flag[Count],~(((rvm_ptr_t)1)<<Pos));
 8021148:	fa07 f10b 	lsl.w	r1, r7, fp
 802114c:	4812      	ldr	r0, [pc, #72]	; (8021198 <RVM_Int+0x84>)
 802114e:	43c9      	mvns	r1, r1
 8021150:	f7fe ff7a 	bl	8020048 <RVM_Fetch_And>
            /* Look for interrupts to handle from the first */
            Int_Num=RVM_Get_Int();
            /* Handle the interrupt here - the interrupt is tail-chained */
            while(Int_Num>=0)
            {
                if(RVM_Vect[Int_Num]!=0)
 8021154:	f856 302b 	ldr.w	r3, [r6, fp, lsl #2]
 8021158:	b12b      	cbz	r3, 8021166 <RVM_Int+0x52>
                {
                    RVM_Int_Active=1;
                    ((void(*)(rvm_ptr_t))RVM_Vect[Int_Num])(Int_Num);
 802115a:	4658      	mov	r0, fp
            /* Handle the interrupt here - the interrupt is tail-chained */
            while(Int_Num>=0)
            {
                if(RVM_Vect[Int_Num]!=0)
                {
                    RVM_Int_Active=1;
 802115c:	f8c8 7000 	str.w	r7, [r8]
                    ((void(*)(rvm_ptr_t))RVM_Vect[Int_Num])(Int_Num);
 8021160:	4798      	blx	r3
                    RVM_Int_Active=0;
 8021162:	f8c8 9000 	str.w	r9, [r8]
{
    rvm_cnt_t Count;
    rvm_cnt_t Pos;
    
    /* See if interrupt enabled */
    if(RVM_Int_Enable==0)
 8021166:	6823      	ldr	r3, [r4, #0]
 8021168:	2b00      	cmp	r3, #0
 802116a:	d0dd      	beq.n	8021128 <RVM_Int+0x14>
    
    /* See which one is ready, and pick it */
    Pos=-1;
    for(Count=RVM_VECT_BITMAP-1;Count>=0;Count--)
    {
        if(RVM_Flag[Count]==0)
 802116c:	6828      	ldr	r0, [r5, #0]
 802116e:	2800      	cmp	r0, #0
 8021170:	d0da      	beq.n	8021128 <RVM_Int+0x14>
            continue;
        
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
 8021172:	f7fe ff62 	bl	802003a <RVM_MSB_Get>
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 8021176:	f1b0 0b00 	subs.w	fp, r0, #0
        RVM_Fetch_And(&RVM_Flag[Count],~(((rvm_ptr_t)1)<<Pos));
 802117a:	4807      	ldr	r0, [pc, #28]	; (8021198 <RVM_Int+0x84>)
 802117c:	fa07 f10b 	lsl.w	r1, r7, fp
        Pos=RVM_MSB_Get(RVM_Flag[Count]);
        Pos+=(Count<<RVM_WORD_ORDER);
        break;
    }
    /* Now kill the bit */
    if(Pos>=0)
 8021180:	dae5      	bge.n	802114e <RVM_Int+0x3a>
{
    rvm_cnt_t Int_Num;
    
    while(1)
    {
        RVM_Int_Rcv();
 8021182:	f7fe ff83 	bl	802008c <RVM_Int_Rcv>
        /* Only try to get interrupts if we didn't mask it */
        if(RVM_Int_Enable!=0)
 8021186:	6823      	ldr	r3, [r4, #0]
 8021188:	2b00      	cmp	r3, #0
 802118a:	d1d1      	bne.n	8021130 <RVM_Int+0x1c>
                }
                Int_Num=RVM_Get_Int();
            }
        }
        else
            RVM_Int_Pend=1;
 802118c:	f8ca 7000 	str.w	r7, [sl]
 8021190:	e7ca      	b.n	8021128 <RVM_Int+0x14>
 8021192:	bf00      	nop
 8021194:	20028564 	.word	0x20028564
 8021198:	200290f0 	.word	0x200290f0
 802119c:	20029108 	.word	0x20029108
 80211a0:	20028560 	.word	0x20028560
 80211a4:	20028568 	.word	0x20028568

080211a8 <RVM_Init>:
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80211a8:	2000      	movs	r0, #0
 80211aa:	4b09      	ldr	r3, [pc, #36]	; (80211d0 <RVM_Init+0x28>)
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Init(void)
{
 80211ac:	b4f0      	push	{r4, r5, r6, r7}
    RVM_Int_Active=0;
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
        RVM_Vect[Count]=0;
 80211ae:	4601      	mov	r1, r0
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80211b0:	4f08      	ldr	r7, [pc, #32]	; (80211d4 <RVM_Init+0x2c>)
    RVM_Int_Pend=0;
 80211b2:	4e09      	ldr	r6, [pc, #36]	; (80211d8 <RVM_Init+0x30>)
 80211b4:	f103 0280 	add.w	r2, r3, #128	; 0x80
    RVM_Int_Active=0;
 80211b8:	4d08      	ldr	r5, [pc, #32]	; (80211dc <RVM_Init+0x34>)
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
 80211ba:	4c09      	ldr	r4, [pc, #36]	; (80211e0 <RVM_Init+0x38>)
******************************************************************************/
void RVM_Init(void)
{
    rvm_cnt_t Count;
    /* Clear all VM-related flags and registration tables */
    RVM_Int_Enable=0;
 80211bc:	6038      	str	r0, [r7, #0]
    RVM_Int_Pend=0;
 80211be:	6030      	str	r0, [r6, #0]
    RVM_Int_Active=0;
 80211c0:	6028      	str	r0, [r5, #0]
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
 80211c2:	6020      	str	r0, [r4, #0]
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
        RVM_Vect[Count]=0;
 80211c4:	f843 1f04 	str.w	r1, [r3, #4]!
    RVM_Int_Pend=0;
    RVM_Int_Active=0;
    /* Clean up all global variables */
    for(Count=0;Count<RVM_VECT_BITMAP;Count++)
        RVM_Flag[Count]=0;
    for(Count=0;Count<RVM_MAX_INTVECT;Count++)
 80211c8:	4293      	cmp	r3, r2
 80211ca:	d1fb      	bne.n	80211c4 <RVM_Init+0x1c>
        RVM_Vect[Count]=0;
}
 80211cc:	bcf0      	pop	{r4, r5, r6, r7}
 80211ce:	4770      	bx	lr
 80211d0:	20029104 	.word	0x20029104
 80211d4:	20028564 	.word	0x20028564
 80211d8:	20028560 	.word	0x20028560
 80211dc:	20028568 	.word	0x20028568
 80211e0:	200290f0 	.word	0x200290f0

080211e4 <RVM_Vect_Init>:
Output      : None.
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
 80211e4:	281f      	cmp	r0, #31
Input       : None
Output      : None.
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
 80211e6:	4603      	mov	r3, r0
    if(Num>=RVM_MAX_INTVECT)
 80211e8:	d804      	bhi.n	80211f4 <RVM_Vect_Init+0x10>
        return -1;
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
 80211ea:	4a04      	ldr	r2, [pc, #16]	; (80211fc <RVM_Vect_Init+0x18>)
    return 0;
 80211ec:	2000      	movs	r0, #0
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
        return -1;
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
 80211ee:	f842 1023 	str.w	r1, [r2, r3, lsl #2]
    return 0;
 80211f2:	4770      	bx	lr
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Vect_Init(rvm_ptr_t Num, void* Handler)
{
    if(Num>=RVM_MAX_INTVECT)
        return -1;
 80211f4:	f04f 30ff 	mov.w	r0, #4294967295
    
    RVM_Vect[Num]=(rvm_ptr_t)Handler;
    return 0;
}
 80211f8:	4770      	bx	lr
 80211fa:	bf00      	nop
 80211fc:	20029108 	.word	0x20029108

08021200 <RVM_Enable_Int>:
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8021200:	4b05      	ldr	r3, [pc, #20]	; (8021218 <RVM_Enable_Int+0x18>)
 8021202:	2200      	movs	r2, #0
Return      : None.
******************************************************************************/
void RVM_Enable_Int(void)
{
    /* Must be successful */
    RVM_Int_Enable=1;
 8021204:	4905      	ldr	r1, [pc, #20]	; (802121c <RVM_Enable_Int+0x1c>)
 8021206:	2001      	movs	r0, #1
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8021208:	601a      	str	r2, [r3, #0]
Return      : None.
******************************************************************************/
void RVM_Enable_Int(void)
{
    /* Must be successful */
    RVM_Int_Enable=1;
 802120a:	6008      	str	r0, [r1, #0]
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
 802120c:	605a      	str	r2, [r3, #4]
    RVM_Param.Param[1]=Param2;
 802120e:	609a      	str	r2, [r3, #8]
    RVM_Param.Param[2]=Param3;
 8021210:	60da      	str	r2, [r3, #12]
    RVM_Param.Param[3]=Param4;
 8021212:	611a      	str	r2, [r3, #16]
    
    /* Do the hypercall */
    _RVM_Hypercall();
 8021214:	f7fe bf30 	b.w	8020078 <_RVM_Hypercall>
 8021218:	200290f4 	.word	0x200290f4
 802121c:	20028564 	.word	0x20028564

08021220 <RVM_Mask_Int>:
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Mask_Int(void)
{
    RVM_Int_Pend=0;
 8021220:	2300      	movs	r3, #0
 8021222:	4902      	ldr	r1, [pc, #8]	; (802122c <RVM_Mask_Int+0xc>)
    RVM_Int_Enable=0;
 8021224:	4a02      	ldr	r2, [pc, #8]	; (8021230 <RVM_Mask_Int+0x10>)
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Mask_Int(void)
{
    RVM_Int_Pend=0;
 8021226:	600b      	str	r3, [r1, #0]
    RVM_Int_Enable=0;
 8021228:	6013      	str	r3, [r2, #0]
 802122a:	4770      	bx	lr
 802122c:	20028560 	.word	0x20028560
 8021230:	20028564 	.word	0x20028564

08021234 <RVM_Unmask_Int>:
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 8021234:	4b05      	ldr	r3, [pc, #20]	; (802124c <RVM_Unmask_Int+0x18>)
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
 8021236:	2001      	movs	r0, #1
 8021238:	4905      	ldr	r1, [pc, #20]	; (8021250 <RVM_Unmask_Int+0x1c>)
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 802123a:	681a      	ldr	r2, [r3, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Unmask_Int(void)
{
    RVM_Int_Enable=1;
 802123c:	6008      	str	r0, [r1, #0]
    /* Trigger interrupt processing if there are pending ones */
    if(RVM_Int_Pend!=0)
 802123e:	b902      	cbnz	r2, 8021242 <RVM_Unmask_Int+0xe>
 8021240:	4770      	bx	lr
    {
        RVM_Int_Pend=0;
 8021242:	2200      	movs	r2, #0
 8021244:	601a      	str	r2, [r3, #0]
        _RVM_Yield();
 8021246:	f7fe bf2d 	b.w	80200a4 <_RVM_Yield>
 802124a:	bf00      	nop
 802124c:	20028560 	.word	0x20028560
 8021250:	20028564 	.word	0x20028564

08021254 <RVM_Reg_Int>:
Output      : None.
Return      : rvm_ret_t - If successful, the interrupt registration ID; else an error code.
******************************************************************************/
rvm_ret_t RVM_Reg_Int(rvm_ptr_t Vect_Num, rvm_ptr_t Int_Num)
{
    if(Vect_Num>=RVM_Img.Kcap_Num)
 8021254:	b960      	cbnz	r0, 8021270 <RVM_Reg_Int+0x1c>
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8021256:	2202      	movs	r2, #2
    RVM_Param.Param[0]=Param1;
 8021258:	231e      	movs	r3, #30
              rvm_ptr_t Int_Num - The virtual vector number.
Output      : None.
Return      : rvm_ret_t - If successful, the interrupt registration ID; else an error code.
******************************************************************************/
rvm_ret_t RVM_Reg_Int(rvm_ptr_t Vect_Num, rvm_ptr_t Int_Num)
{
 802125a:	b510      	push	{r4, lr}
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 802125c:	4c06      	ldr	r4, [pc, #24]	; (8021278 <RVM_Reg_Int+0x24>)
    RVM_Param.Param[0]=Param1;
    RVM_Param.Param[1]=Param2;
    RVM_Param.Param[2]=Param3;
 802125e:	60e0      	str	r0, [r4, #12]
    RVM_Param.Param[3]=Param4;
 8021260:	6120      	str	r0, [r4, #16]
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
    RVM_Param.Param[1]=Param2;
 8021262:	60a1      	str	r1, [r4, #8]
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
 8021264:	e884 000c 	stmia.w	r4, {r2, r3}
    RVM_Param.Param[1]=Param2;
    RVM_Param.Param[2]=Param3;
    RVM_Param.Param[3]=Param4;
    
    /* Do the hypercall */
    _RVM_Hypercall();
 8021268:	f7fe ff06 	bl	8020078 <_RVM_Hypercall>
    
    /* Return the result */
    return RVM_Param.Param[0];
 802126c:	6860      	ldr	r0, [r4, #4]
rvm_ret_t RVM_Reg_Int(rvm_ptr_t Vect_Num, rvm_ptr_t Int_Num)
{
    if(Vect_Num>=RVM_Img.Kcap_Num)
        return -1;
    
    return RVM_Hypercall(RVM_HYP_REGINT,RVM_Img.Kcap[Vect_Num],Int_Num,0,0);
 802126e:	bd10      	pop	{r4, pc}
Return      : rvm_ret_t - If successful, the interrupt registration ID; else an error code.
******************************************************************************/
rvm_ret_t RVM_Reg_Int(rvm_ptr_t Vect_Num, rvm_ptr_t Int_Num)
{
    if(Vect_Num>=RVM_Img.Kcap_Num)
        return -1;
 8021270:	f04f 30ff 	mov.w	r0, #4294967295
 8021274:	4770      	bx	lr
 8021276:	bf00      	nop
 8021278:	200290f4 	.word	0x200290f4

0802127c <RVM_Print>:
Input       : None.
Output      : None.
Return      : rvm_ret_t - If successful, 0; else -1.
******************************************************************************/
rvm_ret_t RVM_Print(void)
{
 802127c:	b510      	push	{r4, lr}
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
 802127e:	2300      	movs	r3, #0
Return      : None.
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
 8021280:	4c05      	ldr	r4, [pc, #20]	; (8021298 <RVM_Print+0x1c>)
    RVM_Param.Param[0]=Param1;
 8021282:	220b      	movs	r2, #11
    RVM_Param.Param[1]=Param2;
 8021284:	60a3      	str	r3, [r4, #8]
    RVM_Param.Param[2]=Param3;
 8021286:	60e3      	str	r3, [r4, #12]
    RVM_Param.Param[3]=Param4;
 8021288:	6123      	str	r3, [r4, #16]
******************************************************************************/
rvm_ret_t RVM_Hypercall(rvm_ptr_t Number, rvm_ptr_t Param1, rvm_ptr_t Param2, rvm_ptr_t Param3, rvm_ptr_t Param4)
{
    /* Pass the parameters */
    RVM_Param.Number=Number;
    RVM_Param.Param[0]=Param1;
 802128a:	e884 000c 	stmia.w	r4, {r2, r3}
    RVM_Param.Param[1]=Param2;
    RVM_Param.Param[2]=Param3;
    RVM_Param.Param[3]=Param4;
    
    /* Do the hypercall */
    _RVM_Hypercall();
 802128e:	f7fe fef3 	bl	8020078 <_RVM_Hypercall>
Return      : rvm_ret_t - If successful, 0; else -1.
******************************************************************************/
rvm_ret_t RVM_Print(void)
{
    return RVM_Hypercall(RVM_HYP_PRINT,0,0,0,0);
}
 8021292:	6860      	ldr	r0, [r4, #4]
 8021294:	bd10      	pop	{r4, pc}
 8021296:	bf00      	nop
 8021298:	200290f4 	.word	0x200290f4

0802129c <RVM_Yield>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RVM_Yield(void)
{
 802129c:	b508      	push	{r3, lr}
    RVM_Fetch_Or(RVM_Flag,0x02);
 802129e:	2102      	movs	r1, #2
 80212a0:	4808      	ldr	r0, [pc, #32]	; (80212c4 <RVM_Yield+0x28>)
 80212a2:	f7fe fedd 	bl	8020060 <RVM_Fetch_Or>
    
    /* We send a new trigger if the interrupt is not masked and we are not in interrupt */
    if(RVM_Int_Active==0)
 80212a6:	4b08      	ldr	r3, [pc, #32]	; (80212c8 <RVM_Yield+0x2c>)
 80212a8:	681b      	ldr	r3, [r3, #0]
 80212aa:	b92b      	cbnz	r3, 80212b8 <RVM_Yield+0x1c>
    {
        if(RVM_Int_Enable!=0)
 80212ac:	4b07      	ldr	r3, [pc, #28]	; (80212cc <RVM_Yield+0x30>)
 80212ae:	681b      	ldr	r3, [r3, #0]
 80212b0:	b91b      	cbnz	r3, 80212ba <RVM_Yield+0x1e>
            _RVM_Yield();
        else
            RVM_Int_Pend=1;
 80212b2:	4b07      	ldr	r3, [pc, #28]	; (80212d0 <RVM_Yield+0x34>)
 80212b4:	2201      	movs	r2, #1
 80212b6:	601a      	str	r2, [r3, #0]
 80212b8:	bd08      	pop	{r3, pc}
    }
}
 80212ba:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
    
    /* We send a new trigger if the interrupt is not masked and we are not in interrupt */
    if(RVM_Int_Active==0)
    {
        if(RVM_Int_Enable!=0)
            _RVM_Yield();
 80212be:	f7fe bef1 	b.w	80200a4 <_RVM_Yield>
 80212c2:	bf00      	nop
 80212c4:	200290f0 	.word	0x200290f0
 80212c8:	20028568 	.word	0x20028568
 80212cc:	20028564 	.word	0x20028564
 80212d0:	20028560 	.word	0x20028560

080212d4 <RVM_HW_Int_Enable>:
Output      : None.
Return      : rvm_ret_t - If successful, 0; else -1.
******************************************************************************/
rvm_ret_t RVM_HW_Int_Enable(rvm_ptr_t Int_ID)
{
    if(Int_ID>=RVM_Img.Kcap_Num)
 80212d4:	4602      	mov	r2, r0
 80212d6:	b918      	cbnz	r0, 80212e0 <RVM_HW_Int_Enable+0xc>
        return -1;
    
    return _RVM_Kern(Int_ID, RVM_CMX_KERN_INT(RVM_Img.Kcap[Int_ID]), RVM_CMX_INT_OP, RVM_CMX_INT_ENABLE);
 80212d8:	2301      	movs	r3, #1
 80212da:	211e      	movs	r1, #30
 80212dc:	f7fe beec 	b.w	80200b8 <_RVM_Kern>
}
 80212e0:	f04f 30ff 	mov.w	r0, #4294967295
 80212e4:	4770      	bx	lr
 80212e6:	bf00      	nop

080212e8 <RVM_HW_Int_Disable>:
Output      : None.
Return      : rvm_ret_t - If successful, 0; else -1.
******************************************************************************/
rvm_ret_t RVM_HW_Int_Disable(rvm_ptr_t Int_ID)
{
    if(Int_ID>=RVM_Img.Kcap_Num)
 80212e8:	4603      	mov	r3, r0
 80212ea:	b918      	cbnz	r0, 80212f4 <RVM_HW_Int_Disable+0xc>
        return -1;
    
    return _RVM_Kern(Int_ID, RVM_CMX_KERN_INT(RVM_Img.Kcap[Int_ID]), RVM_CMX_INT_OP, RVM_CMX_INT_DISABLE);
 80212ec:	4602      	mov	r2, r0
 80212ee:	211e      	movs	r1, #30
 80212f0:	f7fe bee2 	b.w	80200b8 <_RVM_Kern>
}
 80212f4:	f04f 30ff 	mov.w	r0, #4294967295
 80212f8:	4770      	bx	lr
 80212fa:	bf00      	nop

080212fc <RVM_HW_Int_Prio>:
Output      : None.
Return      : rvm_ret_t - If successful, 0; else -1.
******************************************************************************/
rvm_ret_t RVM_HW_Int_Prio(rvm_ptr_t Int_ID, rvm_ptr_t Prio)
{
    if(Int_ID>=RVM_Img.Kcap_Num)
 80212fc:	b920      	cbnz	r0, 8021308 <RVM_HW_Int_Prio+0xc>
        return -1;
    
    return _RVM_Kern(Int_ID, RVM_CMX_KERN_INT(RVM_Img.Kcap[Int_ID]), RVM_CMX_INT_PRIO, Prio);
 80212fe:	460b      	mov	r3, r1
 8021300:	2201      	movs	r2, #1
 8021302:	211e      	movs	r1, #30
 8021304:	f7fe bed8 	b.w	80200b8 <_RVM_Kern>
}
 8021308:	f04f 30ff 	mov.w	r0, #4294967295
 802130c:	4770      	bx	lr
 802130e:	bf00      	nop

08021310 <RMP_Clear>:
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 8021310:	2900      	cmp	r1, #0
              rmp_ptr_t Size - The size to clear.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Clear(volatile void* Addr, rmp_ptr_t Size)
{
 8021312:	b5f0      	push	{r4, r5, r6, r7, lr}
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 8021314:	dd3d      	ble.n	8021392 <RMP_Clear+0x82>
 8021316:	4243      	negs	r3, r0
 8021318:	f003 0303 	and.w	r3, r3, #3
 802131c:	428b      	cmp	r3, r1
 802131e:	bf28      	it	cs
 8021320:	460b      	movcs	r3, r1
 8021322:	2906      	cmp	r1, #6
 8021324:	d839      	bhi.n	802139a <RMP_Clear+0x8a>
 8021326:	460b      	mov	r3, r1
        Ptr[Count]=0;
 8021328:	2200      	movs	r2, #0
 802132a:	2b01      	cmp	r3, #1
 802132c:	7002      	strb	r2, [r0, #0]
 802132e:	d032      	beq.n	8021396 <RMP_Clear+0x86>
 8021330:	2b02      	cmp	r3, #2
 8021332:	7042      	strb	r2, [r0, #1]
 8021334:	d02f      	beq.n	8021396 <RMP_Clear+0x86>
 8021336:	2b03      	cmp	r3, #3
 8021338:	7082      	strb	r2, [r0, #2]
 802133a:	d02c      	beq.n	8021396 <RMP_Clear+0x86>
 802133c:	2b04      	cmp	r3, #4
 802133e:	70c2      	strb	r2, [r0, #3]
 8021340:	d029      	beq.n	8021396 <RMP_Clear+0x86>
 8021342:	2b06      	cmp	r3, #6
 8021344:	7102      	strb	r2, [r0, #4]
 8021346:	d12c      	bne.n	80213a2 <RMP_Clear+0x92>
 8021348:	7142      	strb	r2, [r0, #5]
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 802134a:	461a      	mov	r2, r3
 802134c:	4299      	cmp	r1, r3
 802134e:	d021      	beq.n	8021394 <RMP_Clear+0x84>
 8021350:	1ace      	subs	r6, r1, r3
 8021352:	1e4d      	subs	r5, r1, #1
 8021354:	1f34      	subs	r4, r6, #4
 8021356:	1aed      	subs	r5, r5, r3
 8021358:	08a4      	lsrs	r4, r4, #2
 802135a:	2d02      	cmp	r5, #2
 802135c:	f104 0401 	add.w	r4, r4, #1
 8021360:	ea4f 0784 	mov.w	r7, r4, lsl #2
 8021364:	d90a      	bls.n	802137c <RMP_Clear+0x6c>
 8021366:	2500      	movs	r5, #0
 8021368:	4403      	add	r3, r0
        Ptr[Count]=0;
 802136a:	46ae      	mov	lr, r5
 802136c:	3501      	adds	r5, #1
 802136e:	f843 eb04 	str.w	lr, [r3], #4
 8021372:	42ac      	cmp	r4, r5
 8021374:	d8fa      	bhi.n	802136c <RMP_Clear+0x5c>
 8021376:	42be      	cmp	r6, r7
 8021378:	443a      	add	r2, r7
 802137a:	d00a      	beq.n	8021392 <RMP_Clear+0x82>
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 802137c:	1c54      	adds	r4, r2, #1
        Ptr[Count]=0;
 802137e:	2300      	movs	r3, #0
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 8021380:	42a1      	cmp	r1, r4
        Ptr[Count]=0;
 8021382:	5483      	strb	r3, [r0, r2]
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 8021384:	dd05      	ble.n	8021392 <RMP_Clear+0x82>
 8021386:	3202      	adds	r2, #2
        Ptr[Count]=0;
 8021388:	5503      	strb	r3, [r0, r4]
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 802138a:	4291      	cmp	r1, r2
 802138c:	dd01      	ble.n	8021392 <RMP_Clear+0x82>
        Ptr[Count]=0;
 802138e:	5483      	strb	r3, [r0, r2]
 8021390:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8021392:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8021394:	bdf0      	pop	{r4, r5, r6, r7, pc}
{
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
 8021396:	461a      	mov	r2, r3
 8021398:	e7d8      	b.n	802134c <RMP_Clear+0x3c>
 802139a:	2b00      	cmp	r3, #0
 802139c:	d1c4      	bne.n	8021328 <RMP_Clear+0x18>
 802139e:	461a      	mov	r2, r3
 80213a0:	e7d6      	b.n	8021350 <RMP_Clear+0x40>
 80213a2:	2205      	movs	r2, #5
 80213a4:	e7d2      	b.n	802134c <RMP_Clear+0x3c>
 80213a6:	bf00      	nop

080213a8 <RMP_Print_Int>:
    rmp_cnt_t Count;
    rmp_cnt_t Num;
    rmp_ptr_t Div;
    
    /* how many digits are there? */
    if(Int==0)
 80213a8:	2800      	cmp	r0, #0
Input       : rmp_cnt_t Int - The integer to print.
Output      : None.
Return      : rmp_cnt_t - The length of the string printed.
******************************************************************************/
rmp_cnt_t RMP_Print_Int(rmp_cnt_t Int)
{
 80213aa:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
    rmp_cnt_t Count;
    rmp_cnt_t Num;
    rmp_ptr_t Div;
    
    /* how many digits are there? */
    if(Int==0)
 80213ae:	d052      	beq.n	8021456 <RMP_Print_Int+0xae>
    {
        RMP_Putchar('0');
        return 1;
    }
    else if(Int<0)
 80213b0:	db25      	blt.n	80213fe <RMP_Print_Int+0x56>
    else
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=Int;
 80213b2:	4605      	mov	r5, r0
    }
    else
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
 80213b4:	2401      	movs	r4, #1
        }
    }
    else
    {
        /* How many digits are there? */
        Count=0;
 80213b6:	2600      	movs	r6, #0
        Div=1;
        Iter=Int;
        while(Iter!=0)
        {
            Iter/=10;
 80213b8:	492d      	ldr	r1, [pc, #180]	; (8021470 <RMP_Print_Int+0xc8>)
    else
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=Int;
 80213ba:	4603      	mov	r3, r0
        while(Iter!=0)
        {
            Iter/=10;
 80213bc:	fba1 2303 	umull	r2, r3, r1, r3
            Count++;
            Div*=10;
 80213c0:	eb04 0484 	add.w	r4, r4, r4, lsl #2
        Div=1;
        Iter=Int;
        while(Iter!=0)
        {
            Iter/=10;
            Count++;
 80213c4:	3601      	adds	r6, #1
        Count=0;
        Div=1;
        Iter=Int;
        while(Iter!=0)
        {
            Iter/=10;
 80213c6:	4a2a      	ldr	r2, [pc, #168]	; (8021470 <RMP_Print_Int+0xc8>)
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=Int;
        while(Iter!=0)
 80213c8:	08db      	lsrs	r3, r3, #3
        {
            Iter/=10;
            Count++;
            Div*=10;
 80213ca:	ea4f 0444 	mov.w	r4, r4, lsl #1
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=Int;
        while(Iter!=0)
 80213ce:	d1f5      	bne.n	80213bc <RMP_Print_Int+0x14>
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
 80213d0:	fba2 3404 	umull	r3, r4, r2, r4
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
            Iter=Iter%Div;
            Div/=10;
 80213d4:	4690      	mov	r8, r2
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
 80213d6:	4637      	mov	r7, r6
 80213d8:	08e4      	lsrs	r4, r4, #3
        Iter=Int;
        Num=Count;
        
        while(Count>0)
        {
            Count--;
 80213da:	3f01      	subs	r7, #1
            RMP_Putchar(Iter/Div+'0');
 80213dc:	fbb5 f3f4 	udiv	r3, r5, r4
 80213e0:	f103 0030 	add.w	r0, r3, #48	; 0x30
            Iter=Iter%Div;
 80213e4:	fb04 5513 	mls	r5, r4, r3, r5
            Div/=10;
 80213e8:	fba8 3404 	umull	r3, r4, r8, r4
        Num=Count;
        
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
 80213ec:	b2c0      	uxtb	r0, r0
 80213ee:	f7ff fe5b 	bl	80210a8 <RMP_Putchar>
            Iter=Iter%Div;
            Div/=10;
 80213f2:	08e4      	lsrs	r4, r4, #3
        Div/=10;
        
        Iter=Int;
        Num=Count;
        
        while(Count>0)
 80213f4:	2f00      	cmp	r7, #0
 80213f6:	d1f0      	bne.n	80213da <RMP_Print_Int+0x32>
 80213f8:	4630      	mov	r0, r6
 80213fa:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    else if(Int<0)
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=(rmp_ptr_t)(-Int);
 80213fe:	4245      	negs	r5, r0
        while(Iter!=0)
 8021400:	b37d      	cbz	r5, 8021462 <RMP_Print_Int+0xba>
 8021402:	462b      	mov	r3, r5
 8021404:	2401      	movs	r4, #1
 8021406:	2000      	movs	r0, #0
        {
            Iter/=10;
 8021408:	4919      	ldr	r1, [pc, #100]	; (8021470 <RMP_Print_Int+0xc8>)
 802140a:	e000      	b.n	802140e <RMP_Print_Int+0x66>
            Count++;
 802140c:	4638      	mov	r0, r7
        Count=0;
        Div=1;
        Iter=(rmp_ptr_t)(-Int);
        while(Iter!=0)
        {
            Iter/=10;
 802140e:	fba1 2303 	umull	r2, r3, r1, r3
            Count++;
            Div*=10;
 8021412:	eb04 0484 	add.w	r4, r4, r4, lsl #2
        Count=0;
        Div=1;
        Iter=(rmp_ptr_t)(-Int);
        while(Iter!=0)
        {
            Iter/=10;
 8021416:	4a16      	ldr	r2, [pc, #88]	; (8021470 <RMP_Print_Int+0xc8>)
            Count++;
 8021418:	1c47      	adds	r7, r0, #1
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=(rmp_ptr_t)(-Int);
        while(Iter!=0)
 802141a:	08db      	lsrs	r3, r3, #3
        {
            Iter/=10;
            Count++;
            Div*=10;
 802141c:	ea4f 0444 	mov.w	r4, r4, lsl #1
    {
        /* How many digits are there? */
        Count=0;
        Div=1;
        Iter=(rmp_ptr_t)(-Int);
        while(Iter!=0)
 8021420:	d1f4      	bne.n	802140c <RMP_Print_Int+0x64>
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
 8021422:	fba2 3404 	umull	r3, r4, r2, r4
        
        RMP_Putchar('-');
        Iter=-Int;
        Num=Count+1;
 8021426:	1c86      	adds	r6, r0, #2
            Count++;
            Div*=10;
        }
        Div/=10;
        
        RMP_Putchar('-');
 8021428:	202d      	movs	r0, #45	; 0x2d
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
            Iter=Iter%Div;
            Div/=10;
 802142a:	4690      	mov	r8, r2
        {
            Iter/=10;
            Count++;
            Div*=10;
        }
        Div/=10;
 802142c:	08e4      	lsrs	r4, r4, #3
        
        RMP_Putchar('-');
 802142e:	f7ff fe3b 	bl	80210a8 <RMP_Putchar>
        Iter=-Int;
        Num=Count+1;
        
        while(Count>0)
        {
            Count--;
 8021432:	3f01      	subs	r7, #1
            RMP_Putchar(Iter/Div+'0');
 8021434:	fbb5 f3f4 	udiv	r3, r5, r4
 8021438:	f103 0030 	add.w	r0, r3, #48	; 0x30
            Iter=Iter%Div;
 802143c:	fb04 5513 	mls	r5, r4, r3, r5
            Div/=10;
 8021440:	fba8 3404 	umull	r3, r4, r8, r4
        Num=Count+1;
        
        while(Count>0)
        {
            Count--;
            RMP_Putchar(Iter/Div+'0');
 8021444:	b2c0      	uxtb	r0, r0
 8021446:	f7ff fe2f 	bl	80210a8 <RMP_Putchar>
            Iter=Iter%Div;
            Div/=10;
 802144a:	08e4      	lsrs	r4, r4, #3
        
        RMP_Putchar('-');
        Iter=-Int;
        Num=Count+1;
        
        while(Count>0)
 802144c:	2f00      	cmp	r7, #0
 802144e:	d1f0      	bne.n	8021432 <RMP_Print_Int+0x8a>
 8021450:	4630      	mov	r0, r6
 8021452:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    rmp_ptr_t Div;
    
    /* how many digits are there? */
    if(Int==0)
    {
        RMP_Putchar('0');
 8021456:	2030      	movs	r0, #48	; 0x30
 8021458:	f7ff fe26 	bl	80210a8 <RMP_Putchar>
        return 1;
 802145c:	2001      	movs	r0, #1
 802145e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
            Count++;
            Div*=10;
        }
        Div/=10;
        
        RMP_Putchar('-');
 8021462:	202d      	movs	r0, #45	; 0x2d
 8021464:	f7ff fe20 	bl	80210a8 <RMP_Putchar>
        Iter=-Int;
        Num=Count+1;
 8021468:	2001      	movs	r0, #1
            Div/=10;
        }
    }
    
    return Num;
}
 802146a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 802146e:	bf00      	nop
 8021470:	cccccccd 	.word	0xcccccccd

08021474 <RMP_Print_String>:
Input       : rmp_s8_t* String - The string to print
Output      : None.
Return      : rmp_cnt_t - The length of the string printed, the '\0' is not included.
******************************************************************************/
rmp_cnt_t RMP_Print_String(rmp_s8_t* String)
{
 8021474:	b538      	push	{r3, r4, r5, lr}
 8021476:	1e45      	subs	r5, r0, #1
    rmp_cnt_t Count;
    
    Count=0;
 8021478:	2400      	movs	r4, #0
 802147a:	e004      	b.n	8021486 <RMP_Print_String+0x12>
    while(Count<RMP_KERNEL_DEBUG_MAX_STR)
    {
        if(String[Count]=='\0')
            break;
        
        RMP_Putchar(String[Count++]);
 802147c:	3401      	adds	r4, #1
 802147e:	f7ff fe13 	bl	80210a8 <RMP_Putchar>
rmp_cnt_t RMP_Print_String(rmp_s8_t* String)
{
    rmp_cnt_t Count;
    
    Count=0;
    while(Count<RMP_KERNEL_DEBUG_MAX_STR)
 8021482:	2c80      	cmp	r4, #128	; 0x80
 8021484:	d004      	beq.n	8021490 <RMP_Print_String+0x1c>
    {
        if(String[Count]=='\0')
 8021486:	f915 3f01 	ldrsb.w	r3, [r5, #1]!
            break;
        
        RMP_Putchar(String[Count++]);
 802148a:	b2d8      	uxtb	r0, r3
    rmp_cnt_t Count;
    
    Count=0;
    while(Count<RMP_KERNEL_DEBUG_MAX_STR)
    {
        if(String[Count]=='\0')
 802148c:	2b00      	cmp	r3, #0
 802148e:	d1f5      	bne.n	802147c <RMP_Print_String+0x8>
        
        RMP_Putchar(String[Count++]);
    }
    
    return Count;
}
 8021490:	4620      	mov	r0, r4
 8021492:	bd38      	pop	{r3, r4, r5, pc}

08021494 <RMP_Lock_Sched>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
 8021494:	b508      	push	{r3, lr}
    RMP_MASK_INT();
 8021496:	f7ff fec3 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 802149a:	4b04      	ldr	r3, [pc, #16]	; (80214ac <RMP_Lock_Sched+0x18>)
 802149c:	2101      	movs	r1, #1
    RMP_Sched_Lock_Cnt++;
 802149e:	4a04      	ldr	r2, [pc, #16]	; (80214b0 <RMP_Lock_Sched+0x1c>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 80214a0:	6019      	str	r1, [r3, #0]
    RMP_Sched_Lock_Cnt++;
 80214a2:	6813      	ldr	r3, [r2, #0]
 80214a4:	440b      	add	r3, r1
 80214a6:	6013      	str	r3, [r2, #0]
 80214a8:	bd08      	pop	{r3, pc}
 80214aa:	bf00      	nop
 80214ac:	200287c0 	.word	0x200287c0
 80214b0:	20028774 	.word	0x20028774

080214b4 <RMP_Unlock_Sched>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
 80214b4:	b508      	push	{r3, lr}
    if(RMP_Sched_Lock_Cnt==1)
 80214b6:	4b0f      	ldr	r3, [pc, #60]	; (80214f4 <RMP_Unlock_Sched+0x40>)
 80214b8:	681a      	ldr	r2, [r3, #0]
 80214ba:	2a01      	cmp	r2, #1
 80214bc:	d006      	beq.n	80214cc <RMP_Unlock_Sched+0x18>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80214be:	681a      	ldr	r2, [r3, #0]
 80214c0:	2a01      	cmp	r2, #1
 80214c2:	d90e      	bls.n	80214e2 <RMP_Unlock_Sched+0x2e>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80214c4:	681a      	ldr	r2, [r3, #0]
 80214c6:	3a01      	subs	r2, #1
 80214c8:	601a      	str	r2, [r3, #0]
 80214ca:	bd08      	pop	{r3, pc}
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80214cc:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
 80214ce:	480a      	ldr	r0, [pc, #40]	; (80214f8 <RMP_Unlock_Sched+0x44>)
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80214d0:	490a      	ldr	r1, [pc, #40]	; (80214fc <RMP_Unlock_Sched+0x48>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80214d2:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80214d4:	6002      	str	r2, [r0, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80214d6:	680b      	ldr	r3, [r1, #0]
 80214d8:	b923      	cbnz	r3, 80214e4 <RMP_Unlock_Sched+0x30>
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
        while(1);
}
 80214da:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80214de:	f7ff bea9 	b.w	8021234 <RVM_Unmask_Int>
 80214e2:	e7fe      	b.n	80214e2 <RMP_Unlock_Sched+0x2e>
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 80214e4:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 80214e6:	f7ff fe03 	bl	80210f0 <_RMP_Yield>
        RMP_Sched_Lock_Cnt--;
    }
    /* Trying to unlock a scheduler that is not locked - should never happen */
    else
        while(1);
}
 80214ea:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80214ee:	f7ff bea1 	b.w	8021234 <RVM_Unmask_Int>
 80214f2:	bf00      	nop
 80214f4:	20028774 	.word	0x20028774
 80214f8:	200287c0 	.word	0x200287c0
 80214fc:	20028670 	.word	0x20028670

08021500 <RMP_Init>:
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
void RMP_Init(void)
{
 8021500:	b508      	push	{r3, lr}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 8021502:	f7ff fe8d 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021506:	2101      	movs	r1, #1
    RMP_Sched_Lock_Cnt++;
 8021508:	4a07      	ldr	r2, [pc, #28]	; (8021528 <RMP_Init+0x28>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 802150a:	4b08      	ldr	r3, [pc, #32]	; (802152c <RMP_Init+0x2c>)
 802150c:	6019      	str	r1, [r3, #0]
    RMP_Sched_Lock_Cnt++;
 802150e:	6813      	ldr	r3, [r2, #0]
 8021510:	440b      	add	r3, r1
 8021512:	6013      	str	r3, [r2, #0]
void RMP_Init(void)
{
    RMP_Lock_Sched();
    
    /* Platform will use this hook to do something */
    _RMP_Plat_Hook();
 8021514:	f7ff fdc6 	bl	80210a4 <_RMP_Plat_Hook>
    /* Start the second thread here */
    RMP_Init_Hook();
 8021518:	f7ff fcee 	bl	8020ef8 <RMP_Init_Hook>
    
    RMP_Unlock_Sched();
 802151c:	f7ff ffca 	bl	80214b4 <RMP_Unlock_Sched>
    
    while(1)
        RMP_Init_Idle();
 8021520:	f7ff fd54 	bl	8020fcc <RMP_Init_Idle>
 8021524:	e7fc      	b.n	8021520 <RMP_Init+0x20>
 8021526:	bf00      	nop
 8021528:	20028774 	.word	0x20028774
 802152c:	200287c0 	.word	0x200287c0

08021530 <RMP_Yield>:
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Yield(void)
{
    if(RMP_Sched_Locked==0)
 8021530:	4b04      	ldr	r3, [pc, #16]	; (8021544 <RMP_Yield+0x14>)
 8021532:	681b      	ldr	r3, [r3, #0]
 8021534:	b11b      	cbz	r3, 802153e <RMP_Yield+0xe>
        _RMP_Yield();
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
 8021536:	4b04      	ldr	r3, [pc, #16]	; (8021548 <RMP_Yield+0x18>)
 8021538:	2201      	movs	r2, #1
 802153a:	601a      	str	r2, [r3, #0]
 802153c:	4770      	bx	lr
    if(RMP_Sched_Locked==0)
    {
        RMP_COVERAGE_MARKER();
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        _RMP_Yield();
 802153e:	f7ff bdd7 	b.w	80210f0 <_RMP_Yield>
 8021542:	bf00      	nop
 8021544:	200287c0 	.word	0x200287c0
 8021548:	20028670 	.word	0x20028670

0802154c <_RMP_Get_High_Rdy>:
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Get_High_Rdy(void)
{
 802154c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
    
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
 802154e:	4a1c      	ldr	r2, [pc, #112]	; (80215c0 <_RMP_Get_High_Rdy+0x74>)
 8021550:	2600      	movs	r6, #0
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 8021552:	4c1c      	ldr	r4, [pc, #112]	; (80215c4 <_RMP_Get_High_Rdy+0x78>)
 8021554:	4d1c      	ldr	r5, [pc, #112]	; (80215c8 <_RMP_Get_High_Rdy+0x7c>)
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 8021556:	4b1d      	ldr	r3, [pc, #116]	; (80215cc <_RMP_Get_High_Rdy+0x80>)
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 8021558:	6821      	ldr	r1, [r4, #0]
 802155a:	6828      	ldr	r0, [r5, #0]
    
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
 802155c:	6016      	str	r6, [r2, #0]
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 802155e:	681a      	ldr	r2, [r3, #0]
void _RMP_Get_High_Rdy(void)
{
    rmp_cnt_t Count;
    
    /* Write the SP value to thread structure */
    RMP_Cur_Thd->Stack=RMP_Cur_SP;
 8021560:	6348      	str	r0, [r1, #52]	; 0x34
    /* No need to detect scheduler locks - if this interrupt can be entered, the scheduler is not locked */
    RMP_Sched_Pend=0;
    /* See which one is ready, and pick it */
    for(Count=RMP_BITMAP_SIZE-1;Count>=0;Count--)
    {
        if(RMP_Bitmap[Count]==0)
 8021562:	b91a      	cbnz	r2, 802156c <_RMP_Get_High_Rdy+0x20>
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
 8021564:	6823      	ldr	r3, [r4, #0]
 8021566:	6b5b      	ldr	r3, [r3, #52]	; 0x34
 8021568:	602b      	str	r3, [r5, #0]
 802156a:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
            continue;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Count=RMP_MSB_Get(RMP_Bitmap[Count])+(Count<<RMP_WORD_ORDER);
 802156c:	6818      	ldr	r0, [r3, #0]
 802156e:	f7ff fdc3 	bl	80210f8 <RMP_MSB_Get>
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
 8021572:	4b17      	ldr	r3, [pc, #92]	; (80215d0 <_RMP_Get_High_Rdy+0x84>)
 8021574:	00c0      	lsls	r0, r0, #3
 8021576:	181a      	adds	r2, r3, r0
 8021578:	6851      	ldr	r1, [r2, #4]
 802157a:	6822      	ldr	r2, [r4, #0]
 802157c:	4291      	cmp	r1, r2
 802157e:	d00a      	beq.n	8021596 <_RMP_Get_High_Rdy+0x4a>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
 8021580:	6822      	ldr	r2, [r4, #0]
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
 8021582:	4418      	add	r0, r3
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Replenish timeslices */
        RMP_Cur_Thd->Slices_Left=RMP_Cur_Thd->Slices;
 8021584:	6823      	ldr	r3, [r4, #0]
 8021586:	69db      	ldr	r3, [r3, #28]
 8021588:	6213      	str	r3, [r2, #32]
        RMP_Cur_Thd=(struct RMP_Thd*)(RMP_Run[Count].Next);
 802158a:	6843      	ldr	r3, [r0, #4]
 802158c:	6023      	str	r3, [r4, #0]
        break;
    }
    
    /* Load the SP value from thread structure */
    RMP_Cur_SP=RMP_Cur_Thd->Stack;
 802158e:	6823      	ldr	r3, [r4, #0]
 8021590:	6b5b      	ldr	r3, [r3, #52]	; 0x34
 8021592:	602b      	str	r3, [r5, #0]
 8021594:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
        
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
 8021596:	6822      	ldr	r2, [r4, #0]
 8021598:	6821      	ldr	r1, [r4, #0]
 802159a:	6812      	ldr	r2, [r2, #0]
 802159c:	6849      	ldr	r1, [r1, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802159e:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 80215a0:	6051      	str	r1, [r2, #4]
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 80215a2:	6822      	ldr	r2, [r4, #0]
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
 80215a4:	6821      	ldr	r1, [r4, #0]
 80215a6:	6a49      	ldr	r1, [r1, #36]	; 0x24
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 80215a8:	f853 1031 	ldr.w	r1, [r3, r1, lsl #3]
                         RMP_Run[RMP_Cur_Thd->Prio].Prev,
                         &(RMP_Run[RMP_Cur_Thd->Prio]));
 80215ac:	6826      	ldr	r6, [r4, #0]
 80215ae:	6a76      	ldr	r6, [r6, #36]	; 0x24
        /* See if the current thread and the next thread are the same. If yes, place the current at the end of the queue */
        if(RMP_Cur_Thd==(struct RMP_Thd*)(RMP_Run[Count].Next))
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(RMP_Cur_Thd->Run_Head.Prev, RMP_Cur_Thd->Run_Head.Next);
            RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),
 80215b0:	eb03 07c6 	add.w	r7, r3, r6, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 80215b4:	f843 2036 	str.w	r2, [r3, r6, lsl #3]
    New->Next=(struct RMP_List*)Next;
 80215b8:	6057      	str	r7, [r2, #4]
    New->Prev=(struct RMP_List*)Prev;
 80215ba:	6011      	str	r1, [r2, #0]
    Prev->Next=(struct RMP_List*)New;
 80215bc:	604a      	str	r2, [r1, #4]
 80215be:	e7df      	b.n	8021580 <_RMP_Get_High_Rdy+0x34>
 80215c0:	20028670 	.word	0x20028670
 80215c4:	2002920c 	.word	0x2002920c
 80215c8:	20029208 	.word	0x20029208
 80215cc:	2002866c 	.word	0x2002866c
 80215d0:	20028674 	.word	0x20028674

080215d4 <_RMP_Tick_Handler>:
Input       : rmp_ptr_t Ticks - How many ticks have passed.
Output      : None.
Return      : None.
******************************************************************************/
void _RMP_Tick_Handler(rmp_ptr_t Ticks)
{
 80215d4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
 80215d8:	4d44      	ldr	r5, [pc, #272]	; (80216ec <_RMP_Tick_Handler+0x118>)
    
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>RMP_Cur_Thd->Slices_Left)
 80215da:	4e45      	ldr	r6, [pc, #276]	; (80216f0 <_RMP_Tick_Handler+0x11c>)
******************************************************************************/
void _RMP_Tick_Handler(rmp_ptr_t Ticks)
{
    struct RMP_Thd* Thread;
    /* Increase the timestamp as always */
    RMP_Tick+=Ticks;
 80215dc:	682b      	ldr	r3, [r5, #0]
 80215de:	4403      	add	r3, r0
 80215e0:	602b      	str	r3, [r5, #0]
    
    /* See if the current thread expired. If yes, trigger a scheduler event */
    if(Ticks>RMP_Cur_Thd->Slices_Left)
 80215e2:	6833      	ldr	r3, [r6, #0]
 80215e4:	6a1b      	ldr	r3, [r3, #32]
 80215e6:	4298      	cmp	r0, r3
 80215e8:	d973      	bls.n	80216d2 <_RMP_Tick_Handler+0xfe>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=1;
 80215ea:	4c42      	ldr	r4, [pc, #264]	; (80216f4 <_RMP_Tick_Handler+0x120>)
 80215ec:	2301      	movs	r3, #1
 80215ee:	6023      	str	r3, [r4, #0]
        RMP_COVERAGE_MARKER();
        RMP_Cur_Thd->Slices_Left-=Ticks;
    }
    
    /* Check if there are any timer events */
    if((&RMP_Delay)!=RMP_Delay.Next)
 80215f0:	4a41      	ldr	r2, [pc, #260]	; (80216f8 <_RMP_Tick_Handler+0x124>)
 80215f2:	6853      	ldr	r3, [r2, #4]
 80215f4:	4293      	cmp	r3, r2
 80215f6:	d067      	beq.n	80216c8 <_RMP_Tick_Handler+0xf4>
    {
        RMP_COVERAGE_MARKER();
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 80215f8:	6851      	ldr	r1, [r2, #4]
        /* If the value is less than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)<=(RMP_ALLBITS>>1))
 80215fa:	682b      	ldr	r3, [r5, #0]
 80215fc:	6a09      	ldr	r1, [r1, #32]
 80215fe:	1a5b      	subs	r3, r3, r1
 8021600:	2b00      	cmp	r3, #0
 8021602:	db61      	blt.n	80216c8 <_RMP_Tick_Handler+0xf4>
void _RMP_Timer_Proc(void)
{
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
 8021604:	4692      	mov	sl, r2
            }
            
            case RMP_THD_RCVDLY:
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
 8021606:	f06f 0c04 	mvn.w	ip, #4
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 802160a:	f8df e0f4 	ldr.w	lr, [pc, #244]	; 8021700 <_RMP_Tick_Handler+0x12c>
 802160e:	f04f 0901 	mov.w	r9, #1
void _RMP_Timer_Proc(void)
{
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
 8021612:	6853      	ldr	r3, [r2, #4]
 8021614:	4293      	cmp	r3, r2
 8021616:	d057      	beq.n	80216c8 <_RMP_Tick_Handler+0xf4>
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 8021618:	f8da 7004 	ldr.w	r7, [sl, #4]
        /* If the value is more than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)>(RMP_ALLBITS>>1))
 802161c:	6829      	ldr	r1, [r5, #0]
    struct RMP_Thd* Thread;
    
    /* Process the timer events, if there are any of them */
    while((&RMP_Delay)!=RMP_Delay.Next)
    {
        Thread=RMP_DLY2THD(RMP_Delay.Next);
 802161e:	f1a7 0308 	sub.w	r3, r7, #8
        /* If the value is more than this, then it means that the time have
         * already passed and we have to process this */
        if((RMP_Tick-Thread->Timeout)>(RMP_ALLBITS>>1))
 8021622:	6a98      	ldr	r0, [r3, #40]	; 0x28
 8021624:	1a09      	subs	r1, r1, r0
 8021626:	2900      	cmp	r1, #0
 8021628:	db4e      	blt.n	80216c8 <_RMP_Tick_Handler+0xf4>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 802162a:	6899      	ldr	r1, [r3, #8]
 802162c:	f8d3 800c 	ldr.w	r8, [r3, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021630:	f8c8 1000 	str.w	r1, [r8]
    Prev->Next=(struct RMP_List*)Next;
 8021634:	f8c1 8004 	str.w	r8, [r1, #4]
        else
            RMP_COVERAGE_MARKER();
        
        /* This thread should be processed */
        RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        switch(RMP_THD_STATE(Thread->State))
 8021638:	6999      	ldr	r1, [r3, #24]
 802163a:	fa5f f881 	uxtb.w	r8, r1
 802163e:	f1a8 0803 	sub.w	r8, r8, #3
 8021642:	f1b8 0f05 	cmp.w	r8, #5
 8021646:	d83e      	bhi.n	80216c6 <_RMP_Tick_Handler+0xf2>
 8021648:	e8df f008 	tbb	[pc, r8]
 802164c:	0e3a3d03 	.word	0x0e3a3d03
 8021650:	033d      	.short	0x033d
        {
            case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
            case RMP_THD_SEMDLY:
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8021652:	f857 1c08 	ldr.w	r1, [r7, #-8]
 8021656:	f8d3 8004 	ldr.w	r8, [r3, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802165a:	f8c8 1000 	str.w	r1, [r8]
    Prev->Next=(struct RMP_List*)Next;
 802165e:	f8c1 8004 	str.w	r8, [r1, #4]
 8021662:	6999      	ldr	r1, [r3, #24]
            case RMP_THD_SNDDLY:RMP_COVERAGE_MARKER();
            case RMP_THD_SEMDLY:
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
                Thread->Retval=RMP_ERR_OPER;
 8021664:	f8c3 c038 	str.w	ip, [r3, #56]	; 0x38
            case RMP_THD_DELAYED:RMP_COVERAGE_MARKER();break;
            /* Should not get here */
            default:while(1);
        }

        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 8021668:	f021 01ff 	bic.w	r1, r1, #255	; 0xff
 802166c:	f041 0101 	orr.w	r1, r1, #1
 8021670:	6199      	str	r1, [r3, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021672:	6999      	ldr	r1, [r3, #24]
 8021674:	05c9      	lsls	r1, r1, #23
 8021676:	d4cc      	bmi.n	8021612 <_RMP_Tick_Handler+0x3e>
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021678:	6a59      	ldr	r1, [r3, #36]	; 0x24
 802167a:	4820      	ldr	r0, [pc, #128]	; (80216fc <_RMP_Tick_Handler+0x128>)
 802167c:	f850 1031 	ldr.w	r1, [r0, r1, lsl #3]
 8021680:	f8d3 8024 	ldr.w	r8, [r3, #36]	; 0x24
 8021684:	eb00 0bc8 	add.w	fp, r0, r8, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021688:	f840 3038 	str.w	r3, [r0, r8, lsl #3]
    New->Next=(struct RMP_List*)Next;
 802168c:	f8c3 b004 	str.w	fp, [r3, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021690:	f847 1c08 	str.w	r1, [r7, #-8]
    Prev->Next=(struct RMP_List*)New;
 8021694:	604b      	str	r3, [r1, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021696:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8021698:	6a5f      	ldr	r7, [r3, #36]	; 0x24
 802169a:	f001 011f 	and.w	r1, r1, #31
 802169e:	097f      	lsrs	r7, r7, #5
 80216a0:	fa09 f101 	lsl.w	r1, r9, r1
 80216a4:	f85e 8027 	ldr.w	r8, [lr, r7, lsl #2]
 80216a8:	ea41 0108 	orr.w	r1, r1, r8
 80216ac:	f84e 1027 	str.w	r1, [lr, r7, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 80216b0:	6a59      	ldr	r1, [r3, #36]	; 0x24
 80216b2:	6833      	ldr	r3, [r6, #0]
 80216b4:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 80216b6:	4299      	cmp	r1, r3
 80216b8:	d9ab      	bls.n	8021612 <_RMP_Tick_Handler+0x3e>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 80216ba:	2301      	movs	r3, #1
 80216bc:	6023      	str	r3, [r4, #0]
 80216be:	e7a6      	b.n	802160e <_RMP_Tick_Handler+0x3a>
            }
            
            case RMP_THD_RCVDLY:
            {
                RMP_COVERAGE_MARKER();
                Thread->Retval=RMP_ERR_OPER;
 80216c0:	f8c3 c038 	str.w	ip, [r3, #56]	; 0x38
 80216c4:	e7d0      	b.n	8021668 <_RMP_Tick_Handler+0x94>
 80216c6:	e7fe      	b.n	80216c6 <_RMP_Tick_Handler+0xf2>
            RMP_COVERAGE_MARKER();
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(RMP_Sched_Pend!=0)
 80216c8:	6822      	ldr	r2, [r4, #0]
 80216ca:	4b0a      	ldr	r3, [pc, #40]	; (80216f4 <_RMP_Tick_Handler+0x120>)
 80216cc:	b93a      	cbnz	r2, 80216de <_RMP_Tick_Handler+0x10a>
 80216ce:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
        RMP_Sched_Pend=1;
    }
    else
    {
        RMP_COVERAGE_MARKER();
        RMP_Cur_Thd->Slices_Left-=Ticks;
 80216d2:	6832      	ldr	r2, [r6, #0]
 80216d4:	4c07      	ldr	r4, [pc, #28]	; (80216f4 <_RMP_Tick_Handler+0x120>)
 80216d6:	6a13      	ldr	r3, [r2, #32]
 80216d8:	1a1b      	subs	r3, r3, r0
 80216da:	6213      	str	r3, [r2, #32]
 80216dc:	e788      	b.n	80215f0 <_RMP_Tick_Handler+0x1c>
        RMP_COVERAGE_MARKER();
    
    if(RMP_Sched_Pend!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=0;
 80216de:	2200      	movs	r2, #0
 80216e0:	601a      	str	r2, [r3, #0]
        RMP_COVERAGE_MARKER();
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Tick_Hook(Ticks);
#endif
}
 80216e2:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
    
    if(RMP_Sched_Pend!=0)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Pend=0;
        _RMP_Yield();
 80216e6:	f7ff bd03 	b.w	80210f0 <_RMP_Yield>
 80216ea:	bf00      	nop
 80216ec:	20029210 	.word	0x20029210
 80216f0:	2002920c 	.word	0x2002920c
 80216f4:	20028670 	.word	0x20028670
 80216f8:	200287b8 	.word	0x200287b8
 80216fc:	20028674 	.word	0x20028674
 8021700:	2002866c 	.word	0x2002866c

08021704 <RMP_Thd_Crt>:
              rmp_ptr_t Slices - The number of timeslices to assign to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, void* Entry, void* Stack, void* Arg, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
 8021704:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    /* Check if the priority and timeslice range is correct */
    if(Prio>=RMP_MAX_PREEMPT_PRIO)
 8021708:	9c0a      	ldr	r4, [sp, #40]	; 0x28
              rmp_ptr_t Slices - The number of timeslices to assign to the thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0. on error, return an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Crt(volatile struct RMP_Thd* Thread, void* Entry, void* Stack, void* Arg, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
 802170a:	f8dd a02c 	ldr.w	sl, [sp, #44]	; 0x2c
    /* Check if the priority and timeslice range is correct */
    if(Prio>=RMP_MAX_PREEMPT_PRIO)
 802170e:	2c1f      	cmp	r4, #31
 8021710:	d87e      	bhi.n	8021810 <RMP_Thd_Crt+0x10c>
        return RMP_ERR_PRIO;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
 8021712:	f10a 35ff 	add.w	r5, sl, #4294967295
 8021716:	4c4e      	ldr	r4, [pc, #312]	; (8021850 <RMP_Thd_Crt+0x14c>)
 8021718:	42a5      	cmp	r5, r4
 802171a:	d875      	bhi.n	8021808 <RMP_Thd_Crt+0x104>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
 802171c:	2800      	cmp	r0, #0
 802171e:	d07b      	beq.n	8021818 <RMP_Thd_Crt+0x114>
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021720:	2601      	movs	r6, #1
    RMP_Sched_Lock_Cnt++;
 8021722:	4d4c      	ldr	r5, [pc, #304]	; (8021854 <RMP_Thd_Crt+0x150>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021724:	4f4c      	ldr	r7, [pc, #304]	; (8021858 <RMP_Thd_Crt+0x154>)
 8021726:	4690      	mov	r8, r2
 8021728:	4604      	mov	r4, r0
 802172a:	469b      	mov	fp, r3
 802172c:	4689      	mov	r9, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 802172e:	f7ff fd77 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021732:	603e      	str	r6, [r7, #0]
    RMP_Sched_Lock_Cnt++;
 8021734:	682a      	ldr	r2, [r5, #0]
 8021736:	4432      	add	r2, r6
 8021738:	602a      	str	r2, [r5, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
 802173a:	69a2      	ldr	r2, [r4, #24]
 802173c:	f012 0fff 	tst.w	r2, #255	; 0xff
 8021740:	d155      	bne.n	80217ee <RMP_Thd_Crt+0xea>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Create the thread and insert it into the list */
    Thread->Prio=Prio;
 8021742:	9b0a      	ldr	r3, [sp, #40]	; 0x28
    Thread->Slices=Slices;
    Thread->Slices_Left=Slices;
    Thread->Stack=(rmp_ptr_t)Stack;
    
    /* Initialize its stack and sending list */
    _RMP_Stack_Init((rmp_ptr_t)Entry, (rmp_ptr_t)Stack, (rmp_ptr_t)Arg);
 8021744:	465a      	mov	r2, fp
 8021746:	4641      	mov	r1, r8
 8021748:	4648      	mov	r0, r9
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Create the thread and insert it into the list */
    Thread->Prio=Prio;
 802174a:	6263      	str	r3, [r4, #36]	; 0x24
    Thread->Slices=Slices;
 802174c:	f8c4 a01c 	str.w	sl, [r4, #28]
    Thread->Slices_Left=Slices;
 8021750:	f8c4 a020 	str.w	sl, [r4, #32]
    Thread->Stack=(rmp_ptr_t)Stack;
 8021754:	f8c4 8034 	str.w	r8, [r4, #52]	; 0x34
    
    /* Initialize its stack and sending list */
    _RMP_Stack_Init((rmp_ptr_t)Entry, (rmp_ptr_t)Stack, (rmp_ptr_t)Arg);
 8021758:	f7ff fc80 	bl	802105c <_RMP_Stack_Init>
    RMP_List_Crt(&(Thread->Snd_List));
 802175c:	f104 0310 	add.w	r3, r4, #16
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 8021760:	6123      	str	r3, [r4, #16]
    Head->Next=(struct RMP_List*)Head;
 8021762:	6163      	str	r3, [r4, #20]
    /* Initialize its stack and sending list */
    _RMP_Stack_Init((rmp_ptr_t)Entry, (rmp_ptr_t)Stack, (rmp_ptr_t)Arg);
    RMP_List_Crt(&(Thread->Snd_List));
    
    /* Notify the scheduler that we have created something new, also check locks */
    Thread->State=RMP_THD_RUNNING;
 8021764:	61a6      	str	r6, [r4, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021766:	69a3      	ldr	r3, [r4, #24]
 8021768:	05db      	lsls	r3, r3, #23
 802176a:	d50c      	bpl.n	8021786 <RMP_Thd_Crt+0x82>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 802176c:	682a      	ldr	r2, [r5, #0]
 802176e:	4b39      	ldr	r3, [pc, #228]	; (8021854 <RMP_Thd_Crt+0x150>)
 8021770:	2a01      	cmp	r2, #1
 8021772:	d030      	beq.n	80217d6 <RMP_Thd_Crt+0xd2>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021774:	681a      	ldr	r2, [r3, #0]
 8021776:	2a01      	cmp	r2, #1
 8021778:	d938      	bls.n	80217ec <RMP_Thd_Crt+0xe8>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802177a:	681a      	ldr	r2, [r3, #0]
    Thread->State=RMP_THD_RUNNING;
    _RMP_Set_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
 802177c:	2000      	movs	r0, #0
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802177e:	3a01      	subs	r2, #1
 8021780:	601a      	str	r2, [r3, #0]
 8021782:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021786:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8021788:	4b34      	ldr	r3, [pc, #208]	; (802185c <RMP_Thd_Crt+0x158>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 802178a:	4835      	ldr	r0, [pc, #212]	; (8021860 <RMP_Thd_Crt+0x15c>)
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 802178c:	f853 2032 	ldr.w	r2, [r3, r2, lsl #3]
 8021790:	6a61      	ldr	r1, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021792:	f8df e0d4 	ldr.w	lr, [pc, #212]	; 8021868 <RMP_Thd_Crt+0x164>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021796:	eb03 0cc1 	add.w	ip, r3, r1, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 802179a:	f843 4031 	str.w	r4, [r3, r1, lsl #3]
    New->Next=(struct RMP_List*)Next;
 802179e:	f8c4 c004 	str.w	ip, [r4, #4]
    New->Prev=(struct RMP_List*)Prev;
 80217a2:	6022      	str	r2, [r4, #0]
    Prev->Next=(struct RMP_List*)New;
 80217a4:	6054      	str	r4, [r2, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 80217a6:	6a63      	ldr	r3, [r4, #36]	; 0x24
 80217a8:	6a62      	ldr	r2, [r4, #36]	; 0x24
 80217aa:	f003 031f 	and.w	r3, r3, #31
 80217ae:	0952      	lsrs	r2, r2, #5
 80217b0:	fa06 f303 	lsl.w	r3, r6, r3
 80217b4:	f850 1022 	ldr.w	r1, [r0, r2, lsl #2]
 80217b8:	430b      	orrs	r3, r1
 80217ba:	f840 3022 	str.w	r3, [r0, r2, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 80217be:	6a62      	ldr	r2, [r4, #36]	; 0x24
 80217c0:	f8de 3000 	ldr.w	r3, [lr]
 80217c4:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 80217c6:	429a      	cmp	r2, r3
 80217c8:	d9d0      	bls.n	802176c <RMP_Thd_Crt+0x68>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 80217ca:	4b26      	ldr	r3, [pc, #152]	; (8021864 <RMP_Thd_Crt+0x160>)
 80217cc:	601e      	str	r6, [r3, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80217ce:	682a      	ldr	r2, [r5, #0]
 80217d0:	4b20      	ldr	r3, [pc, #128]	; (8021854 <RMP_Thd_Crt+0x150>)
 80217d2:	2a01      	cmp	r2, #1
 80217d4:	d1ce      	bne.n	8021774 <RMP_Thd_Crt+0x70>
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80217d6:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80217d8:	4922      	ldr	r1, [pc, #136]	; (8021864 <RMP_Thd_Crt+0x160>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80217da:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80217dc:	603a      	str	r2, [r7, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80217de:	680b      	ldr	r3, [r1, #0]
 80217e0:	b9f3      	cbnz	r3, 8021820 <RMP_Thd_Crt+0x11c>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80217e2:	f7ff fd27 	bl	8021234 <RVM_Unmask_Int>
    Thread->State=RMP_THD_RUNNING;
    _RMP_Set_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
 80217e6:	2000      	movs	r0, #0
 80217e8:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 80217ec:	e7fe      	b.n	80217ec <RMP_Thd_Crt+0xe8>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80217ee:	682b      	ldr	r3, [r5, #0]
 80217f0:	42b3      	cmp	r3, r6
 80217f2:	d01e      	beq.n	8021832 <RMP_Thd_Crt+0x12e>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80217f4:	682b      	ldr	r3, [r5, #0]
 80217f6:	2b01      	cmp	r3, #1
 80217f8:	d91a      	bls.n	8021830 <RMP_Thd_Crt+0x12c>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80217fa:	682b      	ldr	r3, [r5, #0]
    
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 80217fc:	f04f 30ff 	mov.w	r0, #4294967295
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021800:	4403      	add	r3, r0
 8021802:	602b      	str	r3, [r5, #0]
 8021804:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
        RMP_COVERAGE_MARKER();
    
    if((Slices==0)||(Slices>=RMP_MAX_SLICES))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
 8021808:	f06f 0002 	mvn.w	r0, #2
 802180c:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
{
    /* Check if the priority and timeslice range is correct */
    if(Prio>=RMP_MAX_PREEMPT_PRIO)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_PRIO;
 8021810:	f06f 0001 	mvn.w	r0, #1
 8021814:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
 8021818:	f04f 30ff 	mov.w	r0, #4294967295
    _RMP_Set_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
}
 802181c:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021820:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 8021822:	f7ff fc65 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021826:	f7ff fd05 	bl	8021234 <RVM_Unmask_Int>
    Thread->State=RMP_THD_RUNNING;
    _RMP_Set_Rdy(Thread);
    
    RMP_Unlock_Sched();

    return 0;
 802182a:	2000      	movs	r0, #0
 802182c:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8021830:	e7fe      	b.n	8021830 <RMP_Thd_Crt+0x12c>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021832:	2300      	movs	r3, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021834:	4a0b      	ldr	r2, [pc, #44]	; (8021864 <RMP_Thd_Crt+0x160>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021836:	602b      	str	r3, [r5, #0]
        RMP_Sched_Locked=0;
 8021838:	603b      	str	r3, [r7, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802183a:	6811      	ldr	r1, [r2, #0]
 802183c:	b111      	cbz	r1, 8021844 <RMP_Thd_Crt+0x140>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802183e:	6013      	str	r3, [r2, #0]
            _RMP_Yield();
 8021840:	f7ff fc56 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021844:	f7ff fcf6 	bl	8021234 <RVM_Unmask_Int>
    
    if(RMP_THD_STATE(Thread->State)!=RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 8021848:	f04f 30ff 	mov.w	r0, #4294967295
 802184c:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8021850:	0001869e 	.word	0x0001869e
 8021854:	20028774 	.word	0x20028774
 8021858:	200287c0 	.word	0x200287c0
 802185c:	20028674 	.word	0x20028674
 8021860:	2002866c 	.word	0x2002866c
 8021864:	20028670 	.word	0x20028670
 8021868:	2002920c 	.word	0x2002920c

0802186c <RMP_Thd_Set>:
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
    /* Check if the priority and timeslice range is correct */
    if(Slices==0)
 802186c:	2a00      	cmp	r2, #0
 802186e:	d07b      	beq.n	8021968 <RMP_Thd_Set+0xfc>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
 8021870:	2800      	cmp	r0, #0
 8021872:	d076      	beq.n	8021962 <RMP_Thd_Set+0xf6>
              rmp_ptr_t Slices - The new timeslice value for this thread.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Set(volatile struct RMP_Thd* Thread, rmp_ptr_t Prio, rmp_ptr_t Slices)
{
 8021874:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8021878:	4604      	mov	r4, r0
 802187a:	4615      	mov	r5, r2
 802187c:	460e      	mov	r6, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 802187e:	f7ff fccf 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021882:	2201      	movs	r2, #1
    RMP_Sched_Lock_Cnt++;
 8021884:	4b54      	ldr	r3, [pc, #336]	; (80219d8 <RMP_Thd_Set+0x16c>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021886:	4855      	ldr	r0, [pc, #340]	; (80219dc <RMP_Thd_Set+0x170>)
 8021888:	6002      	str	r2, [r0, #0]
    RMP_Sched_Lock_Cnt++;
 802188a:	681a      	ldr	r2, [r3, #0]
 802188c:	3201      	adds	r2, #1
 802188e:	601a      	str	r2, [r3, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
 8021890:	69a2      	ldr	r2, [r4, #24]
 8021892:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
 8021896:	d057      	beq.n	8021948 <RMP_Thd_Set+0xdc>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the thread is in running state */
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
 8021898:	69a2      	ldr	r2, [r4, #24]
 802189a:	b2d2      	uxtb	r2, r2
 802189c:	2a01      	cmp	r2, #1
 802189e:	d013      	beq.n	80218c8 <RMP_Thd_Set+0x5c>
    }
    else
    {
        RMP_COVERAGE_MARKER();
        
        if(Prio<RMP_MAX_PREEMPT_PRIO)
 80218a0:	2e1f      	cmp	r6, #31
 80218a2:	d800      	bhi.n	80218a6 <RMP_Thd_Set+0x3a>
        {
            RMP_COVERAGE_MARKER();
            Thread->Prio=Prio;
 80218a4:	6266      	str	r6, [r4, #36]	; 0x24
        }
        else
            RMP_COVERAGE_MARKER();
        
        if(Slices<RMP_MAX_SLICES)
 80218a6:	4a4e      	ldr	r2, [pc, #312]	; (80219e0 <RMP_Thd_Set+0x174>)
 80218a8:	4295      	cmp	r5, r2
 80218aa:	d800      	bhi.n	80218ae <RMP_Thd_Set+0x42>
        {
            RMP_COVERAGE_MARKER();
            Thread->Slices=Slices;
 80218ac:	61e5      	str	r5, [r4, #28]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80218ae:	681a      	ldr	r2, [r3, #0]
 80218b0:	4b49      	ldr	r3, [pc, #292]	; (80219d8 <RMP_Thd_Set+0x16c>)
 80218b2:	2a01      	cmp	r2, #1
 80218b4:	d03c      	beq.n	8021930 <RMP_Thd_Set+0xc4>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80218b6:	681a      	ldr	r2, [r3, #0]
 80218b8:	2a01      	cmp	r2, #1
 80218ba:	d938      	bls.n	802192e <RMP_Thd_Set+0xc2>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80218bc:	681a      	ldr	r2, [r3, #0]
        else
            RMP_COVERAGE_MARKER();
    }
    
    RMP_Unlock_Sched();
    return 0;
 80218be:	2000      	movs	r0, #0
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80218c0:	3a01      	subs	r2, #1
 80218c2:	601a      	str	r2, [r3, #0]
 80218c4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    if(RMP_THD_STATE(Thread->State)==RMP_THD_RUNNING)
    {
        RMP_COVERAGE_MARKER();
        
        /* See if we are gonna change one of it or both */
        if(Prio<RMP_MAX_PREEMPT_PRIO)
 80218c8:	2e1f      	cmp	r6, #31
 80218ca:	d8ec      	bhi.n	80218a6 <RMP_Thd_Set+0x3a>
        {
            RMP_COVERAGE_MARKER();
            if(Thread->Prio!=Prio)
 80218cc:	6a61      	ldr	r1, [r4, #36]	; 0x24
 80218ce:	428e      	cmp	r6, r1
 80218d0:	d0e9      	beq.n	80218a6 <RMP_Thd_Set+0x3a>
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 80218d2:	69a1      	ldr	r1, [r4, #24]
 80218d4:	05c9      	lsls	r1, r1, #23
 80218d6:	d54a      	bpl.n	802196e <RMP_Thd_Set+0x102>
            {
                RMP_COVERAGE_MARKER();
                /* It doesn't matter whether this is suspended or not. 
                 * If suspended, the operations will not be conducted. */
                _RMP_Clr_Rdy(Thread);
                Thread->Prio=Prio;
 80218d8:	6266      	str	r6, [r4, #36]	; 0x24
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 80218da:	69a2      	ldr	r2, [r4, #24]
 80218dc:	05d2      	lsls	r2, r2, #23
 80218de:	d4e2      	bmi.n	80218a6 <RMP_Thd_Set+0x3a>
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 80218e0:	6a61      	ldr	r1, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 80218e2:	2701      	movs	r7, #1
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 80218e4:	4a3f      	ldr	r2, [pc, #252]	; (80219e4 <RMP_Thd_Set+0x178>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 80218e6:	f8df e108 	ldr.w	lr, [pc, #264]	; 80219f0 <RMP_Thd_Set+0x184>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 80218ea:	f852 1031 	ldr.w	r1, [r2, r1, lsl #3]
 80218ee:	6a66      	ldr	r6, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 80218f0:	f8df c0f8 	ldr.w	ip, [pc, #248]	; 80219ec <RMP_Thd_Set+0x180>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 80218f4:	eb02 08c6 	add.w	r8, r2, r6, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 80218f8:	f842 4036 	str.w	r4, [r2, r6, lsl #3]
    New->Next=(struct RMP_List*)Next;
 80218fc:	f8c4 8004 	str.w	r8, [r4, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021900:	6021      	str	r1, [r4, #0]
    Prev->Next=(struct RMP_List*)New;
 8021902:	604c      	str	r4, [r1, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021904:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8021906:	6a61      	ldr	r1, [r4, #36]	; 0x24
 8021908:	f002 021f 	and.w	r2, r2, #31
 802190c:	0949      	lsrs	r1, r1, #5
 802190e:	fa07 f202 	lsl.w	r2, r7, r2
 8021912:	f85e 6021 	ldr.w	r6, [lr, r1, lsl #2]
 8021916:	4332      	orrs	r2, r6
 8021918:	f84e 2021 	str.w	r2, [lr, r1, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 802191c:	6a61      	ldr	r1, [r4, #36]	; 0x24
 802191e:	f8dc 2000 	ldr.w	r2, [ip]
 8021922:	6a52      	ldr	r2, [r2, #36]	; 0x24
 8021924:	4291      	cmp	r1, r2
 8021926:	d9be      	bls.n	80218a6 <RMP_Thd_Set+0x3a>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021928:	4a2f      	ldr	r2, [pc, #188]	; (80219e8 <RMP_Thd_Set+0x17c>)
 802192a:	6017      	str	r7, [r2, #0]
 802192c:	e7bb      	b.n	80218a6 <RMP_Thd_Set+0x3a>
 802192e:	e7fe      	b.n	802192e <RMP_Thd_Set+0xc2>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021930:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021932:	492d      	ldr	r1, [pc, #180]	; (80219e8 <RMP_Thd_Set+0x17c>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021934:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021936:	6002      	str	r2, [r0, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021938:	680b      	ldr	r3, [r1, #0]
 802193a:	2b00      	cmp	r3, #0
 802193c:	d135      	bne.n	80219aa <RMP_Thd_Set+0x13e>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 802193e:	f7ff fc79 	bl	8021234 <RVM_Unmask_Int>
        else
            RMP_COVERAGE_MARKER();
    }
    
    RMP_Unlock_Sched();
    return 0;
 8021942:	2000      	movs	r0, #0
 8021944:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021948:	6819      	ldr	r1, [r3, #0]
 802194a:	2901      	cmp	r1, #1
 802194c:	d036      	beq.n	80219bc <RMP_Thd_Set+0x150>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 802194e:	681a      	ldr	r2, [r3, #0]
 8021950:	2a01      	cmp	r2, #1
 8021952:	d932      	bls.n	80219ba <RMP_Thd_Set+0x14e>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021954:	681a      	ldr	r2, [r3, #0]
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 8021956:	f04f 30ff 	mov.w	r0, #4294967295
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802195a:	4402      	add	r2, r0
 802195c:	601a      	str	r2, [r3, #0]
 802195e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
 8021962:	f04f 30ff 	mov.w	r0, #4294967295
 8021966:	4770      	bx	lr
{
    /* Check if the priority and timeslice range is correct */
    if(Slices==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SLICE;
 8021968:	f06f 0002 	mvn.w	r0, #2
 802196c:	4770      	bx	lr
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
 802196e:	6827      	ldr	r7, [r4, #0]
 8021970:	6861      	ldr	r1, [r4, #4]
 8021972:	428f      	cmp	r7, r1
 8021974:	d10d      	bne.n	8021992 <RMP_Thd_Set+0x126>
        {
            RMP_COVERAGE_MARKER();
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
 8021976:	6a67      	ldr	r7, [r4, #36]	; 0x24
 8021978:	6a61      	ldr	r1, [r4, #36]	; 0x24
 802197a:	f007 071f 	and.w	r7, r7, #31
 802197e:	f8df e070 	ldr.w	lr, [pc, #112]	; 80219f0 <RMP_Thd_Set+0x184>
 8021982:	0949      	lsrs	r1, r1, #5
 8021984:	40ba      	lsls	r2, r7
 8021986:	f85e 7021 	ldr.w	r7, [lr, r1, lsl #2]
 802198a:	ea27 0202 	bic.w	r2, r7, r2
 802198e:	f84e 2021 	str.w	r2, [lr, r1, lsl #2]
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8021992:	6822      	ldr	r2, [r4, #0]
 8021994:	6861      	ldr	r1, [r4, #4]
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
 8021996:	4f15      	ldr	r7, [pc, #84]	; (80219ec <RMP_Thd_Set+0x180>)
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021998:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 802199a:	6051      	str	r1, [r2, #4]
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
 802199c:	683a      	ldr	r2, [r7, #0]
 802199e:	4294      	cmp	r4, r2
 80219a0:	d19a      	bne.n	80218d8 <RMP_Thd_Set+0x6c>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 80219a2:	4a11      	ldr	r2, [pc, #68]	; (80219e8 <RMP_Thd_Set+0x17c>)
 80219a4:	2101      	movs	r1, #1
 80219a6:	6011      	str	r1, [r2, #0]
 80219a8:	e796      	b.n	80218d8 <RMP_Thd_Set+0x6c>
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 80219aa:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 80219ac:	f7ff fba0 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80219b0:	f7ff fc40 	bl	8021234 <RVM_Unmask_Int>
        else
            RMP_COVERAGE_MARKER();
    }
    
    RMP_Unlock_Sched();
    return 0;
 80219b4:	2000      	movs	r0, #0
 80219b6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80219ba:	e7fe      	b.n	80219ba <RMP_Thd_Set+0x14e>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80219bc:	490a      	ldr	r1, [pc, #40]	; (80219e8 <RMP_Thd_Set+0x17c>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80219be:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80219c0:	6002      	str	r2, [r0, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80219c2:	680b      	ldr	r3, [r1, #0]
 80219c4:	b113      	cbz	r3, 80219cc <RMP_Thd_Set+0x160>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 80219c6:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 80219c8:	f7ff fb92 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80219cc:	f7ff fc32 	bl	8021234 <RVM_Unmask_Int>
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 80219d0:	f04f 30ff 	mov.w	r0, #4294967295
 80219d4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80219d8:	20028774 	.word	0x20028774
 80219dc:	200287c0 	.word	0x200287c0
 80219e0:	0001869f 	.word	0x0001869f
 80219e4:	20028674 	.word	0x20028674
 80219e8:	20028670 	.word	0x20028670
 80219ec:	2002920c 	.word	0x2002920c
 80219f0:	2002866c 	.word	0x2002866c

080219f4 <RMP_Thd_Snd>:
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, rmp_ptr_t Data, rmp_ptr_t Slices)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
 80219f4:	2800      	cmp	r0, #0
 80219f6:	f000 8134 	beq.w	8021c62 <RMP_Thd_Snd+0x26e>
              rmp_ptr_t Slices - The timeslice to wait, if the mailbox is already full.
Output      : None.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd(volatile struct RMP_Thd* Thread, rmp_ptr_t Data, rmp_ptr_t Slices)
{
 80219fa:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 80219fe:	4604      	mov	r4, r0
 8021a00:	4616      	mov	r6, r2
 8021a02:	4689      	mov	r9, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 8021a04:	f7ff fc0c 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021a08:	f04f 0e01 	mov.w	lr, #1
    RMP_Sched_Lock_Cnt++;
 8021a0c:	4b9e      	ldr	r3, [pc, #632]	; (8021c88 <RMP_Thd_Snd+0x294>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021a0e:	499f      	ldr	r1, [pc, #636]	; (8021c8c <RMP_Thd_Snd+0x298>)
 8021a10:	f8c1 e000 	str.w	lr, [r1]
    RMP_Sched_Lock_Cnt++;
 8021a14:	681a      	ldr	r2, [r3, #0]
 8021a16:	4472      	add	r2, lr
 8021a18:	601a      	str	r2, [r3, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
 8021a1a:	69a2      	ldr	r2, [r4, #24]
 8021a1c:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
 8021a20:	f000 8112 	beq.w	8021c48 <RMP_Thd_Snd+0x254>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Are we sending to ourself? This is not allowed */
    if(RMP_Cur_Thd==Thread)
 8021a24:	4d9a      	ldr	r5, [pc, #616]	; (8021c90 <RMP_Thd_Snd+0x29c>)
 8021a26:	682a      	ldr	r2, [r5, #0]
 8021a28:	4294      	cmp	r4, r2
 8021a2a:	f000 80ec 	beq.w	8021c06 <RMP_Thd_Snd+0x212>
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Cur_Thd->Retval=0;
 8021a2e:	682a      	ldr	r2, [r5, #0]
 8021a30:	2000      	movs	r0, #0
 8021a32:	6390      	str	r0, [r2, #56]	; 0x38

    /* See if there is already a value in the mailbox, if yes, we block */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
 8021a34:	69a2      	ldr	r2, [r4, #24]
 8021a36:	0597      	lsls	r7, r2, #22
 8021a38:	d53f      	bpl.n	8021aba <RMP_Thd_Snd+0xc6>
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox full, we block, and put ourself into the queue */
        if(Slices==0)
 8021a3a:	2e00      	cmp	r6, #0
 8021a3c:	f000 80d6 	beq.w	8021bec <RMP_Thd_Snd+0x1f8>
        }
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
 8021a40:	682a      	ldr	r2, [r5, #0]
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021a42:	6990      	ldr	r0, [r2, #24]
 8021a44:	05c0      	lsls	r0, r0, #23
 8021a46:	f140 80a2 	bpl.w	8021b8e <RMP_Thd_Snd+0x19a>
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));
 8021a4a:	4620      	mov	r0, r4

        if(Slices<RMP_MAX_SLICES)
 8021a4c:	f8df c24c 	ldr.w	ip, [pc, #588]	; 8021c9c <RMP_Thd_Snd+0x2a8>
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));
 8021a50:	682a      	ldr	r2, [r5, #0]

        if(Slices<RMP_MAX_SLICES)
 8021a52:	4566      	cmp	r6, ip
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));
 8021a54:	f850 ef10 	ldr.w	lr, [r0, #16]!
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021a58:	6122      	str	r2, [r4, #16]
    New->Next=(struct RMP_List*)Next;
 8021a5a:	6050      	str	r0, [r2, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021a5c:	f8c2 e000 	str.w	lr, [r2]
        else
            RMP_COVERAGE_MARKER();

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));
 8021a60:	488b      	ldr	r0, [pc, #556]	; (8021c90 <RMP_Thd_Snd+0x29c>)
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
    Prev->Next=(struct RMP_List*)New;
 8021a62:	f8ce 2004 	str.w	r2, [lr, #4]

        /* We must be running */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));

        if(Slices<RMP_MAX_SLICES)
 8021a66:	d877      	bhi.n	8021b58 <RMP_Thd_Snd+0x164>
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 8021a68:	f8df c234 	ldr.w	ip, [pc, #564]	; 8021ca0 <RMP_Thd_Snd+0x2ac>
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Thread->Snd_List.Prev,&(Thread->Snd_List));

        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
 8021a6c:	6800      	ldr	r0, [r0, #0]
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 8021a6e:	f8dc 2004 	ldr.w	r2, [ip, #4]
    while(Trav_Ptr!=&(RMP_Delay))
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 8021a72:	f8df e230 	ldr.w	lr, [pc, #560]	; 8021ca4 <RMP_Thd_Snd+0x2b0>
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 8021a76:	4562      	cmp	r2, ip
 8021a78:	d103      	bne.n	8021a82 <RMP_Thd_Snd+0x8e>
 8021a7a:	e008      	b.n	8021a8e <RMP_Thd_Snd+0x9a>
            break;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
 8021a7c:	6852      	ldr	r2, [r2, #4]
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 8021a7e:	4562      	cmp	r2, ip
 8021a80:	d005      	beq.n	8021a8e <RMP_Thd_Snd+0x9a>
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 8021a82:	f8de 4000 	ldr.w	r4, [lr]
 8021a86:	6a17      	ldr	r7, [r2, #32]
 8021a88:	1b3c      	subs	r4, r7, r4
 8021a8a:	42a6      	cmp	r6, r4
 8021a8c:	d2f6      	bcs.n	8021a7c <RMP_Thd_Snd+0x88>
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 8021a8e:	f8de 4000 	ldr.w	r4, [lr]
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 8021a92:	f100 0e08 	add.w	lr, r0, #8
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 8021a96:	4426      	add	r6, r4
 8021a98:	6286      	str	r6, [r0, #40]	; 0x28
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 8021a9a:	6814      	ldr	r4, [r2, #0]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021a9c:	f8c2 e000 	str.w	lr, [r2]
    New->Next=(struct RMP_List*)Next;
 8021aa0:	60c2      	str	r2, [r0, #12]
    New->Prev=(struct RMP_List*)Prev;
 8021aa2:	6084      	str	r4, [r0, #8]
    Prev->Next=(struct RMP_List*)New;
 8021aa4:	f8c4 e004 	str.w	lr, [r4, #4]

        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDDLY);
 8021aa8:	6828      	ldr	r0, [r5, #0]
 8021aaa:	682a      	ldr	r2, [r5, #0]
 8021aac:	6992      	ldr	r2, [r2, #24]
 8021aae:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8021ab2:	f042 0203 	orr.w	r2, r2, #3
 8021ab6:	6182      	str	r2, [r0, #24]
 8021ab8:	e056      	b.n	8021b68 <RMP_Thd_Snd+0x174>
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
 8021aba:	69a2      	ldr	r2, [r4, #24]
 8021abc:	b2d2      	uxtb	r2, r2
 8021abe:	2a04      	cmp	r2, #4
 8021ac0:	d017      	beq.n	8021af2 <RMP_Thd_Snd+0xfe>
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
 8021ac2:	69a2      	ldr	r2, [r4, #24]
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
 8021ac4:	b2d2      	uxtb	r2, r2
 8021ac6:	2a05      	cmp	r2, #5
 8021ac8:	d013      	beq.n	8021af2 <RMP_Thd_Snd+0xfe>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
 8021aca:	f8c4 902c 	str.w	r9, [r4, #44]	; 0x2c
        Thread->State|=RMP_THD_MBOXFUL;
 8021ace:	69a2      	ldr	r2, [r4, #24]
 8021ad0:	f442 7200 	orr.w	r2, r2, #512	; 0x200
 8021ad4:	61a2      	str	r2, [r4, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021ad6:	681a      	ldr	r2, [r3, #0]
 8021ad8:	4b6b      	ldr	r3, [pc, #428]	; (8021c88 <RMP_Thd_Snd+0x294>)
 8021ada:	2a01      	cmp	r2, #1
 8021adc:	d04b      	beq.n	8021b76 <RMP_Thd_Snd+0x182>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021ade:	681a      	ldr	r2, [r3, #0]
 8021ae0:	2a01      	cmp	r2, #1
 8021ae2:	d968      	bls.n	8021bb6 <RMP_Thd_Snd+0x1c2>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021ae4:	681a      	ldr	r2, [r3, #0]
 8021ae6:	3a01      	subs	r2, #1
 8021ae8:	601a      	str	r2, [r3, #0]
        Thread->State|=RMP_THD_MBOXFUL;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
 8021aea:	682b      	ldr	r3, [r5, #0]
 8021aec:	6b98      	ldr	r0, [r3, #56]	; 0x38
 8021aee:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();
            
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
 8021af2:	69a2      	ldr	r2, [r4, #24]
 8021af4:	b2d2      	uxtb	r2, r2
 8021af6:	2a05      	cmp	r2, #5
 8021af8:	d05e      	beq.n	8021bb8 <RMP_Thd_Snd+0x1c4>
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
                RMP_COVERAGE_MARKER();
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 8021afa:	69a2      	ldr	r2, [r4, #24]
 8021afc:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8021b00:	f042 0201 	orr.w	r2, r2, #1
 8021b04:	61a2      	str	r2, [r4, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021b06:	69a2      	ldr	r2, [r4, #24]
 8021b08:	05d2      	lsls	r2, r2, #23
 8021b0a:	d4de      	bmi.n	8021aca <RMP_Thd_Snd+0xd6>
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021b0c:	6a60      	ldr	r0, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021b0e:	f04f 0e01 	mov.w	lr, #1
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021b12:	4a60      	ldr	r2, [pc, #384]	; (8021c94 <RMP_Thd_Snd+0x2a0>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021b14:	f8df c190 	ldr.w	ip, [pc, #400]	; 8021ca8 <RMP_Thd_Snd+0x2b4>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021b18:	f852 0030 	ldr.w	r0, [r2, r0, lsl #3]
 8021b1c:	6a66      	ldr	r6, [r4, #36]	; 0x24
 8021b1e:	eb02 08c6 	add.w	r8, r2, r6, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021b22:	f842 4036 	str.w	r4, [r2, r6, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8021b26:	f8c4 8004 	str.w	r8, [r4, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021b2a:	6020      	str	r0, [r4, #0]
    Prev->Next=(struct RMP_List*)New;
 8021b2c:	6044      	str	r4, [r0, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021b2e:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8021b30:	6a60      	ldr	r0, [r4, #36]	; 0x24
 8021b32:	f002 021f 	and.w	r2, r2, #31
 8021b36:	0940      	lsrs	r0, r0, #5
 8021b38:	fa0e f202 	lsl.w	r2, lr, r2
 8021b3c:	f85c 6020 	ldr.w	r6, [ip, r0, lsl #2]
 8021b40:	4332      	orrs	r2, r6
 8021b42:	f84c 2020 	str.w	r2, [ip, r0, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021b46:	6a60      	ldr	r0, [r4, #36]	; 0x24
 8021b48:	682a      	ldr	r2, [r5, #0]
 8021b4a:	6a52      	ldr	r2, [r2, #36]	; 0x24
 8021b4c:	4290      	cmp	r0, r2
 8021b4e:	d9bc      	bls.n	8021aca <RMP_Thd_Snd+0xd6>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021b50:	4a51      	ldr	r2, [pc, #324]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
 8021b52:	f8c2 e000 	str.w	lr, [r2]
 8021b56:	e7b8      	b.n	8021aca <RMP_Thd_Snd+0xd6>
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SNDBLK);
 8021b58:	6804      	ldr	r4, [r0, #0]
 8021b5a:	6802      	ldr	r2, [r0, #0]
 8021b5c:	6992      	ldr	r2, [r2, #24]
 8021b5e:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8021b62:	f042 0202 	orr.w	r2, r2, #2
 8021b66:	61a2      	str	r2, [r4, #24]
        }

        RMP_Cur_Thd->Data=Data;
 8021b68:	682a      	ldr	r2, [r5, #0]
 8021b6a:	f8c2 9030 	str.w	r9, [r2, #48]	; 0x30
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021b6e:	681a      	ldr	r2, [r3, #0]
 8021b70:	4b45      	ldr	r3, [pc, #276]	; (8021c88 <RMP_Thd_Snd+0x294>)
 8021b72:	2a01      	cmp	r2, #1
 8021b74:	d1b3      	bne.n	8021ade <RMP_Thd_Snd+0xea>
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021b76:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021b78:	4847      	ldr	r0, [pc, #284]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021b7a:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021b7c:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021b7e:	6803      	ldr	r3, [r0, #0]
 8021b80:	bb83      	cbnz	r3, 8021be4 <RMP_Thd_Snd+0x1f0>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021b82:	f7ff fb57 	bl	8021234 <RVM_Unmask_Int>
        Thread->State|=RMP_THD_MBOXFUL;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
 8021b86:	682b      	ldr	r3, [r5, #0]
 8021b88:	6b98      	ldr	r0, [r3, #56]	; 0x38
 8021b8a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
 8021b8e:	f8d2 c000 	ldr.w	ip, [r2]
 8021b92:	6850      	ldr	r0, [r2, #4]
 8021b94:	4584      	cmp	ip, r0
 8021b96:	d014      	beq.n	8021bc2 <RMP_Thd_Snd+0x1ce>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8021b98:	6810      	ldr	r0, [r2, #0]
 8021b9a:	f8d2 e004 	ldr.w	lr, [r2, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021b9e:	f8ce 0000 	str.w	r0, [lr]
    Prev->Next=(struct RMP_List*)Next;
 8021ba2:	f8c0 e004 	str.w	lr, [r0, #4]
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
 8021ba6:	6828      	ldr	r0, [r5, #0]
 8021ba8:	4282      	cmp	r2, r0
 8021baa:	f47f af4e 	bne.w	8021a4a <RMP_Thd_Snd+0x56>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021bae:	4a3a      	ldr	r2, [pc, #232]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
 8021bb0:	2001      	movs	r0, #1
 8021bb2:	6010      	str	r0, [r2, #0]
 8021bb4:	e749      	b.n	8021a4a <RMP_Thd_Snd+0x56>
 8021bb6:	e7fe      	b.n	8021bb6 <RMP_Thd_Snd+0x1c2>
            
            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 8021bb8:	68a2      	ldr	r2, [r4, #8]
 8021bba:	68e0      	ldr	r0, [r4, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021bbc:	6002      	str	r2, [r0, #0]
    Prev->Next=(struct RMP_List*)Next;
 8021bbe:	6050      	str	r0, [r2, #4]
 8021bc0:	e79b      	b.n	8021afa <RMP_Thd_Snd+0x106>
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        {
            RMP_COVERAGE_MARKER();
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
 8021bc2:	f8d2 c024 	ldr.w	ip, [r2, #36]	; 0x24
 8021bc6:	6a50      	ldr	r0, [r2, #36]	; 0x24
 8021bc8:	f00c 0c1f 	and.w	ip, ip, #31
 8021bcc:	f8df 80d8 	ldr.w	r8, [pc, #216]	; 8021ca8 <RMP_Thd_Snd+0x2b4>
 8021bd0:	0940      	lsrs	r0, r0, #5
 8021bd2:	fa0e fe0c 	lsl.w	lr, lr, ip
 8021bd6:	f858 c020 	ldr.w	ip, [r8, r0, lsl #2]
 8021bda:	ea2c 0e0e 	bic.w	lr, ip, lr
 8021bde:	f848 e020 	str.w	lr, [r8, r0, lsl #2]
 8021be2:	e7d9      	b.n	8021b98 <RMP_Thd_Snd+0x1a4>
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021be4:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8021be6:	f7ff fa83 	bl	80210f0 <_RMP_Yield>
 8021bea:	e7ca      	b.n	8021b82 <RMP_Thd_Snd+0x18e>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021bec:	681a      	ldr	r2, [r3, #0]
 8021bee:	2a01      	cmp	r2, #1
 8021bf0:	d020      	beq.n	8021c34 <RMP_Thd_Snd+0x240>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021bf2:	681a      	ldr	r2, [r3, #0]
 8021bf4:	2a01      	cmp	r2, #1
 8021bf6:	d91c      	bls.n	8021c32 <RMP_Thd_Snd+0x23e>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021bf8:	681a      	ldr	r2, [r3, #0]
        /* Mailbox full, we block, and put ourself into the queue */
        if(Slices==0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
 8021bfa:	f06f 0004 	mvn.w	r0, #4
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021bfe:	3a01      	subs	r2, #1
 8021c00:	601a      	str	r2, [r3, #0]
 8021c02:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021c06:	681a      	ldr	r2, [r3, #0]
 8021c08:	2a01      	cmp	r2, #1
 8021c0a:	d003      	beq.n	8021c14 <RMP_Thd_Snd+0x220>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021c0c:	681a      	ldr	r2, [r3, #0]
 8021c0e:	2a01      	cmp	r2, #1
 8021c10:	d8f2      	bhi.n	8021bf8 <RMP_Thd_Snd+0x204>
 8021c12:	e7fe      	b.n	8021c12 <RMP_Thd_Snd+0x21e>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021c14:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c16:	4820      	ldr	r0, [pc, #128]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021c18:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021c1a:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c1c:	6803      	ldr	r3, [r0, #0]
 8021c1e:	b113      	cbz	r3, 8021c26 <RMP_Thd_Snd+0x232>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021c20:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8021c22:	f7ff fa65 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021c26:	f7ff fb05 	bl	8021234 <RVM_Unmask_Int>
    /* Are we sending to ourself? This is not allowed */
    if(RMP_Cur_Thd==Thread)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
 8021c2a:	f06f 0004 	mvn.w	r0, #4
 8021c2e:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8021c32:	e7fe      	b.n	8021c32 <RMP_Thd_Snd+0x23e>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c34:	4a18      	ldr	r2, [pc, #96]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021c36:	601e      	str	r6, [r3, #0]
        RMP_Sched_Locked=0;
 8021c38:	600e      	str	r6, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c3a:	6813      	ldr	r3, [r2, #0]
 8021c3c:	2b00      	cmp	r3, #0
 8021c3e:	d0f2      	beq.n	8021c26 <RMP_Thd_Snd+0x232>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021c40:	6016      	str	r6, [r2, #0]
            _RMP_Yield();
 8021c42:	f7ff fa55 	bl	80210f0 <_RMP_Yield>
 8021c46:	e7ee      	b.n	8021c26 <RMP_Thd_Snd+0x232>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021c48:	6818      	ldr	r0, [r3, #0]
 8021c4a:	4570      	cmp	r0, lr
 8021c4c:	d00d      	beq.n	8021c6a <RMP_Thd_Snd+0x276>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021c4e:	681a      	ldr	r2, [r3, #0]
 8021c50:	2a01      	cmp	r2, #1
 8021c52:	d909      	bls.n	8021c68 <RMP_Thd_Snd+0x274>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021c54:	681a      	ldr	r2, [r3, #0]
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 8021c56:	f04f 30ff 	mov.w	r0, #4294967295
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021c5a:	4402      	add	r2, r0
 8021c5c:	601a      	str	r2, [r3, #0]
 8021c5e:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
 8021c62:	f04f 30ff 	mov.w	r0, #4294967295
 8021c66:	4770      	bx	lr
 8021c68:	e7fe      	b.n	8021c68 <RMP_Thd_Snd+0x274>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c6a:	480b      	ldr	r0, [pc, #44]	; (8021c98 <RMP_Thd_Snd+0x2a4>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021c6c:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021c6e:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021c70:	6803      	ldr	r3, [r0, #0]
 8021c72:	b113      	cbz	r3, 8021c7a <RMP_Thd_Snd+0x286>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021c74:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8021c76:	f7ff fa3b 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021c7a:	f7ff fadb 	bl	8021234 <RVM_Unmask_Int>
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_THD;
 8021c7e:	f04f 30ff 	mov.w	r0, #4294967295
 8021c82:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8021c86:	bf00      	nop
 8021c88:	20028774 	.word	0x20028774
 8021c8c:	200287c0 	.word	0x200287c0
 8021c90:	2002920c 	.word	0x2002920c
 8021c94:	20028674 	.word	0x20028674
 8021c98:	20028670 	.word	0x20028670
 8021c9c:	0001869f 	.word	0x0001869f
 8021ca0:	200287b8 	.word	0x200287b8
 8021ca4:	20029210 	.word	0x20029210
 8021ca8:	2002866c 	.word	0x2002866c

08021cac <RMP_Thd_Snd_ISR>:
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, rmp_ptr_t Data)
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
 8021cac:	2800      	cmp	r0, #0
 8021cae:	d05e      	beq.n	8021d6e <RMP_Thd_Snd_ISR+0xc2>
              rmp_ptr_t Data - The data to send to that thread.
Output      : None.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Snd_ISR(volatile struct RMP_Thd* Thread, rmp_ptr_t Data)
{
 8021cb0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
        return RMP_ERR_THD;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(RMP_THD_STATE(Thread->State)==RMP_THD_FREE)
 8021cb2:	6983      	ldr	r3, [r0, #24]
 8021cb4:	f013 0fff 	tst.w	r3, #255	; 0xff
 8021cb8:	d053      	beq.n	8021d62 <RMP_Thd_Snd_ISR+0xb6>
    }
    else
        RMP_COVERAGE_MARKER();

    /* See if there is already a value in the mailbox, if yes, we abort */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
 8021cba:	6983      	ldr	r3, [r0, #24]
 8021cbc:	059a      	lsls	r2, r3, #22
 8021cbe:	d453      	bmi.n	8021d68 <RMP_Thd_Snd_ISR+0xbc>
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
 8021cc0:	6983      	ldr	r3, [r0, #24]
 8021cc2:	4604      	mov	r4, r0
 8021cc4:	460d      	mov	r5, r1
 8021cc6:	b2db      	uxtb	r3, r3
 8021cc8:	2b04      	cmp	r3, #4
 8021cca:	d00a      	beq.n	8021ce2 <RMP_Thd_Snd_ISR+0x36>
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
 8021ccc:	6983      	ldr	r3, [r0, #24]
    else
    {
        RMP_COVERAGE_MARKER();
        
        /* Mailbox not full. We need to check if the receiver is waiting for us */
        if((RMP_THD_STATE(Thread->State)==RMP_THD_RCVBLK)||
 8021cce:	b2db      	uxtb	r3, r3
 8021cd0:	2b05      	cmp	r3, #5
 8021cd2:	d006      	beq.n	8021ce2 <RMP_Thd_Snd_ISR+0x36>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
 8021cd4:	62e5      	str	r5, [r4, #44]	; 0x2c
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
 8021cd6:	2000      	movs	r0, #0
        else
            RMP_COVERAGE_MARKER();
        
        /* Set the mailbox */
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
 8021cd8:	69a3      	ldr	r3, [r4, #24]
 8021cda:	f443 7300 	orr.w	r3, r3, #512	; 0x200
 8021cde:	61a3      	str	r3, [r4, #24]
    }

    return 0;
 8021ce0:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
           (RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY))
        {
            RMP_COVERAGE_MARKER();

            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
 8021ce2:	69a3      	ldr	r3, [r4, #24]
 8021ce4:	b2db      	uxtb	r3, r3
 8021ce6:	2b05      	cmp	r3, #5
 8021ce8:	d036      	beq.n	8021d58 <RMP_Thd_Snd_ISR+0xac>
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
            }
            else
                RMP_COVERAGE_MARKER();
            
            RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 8021cea:	69a3      	ldr	r3, [r4, #24]
 8021cec:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
 8021cf0:	f043 0301 	orr.w	r3, r3, #1
 8021cf4:	61a3      	str	r3, [r4, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021cf6:	69a3      	ldr	r3, [r4, #24]
 8021cf8:	05db      	lsls	r3, r3, #23
 8021cfa:	d509      	bpl.n	8021d10 <RMP_Thd_Snd_ISR+0x64>
 8021cfc:	4b1d      	ldr	r3, [pc, #116]	; (8021d74 <RMP_Thd_Snd_ISR+0xc8>)

            /* Set to running if not suspended */
            _RMP_Set_Rdy(Thread);

            /* If schedule pending, trigger it now because we are in ISR */
            if(RMP_Sched_Pend!=0)
 8021cfe:	681b      	ldr	r3, [r3, #0]
 8021d00:	4a1c      	ldr	r2, [pc, #112]	; (8021d74 <RMP_Thd_Snd_ISR+0xc8>)
 8021d02:	2b00      	cmp	r3, #0
 8021d04:	d0e6      	beq.n	8021cd4 <RMP_Thd_Snd_ISR+0x28>
            {
                RMP_COVERAGE_MARKER();
                RMP_Sched_Pend=0;
 8021d06:	2300      	movs	r3, #0
 8021d08:	6013      	str	r3, [r2, #0]
                _RMP_Yield();
 8021d0a:	f7ff f9f1 	bl	80210f0 <_RMP_Yield>
 8021d0e:	e7e1      	b.n	8021cd4 <RMP_Thd_Snd_ISR+0x28>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021d10:	6a62      	ldr	r2, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021d12:	2601      	movs	r6, #1
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021d14:	4b18      	ldr	r3, [pc, #96]	; (8021d78 <RMP_Thd_Snd_ISR+0xcc>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021d16:	4819      	ldr	r0, [pc, #100]	; (8021d7c <RMP_Thd_Snd_ISR+0xd0>)
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021d18:	f853 2032 	ldr.w	r2, [r3, r2, lsl #3]
 8021d1c:	6a61      	ldr	r1, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021d1e:	4f18      	ldr	r7, [pc, #96]	; (8021d80 <RMP_Thd_Snd_ISR+0xd4>)
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021d20:	eb03 0ec1 	add.w	lr, r3, r1, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021d24:	f843 4031 	str.w	r4, [r3, r1, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8021d28:	f8c4 e004 	str.w	lr, [r4, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021d2c:	6022      	str	r2, [r4, #0]
    Prev->Next=(struct RMP_List*)New;
 8021d2e:	6054      	str	r4, [r2, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021d30:	6a63      	ldr	r3, [r4, #36]	; 0x24
 8021d32:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8021d34:	f003 031f 	and.w	r3, r3, #31
 8021d38:	0952      	lsrs	r2, r2, #5
 8021d3a:	fa06 f303 	lsl.w	r3, r6, r3
 8021d3e:	f850 1022 	ldr.w	r1, [r0, r2, lsl #2]
 8021d42:	430b      	orrs	r3, r1
 8021d44:	f840 3022 	str.w	r3, [r0, r2, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021d48:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8021d4a:	683b      	ldr	r3, [r7, #0]
 8021d4c:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 8021d4e:	429a      	cmp	r2, r3
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021d50:	4b08      	ldr	r3, [pc, #32]	; (8021d74 <RMP_Thd_Snd_ISR+0xc8>)
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021d52:	d9d4      	bls.n	8021cfe <RMP_Thd_Snd_ISR+0x52>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021d54:	601e      	str	r6, [r3, #0]
 8021d56:	e7d2      	b.n	8021cfe <RMP_Thd_Snd_ISR+0x52>

            /* The receiver is blocked, wake it up and return the value */
            if(RMP_THD_STATE(Thread->State)==RMP_THD_RCVDLY)
            {
                RMP_COVERAGE_MARKER();
                RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 8021d58:	68a3      	ldr	r3, [r4, #8]
 8021d5a:	68e2      	ldr	r2, [r4, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021d5c:	6013      	str	r3, [r2, #0]
    Prev->Next=(struct RMP_List*)Next;
 8021d5e:	605a      	str	r2, [r3, #4]
 8021d60:	e7c3      	b.n	8021cea <RMP_Thd_Snd_ISR+0x3e>
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
 8021d62:	f04f 30ff 	mov.w	r0, #4294967295
 8021d66:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}

    /* See if there is already a value in the mailbox, if yes, we abort */
    if((Thread->State&RMP_THD_MBOXFUL)!=0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
 8021d68:	f06f 0004 	mvn.w	r0, #4
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
}
 8021d6c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
{
    /* Check if this thread structure could possibly be in use */
    if(Thread==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_THD;
 8021d6e:	f04f 30ff 	mov.w	r0, #4294967295
        Thread->Mailbox=Data;
        Thread->State|=RMP_THD_MBOXFUL;
    }

    return 0;
}
 8021d72:	4770      	bx	lr
 8021d74:	20028670 	.word	0x20028670
 8021d78:	20028674 	.word	0x20028674
 8021d7c:	2002866c 	.word	0x2002866c
 8021d80:	2002920c 	.word	0x2002920c

08021d84 <RMP_Thd_Rcv>:
******************************************************************************/
rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data, rmp_ptr_t Slices)
{
    struct RMP_Thd* Sender;
    
    if(Data==0)
 8021d84:	2800      	cmp	r0, #0
 8021d86:	f000 8163 	beq.w	8022050 <RMP_Thd_Rcv+0x2cc>
Input       : rmp_ptr_t Slices - The timeslices to wait, if the mailbox is empty.
Output      : rmp_ptr_t* Data - The pointer to put the data to.
Return      : rmp_ret_t - If successful,0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Thd_Rcv(rmp_ptr_t* Data, rmp_ptr_t Slices)
{
 8021d8a:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8021d8e:	4607      	mov	r7, r0
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
    RMP_Sched_Lock_Cnt++;
 8021d90:	4da6      	ldr	r5, [pc, #664]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021d92:	460e      	mov	r6, r1
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021d94:	f8df 82ac 	ldr.w	r8, [pc, #684]	; 8022044 <RMP_Thd_Rcv+0x2c0>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 8021d98:	f7ff fa42 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021d9c:	2001      	movs	r0, #1
    
    RMP_Lock_Sched();
    
    /* Is there any other guy waiting on us? If there is, unblock it and set it running */
    Sender=0;
    if(&(RMP_Cur_Thd->Snd_List)!=RMP_Cur_Thd->Snd_List.Next)
 8021d9e:	4ca4      	ldr	r4, [pc, #656]	; (8022030 <RMP_Thd_Rcv+0x2ac>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 8021da0:	f8c8 0000 	str.w	r0, [r8]
    RMP_Sched_Lock_Cnt++;
 8021da4:	682b      	ldr	r3, [r5, #0]
 8021da6:	4403      	add	r3, r0
 8021da8:	602b      	str	r3, [r5, #0]
    
    RMP_Lock_Sched();
    
    /* Is there any other guy waiting on us? If there is, unblock it and set it running */
    Sender=0;
    if(&(RMP_Cur_Thd->Snd_List)!=RMP_Cur_Thd->Snd_List.Next)
 8021daa:	6823      	ldr	r3, [r4, #0]
 8021dac:	6822      	ldr	r2, [r4, #0]
 8021dae:	3310      	adds	r3, #16
 8021db0:	6952      	ldr	r2, [r2, #20]
 8021db2:	429a      	cmp	r2, r3
    {
        RMP_COVERAGE_MARKER();
        /* Read the data */
        Sender=(struct RMP_Thd*)(RMP_Cur_Thd->Snd_List.Next);
 8021db4:	6823      	ldr	r3, [r4, #0]
    
    RMP_Lock_Sched();
    
    /* Is there any other guy waiting on us? If there is, unblock it and set it running */
    Sender=0;
    if(&(RMP_Cur_Thd->Snd_List)!=RMP_Cur_Thd->Snd_List.Next)
 8021db6:	d074      	beq.n	8021ea2 <RMP_Thd_Rcv+0x11e>
    {
        RMP_COVERAGE_MARKER();
        /* Read the data */
        Sender=(struct RMP_Thd*)(RMP_Cur_Thd->Snd_List.Next);
 8021db8:	695b      	ldr	r3, [r3, #20]
        RMP_List_Del(Sender->Run_Head.Prev,Sender->Run_Head.Next);
 8021dba:	681a      	ldr	r2, [r3, #0]
 8021dbc:	6859      	ldr	r1, [r3, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021dbe:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 8021dc0:	6051      	str	r1, [r2, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Read the data */
        Sender=(struct RMP_Thd*)(RMP_Cur_Thd->Snd_List.Next);
        RMP_List_Del(Sender->Run_Head.Prev,Sender->Run_Head.Next);
        *Data=Sender->Data;
 8021dc2:	6b1a      	ldr	r2, [r3, #48]	; 0x30
 8021dc4:	603a      	str	r2, [r7, #0]
        /* Now we unblock it - what state is it in? */
        if((RMP_THD_STATE(Sender->State)==RMP_THD_SNDDLY))
 8021dc6:	699a      	ldr	r2, [r3, #24]
 8021dc8:	b2d1      	uxtb	r1, r2
 8021dca:	2903      	cmp	r1, #3
 8021dcc:	d104      	bne.n	8021dd8 <RMP_Thd_Rcv+0x54>
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Sender->Dly_Head.Prev,Sender->Dly_Head.Next);
 8021dce:	689a      	ldr	r2, [r3, #8]
 8021dd0:	68d9      	ldr	r1, [r3, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8021dd2:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 8021dd4:	6051      	str	r1, [r2, #4]
 8021dd6:	699a      	ldr	r2, [r3, #24]
            RMP_List_Del(Sender->Dly_Head.Prev,Sender->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Sender->State,RMP_THD_RUNNING);
 8021dd8:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8021ddc:	f042 0201 	orr.w	r2, r2, #1
 8021de0:	619a      	str	r2, [r3, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021de2:	699a      	ldr	r2, [r3, #24]
 8021de4:	05d1      	lsls	r1, r2, #23
 8021de6:	d537      	bpl.n	8021e58 <RMP_Thd_Rcv+0xd4>
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Sender);
    }

    /* Check if there is a value in our mailbox. If yes, we return with that value */
    if((RMP_Cur_Thd->State&RMP_THD_MBOXFUL)!=0)
 8021de8:	6821      	ldr	r1, [r4, #0]
 8021dea:	4a91      	ldr	r2, [pc, #580]	; (8022030 <RMP_Thd_Rcv+0x2ac>)
 8021dec:	6988      	ldr	r0, [r1, #24]
 8021dee:	f410 7000 	ands.w	r0, r0, #512	; 0x200
 8021df2:	d020      	beq.n	8021e36 <RMP_Thd_Rcv+0xb2>
    {
        RMP_COVERAGE_MARKER();
        /* Get the value from mailbox */
        *Data=RMP_Cur_Thd->Mailbox;
 8021df4:	6811      	ldr	r1, [r2, #0]
 8021df6:	6ac9      	ldr	r1, [r1, #44]	; 0x2c
 8021df8:	6039      	str	r1, [r7, #0]
        /* See if we unblocked a sender. If yes, we place the new value into 
         * our mailbox and it is still full */
        if(Sender!=0)
            RMP_Cur_Thd->Mailbox=Sender->Data;
 8021dfa:	6812      	ldr	r2, [r2, #0]
 8021dfc:	6b1b      	ldr	r3, [r3, #48]	; 0x30
 8021dfe:	62d3      	str	r3, [r2, #44]	; 0x2c
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021e00:	682a      	ldr	r2, [r5, #0]
 8021e02:	4b8a      	ldr	r3, [pc, #552]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021e04:	2a01      	cmp	r2, #1
 8021e06:	d008      	beq.n	8021e1a <RMP_Thd_Rcv+0x96>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021e08:	681a      	ldr	r2, [r3, #0]
 8021e0a:	2a01      	cmp	r2, #1
 8021e0c:	d948      	bls.n	8021ea0 <RMP_Thd_Rcv+0x11c>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021e0e:	681a      	ldr	r2, [r3, #0]
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        
        RMP_Unlock_Sched();
        return 0;
 8021e10:	2000      	movs	r0, #0
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021e12:	3a01      	subs	r2, #1
 8021e14:	601a      	str	r2, [r3, #0]
 8021e16:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021e1a:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021e1c:	4985      	ldr	r1, [pc, #532]	; (8022034 <RMP_Thd_Rcv+0x2b0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021e1e:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021e20:	f8c8 2000 	str.w	r2, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021e24:	680b      	ldr	r3, [r1, #0]
 8021e26:	2b00      	cmp	r3, #0
 8021e28:	f040 80d9 	bne.w	8021fde <RMP_Thd_Rcv+0x25a>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021e2c:	f7ff fa02 	bl	8021234 <RVM_Unmask_Int>
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        
        RMP_Unlock_Sched();
        return 0;
 8021e30:	2000      	movs	r0, #0
 8021e32:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
        
        /* Box empty. Do we have somebody waiting? */
        if(Sender!=0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Cur_Thd->Mailbox=Sender->Data;
 8021e36:	6811      	ldr	r1, [r2, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021e38:	682a      	ldr	r2, [r5, #0]
        
        /* Box empty. Do we have somebody waiting? */
        if(Sender!=0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Cur_Thd->Mailbox=Sender->Data;
 8021e3a:	6b1b      	ldr	r3, [r3, #48]	; 0x30
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021e3c:	2a01      	cmp	r2, #1
        
        /* Box empty. Do we have somebody waiting? */
        if(Sender!=0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Cur_Thd->Mailbox=Sender->Data;
 8021e3e:	62cb      	str	r3, [r1, #44]	; 0x2c
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021e40:	4b7a      	ldr	r3, [pc, #488]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021e42:	f000 80a3 	beq.w	8021f8c <RMP_Thd_Rcv+0x208>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021e46:	681a      	ldr	r2, [r3, #0]
 8021e48:	2a01      	cmp	r2, #1
 8021e4a:	f240 809e 	bls.w	8021f8a <RMP_Thd_Rcv+0x206>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021e4e:	681a      	ldr	r2, [r3, #0]
 8021e50:	3a01      	subs	r2, #1
 8021e52:	601a      	str	r2, [r3, #0]
 8021e54:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021e58:	6a59      	ldr	r1, [r3, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021e5a:	2601      	movs	r6, #1
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021e5c:	4a76      	ldr	r2, [pc, #472]	; (8022038 <RMP_Thd_Rcv+0x2b4>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021e5e:	f8df e1e8 	ldr.w	lr, [pc, #488]	; 8022048 <RMP_Thd_Rcv+0x2c4>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8021e62:	f852 1031 	ldr.w	r1, [r2, r1, lsl #3]
 8021e66:	6a58      	ldr	r0, [r3, #36]	; 0x24
 8021e68:	eb02 0cc0 	add.w	ip, r2, r0, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021e6c:	f842 3030 	str.w	r3, [r2, r0, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8021e70:	f8c3 c004 	str.w	ip, [r3, #4]
    New->Prev=(struct RMP_List*)Prev;
 8021e74:	6019      	str	r1, [r3, #0]
    Prev->Next=(struct RMP_List*)New;
 8021e76:	604b      	str	r3, [r1, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8021e78:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 8021e7a:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8021e7c:	f002 021f 	and.w	r2, r2, #31
 8021e80:	0949      	lsrs	r1, r1, #5
 8021e82:	fa06 f202 	lsl.w	r2, r6, r2
 8021e86:	f85e 0021 	ldr.w	r0, [lr, r1, lsl #2]
 8021e8a:	4302      	orrs	r2, r0
 8021e8c:	f84e 2021 	str.w	r2, [lr, r1, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8021e90:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8021e92:	6822      	ldr	r2, [r4, #0]
 8021e94:	6a52      	ldr	r2, [r2, #36]	; 0x24
 8021e96:	4291      	cmp	r1, r2
 8021e98:	d9a6      	bls.n	8021de8 <RMP_Thd_Rcv+0x64>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8021e9a:	4a66      	ldr	r2, [pc, #408]	; (8022034 <RMP_Thd_Rcv+0x2b0>)
 8021e9c:	6016      	str	r6, [r2, #0]
 8021e9e:	e7a3      	b.n	8021de8 <RMP_Thd_Rcv+0x64>
 8021ea0:	e7fe      	b.n	8021ea0 <RMP_Thd_Rcv+0x11c>
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Sender);
    }

    /* Check if there is a value in our mailbox. If yes, we return with that value */
    if((RMP_Cur_Thd->State&RMP_THD_MBOXFUL)!=0)
 8021ea2:	699b      	ldr	r3, [r3, #24]
 8021ea4:	059b      	lsls	r3, r3, #22
 8021ea6:	d50c      	bpl.n	8021ec2 <RMP_Thd_Rcv+0x13e>
    {
        RMP_COVERAGE_MARKER();
        /* Get the value from mailbox */
        *Data=RMP_Cur_Thd->Mailbox;
 8021ea8:	6823      	ldr	r3, [r4, #0]
 8021eaa:	6adb      	ldr	r3, [r3, #44]	; 0x2c
 8021eac:	603b      	str	r3, [r7, #0]
        /* See if we unblocked a sender. If yes, we place the new value into 
         * our mailbox and it is still full */
        if(Sender!=0)
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
 8021eae:	6822      	ldr	r2, [r4, #0]
 8021eb0:	6993      	ldr	r3, [r2, #24]
 8021eb2:	f423 7300 	bic.w	r3, r3, #512	; 0x200
 8021eb6:	6193      	str	r3, [r2, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021eb8:	682a      	ldr	r2, [r5, #0]
 8021eba:	4b5c      	ldr	r3, [pc, #368]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021ebc:	2a01      	cmp	r2, #1
 8021ebe:	d1a3      	bne.n	8021e08 <RMP_Thd_Rcv+0x84>
 8021ec0:	e7ab      	b.n	8021e1a <RMP_Thd_Rcv+0x96>
        /* No sender waiting on us and box empty, we need to block */
        else
        {
            RMP_COVERAGE_MARKER();
            
            if(Slices==0)
 8021ec2:	2e00      	cmp	r6, #0
 8021ec4:	f000 80d8 	beq.w	8022078 <RMP_Thd_Rcv+0x2f4>
            }
            else
                RMP_COVERAGE_MARKER();

            /* We must be running and not suspended so we will surely be deleted from queue */
            _RMP_Clr_Rdy(RMP_Cur_Thd);
 8021ec8:	6823      	ldr	r3, [r4, #0]
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8021eca:	699a      	ldr	r2, [r3, #24]
 8021ecc:	05d2      	lsls	r2, r2, #23
 8021ece:	f140 808e 	bpl.w	8021fee <RMP_Thd_Rcv+0x26a>
                RMP_COVERAGE_MARKER();

            /* We must be running and not suspended so we will surely be deleted from queue */
            _RMP_Clr_Rdy(RMP_Cur_Thd);

            if(Slices<RMP_MAX_SLICES)
 8021ed2:	4b5a      	ldr	r3, [pc, #360]	; (802203c <RMP_Thd_Rcv+0x2b8>)
 8021ed4:	429e      	cmp	r6, r3
 8021ed6:	d869      	bhi.n	8021fac <RMP_Thd_Rcv+0x228>
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 8021ed8:	f8df e170 	ldr.w	lr, [pc, #368]	; 802204c <RMP_Thd_Rcv+0x2c8>
            _RMP_Clr_Rdy(RMP_Cur_Thd);

            if(Slices<RMP_MAX_SLICES)
            {
                RMP_COVERAGE_MARKER();
                _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
 8021edc:	f8d4 c000 	ldr.w	ip, [r4]
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 8021ee0:	f8de 3004 	ldr.w	r3, [lr, #4]
    while(Trav_Ptr!=&(RMP_Delay))
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 8021ee4:	4856      	ldr	r0, [pc, #344]	; (8022040 <RMP_Thd_Rcv+0x2bc>)
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 8021ee6:	4573      	cmp	r3, lr
 8021ee8:	d00d      	beq.n	8021f06 <RMP_Thd_Rcv+0x182>
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 8021eea:	6801      	ldr	r1, [r0, #0]
 8021eec:	6a1a      	ldr	r2, [r3, #32]
 8021eee:	1a51      	subs	r1, r2, r1
 8021ef0:	42b1      	cmp	r1, r6
 8021ef2:	d905      	bls.n	8021f00 <RMP_Thd_Rcv+0x17c>
 8021ef4:	e007      	b.n	8021f06 <RMP_Thd_Rcv+0x182>
 8021ef6:	6801      	ldr	r1, [r0, #0]
 8021ef8:	6a1a      	ldr	r2, [r3, #32]
 8021efa:	1a51      	subs	r1, r2, r1
 8021efc:	428e      	cmp	r6, r1
 8021efe:	d302      	bcc.n	8021f06 <RMP_Thd_Rcv+0x182>
            break;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
 8021f00:	685b      	ldr	r3, [r3, #4]
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 8021f02:	4573      	cmp	r3, lr
 8021f04:	d1f7      	bne.n	8021ef6 <RMP_Thd_Rcv+0x172>
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 8021f06:	6801      	ldr	r1, [r0, #0]
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 8021f08:	f10c 0008 	add.w	r0, ip, #8
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 8021f0c:	440e      	add	r6, r1
 8021f0e:	f8cc 6028 	str.w	r6, [ip, #40]	; 0x28
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 8021f12:	6819      	ldr	r1, [r3, #0]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8021f14:	6018      	str	r0, [r3, #0]
    New->Next=(struct RMP_List*)Next;
 8021f16:	f8cc 300c 	str.w	r3, [ip, #12]
    New->Prev=(struct RMP_List*)Prev;
 8021f1a:	f8cc 1008 	str.w	r1, [ip, #8]
    Prev->Next=(struct RMP_List*)New;
 8021f1e:	6048      	str	r0, [r1, #4]

            if(Slices<RMP_MAX_SLICES)
            {
                RMP_COVERAGE_MARKER();
                _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVDLY);
 8021f20:	6822      	ldr	r2, [r4, #0]
 8021f22:	6823      	ldr	r3, [r4, #0]
 8021f24:	699b      	ldr	r3, [r3, #24]
 8021f26:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
 8021f2a:	f043 0305 	orr.w	r3, r3, #5
 8021f2e:	6193      	str	r3, [r2, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021f30:	682a      	ldr	r2, [r5, #0]
 8021f32:	4b3e      	ldr	r3, [pc, #248]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021f34:	2a01      	cmp	r2, #1
 8021f36:	d045      	beq.n	8021fc4 <RMP_Thd_Rcv+0x240>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021f38:	681a      	ldr	r2, [r3, #0]
 8021f3a:	2a01      	cmp	r2, #1
 8021f3c:	f240 809b 	bls.w	8022076 <RMP_Thd_Rcv+0x2f2>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021f40:	681a      	ldr	r2, [r3, #0]
 8021f42:	3a01      	subs	r2, #1
 8021f44:	601a      	str	r2, [r3, #0]
            }
            RMP_Unlock_Sched();
            
            /* Dummy read - to separate the lock & unlock. If the compiler optimizes these two
             * functions(inline them) on some architectures sometimes we never block. */
            *Data=RMP_Cur_Thd->Mailbox;
 8021f46:	6823      	ldr	r3, [r4, #0]
 8021f48:	6adb      	ldr	r3, [r3, #44]	; 0x2c
 8021f4a:	603b      	str	r3, [r7, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 8021f4c:	f7ff f968 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 8021f50:	2301      	movs	r3, #1
 8021f52:	483c      	ldr	r0, [pc, #240]	; (8022044 <RMP_Thd_Rcv+0x2c0>)
 8021f54:	f8c8 3000 	str.w	r3, [r8]
    RMP_Sched_Lock_Cnt++;
 8021f58:	682a      	ldr	r2, [r5, #0]
 8021f5a:	4b34      	ldr	r3, [pc, #208]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021f5c:	3201      	adds	r2, #1
 8021f5e:	602a      	str	r2, [r5, #0]
             * functions(inline them) on some architectures sometimes we never block. */
            *Data=RMP_Cur_Thd->Mailbox;
            
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Lock_Sched();
            *Data=RMP_Cur_Thd->Mailbox;
 8021f60:	6822      	ldr	r2, [r4, #0]
 8021f62:	6ad2      	ldr	r2, [r2, #44]	; 0x2c
 8021f64:	603a      	str	r2, [r7, #0]
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
 8021f66:	6821      	ldr	r1, [r4, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021f68:	682d      	ldr	r5, [r5, #0]
            *Data=RMP_Cur_Thd->Mailbox;
            
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Lock_Sched();
            *Data=RMP_Cur_Thd->Mailbox;
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
 8021f6a:	698a      	ldr	r2, [r1, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021f6c:	2d01      	cmp	r5, #1
            *Data=RMP_Cur_Thd->Mailbox;
            
            /* We've been unblocked. There must be something in our mbox, or we should have failed */
            RMP_Lock_Sched();
            *Data=RMP_Cur_Thd->Mailbox;
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
 8021f6e:	f422 7200 	bic.w	r2, r2, #512	; 0x200
 8021f72:	618a      	str	r2, [r1, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021f74:	d070      	beq.n	8022058 <RMP_Thd_Rcv+0x2d4>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8021f76:	681a      	ldr	r2, [r3, #0]
 8021f78:	2a01      	cmp	r2, #1
 8021f7a:	d96c      	bls.n	8022056 <RMP_Thd_Rcv+0x2d2>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8021f7c:	681a      	ldr	r2, [r3, #0]
 8021f7e:	3a01      	subs	r2, #1
 8021f80:	601a      	str	r2, [r3, #0]
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        }
    }
    
    RMP_Unlock_Sched();
    return RMP_Cur_Thd->Retval;
 8021f82:	6823      	ldr	r3, [r4, #0]
 8021f84:	6b98      	ldr	r0, [r3, #56]	; 0x38
 8021f86:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8021f8a:	e7fe      	b.n	8021f8a <RMP_Thd_Rcv+0x206>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021f8c:	4a29      	ldr	r2, [pc, #164]	; (8022034 <RMP_Thd_Rcv+0x2b0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021f8e:	6018      	str	r0, [r3, #0]
        RMP_Sched_Locked=0;
 8021f90:	f8c8 0000 	str.w	r0, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021f94:	6813      	ldr	r3, [r2, #0]
 8021f96:	2b00      	cmp	r3, #0
 8021f98:	f43f af48 	beq.w	8021e2c <RMP_Thd_Rcv+0xa8>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021f9c:	6010      	str	r0, [r2, #0]
            _RMP_Yield();
 8021f9e:	f7ff f8a7 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021fa2:	f7ff f947 	bl	8021234 <RVM_Unmask_Int>
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        
        RMP_Unlock_Sched();
        return 0;
 8021fa6:	2000      	movs	r0, #0
 8021fa8:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVDLY);
            }
            else
            {
                RMP_COVERAGE_MARKER();
                RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_RCVBLK);
 8021fac:	6822      	ldr	r2, [r4, #0]
 8021fae:	6823      	ldr	r3, [r4, #0]
 8021fb0:	699b      	ldr	r3, [r3, #24]
 8021fb2:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
 8021fb6:	f043 0304 	orr.w	r3, r3, #4
 8021fba:	6193      	str	r3, [r2, #24]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8021fbc:	682a      	ldr	r2, [r5, #0]
 8021fbe:	4b1b      	ldr	r3, [pc, #108]	; (802202c <RMP_Thd_Rcv+0x2a8>)
 8021fc0:	2a01      	cmp	r2, #1
 8021fc2:	d1b9      	bne.n	8021f38 <RMP_Thd_Rcv+0x1b4>
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021fc4:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021fc6:	491b      	ldr	r1, [pc, #108]	; (8022034 <RMP_Thd_Rcv+0x2b0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8021fc8:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8021fca:	f8c8 2000 	str.w	r2, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8021fce:	680b      	ldr	r3, [r1, #0]
 8021fd0:	b113      	cbz	r3, 8021fd8 <RMP_Thd_Rcv+0x254>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021fd2:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 8021fd4:	f7ff f88c 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021fd8:	f7ff f92c 	bl	8021234 <RVM_Unmask_Int>
 8021fdc:	e7b3      	b.n	8021f46 <RMP_Thd_Rcv+0x1c2>
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8021fde:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 8021fe0:	f7ff f886 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8021fe4:	f7ff f926 	bl	8021234 <RVM_Unmask_Int>
            RMP_Cur_Thd->Mailbox=Sender->Data;
        else
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        
        RMP_Unlock_Sched();
        return 0;
 8021fe8:	2000      	movs	r0, #0
 8021fea:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
 8021fee:	6819      	ldr	r1, [r3, #0]
 8021ff0:	685a      	ldr	r2, [r3, #4]
 8021ff2:	4291      	cmp	r1, r2
 8021ff4:	d10e      	bne.n	8022014 <RMP_Thd_Rcv+0x290>
        {
            RMP_COVERAGE_MARKER();
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
 8021ff6:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 8021ff8:	6a59      	ldr	r1, [r3, #36]	; 0x24
 8021ffa:	f8df e04c 	ldr.w	lr, [pc, #76]	; 8022048 <RMP_Thd_Rcv+0x2c4>
 8021ffe:	f002 021f 	and.w	r2, r2, #31
 8022002:	0949      	lsrs	r1, r1, #5
 8022004:	fa00 f202 	lsl.w	r2, r0, r2
 8022008:	f85e 0021 	ldr.w	r0, [lr, r1, lsl #2]
 802200c:	ea20 0202 	bic.w	r2, r0, r2
 8022010:	f84e 2021 	str.w	r2, [lr, r1, lsl #2]
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8022014:	681a      	ldr	r2, [r3, #0]
 8022016:	6859      	ldr	r1, [r3, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022018:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 802201a:	6051      	str	r1, [r2, #4]
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
 802201c:	6822      	ldr	r2, [r4, #0]
 802201e:	4293      	cmp	r3, r2
 8022020:	f47f af57 	bne.w	8021ed2 <RMP_Thd_Rcv+0x14e>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8022024:	4b03      	ldr	r3, [pc, #12]	; (8022034 <RMP_Thd_Rcv+0x2b0>)
 8022026:	2201      	movs	r2, #1
 8022028:	601a      	str	r2, [r3, #0]
 802202a:	e752      	b.n	8021ed2 <RMP_Thd_Rcv+0x14e>
 802202c:	20028774 	.word	0x20028774
 8022030:	2002920c 	.word	0x2002920c
 8022034:	20028670 	.word	0x20028670
 8022038:	20028674 	.word	0x20028674
 802203c:	0001869f 	.word	0x0001869f
 8022040:	20029210 	.word	0x20029210
 8022044:	200287c0 	.word	0x200287c0
 8022048:	2002866c 	.word	0x2002866c
 802204c:	200287b8 	.word	0x200287b8
    struct RMP_Thd* Sender;
    
    if(Data==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
 8022050:	f06f 0004 	mvn.w	r0, #4
        }
    }
    
    RMP_Unlock_Sched();
    return RMP_Cur_Thd->Retval;
}
 8022054:	4770      	bx	lr
 8022056:	e7fe      	b.n	8022056 <RMP_Thd_Rcv+0x2d2>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022058:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802205a:	4916      	ldr	r1, [pc, #88]	; (80220b4 <RMP_Thd_Rcv+0x330>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 802205c:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 802205e:	6002      	str	r2, [r0, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022060:	680b      	ldr	r3, [r1, #0]
 8022062:	b113      	cbz	r3, 802206a <RMP_Thd_Rcv+0x2e6>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022064:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 8022066:	f7ff f843 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 802206a:	f7ff f8e3 	bl	8021234 <RVM_Unmask_Int>
            RMP_Cur_Thd->State&=~RMP_THD_MBOXFUL;
        }
    }
    
    RMP_Unlock_Sched();
    return RMP_Cur_Thd->Retval;
 802206e:	6823      	ldr	r3, [r4, #0]
 8022070:	6b98      	ldr	r0, [r3, #56]	; 0x38
 8022072:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8022076:	e7fe      	b.n	8022076 <RMP_Thd_Rcv+0x2f2>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022078:	682b      	ldr	r3, [r5, #0]
 802207a:	2b01      	cmp	r3, #1
 802207c:	d009      	beq.n	8022092 <RMP_Thd_Rcv+0x30e>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 802207e:	682b      	ldr	r3, [r5, #0]
 8022080:	2b01      	cmp	r3, #1
 8022082:	d915      	bls.n	80220b0 <RMP_Thd_Rcv+0x32c>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022084:	682b      	ldr	r3, [r5, #0]
            
            if(Slices==0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Unlock_Sched();
                return RMP_ERR_OPER;
 8022086:	f06f 0004 	mvn.w	r0, #4
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802208a:	3b01      	subs	r3, #1
 802208c:	602b      	str	r3, [r5, #0]
 802208e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022092:	4b08      	ldr	r3, [pc, #32]	; (80220b4 <RMP_Thd_Rcv+0x330>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022094:	602e      	str	r6, [r5, #0]
        RMP_Sched_Locked=0;
 8022096:	f8c8 6000 	str.w	r6, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802209a:	681a      	ldr	r2, [r3, #0]
 802209c:	b112      	cbz	r2, 80220a4 <RMP_Thd_Rcv+0x320>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802209e:	601e      	str	r6, [r3, #0]
            _RMP_Yield();
 80220a0:	f7ff f826 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80220a4:	f7ff f8c6 	bl	8021234 <RVM_Unmask_Int>
            
            if(Slices==0)
            {
                RMP_COVERAGE_MARKER();
                RMP_Unlock_Sched();
                return RMP_ERR_OPER;
 80220a8:	f06f 0004 	mvn.w	r0, #4
 80220ac:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80220b0:	e7fe      	b.n	80220b0 <RMP_Thd_Rcv+0x32c>
 80220b2:	bf00      	nop
 80220b4:	20028670 	.word	0x20028670

080220b8 <RMP_Sem_Crt>:
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
 80220b8:	2800      	cmp	r0, #0
 80220ba:	d04f      	beq.n	802215c <RMP_Sem_Crt+0xa4>
              rmp_ptr_t Number - The initial number of this semaphore.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Crt(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
 80220bc:	b538      	push	{r3, r4, r5, lr}
 80220be:	4604      	mov	r4, r0
 80220c0:	460d      	mov	r5, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 80220c2:	f7ff f8ad 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 80220c6:	2001      	movs	r0, #1
    RMP_Sched_Lock_Cnt++;
 80220c8:	4b31      	ldr	r3, [pc, #196]	; (8022190 <RMP_Sem_Crt+0xd8>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 80220ca:	4932      	ldr	r1, [pc, #200]	; (8022194 <RMP_Sem_Crt+0xdc>)
 80220cc:	6008      	str	r0, [r1, #0]
    RMP_Sched_Lock_Cnt++;
 80220ce:	681a      	ldr	r2, [r3, #0]
 80220d0:	4402      	add	r2, r0
 80220d2:	601a      	str	r2, [r3, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_FREE)
 80220d4:	68a2      	ldr	r2, [r4, #8]
 80220d6:	bb3a      	cbnz	r2, 8022128 <RMP_Sem_Crt+0x70>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_MAX_NUM)
 80220d8:	f5b5 7f7a 	cmp.w	r5, #1000	; 0x3e8
 80220dc:	d218      	bcs.n	8022110 <RMP_Sem_Crt+0x58>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Initialize contents */
    Semaphore->Cur_Num=Number;
 80220de:	60e5      	str	r5, [r4, #12]
    Semaphore->State=RMP_SEM_USED;
 80220e0:	60a0      	str	r0, [r4, #8]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80220e2:	6024      	str	r4, [r4, #0]
    Head->Next=(struct RMP_List*)Head;
 80220e4:	6064      	str	r4, [r4, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80220e6:	6818      	ldr	r0, [r3, #0]
 80220e8:	2801      	cmp	r0, #1
 80220ea:	d007      	beq.n	80220fc <RMP_Sem_Crt+0x44>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80220ec:	6819      	ldr	r1, [r3, #0]
 80220ee:	2901      	cmp	r1, #1
 80220f0:	d90d      	bls.n	802210e <RMP_Sem_Crt+0x56>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80220f2:	6819      	ldr	r1, [r3, #0]
    Semaphore->State=RMP_SEM_USED;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    RMP_Unlock_Sched();
    
    return 0;
 80220f4:	4610      	mov	r0, r2
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80220f6:	1e4a      	subs	r2, r1, #1
 80220f8:	601a      	str	r2, [r3, #0]
 80220fa:	bd38      	pop	{r3, r4, r5, pc}
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80220fc:	4826      	ldr	r0, [pc, #152]	; (8022198 <RMP_Sem_Crt+0xe0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80220fe:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8022100:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022102:	6803      	ldr	r3, [r0, #0]
 8022104:	bb6b      	cbnz	r3, 8022162 <RMP_Sem_Crt+0xaa>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022106:	f7ff f895 	bl	8021234 <RVM_Unmask_Int>
    Semaphore->State=RMP_SEM_USED;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    RMP_Unlock_Sched();
    
    return 0;
 802210a:	2000      	movs	r0, #0
 802210c:	bd38      	pop	{r3, r4, r5, pc}
 802210e:	e7fe      	b.n	802210e <RMP_Sem_Crt+0x56>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022110:	6818      	ldr	r0, [r3, #0]
 8022112:	2801      	cmp	r0, #1
 8022114:	d015      	beq.n	8022142 <RMP_Sem_Crt+0x8a>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8022116:	681a      	ldr	r2, [r3, #0]
 8022118:	2a01      	cmp	r2, #1
 802211a:	d911      	bls.n	8022140 <RMP_Sem_Crt+0x88>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802211c:	681a      	ldr	r2, [r3, #0]
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
 802211e:	f06f 0004 	mvn.w	r0, #4
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022122:	3a01      	subs	r2, #1
 8022124:	601a      	str	r2, [r3, #0]
 8022126:	bd38      	pop	{r3, r4, r5, pc}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022128:	681a      	ldr	r2, [r3, #0]
 802212a:	4282      	cmp	r2, r0
 802212c:	d021      	beq.n	8022172 <RMP_Sem_Crt+0xba>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 802212e:	681a      	ldr	r2, [r3, #0]
 8022130:	2a01      	cmp	r2, #1
 8022132:	d91d      	bls.n	8022170 <RMP_Sem_Crt+0xb8>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022134:	681a      	ldr	r2, [r3, #0]
    
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 8022136:	f06f 0005 	mvn.w	r0, #5
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802213a:	3a01      	subs	r2, #1
 802213c:	601a      	str	r2, [r3, #0]
 802213e:	bd38      	pop	{r3, r4, r5, pc}
 8022140:	e7fe      	b.n	8022140 <RMP_Sem_Crt+0x88>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022142:	4815      	ldr	r0, [pc, #84]	; (8022198 <RMP_Sem_Crt+0xe0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022144:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8022146:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022148:	6803      	ldr	r3, [r0, #0]
 802214a:	b113      	cbz	r3, 8022152 <RMP_Sem_Crt+0x9a>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802214c:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 802214e:	f7fe ffcf 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022152:	f7ff f86f 	bl	8021234 <RVM_Unmask_Int>
    /* Is the number too great to initialize? */
    if(Number>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
 8022156:	f06f 0004 	mvn.w	r0, #4
 802215a:	bd38      	pop	{r3, r4, r5, pc}
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
 802215c:	f06f 0005 	mvn.w	r0, #5
 8022160:	4770      	bx	lr
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022162:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8022164:	f7fe ffc4 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022168:	f7ff f864 	bl	8021234 <RVM_Unmask_Int>
    Semaphore->State=RMP_SEM_USED;
    RMP_List_Crt(&(Semaphore->Wait_List));
    
    RMP_Unlock_Sched();
    
    return 0;
 802216c:	2000      	movs	r0, #0
 802216e:	bd38      	pop	{r3, r4, r5, pc}
 8022170:	e7fe      	b.n	8022170 <RMP_Sem_Crt+0xb8>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022172:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022174:	4808      	ldr	r0, [pc, #32]	; (8022198 <RMP_Sem_Crt+0xe0>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022176:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8022178:	600a      	str	r2, [r1, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802217a:	6803      	ldr	r3, [r0, #0]
 802217c:	b113      	cbz	r3, 8022184 <RMP_Sem_Crt+0xcc>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802217e:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8022180:	f7fe ffb6 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022184:	f7ff f856 	bl	8021234 <RVM_Unmask_Int>
    
    if(Semaphore->State!=RMP_SEM_FREE)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 8022188:	f06f 0005 	mvn.w	r0, #5
 802218c:	bd38      	pop	{r3, r4, r5, pc}
 802218e:	bf00      	nop
 8022190:	20028774 	.word	0x20028774
 8022194:	200287c0 	.word	0x200287c0
 8022198:	20028670 	.word	0x20028670

0802219c <RMP_Sem_Pend>:
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Slices)
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
 802219c:	2800      	cmp	r0, #0
 802219e:	f000 80a2 	beq.w	80222e6 <RMP_Sem_Pend+0x14a>
              rmp_ptr_t Slices - The number of slices to wait.
Output      : None.
Return      : rmp_ret_t - If successful, the current semaphore number; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Pend(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Slices)
{
 80221a2:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 80221a6:	4604      	mov	r4, r0
 80221a8:	460d      	mov	r5, r1
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 80221aa:	f8df 81f8 	ldr.w	r8, [pc, #504]	; 80223a4 <RMP_Sem_Pend+0x208>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 80221ae:	f7ff f837 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 80221b2:	2201      	movs	r2, #1
    RMP_Sched_Lock_Cnt++;
 80221b4:	4b76      	ldr	r3, [pc, #472]	; (8022390 <RMP_Sem_Pend+0x1f4>)
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 80221b6:	f8c8 2000 	str.w	r2, [r8]
    RMP_Sched_Lock_Cnt++;
 80221ba:	681a      	ldr	r2, [r3, #0]
 80221bc:	3201      	adds	r2, #1
 80221be:	601a      	str	r2, [r3, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
 80221c0:	68a7      	ldr	r7, [r4, #8]
 80221c2:	2f01      	cmp	r7, #1
 80221c4:	f040 8092 	bne.w	80222ec <RMP_Sem_Pend+0x150>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if we can get one immediately */
    if(Semaphore->Cur_Num!=0)
 80221c8:	68e2      	ldr	r2, [r4, #12]
 80221ca:	2a00      	cmp	r2, #0
 80221cc:	d14e      	bne.n	802226c <RMP_Sem_Pend+0xd0>
    }
    else
    {
        RMP_COVERAGE_MARKER();
        /* Cannot get one, we need to block */
        if(Slices==0)
 80221ce:	2d00      	cmp	r5, #0
 80221d0:	f000 8099 	beq.w	8022306 <RMP_Sem_Pend+0x16a>
        }
        else
            RMP_COVERAGE_MARKER();

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
 80221d4:	4e6f      	ldr	r6, [pc, #444]	; (8022394 <RMP_Sem_Pend+0x1f8>)
 80221d6:	6832      	ldr	r2, [r6, #0]
Return      : None.
******************************************************************************/
void _RMP_Clr_Rdy(volatile struct RMP_Thd* Thread)
{
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 80221d8:	6990      	ldr	r0, [r2, #24]
 80221da:	05c1      	lsls	r1, r0, #23
 80221dc:	d557      	bpl.n	802228e <RMP_Sem_Pend+0xf2>

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
        
        if(Slices<RMP_MAX_SLICES)
 80221de:	4f6e      	ldr	r7, [pc, #440]	; (8022398 <RMP_Sem_Pend+0x1fc>)
        else
            RMP_COVERAGE_MARKER();

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
 80221e0:	6832      	ldr	r2, [r6, #0]
        
        if(Slices<RMP_MAX_SLICES)
 80221e2:	42bd      	cmp	r5, r7
        else
            RMP_COVERAGE_MARKER();

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
 80221e4:	6820      	ldr	r0, [r4, #0]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 80221e6:	6022      	str	r2, [r4, #0]
    New->Next=(struct RMP_List*)Next;
 80221e8:	6054      	str	r4, [r2, #4]
    New->Prev=(struct RMP_List*)Prev;
 80221ea:	6010      	str	r0, [r2, #0]
    Prev->Next=(struct RMP_List*)New;
 80221ec:	6042      	str	r2, [r0, #4]
        else
            RMP_COVERAGE_MARKER();

        /* We must be running - place into waitlist now */
        _RMP_Clr_Rdy(RMP_Cur_Thd);
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
 80221ee:	4a69      	ldr	r2, [pc, #420]	; (8022394 <RMP_Sem_Pend+0x1f8>)
        
        if(Slices<RMP_MAX_SLICES)
 80221f0:	d833      	bhi.n	802225a <RMP_Sem_Pend+0xbe>
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 80221f2:	f8df e1b4 	ldr.w	lr, [pc, #436]	; 80223a8 <RMP_Sem_Pend+0x20c>
        RMP_List_Ins(&(RMP_Cur_Thd->Run_Head),Semaphore->Wait_List.Prev,&(Semaphore->Wait_List));
        
        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
 80221f6:	6810      	ldr	r0, [r2, #0]
{
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
 80221f8:	f8de 2004 	ldr.w	r2, [lr, #4]
    while(Trav_Ptr!=&(RMP_Delay))
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 80221fc:	4c67      	ldr	r4, [pc, #412]	; (802239c <RMP_Sem_Pend+0x200>)
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 80221fe:	4572      	cmp	r2, lr
 8022200:	d103      	bne.n	802220a <RMP_Sem_Pend+0x6e>
 8022202:	e007      	b.n	8022214 <RMP_Sem_Pend+0x78>
            break;
        }
        else
            RMP_COVERAGE_MARKER();
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
 8022204:	6852      	ldr	r2, [r2, #4]
    struct RMP_List* Trav_Ptr;
    struct RMP_Thd* Trav_Thd;
    
    /* Find a place to insert this timer */
    Trav_Ptr=(struct RMP_List*)RMP_Delay.Next;
    while(Trav_Ptr!=&(RMP_Delay))
 8022206:	4572      	cmp	r2, lr
 8022208:	d004      	beq.n	8022214 <RMP_Sem_Pend+0x78>
    {
        Trav_Thd=RMP_DLY2THD(Trav_Ptr);
        if((Trav_Thd->Timeout-RMP_Tick)>Slices)
 802220a:	6827      	ldr	r7, [r4, #0]
 802220c:	6a11      	ldr	r1, [r2, #32]
 802220e:	1bcf      	subs	r7, r1, r7
 8022210:	42bd      	cmp	r5, r7
 8022212:	d2f7      	bcs.n	8022204 <RMP_Sem_Pend+0x68>
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 8022214:	6824      	ldr	r4, [r4, #0]
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 8022216:	f100 0708 	add.w	r7, r0, #8
        
        Trav_Ptr=(struct RMP_List*)(Trav_Ptr->Next);
    }

    /* Insert this into the list */
    Thread->Timeout=RMP_Tick+Slices;
 802221a:	4425      	add	r5, r4
 802221c:	6285      	str	r5, [r0, #40]	; 0x28
    RMP_List_Ins(&Thread->Dly_Head,Trav_Ptr->Prev,Trav_Ptr);
 802221e:	6814      	ldr	r4, [r2, #0]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022220:	6017      	str	r7, [r2, #0]
    New->Next=(struct RMP_List*)Next;
 8022222:	60c2      	str	r2, [r0, #12]
    New->Prev=(struct RMP_List*)Prev;
 8022224:	6084      	str	r4, [r0, #8]
    Prev->Next=(struct RMP_List*)New;
 8022226:	6067      	str	r7, [r4, #4]
        
        if(Slices<RMP_MAX_SLICES)
        {
            RMP_COVERAGE_MARKER();
            _RMP_Dly_Ins(RMP_Cur_Thd, Slices);
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMDLY);
 8022228:	6830      	ldr	r0, [r6, #0]
 802222a:	6832      	ldr	r2, [r6, #0]
 802222c:	6992      	ldr	r2, [r2, #24]
 802222e:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8022232:	f042 0208 	orr.w	r2, r2, #8
 8022236:	6182      	str	r2, [r0, #24]
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMBLK);
        }
        
        RMP_Cur_Thd->Retval=0;
 8022238:	6834      	ldr	r4, [r6, #0]
 802223a:	2200      	movs	r2, #0
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 802223c:	6818      	ldr	r0, [r3, #0]
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMBLK);
        }
        
        RMP_Cur_Thd->Retval=0;
 802223e:	63a2      	str	r2, [r4, #56]	; 0x38
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022240:	2801      	cmp	r0, #1
 8022242:	4b53      	ldr	r3, [pc, #332]	; (8022390 <RMP_Sem_Pend+0x1f4>)
 8022244:	d033      	beq.n	80222ae <RMP_Sem_Pend+0x112>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8022246:	681a      	ldr	r2, [r3, #0]
 8022248:	2a01      	cmp	r2, #1
 802224a:	d91f      	bls.n	802228c <RMP_Sem_Pend+0xf0>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802224c:	681a      	ldr	r2, [r3, #0]
 802224e:	3a01      	subs	r2, #1
 8022250:	601a      	str	r2, [r3, #0]
        RMP_Cur_Thd->Retval=0;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
 8022252:	6833      	ldr	r3, [r6, #0]
 8022254:	6b98      	ldr	r0, [r3, #56]	; 0x38
 8022256:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMDLY);
        }
        else
        {
            RMP_COVERAGE_MARKER();
            RMP_THD_STATE_SET(RMP_Cur_Thd->State,RMP_THD_SEMBLK);
 802225a:	6810      	ldr	r0, [r2, #0]
 802225c:	6812      	ldr	r2, [r2, #0]
 802225e:	6992      	ldr	r2, [r2, #24]
 8022260:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8022264:	f042 0207 	orr.w	r2, r2, #7
 8022268:	6182      	str	r2, [r0, #24]
 802226a:	e7e5      	b.n	8022238 <RMP_Sem_Pend+0x9c>
    
    /* Check if we can get one immediately */
    if(Semaphore->Cur_Num!=0)
    {
        RMP_COVERAGE_MARKER();
        Semaphore->Cur_Num--;
 802226c:	68e2      	ldr	r2, [r4, #12]
 802226e:	3a01      	subs	r2, #1
 8022270:	60e2      	str	r2, [r4, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022272:	681a      	ldr	r2, [r3, #0]
 8022274:	2a01      	cmp	r2, #1
 8022276:	d027      	beq.n	80222c8 <RMP_Sem_Pend+0x12c>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8022278:	681a      	ldr	r2, [r3, #0]
 802227a:	2a01      	cmp	r2, #1
 802227c:	f240 8086 	bls.w	802238c <RMP_Sem_Pend+0x1f0>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022280:	681a      	ldr	r2, [r3, #0]
 8022282:	3a01      	subs	r2, #1
 8022284:	601a      	str	r2, [r3, #0]
    if(Semaphore->Cur_Num!=0)
    {
        RMP_COVERAGE_MARKER();
        Semaphore->Cur_Num--;
        RMP_Unlock_Sched();
        return Semaphore->Cur_Num;
 8022286:	68e0      	ldr	r0, [r4, #12]
 8022288:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 802228c:	e7fe      	b.n	802228c <RMP_Sem_Pend+0xf0>
    /* Is it suspended? If yes, no need to delete again */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
 802228e:	f8d2 e000 	ldr.w	lr, [r2]
 8022292:	6850      	ldr	r0, [r2, #4]
 8022294:	4586      	cmp	lr, r0
 8022296:	d068      	beq.n	802236a <RMP_Sem_Pend+0x1ce>
        }
        else
            RMP_COVERAGE_MARKER();
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8022298:	6810      	ldr	r0, [r2, #0]
 802229a:	6857      	ldr	r7, [r2, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802229c:	6038      	str	r0, [r7, #0]
    Prev->Next=(struct RMP_List*)Next;
 802229e:	6047      	str	r7, [r0, #4]
        
        /* Delete this from the corresponding runqueue */
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        /* If it is the current thread, request a context switch */
        if(Thread==RMP_Cur_Thd)
 80222a0:	6830      	ldr	r0, [r6, #0]
 80222a2:	4282      	cmp	r2, r0
 80222a4:	d19b      	bne.n	80221de <RMP_Sem_Pend+0x42>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 80222a6:	4a3e      	ldr	r2, [pc, #248]	; (80223a0 <RMP_Sem_Pend+0x204>)
 80222a8:	2001      	movs	r0, #1
 80222aa:	6010      	str	r0, [r2, #0]
 80222ac:	e797      	b.n	80221de <RMP_Sem_Pend+0x42>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80222ae:	483c      	ldr	r0, [pc, #240]	; (80223a0 <RMP_Sem_Pend+0x204>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80222b0:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80222b2:	f8c8 2000 	str.w	r2, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80222b6:	6803      	ldr	r3, [r0, #0]
 80222b8:	2b00      	cmp	r3, #0
 80222ba:	d152      	bne.n	8022362 <RMP_Sem_Pend+0x1c6>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80222bc:	f7fe ffba 	bl	8021234 <RVM_Unmask_Int>
        RMP_Cur_Thd->Retval=0;
    }
    
    RMP_Unlock_Sched();

    return RMP_Cur_Thd->Retval;
 80222c0:	6833      	ldr	r3, [r6, #0]
 80222c2:	6b98      	ldr	r0, [r3, #56]	; 0x38
 80222c4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80222c8:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80222ca:	4835      	ldr	r0, [pc, #212]	; (80223a0 <RMP_Sem_Pend+0x204>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80222cc:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80222ce:	f8c8 2000 	str.w	r2, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80222d2:	6803      	ldr	r3, [r0, #0]
 80222d4:	b113      	cbz	r3, 80222dc <RMP_Sem_Pend+0x140>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 80222d6:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 80222d8:	f7fe ff0a 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80222dc:	f7fe ffaa 	bl	8021234 <RVM_Unmask_Int>
    if(Semaphore->Cur_Num!=0)
    {
        RMP_COVERAGE_MARKER();
        Semaphore->Cur_Num--;
        RMP_Unlock_Sched();
        return Semaphore->Cur_Num;
 80222e0:	68e0      	ldr	r0, [r4, #12]
 80222e2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
{
    /* Check if this semaphore structure could possibly be in use */
    if(Semaphore==0)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
 80222e6:	f06f 0005 	mvn.w	r0, #5
 80222ea:	4770      	bx	lr
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80222ec:	681a      	ldr	r2, [r3, #0]
 80222ee:	2a01      	cmp	r2, #1
 80222f0:	d017      	beq.n	8022322 <RMP_Sem_Pend+0x186>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80222f2:	681a      	ldr	r2, [r3, #0]
 80222f4:	2a01      	cmp	r2, #1
 80222f6:	d913      	bls.n	8022320 <RMP_Sem_Pend+0x184>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80222f8:	681a      	ldr	r2, [r3, #0]
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 80222fa:	f06f 0005 	mvn.w	r0, #5
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80222fe:	3a01      	subs	r2, #1
 8022300:	601a      	str	r2, [r3, #0]
 8022302:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 8022306:	681a      	ldr	r2, [r3, #0]
 8022308:	2a01      	cmp	r2, #1
 802230a:	d01b      	beq.n	8022344 <RMP_Sem_Pend+0x1a8>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 802230c:	681a      	ldr	r2, [r3, #0]
 802230e:	2a01      	cmp	r2, #1
 8022310:	d917      	bls.n	8022342 <RMP_Sem_Pend+0x1a6>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022312:	681a      	ldr	r2, [r3, #0]
        /* Cannot get one, we need to block */
        if(Slices==0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
 8022314:	f06f 0004 	mvn.w	r0, #4
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022318:	3a01      	subs	r2, #1
 802231a:	601a      	str	r2, [r3, #0]
 802231c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8022320:	e7fe      	b.n	8022320 <RMP_Sem_Pend+0x184>
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022322:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022324:	481e      	ldr	r0, [pc, #120]	; (80223a0 <RMP_Sem_Pend+0x204>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022326:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 8022328:	f8c8 2000 	str.w	r2, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802232c:	6803      	ldr	r3, [r0, #0]
 802232e:	b113      	cbz	r3, 8022336 <RMP_Sem_Pend+0x19a>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022330:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8022332:	f7fe fedd 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022336:	f7fe ff7d 	bl	8021234 <RVM_Unmask_Int>
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 802233a:	f06f 0005 	mvn.w	r0, #5
 802233e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8022342:	e7fe      	b.n	8022342 <RMP_Sem_Pend+0x1a6>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022344:	4a16      	ldr	r2, [pc, #88]	; (80223a0 <RMP_Sem_Pend+0x204>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022346:	601d      	str	r5, [r3, #0]
        RMP_Sched_Locked=0;
 8022348:	f8c8 5000 	str.w	r5, [r8]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802234c:	6813      	ldr	r3, [r2, #0]
 802234e:	b113      	cbz	r3, 8022356 <RMP_Sem_Pend+0x1ba>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022350:	6015      	str	r5, [r2, #0]
            _RMP_Yield();
 8022352:	f7fe fecd 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022356:	f7fe ff6d 	bl	8021234 <RVM_Unmask_Int>
        /* Cannot get one, we need to block */
        if(Slices==0)
        {
            RMP_COVERAGE_MARKER();
            RMP_Unlock_Sched();
            return RMP_ERR_OPER;
 802235a:	f06f 0004 	mvn.w	r0, #4
 802235e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022362:	6002      	str	r2, [r0, #0]
            _RMP_Yield();
 8022364:	f7fe fec4 	bl	80210f0 <_RMP_Yield>
 8022368:	e7a8      	b.n	80222bc <RMP_Sem_Pend+0x120>
        RMP_COVERAGE_MARKER();
        /* See if it is the last thread on the priority level */
        if(Thread->Run_Head.Prev==Thread->Run_Head.Next)
        {
            RMP_COVERAGE_MARKER();
            RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]&=~(((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK));
 802236a:	f8d2 e024 	ldr.w	lr, [r2, #36]	; 0x24
 802236e:	6a50      	ldr	r0, [r2, #36]	; 0x24
 8022370:	f00e 0e1f 	and.w	lr, lr, #31
 8022374:	f8df c034 	ldr.w	ip, [pc, #52]	; 80223ac <RMP_Sem_Pend+0x210>
 8022378:	0940      	lsrs	r0, r0, #5
 802237a:	fa07 f70e 	lsl.w	r7, r7, lr
 802237e:	f85c e020 	ldr.w	lr, [ip, r0, lsl #2]
 8022382:	ea2e 0707 	bic.w	r7, lr, r7
 8022386:	f84c 7020 	str.w	r7, [ip, r0, lsl #2]
 802238a:	e785      	b.n	8022298 <RMP_Sem_Pend+0xfc>
 802238c:	e7fe      	b.n	802238c <RMP_Sem_Pend+0x1f0>
 802238e:	bf00      	nop
 8022390:	20028774 	.word	0x20028774
 8022394:	2002920c 	.word	0x2002920c
 8022398:	0001869f 	.word	0x0001869f
 802239c:	20029210 	.word	0x20029210
 80223a0:	20028670 	.word	0x20028670
 80223a4:	200287c0 	.word	0x200287c0
 80223a8:	200287b8 	.word	0x200287b8
 80223ac:	2002866c 	.word	0x2002866c

080223b0 <RMP_Sem_Post>:
rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
 80223b0:	2800      	cmp	r0, #0
 80223b2:	f000 80c3 	beq.w	802253c <RMP_Sem_Post+0x18c>
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
 80223b6:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
    struct RMP_Thd* Thread;
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
 80223ba:	fab1 f781 	clz	r7, r1
 80223be:	097f      	lsrs	r7, r7, #5
 80223c0:	2f00      	cmp	r7, #0
 80223c2:	f040 80a8 	bne.w	8022516 <RMP_Sem_Post+0x166>
 80223c6:	4604      	mov	r4, r0
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
    RMP_Sched_Lock_Cnt++;
 80223c8:	4d69      	ldr	r5, [pc, #420]	; (8022570 <RMP_Sem_Post+0x1c0>)
 80223ca:	4688      	mov	r8, r1
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
    RMP_Sched_Locked=1;
 80223cc:	4e69      	ldr	r6, [pc, #420]	; (8022574 <RMP_Sem_Post+0x1c4>)
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Lock_Sched(void)
{
    RMP_MASK_INT();
 80223ce:	f7fe ff27 	bl	8021220 <RVM_Mask_Int>
    RMP_Sched_Locked=1;
 80223d2:	2301      	movs	r3, #1
 80223d4:	6033      	str	r3, [r6, #0]
    RMP_Sched_Lock_Cnt++;
 80223d6:	682b      	ldr	r3, [r5, #0]
 80223d8:	3301      	adds	r3, #1
 80223da:	602b      	str	r3, [r5, #0]
    else
        RMP_COVERAGE_MARKER();
    
    RMP_Lock_Sched();
    
    if(Semaphore->State!=RMP_SEM_USED)
 80223dc:	68a0      	ldr	r0, [r4, #8]
 80223de:	2801      	cmp	r0, #1
 80223e0:	f040 808c 	bne.w	80224fc <RMP_Sem_Post+0x14c>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
 80223e4:	68e3      	ldr	r3, [r4, #12]
 80223e6:	4443      	add	r3, r8
 80223e8:	f5b3 7f7a 	cmp.w	r3, #1000	; 0x3e8
 80223ec:	d279      	bcs.n	80224e2 <RMP_Sem_Post+0x132>
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
 80223ee:	68e3      	ldr	r3, [r4, #12]
 80223f0:	4443      	add	r3, r8
 80223f2:	60e3      	str	r3, [r4, #12]
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
 80223f4:	6863      	ldr	r3, [r4, #4]
 80223f6:	429c      	cmp	r4, r3
 80223f8:	d059      	beq.n	80224ae <RMP_Sem_Post+0xfe>
 80223fa:	68e3      	ldr	r3, [r4, #12]
 80223fc:	2b00      	cmp	r3, #0
 80223fe:	d056      	beq.n	80224ae <RMP_Sem_Post+0xfe>
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);

        /* Finally, return success */
        Thread->Retval=0;
 8022400:	4639      	mov	r1, r7
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8022402:	f8df e17c 	ldr.w	lr, [pc, #380]	; 8022580 <RMP_Sem_Post+0x1d0>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8022406:	4f5c      	ldr	r7, [pc, #368]	; (8022578 <RMP_Sem_Post+0x1c8>)
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 8022408:	f8df 8178 	ldr.w	r8, [pc, #376]	; 8022584 <RMP_Sem_Post+0x1d4>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 802240c:	f8df c16c 	ldr.w	ip, [pc, #364]	; 802257c <RMP_Sem_Post+0x1cc>
 8022410:	e011      	b.n	8022436 <RMP_Sem_Post+0x86>
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 8022412:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
 8022416:	f042 0201 	orr.w	r2, r2, #1
 802241a:	619a      	str	r2, [r3, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 802241c:	699a      	ldr	r2, [r3, #24]
 802241e:	05d2      	lsls	r2, r2, #23
 8022420:	d51f      	bpl.n	8022462 <RMP_Sem_Post+0xb2>
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);

        /* Finally, return success */
        Thread->Retval=0;
 8022422:	6399      	str	r1, [r3, #56]	; 0x38
        Semaphore->Cur_Num--;
 8022424:	68e3      	ldr	r3, [r4, #12]
 8022426:	3b01      	subs	r3, #1
 8022428:	60e3      	str	r3, [r4, #12]
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
 802242a:	6863      	ldr	r3, [r4, #4]
 802242c:	42a3      	cmp	r3, r4
 802242e:	d03e      	beq.n	80224ae <RMP_Sem_Post+0xfe>
 8022430:	68e3      	ldr	r3, [r4, #12]
 8022432:	2b00      	cmp	r3, #0
 8022434:	d03b      	beq.n	80224ae <RMP_Sem_Post+0xfe>
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
 8022436:	6863      	ldr	r3, [r4, #4]
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 8022438:	e893 0204 	ldmia.w	r3, {r2, r9}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802243c:	f8c9 2000 	str.w	r2, [r9]
    Prev->Next=(struct RMP_List*)Next;
 8022440:	f8c2 9004 	str.w	r9, [r2, #4]
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
 8022444:	699a      	ldr	r2, [r3, #24]
 8022446:	fa5f f982 	uxtb.w	r9, r2
 802244a:	f1b9 0f08 	cmp.w	r9, #8
 802244e:	d1e0      	bne.n	8022412 <RMP_Sem_Post+0x62>
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 8022450:	689a      	ldr	r2, [r3, #8]
 8022452:	f8d3 900c 	ldr.w	r9, [r3, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022456:	f8c9 2000 	str.w	r2, [r9]
    Prev->Next=(struct RMP_List*)Next;
 802245a:	f8c2 9004 	str.w	r9, [r2, #4]
 802245e:	699a      	ldr	r2, [r3, #24]
 8022460:	e7d7      	b.n	8022412 <RMP_Sem_Post+0x62>
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8022462:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 8022464:	f857 2032 	ldr.w	r2, [r7, r2, lsl #3]
 8022468:	f8d3 9024 	ldr.w	r9, [r3, #36]	; 0x24
 802246c:	eb07 0ac9 	add.w	sl, r7, r9, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022470:	f847 3039 	str.w	r3, [r7, r9, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8022474:	f8c3 a004 	str.w	sl, [r3, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022478:	601a      	str	r2, [r3, #0]
    Prev->Next=(struct RMP_List*)New;
 802247a:	6053      	str	r3, [r2, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 802247c:	6a5a      	ldr	r2, [r3, #36]	; 0x24
 802247e:	f8d3 9024 	ldr.w	r9, [r3, #36]	; 0x24
 8022482:	f002 021f 	and.w	r2, r2, #31
 8022486:	ea4f 1959 	mov.w	r9, r9, lsr #5
 802248a:	fa00 f202 	lsl.w	r2, r0, r2
 802248e:	f85e a029 	ldr.w	sl, [lr, r9, lsl #2]
 8022492:	ea42 020a 	orr.w	r2, r2, sl
 8022496:	f84e 2029 	str.w	r2, [lr, r9, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 802249a:	f8d3 9024 	ldr.w	r9, [r3, #36]	; 0x24
 802249e:	f8d8 2000 	ldr.w	r2, [r8]
 80224a2:	6a52      	ldr	r2, [r2, #36]	; 0x24
 80224a4:	4591      	cmp	r9, r2
 80224a6:	d9bc      	bls.n	8022422 <RMP_Sem_Post+0x72>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 80224a8:	f8cc 0000 	str.w	r0, [ip]
 80224ac:	e7b9      	b.n	8022422 <RMP_Sem_Post+0x72>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80224ae:	682a      	ldr	r2, [r5, #0]
 80224b0:	4b2f      	ldr	r3, [pc, #188]	; (8022570 <RMP_Sem_Post+0x1c0>)
 80224b2:	2a01      	cmp	r2, #1
 80224b4:	d008      	beq.n	80224c8 <RMP_Sem_Post+0x118>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80224b6:	681a      	ldr	r2, [r3, #0]
 80224b8:	2a01      	cmp	r2, #1
 80224ba:	d911      	bls.n	80224e0 <RMP_Sem_Post+0x130>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80224bc:	681a      	ldr	r2, [r3, #0]
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();
    return 0;
 80224be:	2000      	movs	r0, #0
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80224c0:	3a01      	subs	r2, #1
 80224c2:	601a      	str	r2, [r3, #0]
 80224c4:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80224c8:	2200      	movs	r2, #0
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80224ca:	492c      	ldr	r1, [pc, #176]	; (802257c <RMP_Sem_Post+0x1cc>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 80224cc:	601a      	str	r2, [r3, #0]
        RMP_Sched_Locked=0;
 80224ce:	6032      	str	r2, [r6, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 80224d0:	680b      	ldr	r3, [r1, #0]
 80224d2:	2b00      	cmp	r3, #0
 80224d4:	d135      	bne.n	8022542 <RMP_Sem_Post+0x192>
            _RMP_Yield();
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 80224d6:	f7fe fead 	bl	8021234 <RVM_Unmask_Int>
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();
    return 0;
 80224da:	2000      	movs	r0, #0
 80224dc:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80224e0:	e7fe      	b.n	80224e0 <RMP_Sem_Post+0x130>
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80224e2:	682b      	ldr	r3, [r5, #0]
 80224e4:	2b01      	cmp	r3, #1
 80224e6:	d035      	beq.n	8022554 <RMP_Sem_Post+0x1a4>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 80224e8:	682b      	ldr	r3, [r5, #0]
 80224ea:	2b01      	cmp	r3, #1
 80224ec:	d931      	bls.n	8022552 <RMP_Sem_Post+0x1a2>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80224ee:	682b      	ldr	r3, [r5, #0]
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
 80224f0:	f06f 0004 	mvn.w	r0, #4
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 80224f4:	3b01      	subs	r3, #1
 80224f6:	602b      	str	r3, [r5, #0]
 80224f8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Unlock_Sched(void)
{
    if(RMP_Sched_Lock_Cnt==1)
 80224fc:	682b      	ldr	r3, [r5, #0]
 80224fe:	2b01      	cmp	r3, #1
 8022500:	d00e      	beq.n	8022520 <RMP_Sem_Post+0x170>
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
 8022502:	682b      	ldr	r3, [r5, #0]
 8022504:	2b01      	cmp	r3, #1
 8022506:	d90a      	bls.n	802251e <RMP_Sem_Post+0x16e>
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 8022508:	682b      	ldr	r3, [r5, #0]
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 802250a:	f06f 0005 	mvn.w	r0, #5
        RMP_UNMASK_INT();
    }
    else if(RMP_Sched_Lock_Cnt>1)
    {
        RMP_COVERAGE_MARKER();
        RMP_Sched_Lock_Cnt--;
 802250e:	3b01      	subs	r3, #1
 8022510:	602b      	str	r3, [r5, #0]
 8022512:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
 8022516:	f06f 0005 	mvn.w	r0, #5
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();
    return 0;
}
 802251a:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 802251e:	e7fe      	b.n	802251e <RMP_Sem_Post+0x16e>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022520:	4b16      	ldr	r3, [pc, #88]	; (802257c <RMP_Sem_Post+0x1cc>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022522:	602f      	str	r7, [r5, #0]
        RMP_Sched_Locked=0;
 8022524:	6037      	str	r7, [r6, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022526:	681a      	ldr	r2, [r3, #0]
 8022528:	b112      	cbz	r2, 8022530 <RMP_Sem_Post+0x180>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802252a:	601f      	str	r7, [r3, #0]
            _RMP_Yield();
 802252c:	f7fe fde0 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022530:	f7fe fe80 	bl	8021234 <RVM_Unmask_Int>
    
    if(Semaphore->State!=RMP_SEM_USED)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_SEM;
 8022534:	f06f 0005 	mvn.w	r0, #5
 8022538:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
    
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
 802253c:	f06f 0005 	mvn.w	r0, #5
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();
    return 0;
}
 8022540:	4770      	bx	lr
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 8022542:	600a      	str	r2, [r1, #0]
            _RMP_Yield();
 8022544:	f7fe fdd4 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022548:	f7fe fe74 	bl	8021234 <RVM_Unmask_Int>
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    RMP_Unlock_Sched();
    return 0;
 802254c:	2000      	movs	r0, #0
 802254e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8022552:	e7fe      	b.n	8022552 <RMP_Sem_Post+0x1a2>
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
        RMP_Sched_Locked=0;
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 8022554:	4b09      	ldr	r3, [pc, #36]	; (802257c <RMP_Sem_Post+0x1cc>)
{
    if(RMP_Sched_Lock_Cnt==1)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the count before enabling */
        RMP_Sched_Lock_Cnt=0;
 8022556:	602f      	str	r7, [r5, #0]
        RMP_Sched_Locked=0;
 8022558:	6037      	str	r7, [r6, #0]
        /* Now see if the scheduler scheduling action is pended in the lock-unlock 
         * period. If yes, perform a schedule now */
        if(RMP_Sched_Pend!=0)
 802255a:	681a      	ldr	r2, [r3, #0]
 802255c:	b112      	cbz	r2, 8022564 <RMP_Sem_Post+0x1b4>
        {
            RMP_COVERAGE_MARKER();
            /* Reset the count and trigger the context switch */
            RMP_Sched_Pend=0;
 802255e:	601f      	str	r7, [r3, #0]
            _RMP_Yield();
 8022560:	f7fe fdc6 	bl	80210f0 <_RMP_Yield>
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_UNMASK_INT();
 8022564:	f7fe fe66 	bl	8021234 <RVM_Unmask_Int>
    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        RMP_Unlock_Sched();
        return RMP_ERR_OPER;
 8022568:	f06f 0004 	mvn.w	r0, #4
 802256c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8022570:	20028774 	.word	0x20028774
 8022574:	200287c0 	.word	0x200287c0
 8022578:	20028674 	.word	0x20028674
 802257c:	20028670 	.word	0x20028670
 8022580:	2002866c 	.word	0x2002866c
 8022584:	2002920c 	.word	0x2002920c

08022588 <RMP_Sem_Post_ISR>:
******************************************************************************/
rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
    struct RMP_Thd* Thread;
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
 8022588:	2800      	cmp	r0, #0
 802258a:	d071      	beq.n	8022670 <RMP_Sem_Post_ISR+0xe8>
 802258c:	2900      	cmp	r1, #0
 802258e:	d06f      	beq.n	8022670 <RMP_Sem_Post_ISR+0xe8>
              rmp_ptr_t Number - The number to post.
Output      : None.
Return      : rmp_ret_t - If successful, 0; or an error code.
******************************************************************************/
rmp_ret_t RMP_Sem_Post_ISR(volatile struct RMP_Sem* Semaphore, rmp_ptr_t Number)
{
 8022590:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
        return RMP_ERR_SEM;
    }
    else
        RMP_COVERAGE_MARKER();
    
    if(Semaphore->State!=RMP_SEM_USED)
 8022594:	6883      	ldr	r3, [r0, #8]
 8022596:	2b01      	cmp	r3, #1
 8022598:	d166      	bne.n	8022668 <RMP_Sem_Post_ISR+0xe0>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
 802259a:	68c3      	ldr	r3, [r0, #12]
 802259c:	440b      	add	r3, r1
 802259e:	f5b3 7f7a 	cmp.w	r3, #1000	; 0x3e8
 80225a2:	d268      	bcs.n	8022676 <RMP_Sem_Post_ISR+0xee>
        return RMP_ERR_OPER;
    }
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
 80225a4:	68c3      	ldr	r3, [r0, #12]
 80225a6:	4419      	add	r1, r3
 80225a8:	60c1      	str	r1, [r0, #12]
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
 80225aa:	6843      	ldr	r3, [r0, #4]
 80225ac:	4298      	cmp	r0, r3
 80225ae:	d058      	beq.n	8022662 <RMP_Sem_Post_ISR+0xda>
 80225b0:	68c3      	ldr	r3, [r0, #12]
 80225b2:	2b00      	cmp	r3, #0
 80225b4:	d055      	beq.n	8022662 <RMP_Sem_Post_ISR+0xda>
 80225b6:	4605      	mov	r5, r0
 80225b8:	4f31      	ldr	r7, [pc, #196]	; (8022680 <RMP_Sem_Post_ISR+0xf8>)
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 80225ba:	f8df 80cc 	ldr.w	r8, [pc, #204]	; 8022688 <RMP_Sem_Post_ISR+0x100>
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 80225be:	f8df 90cc 	ldr.w	r9, [pc, #204]	; 802268c <RMP_Sem_Post_ISR+0x104>
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 80225c2:	4e30      	ldr	r6, [pc, #192]	; (8022684 <RMP_Sem_Post_ISR+0xfc>)
 80225c4:	e00d      	b.n	80225e2 <RMP_Sem_Post_ISR+0x5a>
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);
        
        /* If schedule pending, trigger it now because we are in ISR */
        if(RMP_Sched_Pend!=0)
 80225c6:	683b      	ldr	r3, [r7, #0]
 80225c8:	2b00      	cmp	r3, #0
 80225ca:	d13f      	bne.n	802264c <RMP_Sem_Post_ISR+0xc4>
        }
        else
            RMP_COVERAGE_MARKER();

        /* Finally, return success */
        Thread->Retval=0;
 80225cc:	2300      	movs	r3, #0
 80225ce:	63a3      	str	r3, [r4, #56]	; 0x38
        Semaphore->Cur_Num--;
 80225d0:	68eb      	ldr	r3, [r5, #12]
 80225d2:	3b01      	subs	r3, #1
 80225d4:	60eb      	str	r3, [r5, #12]
    else
        RMP_COVERAGE_MARKER();
    
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
 80225d6:	686b      	ldr	r3, [r5, #4]
 80225d8:	42ab      	cmp	r3, r5
 80225da:	d042      	beq.n	8022662 <RMP_Sem_Post_ISR+0xda>
 80225dc:	68eb      	ldr	r3, [r5, #12]
 80225de:	2b00      	cmp	r3, #0
 80225e0:	d03f      	beq.n	8022662 <RMP_Sem_Post_ISR+0xda>
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
 80225e2:	686c      	ldr	r4, [r5, #4]
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);
        
        /* If schedule pending, trigger it now because we are in ISR */
        if(RMP_Sched_Pend!=0)
 80225e4:	4926      	ldr	r1, [pc, #152]	; (8022680 <RMP_Sem_Post_ISR+0xf8>)
    Semaphore->Cur_Num+=Number;
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
 80225e6:	6823      	ldr	r3, [r4, #0]
 80225e8:	6862      	ldr	r2, [r4, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 80225ea:	6013      	str	r3, [r2, #0]
    Prev->Next=(struct RMP_List*)Next;
 80225ec:	605a      	str	r2, [r3, #4]
    /* Is there any thread waiting on it? If there are, clean them up*/
    while((&(Semaphore->Wait_List)!=Semaphore->Wait_List.Next)&&(Semaphore->Cur_Num!=0))
    {
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
 80225ee:	69a3      	ldr	r3, [r4, #24]
 80225f0:	b2da      	uxtb	r2, r3
 80225f2:	2a08      	cmp	r2, #8
 80225f4:	d02f      	beq.n	8022656 <RMP_Sem_Post_ISR+0xce>
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
        }
        else
            RMP_COVERAGE_MARKER();
        
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
 80225f6:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
 80225fa:	f043 0301 	orr.w	r3, r3, #1
 80225fe:	61a3      	str	r3, [r4, #24]
Return      : None.
******************************************************************************/
void _RMP_Set_Rdy(volatile struct RMP_Thd* Thread)
{        
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
 8022600:	69a3      	ldr	r3, [r4, #24]
 8022602:	05db      	lsls	r3, r3, #23
 8022604:	d4df      	bmi.n	80225c6 <RMP_Sem_Post_ISR+0x3e>
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 8022606:	6a63      	ldr	r3, [r4, #36]	; 0x24
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8022608:	2001      	movs	r0, #1
    /* Is it suspended? If yes, we can't directly set it running */
    if((Thread->State&RMP_THD_SUSPENDED)==0)
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
 802260a:	f858 3033 	ldr.w	r3, [r8, r3, lsl #3]
 802260e:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8022610:	eb08 0ec2 	add.w	lr, r8, r2, lsl #3
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022614:	f848 4032 	str.w	r4, [r8, r2, lsl #3]
    New->Next=(struct RMP_List*)Next;
 8022618:	f8c4 e004 	str.w	lr, [r4, #4]
    New->Prev=(struct RMP_List*)Prev;
 802261c:	6023      	str	r3, [r4, #0]
    Prev->Next=(struct RMP_List*)New;
 802261e:	605c      	str	r4, [r3, #4]
    {
        RMP_COVERAGE_MARKER();
        /* Insert this into the corresponding runqueue */
        RMP_List_Ins(&(Thread->Run_Head),RMP_Run[Thread->Prio].Prev,&(RMP_Run[Thread->Prio]));
        /* Set this runlevel as active */
        RMP_Bitmap[Thread->Prio>>RMP_WORD_ORDER]|=((rmp_ptr_t)1)<<(Thread->Prio&RMP_WORD_MASK);
 8022620:	6a63      	ldr	r3, [r4, #36]	; 0x24
 8022622:	6a62      	ldr	r2, [r4, #36]	; 0x24
 8022624:	f003 031f 	and.w	r3, r3, #31
 8022628:	0952      	lsrs	r2, r2, #5
 802262a:	fa00 f303 	lsl.w	r3, r0, r3
 802262e:	f859 e022 	ldr.w	lr, [r9, r2, lsl #2]
 8022632:	ea43 030e 	orr.w	r3, r3, lr
 8022636:	f849 3022 	str.w	r3, [r9, r2, lsl #2]
        
        /* Compare this with the current one to see if we need a context switch */
        if(Thread->Prio>RMP_Cur_Thd->Prio)
 802263a:	6a62      	ldr	r2, [r4, #36]	; 0x24
 802263c:	6833      	ldr	r3, [r6, #0]
 802263e:	6a5b      	ldr	r3, [r3, #36]	; 0x24
 8022640:	429a      	cmp	r2, r3
 8022642:	d9c0      	bls.n	80225c6 <RMP_Sem_Post_ISR+0x3e>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=1;
 8022644:	6038      	str	r0, [r7, #0]
        RMP_THD_STATE_SET(Thread->State,RMP_THD_RUNNING);
        /* Set to running if not suspended */
        _RMP_Set_Rdy(Thread);
        
        /* If schedule pending, trigger it now because we are in ISR */
        if(RMP_Sched_Pend!=0)
 8022646:	683b      	ldr	r3, [r7, #0]
 8022648:	2b00      	cmp	r3, #0
 802264a:	d0bf      	beq.n	80225cc <RMP_Sem_Post_ISR+0x44>
        {
            RMP_COVERAGE_MARKER();
            RMP_Sched_Pend=0;
 802264c:	2300      	movs	r3, #0
 802264e:	600b      	str	r3, [r1, #0]
            _RMP_Yield();   
 8022650:	f7fe fd4e 	bl	80210f0 <_RMP_Yield>
 8022654:	e7ba      	b.n	80225cc <RMP_Sem_Post_ISR+0x44>
        Thread=(struct RMP_Thd*)(Semaphore->Wait_List.Next);
        RMP_List_Del(Thread->Run_Head.Prev,Thread->Run_Head.Next);
        if(RMP_THD_STATE(Thread->State)==RMP_THD_SEMDLY)
        {
            RMP_COVERAGE_MARKER();
            RMP_List_Del(Thread->Dly_Head.Prev,Thread->Dly_Head.Next);
 8022656:	68a3      	ldr	r3, [r4, #8]
 8022658:	68e2      	ldr	r2, [r4, #12]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 802265a:	6013      	str	r3, [r2, #0]
    Prev->Next=(struct RMP_List*)Next;
 802265c:	605a      	str	r2, [r3, #4]
 802265e:	69a3      	ldr	r3, [r4, #24]
 8022660:	e7c9      	b.n	80225f6 <RMP_Sem_Post_ISR+0x6e>
        /* Finally, return success */
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    return 0;
 8022662:	2000      	movs	r0, #0
 8022664:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
    struct RMP_Thd* Thread;
    /* Check if this semaphore structure could possibly be in use */
    if((Semaphore==0)||(Number==0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_SEM;
 8022668:	f06f 0005 	mvn.w	r0, #5
 802266c:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8022670:	f06f 0005 	mvn.w	r0, #5
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    return 0;
}
 8022674:	4770      	bx	lr

    /* Would the maximum value be exceeded if this is posted? */
    if((Semaphore->Cur_Num+Number)>=RMP_SEM_MAX_NUM)
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_OPER;
 8022676:	f06f 0004 	mvn.w	r0, #4
        Thread->Retval=0;
        Semaphore->Cur_Num--;
    }

    return 0;
}
 802267a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 802267e:	bf00      	nop
 8022680:	20028670 	.word	0x20028670
 8022684:	2002920c 	.word	0x2002920c
 8022688:	20028674 	.word	0x20028674
 802268c:	2002866c 	.word	0x2002866c

08022690 <RMP_Save_Ctx>:
 8022690:	4770      	bx	lr
 8022692:	bf00      	nop

08022694 <RMP_Load_Ctx>:
Output      : None.
Return      : None.
******************************************************************************/
#if(RMP_USE_HOOKS==RMP_FALSE)
void RMP_Load_Ctx(void)
{
 8022694:	4770      	bx	lr
 8022696:	bf00      	nop

08022698 <main>:
Input       : None.
Output      : None.
Return      : int - This function never returns.
******************************************************************************/
int main(void)
{
 8022698:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
    for(Count=0;Count<RMP_COVERAGE_LINES;Count++)
        RMP_Coverage[Count]=0;
#endif
    
    /* Initialize the kernel data structures first */
    _RMP_Low_Level_Init();
 802269a:	f7fe fced 	bl	8021078 <_RMP_Low_Level_Init>
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
 802269e:	4d22      	ldr	r5, [pc, #136]	; (8022728 <main+0x90>)
 80226a0:	2000      	movs	r0, #0
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
 80226a2:	4c22      	ldr	r4, [pc, #136]	; (802272c <main+0x94>)
 80226a4:	4922      	ldr	r1, [pc, #136]	; (8022730 <main+0x98>)
    
#if(RMP_USE_HOOKS==RMP_TRUE)
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
 80226a6:	6028      	str	r0, [r5, #0]
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 80226a8:	4603      	mov	r3, r0
    RMP_Start_Hook();
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
 80226aa:	6020      	str	r0, [r4, #0]
 80226ac:	460a      	mov	r2, r1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226ae:	4c21      	ldr	r4, [pc, #132]	; (8022734 <main+0x9c>)
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
    RMP_Sched_Locked=0;
 80226b0:	4f21      	ldr	r7, [pc, #132]	; (8022738 <main+0xa0>)
    RMP_Sched_Pend=0;
 80226b2:	4e22      	ldr	r6, [pc, #136]	; (802273c <main+0xa4>)
    RMP_Timer_Pend=0;
 80226b4:	4d22      	ldr	r5, [pc, #136]	; (8022740 <main+0xa8>)
#endif
    
    RMP_Tick=0;
    /* Now initialize the kernel data structures */
    RMP_Sched_Lock_Cnt=0;
    RMP_Sched_Locked=0;
 80226b6:	6038      	str	r0, [r7, #0]
    RMP_Sched_Pend=0;
 80226b8:	6030      	str	r0, [r6, #0]
    RMP_Timer_Pend=0;
 80226ba:	6028      	str	r0, [r5, #0]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226bc:	6024      	str	r4, [r4, #0]
    Head->Next=(struct RMP_List*)Head;
 80226be:	6064      	str	r4, [r4, #4]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226c0:	f841 2033 	str.w	r2, [r1, r3, lsl #3]
    Head->Next=(struct RMP_List*)Head;
 80226c4:	eb01 00c3 	add.w	r0, r1, r3, lsl #3
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 80226c8:	3301      	adds	r3, #1
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226ca:	4c19      	ldr	r4, [pc, #100]	; (8022730 <main+0x98>)
    Head->Next=(struct RMP_List*)Head;
 80226cc:	6042      	str	r2, [r0, #4]
 80226ce:	3208      	adds	r2, #8
    RMP_Sched_Pend=0;
    RMP_Timer_Pend=0;
    
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
 80226d0:	2b20      	cmp	r3, #32
 80226d2:	d1f5      	bne.n	80226c0 <main+0x28>
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 80226d4:	4b1b      	ldr	r3, [pc, #108]	; (8022744 <main+0xac>)
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
        RMP_List_Crt(&RMP_Run[Count]);
    for(Count=0;Count<RMP_BITMAP_SIZE;Count++)
        RMP_Bitmap[Count]=0;
 80226d6:	2500      	movs	r5, #0
 80226d8:	4a1b      	ldr	r2, [pc, #108]	; (8022748 <main+0xb0>)
        
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
    RMP_Init_Thd.Slices=10;
 80226da:	200a      	movs	r0, #10
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226dc:	f103 0110 	add.w	r1, r3, #16
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
    RMP_Init_Thd.Slices=10;
    RMP_Init_Thd.Slices_Left=10;
    RMP_Init_Thd.State=RMP_THD_RUNNING;
 80226e0:	2701      	movs	r7, #1
    RMP_Init_Thd.Stack=RMP_INIT_STACK;
 80226e2:	4e1a      	ldr	r6, [pc, #104]	; (802274c <main+0xb4>)
    /* Linked lists */
    RMP_List_Crt(&RMP_Delay);
    for(Count=0;Count<RMP_MAX_PREEMPT_PRIO;Count++)
        RMP_List_Crt(&RMP_Run[Count]);
    for(Count=0;Count<RMP_BITMAP_SIZE;Count++)
        RMP_Bitmap[Count]=0;
 80226e4:	6015      	str	r5, [r2, #0]
        
    /* Now boot into the first thread */
    RMP_Clear(&RMP_Init_Thd,sizeof(struct RMP_Thd));
    RMP_Init_Thd.Prio=0;
 80226e6:	625d      	str	r5, [r3, #36]	; 0x24
    RMP_Init_Thd.Slices=10;
 80226e8:	61d8      	str	r0, [r3, #28]
    RMP_Init_Thd.Slices_Left=10;
 80226ea:	6218      	str	r0, [r3, #32]
    RMP_Init_Thd.State=RMP_THD_RUNNING;
 80226ec:	619f      	str	r7, [r3, #24]
    RMP_Init_Thd.Stack=RMP_INIT_STACK;
 80226ee:	635e      	str	r6, [r3, #52]	; 0x34
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80226f0:	6119      	str	r1, [r3, #16]
    Head->Next=(struct RMP_List*)Head;
 80226f2:	6159      	str	r1, [r3, #20]
    
    /* Initialize sending list */
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
 80226f4:	6821      	ldr	r1, [r4, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 80226f6:	609d      	str	r5, [r3, #8]
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 80226f8:	6023      	str	r3, [r4, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 80226fa:	60dd      	str	r5, [r3, #12]
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
 80226fc:	605c      	str	r4, [r3, #4]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 80226fe:	629d      	str	r5, [r3, #40]	; 0x28
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
 8022700:	6019      	str	r1, [r3, #0]
    rmp_u8_t* Ptr;
    rmp_cnt_t Count;
   
    Ptr=(rmp_u8_t*)Addr;
    for(Count=0;Count<(rmp_cnt_t)Size;Count++)
        Ptr[Count]=0;
 8022702:	62dd      	str	r5, [r3, #44]	; 0x2c
 8022704:	631d      	str	r5, [r3, #48]	; 0x30
 8022706:	639d      	str	r5, [r3, #56]	; 0x38
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
    New->Next=(struct RMP_List*)Next;
    New->Prev=(struct RMP_List*)Prev;
    Prev->Next=(struct RMP_List*)New;
 8022708:	604b      	str	r3, [r1, #4]
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 802270a:	6811      	ldr	r1, [r2, #0]
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
 802270c:	4e10      	ldr	r6, [pc, #64]	; (8022750 <main+0xb8>)
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 802270e:	4339      	orrs	r1, r7
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
    RMP_Cur_SP=RMP_Init_Thd.Stack;
 8022710:	4c10      	ldr	r4, [pc, #64]	; (8022754 <main+0xbc>)
    
    /* Now jump to the user function and will never return. Initialization of stack is not needed */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
 8022712:	4811      	ldr	r0, [pc, #68]	; (8022758 <main+0xc0>)
    RMP_List_Crt(&(RMP_Init_Thd.Snd_List));
    
    /* Insert this into the corresponding runqueue */
    RMP_List_Ins(&(RMP_Init_Thd.Run_Head),RMP_Run[0].Prev,&(RMP_Run[0]));
    /* Set this runlevel as active - in fact it is always active */
    RMP_Bitmap[0]|=1;
 8022714:	6011      	str	r1, [r2, #0]
    
    /* Set current thread and stack */
    RMP_Cur_Thd=(struct RMP_Thd*)(&RMP_Init_Thd);
 8022716:	6033      	str	r3, [r6, #0]
    RMP_Cur_SP=RMP_Init_Thd.Stack;
 8022718:	6b5a      	ldr	r2, [r3, #52]	; 0x34
 802271a:	6022      	str	r2, [r4, #0]
    
    /* Now jump to the user function and will never return. Initialization of stack is not needed */
    _RMP_Start((rmp_ptr_t)RMP_Init, (rmp_ptr_t)RMP_Init_Thd.Stack);
 802271c:	6b59      	ldr	r1, [r3, #52]	; 0x34
 802271e:	f7fe fcf2 	bl	8021106 <_RMP_Start>
    
    return 0;
}
 8022722:	4628      	mov	r0, r5
 8022724:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 8022726:	bf00      	nop
 8022728:	20029210 	.word	0x20029210
 802272c:	20028774 	.word	0x20028774
 8022730:	20028674 	.word	0x20028674
 8022734:	200287b8 	.word	0x200287b8
 8022738:	200287c0 	.word	0x200287c0
 802273c:	20028670 	.word	0x20028670
 8022740:	20028778 	.word	0x20028778
 8022744:	2002877c 	.word	0x2002877c
 8022748:	2002866c 	.word	0x2002866c
 802274c:	20028628 	.word	0x20028628
 8022750:	2002920c 	.word	0x2002920c
 8022754:	20029208 	.word	0x20029208
 8022758:	08021501 	.word	0x08021501

0802275c <RMP_Mem_Init>:
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_Mem_Head* Mem_Head;
    
    /* See if the memory pool is large enough to enable dynamic allocation - at
     * least 4096 words and no more than 128MB */
    if((Pool==0)||(Size<(4096*sizeof(rmp_ptr_t)))||(((Size>>15)>>12)>0))
 802275c:	2800      	cmp	r0, #0
 802275e:	f000 809c 	beq.w	802289a <RMP_Mem_Init+0x13e>
 8022762:	f5b1 4f80 	cmp.w	r1, #16384	; 0x4000
 8022766:	f0c0 8098 	bcc.w	802289a <RMP_Mem_Init+0x13e>
 802276a:	0ecb      	lsrs	r3, r1, #27
 802276c:	f040 8095 	bne.w	802289a <RMP_Mem_Init+0x13e>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address and size is word-aligned */
    if((((rmp_ptr_t)Pool&(RMP_WORD_MASK>>3))!=0)||((Size&(RMP_WORD_MASK>>3))!=0))
 8022770:	0783      	lsls	r3, r0, #30
 8022772:	f040 8092 	bne.w	802289a <RMP_Mem_Init+0x13e>
              rmp_ptr_t Size - The size of the memory pool, word-aligned.
Output      : None.
Return      : rmp_ret_t - If successful, 0; else an error code.
******************************************************************************/
rmp_ret_t RMP_Mem_Init(volatile void* Pool, rmp_ptr_t Size)
{
 8022776:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address and size is word-aligned */
    if((((rmp_ptr_t)Pool&(RMP_WORD_MASK>>3))!=0)||((Size&(RMP_WORD_MASK>>3))!=0))
 802277a:	f011 0503 	ands.w	r5, r1, #3
 802277e:	f040 808f 	bne.w	80228a0 <RMP_Mem_Init+0x144>
 8022782:	4604      	mov	r4, r0
    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Initialize the allocated block list */
    RMP_List_Crt(&(Mem->Alloc));
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
 8022784:	f5a1 700a 	sub.w	r0, r1, #552	; 0x228
 8022788:	468a      	mov	sl, r1
    }
    else
        RMP_COVERAGE_MARKER();
        
    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
 802278a:	6121      	str	r1, [r4, #16]
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802278c:	6024      	str	r4, [r4, #0]
    Head->Next=(struct RMP_List*)Head;
 802278e:	6064      	str	r4, [r4, #4]
    Mem=(volatile struct RMP_Mem*)Pool;
    Mem->Size=Size;
    /* Initialize the allocated block list */
    RMP_List_Crt(&(Mem->Alloc));
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
 8022790:	f7fe fcb2 	bl	80210f8 <RMP_MSB_Get>
 8022794:	3805      	subs	r0, #5
 8022796:	60a0      	str	r0, [r4, #8]
    /* Initialize the TLSF allocation table first */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
 8022798:	68a3      	ldr	r3, [r4, #8]
 802279a:	2b00      	cmp	r3, #0
 802279c:	dd3c      	ble.n	8022818 <RMP_Mem_Init+0xbc>
 802279e:	4628      	mov	r0, r5
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
 80227a0:	00c2      	lsls	r2, r0, #3
 80227a2:	eb04 1380 	add.w	r3, r4, r0, lsl #6
    /* Initialize the allocated block list */
    RMP_List_Crt(&(Mem->Alloc));
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
    /* Initialize the TLSF allocation table first */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
 80227a6:	3001      	adds	r0, #1
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
 80227a8:	f102 0905 	add.w	r9, r2, #5
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
 80227ac:	f102 0806 	add.w	r8, r2, #6
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
 80227b0:	f102 0c07 	add.w	ip, r2, #7
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
 80227b4:	f102 0e08 	add.w	lr, r2, #8
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
 80227b8:	f102 0109 	add.w	r1, r2, #9
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
 80227bc:	f102 060a 	add.w	r6, r2, #10
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
 80227c0:	f102 050b 	add.w	r5, r2, #11
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,7)]));
 80227c4:	320c      	adds	r2, #12
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
    /* Initialize the TLSF allocation table first */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
 80227c6:	eb04 09c9 	add.w	r9, r4, r9, lsl #3
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
 80227ca:	eb04 08c8 	add.w	r8, r4, r8, lsl #3
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
 80227ce:	eb04 0ccc 	add.w	ip, r4, ip, lsl #3
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80227d2:	f8c3 9028 	str.w	r9, [r3, #40]	; 0x28
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
 80227d6:	eb04 07ce 	add.w	r7, r4, lr, lsl #3
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
    Head->Next=(struct RMP_List*)Head;
 80227da:	f8c3 902c 	str.w	r9, [r3, #44]	; 0x2c
    {
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
 80227de:	eb04 01c1 	add.w	r1, r4, r1, lsl #3
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80227e2:	f8c3 8030 	str.w	r8, [r3, #48]	; 0x30
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,0)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
 80227e6:	eb04 06c6 	add.w	r6, r4, r6, lsl #3
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
    Head->Next=(struct RMP_List*)Head;
 80227ea:	f8c3 8034 	str.w	r8, [r3, #52]	; 0x34
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,1)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
 80227ee:	eb04 05c5 	add.w	r5, r4, r5, lsl #3
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80227f2:	f8c3 c038 	str.w	ip, [r3, #56]	; 0x38
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,2)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,3)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,4)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,7)]));
 80227f6:	eb04 02c2 	add.w	r2, r4, r2, lsl #3
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
    Head->Next=(struct RMP_List*)Head;
 80227fa:	f8c3 c03c 	str.w	ip, [r3, #60]	; 0x3c
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 80227fe:	641f      	str	r7, [r3, #64]	; 0x40
    Head->Next=(struct RMP_List*)Head;
 8022800:	645f      	str	r7, [r3, #68]	; 0x44
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 8022802:	6499      	str	r1, [r3, #72]	; 0x48
    Head->Next=(struct RMP_List*)Head;
 8022804:	64d9      	str	r1, [r3, #76]	; 0x4c
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 8022806:	651e      	str	r6, [r3, #80]	; 0x50
    Head->Next=(struct RMP_List*)Head;
 8022808:	655e      	str	r6, [r3, #84]	; 0x54
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802280a:	659d      	str	r5, [r3, #88]	; 0x58
    Head->Next=(struct RMP_List*)Head;
 802280c:	65dd      	str	r5, [r3, #92]	; 0x5c
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Crt(volatile struct RMP_List* Head)
{
    Head->Prev=(struct RMP_List*)Head;
 802280e:	661a      	str	r2, [r3, #96]	; 0x60
    Head->Next=(struct RMP_List*)Head;
 8022810:	665a      	str	r2, [r3, #100]	; 0x64
    /* Initialize the allocated block list */
    RMP_List_Crt(&(Mem->Alloc));
    /* Calculate the FLI value needed for this - we always align to 64 byte */
    Mem->FLI_Num=RMP_MSB_Get(Size-sizeof(struct RMP_Mem))-6+1;
    /* Initialize the TLSF allocation table first */
    for(FLI_Cnt=0;FLI_Cnt<(rmp_cnt_t)(Mem->FLI_Num);FLI_Cnt++)
 8022812:	68a3      	ldr	r3, [r4, #8]
 8022814:	4283      	cmp	r3, r0
 8022816:	dcc3      	bgt.n	80227a0 <RMP_Mem_Init+0x44>
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,7)]));
    }
    for(FLI_Cnt=0;FLI_Cnt<5;FLI_Cnt++)
        Mem->Bitmap[FLI_Cnt]=0;
 8022818:	2300      	movs	r3, #0
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 802281a:	f1aa 0704 	sub.w	r7, sl, #4
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,5)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,6)]));
        RMP_List_Crt(&(Mem->Table[RMP_MEM_POS(FLI_Cnt,7)]));
    }
    for(FLI_Cnt=0;FLI_Cnt<5;FLI_Cnt++)
        Mem->Bitmap[FLI_Cnt]=0;
 802281e:	6163      	str	r3, [r4, #20]
 8022820:	61a3      	str	r3, [r4, #24]
 8022822:	61e3      	str	r3, [r4, #28]
 8022824:	6223      	str	r3, [r4, #32]
 8022826:	6263      	str	r3, [r4, #36]	; 0x24
    
    /* Get the big memory block's size and position */
    Usable_Size=sizeof(struct RMP_Mem)+(sizeof(struct RMP_List)<<3)*(Mem->FLI_Num-8);
 8022828:	68a2      	ldr	r2, [r4, #8]
 802282a:	ea4f 1882 	mov.w	r8, r2, lsl #6
 802282e:	f108 0828 	add.w	r8, r8, #40	; 0x28
    Mem_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)Pool)+Usable_Size);
 8022832:	eb04 0608 	add.w	r6, r4, r8
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022836:	ebc8 0707 	rsb	r7, r8, r7
 802283a:	4437      	add	r7, r6
        Mem->Bitmap[FLI_Cnt]=0;
    
    /* Get the big memory block's size and position */
    Usable_Size=sizeof(struct RMP_Mem)+(sizeof(struct RMP_List)<<3)*(Mem->FLI_Num-8);
    Mem_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)Pool)+Usable_Size);
    Mem->Start=(rmp_ptr_t)Mem_Head;
 802283c:	60e6      	str	r6, [r4, #12]
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 802283e:	60b3      	str	r3, [r6, #8]
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022840:	60f7      	str	r7, [r6, #12]
    Mem_Head->Tail->Head=Mem_Head;
 8022842:	68f3      	ldr	r3, [r6, #12]
 8022844:	601e      	str	r6, [r3, #0]
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022846:	68f5      	ldr	r5, [r6, #12]
 8022848:	3d10      	subs	r5, #16
 802284a:	1bad      	subs	r5, r5, r6

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 802284c:	4628      	mov	r0, r5
 802284e:	f7fe fc53 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022852:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022854:	3806      	subs	r0, #6
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022856:	40dd      	lsrs	r5, r3
 8022858:	f005 0507 	and.w	r5, r5, #7
 802285c:	eb05 00c0 	add.w	r0, r5, r0, lsl #3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
 8022860:	1d43      	adds	r3, r0, #5
 8022862:	eb04 01c0 	add.w	r1, r4, r0, lsl #3
 8022866:	eb04 03c3 	add.w	r3, r4, r3, lsl #3

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
 802286a:	6aca      	ldr	r2, [r1, #44]	; 0x2c
 802286c:	4293      	cmp	r3, r2
 802286e:	d008      	beq.n	8022882 <RMP_Mem_Init+0x126>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
 8022870:	6aca      	ldr	r2, [r1, #44]	; 0x2c
    /* Initialize the big block */
    _RMP_Mem_Block(Mem_Head,Usable_Size);
    
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool, Mem_Head);
    return 0;
 8022872:	2000      	movs	r0, #0
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022874:	6016      	str	r6, [r2, #0]
    New->Next=(struct RMP_List*)Next;
 8022876:	6072      	str	r2, [r6, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022878:	f844 3008 	str.w	r3, [r4, r8]
    Prev->Next=(struct RMP_List*)New;
 802287c:	62ce      	str	r6, [r1, #44]	; 0x2c
 802287e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8022882:	0942      	lsrs	r2, r0, #5
    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=1<<(Level&RMP_WORD_MASK);
 8022884:	2501      	movs	r5, #1
 8022886:	f000 001f 	and.w	r0, r0, #31
 802288a:	eb04 0282 	add.w	r2, r4, r2, lsl #2
 802288e:	fa05 f000 	lsl.w	r0, r5, r0
 8022892:	6955      	ldr	r5, [r2, #20]
 8022894:	4328      	orrs	r0, r5
 8022896:	6150      	str	r0, [r2, #20]
 8022898:	e7ea      	b.n	8022870 <RMP_Mem_Init+0x114>
    /* See if the memory pool is large enough to enable dynamic allocation - at
     * least 4096 words and no more than 128MB */
    if((Pool==0)||(Size<(4096*sizeof(rmp_ptr_t)))||(((Size>>15)>>12)>0))
    {
        RMP_COVERAGE_MARKER();
        return RMP_ERR_MEM;
 802289a:	f06f 0006 	mvn.w	r0, #6
 802289e:	4770      	bx	lr
 80228a0:	f06f 0006 	mvn.w	r0, #6
    _RMP_Mem_Block(Mem_Head,Usable_Size);
    
    /* Insert the memory into the corresponding level */
    _RMP_Mem_Ins(Pool, Mem_Head);
    return 0;
}
 80228a4:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}

080228a8 <RMP_Malloc>:
    volatile struct RMP_Mem_Head* Mem_Head;
    rmp_ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    rmp_ptr_t New_Size;
    
    if((Pool==0)||(Size==0))
 80228a8:	2800      	cmp	r0, #0
 80228aa:	f000 8135 	beq.w	8022b18 <RMP_Malloc+0x270>
              rmp_ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available, 0 is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool, rmp_ptr_t Size)                                                       
{    
 80228ae:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
    volatile struct RMP_Mem_Head* Mem_Head;
    rmp_ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    rmp_ptr_t New_Size;
    
    if((Pool==0)||(Size==0))
 80228b2:	fab1 f781 	clz	r7, r1
              rmp_ptr_t Size - The size of the RAM needed to allocate.
Output      : None.
Return      : void* - The pointer to the memory. If no memory available, 0 is returned.
******************************************************************************/
void* RMP_Malloc(volatile void* Pool, rmp_ptr_t Size)                                                       
{    
 80228b6:	b083      	sub	sp, #12
    volatile struct RMP_Mem_Head* Mem_Head;
    rmp_ptr_t Rounded_Size;
    volatile struct RMP_Mem_Head* New_Mem;
    rmp_ptr_t New_Size;
    
    if((Pool==0)||(Size==0))
 80228b8:	097f      	lsrs	r7, r7, #5
 80228ba:	2f00      	cmp	r7, #0
 80228bc:	f040 8089 	bne.w	80229d2 <RMP_Malloc+0x12a>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Round up the size:a multiple of 8 and bigger than 64B */
    Rounded_Size=(((Size-1)>>3)+1)<<3;
 80228c0:	1e4c      	subs	r4, r1, #1
 80228c2:	4605      	mov	r5, r0
 80228c4:	f024 0407 	bic.w	r4, r4, #7
 80228c8:	3408      	adds	r4, #8
    /* See if it is smaller than the smallest block */
    Rounded_Size=(Rounded_Size>64)?Rounded_Size:64;
 80228ca:	2c40      	cmp	r4, #64	; 0x40
 80228cc:	bf38      	it	cc
 80228ce:	2440      	movcc	r4, #64	; 0x40
    rmp_cnt_t Level;
    rmp_ptr_t LSB;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64. 64=2^6 */
    FLI_Level_Temp=RMP_MSB_Get(Size)-6;
 80228d0:	4620      	mov	r0, r4
 80228d2:	f7fe fc11 	bl	80210f8 <RMP_MSB_Get>
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * one of the levels, then we don't need to jump to the next level and can fit directly */
    SLI_Level_Temp=(Size>>(FLI_Level_Temp+3))&0x07;
 80228d6:	1ec1      	subs	r1, r0, #3
    rmp_cnt_t Level;
    rmp_ptr_t LSB;
    volatile struct RMP_Mem* Mem;

    /* Make sure that it is bigger than 64. 64=2^6 */
    FLI_Level_Temp=RMP_MSB_Get(Size)-6;
 80228d8:	1f86      	subs	r6, r0, #6
    
    /* Decide the SLI level directly from the FLI level. We plus the number by one here
     * so that we can avoid the list search. However, when the allocated memory is just
     * one of the levels, then we don't need to jump to the next level and can fit directly */
    SLI_Level_Temp=(Size>>(FLI_Level_Temp+3))&0x07;
 80228da:	fa24 f201 	lsr.w	r2, r4, r1
 80228de:	f002 0307 	and.w	r3, r2, #7
    if(((rmp_cnt_t)Size)!=((1<<(FLI_Level_Temp+3))*(SLI_Level_Temp+8)))
 80228e2:	f103 0208 	add.w	r2, r3, #8
 80228e6:	408a      	lsls	r2, r1
 80228e8:	42a2      	cmp	r2, r4
 80228ea:	d003      	beq.n	80228f4 <RMP_Malloc+0x4c>
    {
        RMP_COVERAGE_MARKER();
        SLI_Level_Temp++;
 80228ec:	3301      	adds	r3, #1
        
        /* If the SLI level is the largest of the SLI level, then jump to the next FLI level */
        if(SLI_Level_Temp==8)
 80228ee:	2b08      	cmp	r3, #8
 80228f0:	f000 80cd 	beq.w	8022a8e <RMP_Malloc+0x1e6>
    else
        RMP_COVERAGE_MARKER();
    
    /* Check if the FLI level is over the boundary */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((rmp_ptr_t)FLI_Level_Temp>=Mem->FLI_Num)
 80228f4:	68aa      	ldr	r2, [r5, #8]
 80228f6:	42b2      	cmp	r2, r6
 80228f8:	d96b      	bls.n	80229d2 <RMP_Malloc+0x12a>
    }
    else
        RMP_COVERAGE_MARKER();
    
    /* Try to find one position on this processor word level */
    Level=(FLI_Level_Temp<<3)+SLI_Level_Temp;
 80228fa:	eb03 06c6 	add.w	r6, r3, r6, lsl #3
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 80228fe:	f8df 924c 	ldr.w	r9, [pc, #588]	; 8022b4c <RMP_Malloc+0x2a4>
    else
        RMP_COVERAGE_MARKER();
    
    /* Try to find one position on this processor word level */
    Level=(FLI_Level_Temp<<3)+SLI_Level_Temp;
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
 8022902:	ea4f 1866 	mov.w	r8, r6, asr #5
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;
    
    Src=Val;
 8022906:	f006 031f 	and.w	r3, r6, #31
 802290a:	eb05 0788 	add.w	r7, r5, r8, lsl #2
    else
        RMP_COVERAGE_MARKER();
    
    /* Try to find one position on this processor word level */
    Level=(FLI_Level_Temp<<3)+SLI_Level_Temp;
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
 802290e:	697a      	ldr	r2, [r7, #20]
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;
    
    Src=Val;
 8022910:	40da      	lsrs	r2, r3
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022912:	ea4f 6e12 	mov.w	lr, r2, lsr #24
    To[1]=RMP_RBIT_Table[From[2]];
 8022916:	f3c2 4007 	ubfx	r0, r2, #16, #8
    To[2]=RMP_RBIT_Table[From[1]];
 802291a:	f3c2 2107 	ubfx	r1, r2, #8, #8
    To[3]=RMP_RBIT_Table[From[0]];
 802291e:	b2d3      	uxtb	r3, r2
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
    To[1]=RMP_RBIT_Table[From[2]];
 8022920:	f819 0000 	ldrb.w	r0, [r9, r0]
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022924:	f819 e00e 	ldrb.w	lr, [r9, lr]
    To[1]=RMP_RBIT_Table[From[2]];
    To[2]=RMP_RBIT_Table[From[1]];
 8022928:	f819 1001 	ldrb.w	r1, [r9, r1]
    To[3]=RMP_RBIT_Table[From[0]];
 802292c:	f819 3003 	ldrb.w	r3, [r9, r3]
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022930:	f88d e000 	strb.w	lr, [sp]
    To[1]=RMP_RBIT_Table[From[2]];
 8022934:	f88d 0001 	strb.w	r0, [sp, #1]
    To[2]=RMP_RBIT_Table[From[1]];
 8022938:	f88d 1002 	strb.w	r1, [sp, #2]
    To[3]=RMP_RBIT_Table[From[0]];
 802293c:	f88d 3003 	strb.w	r3, [sp, #3]
Output      : None.
Return      : rmp_ptr_t - The LSB found.
******************************************************************************/
rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val)
{
    return RMP_WORD_SIZE-1-RMP_MSB_Get(RMP_RBIT_Get(Val));
 8022940:	9800      	ldr	r0, [sp, #0]
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;
    
    Src=Val;
 8022942:	9201      	str	r2, [sp, #4]
Output      : None.
Return      : rmp_ptr_t - The LSB found.
******************************************************************************/
rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val)
{
    return RMP_WORD_SIZE-1-RMP_MSB_Get(RMP_RBIT_Get(Val));
 8022944:	f7fe fbd8 	bl	80210f8 <RMP_MSB_Get>
 8022948:	f1c0 001f 	rsb	r0, r0, #31
    
    /* Try to find one position on this processor word level */
    Level=(FLI_Level_Temp<<3)+SLI_Level_Temp;
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
    /* If there's at least one block that matches the query, return the level */
    if(LSB<RMP_WORD_SIZE)
 802294c:	281f      	cmp	r0, #31
 802294e:	d944      	bls.n	80229da <RMP_Malloc+0x132>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 8022950:	f108 0601 	add.w	r6, r8, #1
 8022954:	2e04      	cmp	r6, #4
 8022956:	dc3c      	bgt.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 8022958:	69bb      	ldr	r3, [r7, #24]
 802295a:	2b00      	cmp	r3, #0
 802295c:	f040 80ea 	bne.w	8022b34 <RMP_Malloc+0x28c>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 8022960:	f108 0602 	add.w	r6, r8, #2
 8022964:	2e05      	cmp	r6, #5
 8022966:	d034      	beq.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 8022968:	f108 0206 	add.w	r2, r8, #6
 802296c:	eb05 0382 	add.w	r3, r5, r2, lsl #2
 8022970:	685b      	ldr	r3, [r3, #4]
 8022972:	2b00      	cmp	r3, #0
 8022974:	f040 80e1 	bne.w	8022b3a <RMP_Malloc+0x292>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 8022978:	f108 0603 	add.w	r6, r8, #3
 802297c:	2e05      	cmp	r6, #5
 802297e:	d028      	beq.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 8022980:	f108 0307 	add.w	r3, r8, #7
 8022984:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 8022988:	685b      	ldr	r3, [r3, #4]
 802298a:	2b00      	cmp	r3, #0
 802298c:	d158      	bne.n	8022a40 <RMP_Malloc+0x198>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 802298e:	f108 0604 	add.w	r6, r8, #4
 8022992:	2e05      	cmp	r6, #5
 8022994:	d01d      	beq.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 8022996:	1d33      	adds	r3, r6, #4
 8022998:	eb05 0183 	add.w	r1, r5, r3, lsl #2
 802299c:	6849      	ldr	r1, [r1, #4]
 802299e:	2900      	cmp	r1, #0
 80229a0:	d150      	bne.n	8022a44 <RMP_Malloc+0x19c>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 80229a2:	f108 0605 	add.w	r6, r8, #5
 80229a6:	f1b8 0f00 	cmp.w	r8, #0
 80229aa:	d012      	beq.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 80229ac:	f108 0309 	add.w	r3, r8, #9
 80229b0:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 80229b4:	685b      	ldr	r3, [r3, #4]
 80229b6:	2b00      	cmp	r3, #0
 80229b8:	f040 80c4 	bne.w	8022b44 <RMP_Malloc+0x29c>
    /* No one exactly fits */
    else
    {
        RMP_COVERAGE_MARKER();
        /* From the next word, query one by one. 5 is because we never have more than 128MB memory */
        for(Level=(Level>>RMP_WORD_ORDER)+1;Level<5;Level++)
 80229bc:	2a05      	cmp	r2, #5
 80229be:	4616      	mov	r6, r2
 80229c0:	d007      	beq.n	80229d2 <RMP_Malloc+0x12a>
        {
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
 80229c2:	f108 030a 	add.w	r3, r8, #10
 80229c6:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 80229ca:	685b      	ldr	r3, [r3, #4]
 80229cc:	2b00      	cmp	r3, #0
 80229ce:	f040 80b6 	bne.w	8022b3e <RMP_Malloc+0x296>

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool,Rounded_Size,&FLI_Level,&SLI_Level)!=0)
    {
        RMP_COVERAGE_MARKER();
        return (void*)(0);
 80229d2:	2000      	movs	r0, #0
    RMP_List_Ins(&(Mem_Head->Head),&(Mem->Alloc),Mem->Alloc.Next);
    Mem_Head->State=RMP_MEM_USED;

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
}
 80229d4:	b003      	add	sp, #12
 80229d6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
    LSB=RMP_LSB_Get(Mem->Bitmap[Level>>RMP_WORD_ORDER]>>(Level&RMP_WORD_MASK));
    /* If there's at least one block that matches the query, return the level */
    if(LSB<RMP_WORD_SIZE)
    {
        RMP_COVERAGE_MARKER();
        Level=(Level&(~RMP_WORD_MASK))+LSB+(Level&RMP_WORD_MASK);
 80229da:	4430      	add	r0, r6
        *FLI_Level=Level>>3;
 80229dc:	10c3      	asrs	r3, r0, #3
        *SLI_Level=Level&0x07;
 80229de:	f000 0007 	and.w	r0, r0, #7
        RMP_COVERAGE_MARKER();
    
    Mem=(volatile struct RMP_Mem*)Pool;
    
    /* There is such block. Get it and delete it from the TLSF list. */
    Mem_Head=(struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)].Next);
 80229e2:	eb00 03c3 	add.w	r3, r0, r3, lsl #3
 80229e6:	3305      	adds	r3, #5
 80229e8:	eb05 03c3 	add.w	r3, r5, r3, lsl #3
 80229ec:	685e      	ldr	r6, [r3, #4]
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 80229ee:	68f7      	ldr	r7, [r6, #12]
 80229f0:	3f10      	subs	r7, #16
 80229f2:	1bbf      	subs	r7, r7, r6
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 80229f4:	4638      	mov	r0, r7
 80229f6:	f7fe fb7f 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 80229fa:	1ec1      	subs	r1, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 80229fc:	3806      	subs	r0, #6
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 80229fe:	6833      	ldr	r3, [r6, #0]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022a00:	40cf      	lsrs	r7, r1
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022a02:	6872      	ldr	r2, [r6, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022a04:	f007 0707 	and.w	r7, r7, #7
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022a08:	6013      	str	r3, [r2, #0]
    Prev->Next=(struct RMP_List*)Next;
 8022a0a:	605a      	str	r2, [r3, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022a0c:	eb07 00c0 	add.w	r0, r7, r0, lsl #3
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last block */
    if(Slot==Slot->Next)
 8022a10:	1d43      	adds	r3, r0, #5
 8022a12:	eb05 03c3 	add.w	r3, r5, r3, lsl #3
 8022a16:	685a      	ldr	r2, [r3, #4]
 8022a18:	429a      	cmp	r2, r3
 8022a1a:	d070      	beq.n	8022afe <RMP_Malloc+0x256>
    Mem_Head=(struct RMP_Mem_Head*)(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)].Next);
    _RMP_Mem_Del(Pool, Mem_Head);

    /* Allocate and calculate if the space left could be big enough to be a new 
     * block. If so, we will put the block back into the TLSF table */
    New_Size=((rmp_ptr_t)(Mem_Head->Tail))-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
 8022a1c:	68f3      	ldr	r3, [r6, #12]
 8022a1e:	3b10      	subs	r3, #16
 8022a20:	1b9b      	subs	r3, r3, r6
 8022a22:	1b1b      	subs	r3, r3, r4
    if(New_Size>=(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
 8022a24:	2b53      	cmp	r3, #83	; 0x53
 8022a26:	d835      	bhi.n	8022a94 <RMP_Malloc+0x1ec>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Insert the allocated block into the lists */
    RMP_List_Ins(&(Mem_Head->Head),&(Mem->Alloc),Mem->Alloc.Next);
 8022a28:	686b      	ldr	r3, [r5, #4]
    Mem_Head->State=RMP_MEM_USED;
 8022a2a:	2201      	movs	r2, #1

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
 8022a2c:	f106 0010 	add.w	r0, r6, #16
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022a30:	601e      	str	r6, [r3, #0]
    New->Next=(struct RMP_List*)Next;
 8022a32:	6073      	str	r3, [r6, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022a34:	6035      	str	r5, [r6, #0]
    Prev->Next=(struct RMP_List*)New;
 8022a36:	606e      	str	r6, [r5, #4]
    else
        RMP_COVERAGE_MARKER();

    /* Insert the allocated block into the lists */
    RMP_List_Ins(&(Mem_Head->Head),&(Mem->Alloc),Mem->Alloc.Next);
    Mem_Head->State=RMP_MEM_USED;
 8022a38:	60b2      	str	r2, [r6, #8]

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
}
 8022a3a:	b003      	add	sp, #12
 8022a3c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
 8022a40:	f108 0307 	add.w	r3, r8, #7
            /* if the level has blocks of one FLI level */
            if(Mem->Bitmap[Level]!=0)
            {
                RMP_COVERAGE_MARKER();
                /* Find the actual level */ 
                LSB=RMP_LSB_Get(Mem->Bitmap[Level]);
 8022a44:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 8022a48:	685b      	ldr	r3, [r3, #4]
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022a4a:	0e1f      	lsrs	r7, r3, #24
    To[1]=RMP_RBIT_Table[From[2]];
 8022a4c:	f3c3 4007 	ubfx	r0, r3, #16, #8
    To[2]=RMP_RBIT_Table[From[1]];
 8022a50:	f3c3 2107 	ubfx	r1, r3, #8, #8
    To[3]=RMP_RBIT_Table[From[0]];
 8022a54:	b2da      	uxtb	r2, r3
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
    To[1]=RMP_RBIT_Table[From[2]];
 8022a56:	f819 0000 	ldrb.w	r0, [r9, r0]
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022a5a:	f819 7007 	ldrb.w	r7, [r9, r7]
    To[1]=RMP_RBIT_Table[From[2]];
    To[2]=RMP_RBIT_Table[From[1]];
 8022a5e:	f819 1001 	ldrb.w	r1, [r9, r1]
    To[3]=RMP_RBIT_Table[From[0]];
 8022a62:	f819 2002 	ldrb.w	r2, [r9, r2]
    
#if(RMP_WORD_ORDER==4)
    To[0]=RMP_RBIT_Table[From[1]];
    To[1]=RMP_RBIT_Table[From[0]];
#elif(RMP_WORD_ORDER==5)
    To[0]=RMP_RBIT_Table[From[3]];
 8022a66:	f88d 7000 	strb.w	r7, [sp]
    To[1]=RMP_RBIT_Table[From[2]];
 8022a6a:	f88d 0001 	strb.w	r0, [sp, #1]
    To[2]=RMP_RBIT_Table[From[1]];
 8022a6e:	f88d 1002 	strb.w	r1, [sp, #2]
    To[3]=RMP_RBIT_Table[From[0]];
 8022a72:	f88d 2003 	strb.w	r2, [sp, #3]
Output      : None.
Return      : rmp_ptr_t - The LSB found.
******************************************************************************/
rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val)
{
    return RMP_WORD_SIZE-1-RMP_MSB_Get(RMP_RBIT_Get(Val));
 8022a76:	9800      	ldr	r0, [sp, #0]
    rmp_ptr_t Ret;
    rmp_ptr_t Src;
    rmp_u8_t* To;
    rmp_u8_t* From;
    
    Src=Val;
 8022a78:	9301      	str	r3, [sp, #4]
Output      : None.
Return      : rmp_ptr_t - The LSB found.
******************************************************************************/
rmp_ptr_t RMP_LSB_Get(rmp_ptr_t Val)
{
    return RMP_WORD_SIZE-1-RMP_MSB_Get(RMP_RBIT_Get(Val));
 8022a7a:	f7fe fb3d 	bl	80210f8 <RMP_MSB_Get>
 8022a7e:	f1c0 001f 	rsb	r0, r0, #31
            if(Mem->Bitmap[Level]!=0)
            {
                RMP_COVERAGE_MARKER();
                /* Find the actual level */ 
                LSB=RMP_LSB_Get(Mem->Bitmap[Level]);
                *FLI_Level=((Level<<RMP_WORD_ORDER)+LSB)>>3;
 8022a82:	eb00 1346 	add.w	r3, r0, r6, lsl #5
                *SLI_Level=LSB&0x07;
 8022a86:	f000 0007 	and.w	r0, r0, #7
            if(Mem->Bitmap[Level]!=0)
            {
                RMP_COVERAGE_MARKER();
                /* Find the actual level */ 
                LSB=RMP_LSB_Get(Mem->Bitmap[Level]);
                *FLI_Level=((Level<<RMP_WORD_ORDER)+LSB)>>3;
 8022a8a:	08db      	lsrs	r3, r3, #3
 8022a8c:	e7a9      	b.n	80229e2 <RMP_Malloc+0x13a>
        
        /* If the SLI level is the largest of the SLI level, then jump to the next FLI level */
        if(SLI_Level_Temp==8)
        {
            RMP_COVERAGE_MARKER();
            FLI_Level_Temp+=1;
 8022a8e:	1f46      	subs	r6, r0, #5
            SLI_Level_Temp=0;
 8022a90:	463b      	mov	r3, r7
 8022a92:	e72f      	b.n	80228f4 <RMP_Malloc+0x4c>
    New_Size=((rmp_ptr_t)(Mem_Head->Tail))-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
    if(New_Size>=(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
        New_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);
 8022a94:	f106 0914 	add.w	r9, r6, #20
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 8022a98:	2200      	movs	r2, #0
    New_Size=((rmp_ptr_t)(Mem_Head->Tail))-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head)-Rounded_Size;
    if(New_Size>=(sizeof(struct RMP_Mem_Head)+64+sizeof(struct RMP_Mem_Tail)))
    {
        RMP_COVERAGE_MARKER();
        Old_Size=sizeof(struct RMP_Mem_Head)+Rounded_Size+sizeof(struct RMP_Mem_Tail);
        New_Mem=(volatile struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Head)+Old_Size);
 8022a9a:	eb09 0804 	add.w	r8, r9, r4
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 8022a9e:	60b2      	str	r2, [r6, #8]
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022aa0:	f1a8 0104 	sub.w	r1, r8, #4
 8022aa4:	60f1      	str	r1, [r6, #12]
 8022aa6:	440b      	add	r3, r1
    Mem_Head->Tail->Head=Mem_Head;
 8022aa8:	68f1      	ldr	r1, [r6, #12]
 8022aaa:	600e      	str	r6, [r1, #0]
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 8022aac:	f8c8 2008 	str.w	r2, [r8, #8]
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022ab0:	f8c8 300c 	str.w	r3, [r8, #12]
    Mem_Head->Tail->Head=Mem_Head;
 8022ab4:	f8d8 300c 	ldr.w	r3, [r8, #12]
 8022ab8:	f8c3 8000 	str.w	r8, [r3]
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022abc:	f8d8 700c 	ldr.w	r7, [r8, #12]
 8022ac0:	3f10      	subs	r7, #16
 8022ac2:	ebc8 0707 	rsb	r7, r8, r7

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022ac6:	4638      	mov	r0, r7
 8022ac8:	f7fe fb16 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022acc:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022ace:	3806      	subs	r0, #6
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022ad0:	40df      	lsrs	r7, r3
 8022ad2:	f007 0707 	and.w	r7, r7, #7
 8022ad6:	eb07 03c0 	add.w	r3, r7, r0, lsl #3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
 8022ada:	1d5a      	adds	r2, r3, #5
 8022adc:	eb05 00c3 	add.w	r0, r5, r3, lsl #3
 8022ae0:	eb05 02c2 	add.w	r2, r5, r2, lsl #3

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
 8022ae4:	6ac1      	ldr	r1, [r0, #44]	; 0x2c
 8022ae6:	428a      	cmp	r2, r1
 8022ae8:	d018      	beq.n	8022b1c <RMP_Malloc+0x274>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
 8022aea:	6ac3      	ldr	r3, [r0, #44]	; 0x2c
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022aec:	f8c3 8000 	str.w	r8, [r3]
    New->Next=(struct RMP_List*)Next;
 8022af0:	f8c8 3004 	str.w	r3, [r8, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022af4:	f849 2004 	str.w	r2, [r9, r4]
    Prev->Next=(struct RMP_List*)New;
 8022af8:	f8c0 802c 	str.w	r8, [r0, #44]	; 0x2c
 8022afc:	e794      	b.n	8022a28 <RMP_Malloc+0x180>
 8022afe:	0943      	lsrs	r3, r0, #5
     * what we deleted is the last block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]&=~(1<<(Level&RMP_WORD_MASK));
 8022b00:	2201      	movs	r2, #1
 8022b02:	f000 001f 	and.w	r0, r0, #31
 8022b06:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 8022b0a:	fa02 f000 	lsl.w	r0, r2, r0
 8022b0e:	695a      	ldr	r2, [r3, #20]
 8022b10:	ea22 0000 	bic.w	r0, r2, r0
 8022b14:	6158      	str	r0, [r3, #20]
 8022b16:	e781      	b.n	8022a1c <RMP_Malloc+0x174>

    /* See if such block exists, if not, abort */
    if(_RMP_Mem_Search(Pool,Rounded_Size,&FLI_Level,&SLI_Level)!=0)
    {
        RMP_COVERAGE_MARKER();
        return (void*)(0);
 8022b18:	2000      	movs	r0, #0
    RMP_List_Ins(&(Mem_Head->Head),&(Mem->Alloc),Mem->Alloc.Next);
    Mem_Head->State=RMP_MEM_USED;

    /* Finally, return the start address */
    return (void*)(((rmp_ptr_t)Mem_Head)+sizeof(struct RMP_Mem_Head));
}
 8022b1a:	4770      	bx	lr
 8022b1c:	0959      	lsrs	r1, r3, #5
    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=1<<(Level&RMP_WORD_MASK);
 8022b1e:	2701      	movs	r7, #1
 8022b20:	f003 031f 	and.w	r3, r3, #31
 8022b24:	eb05 0181 	add.w	r1, r5, r1, lsl #2
 8022b28:	fa07 f303 	lsl.w	r3, r7, r3
 8022b2c:	694f      	ldr	r7, [r1, #20]
 8022b2e:	433b      	orrs	r3, r7
 8022b30:	614b      	str	r3, [r1, #20]
 8022b32:	e7da      	b.n	8022aea <RMP_Malloc+0x242>
 8022b34:	f108 0305 	add.w	r3, r8, #5
 8022b38:	e784      	b.n	8022a44 <RMP_Malloc+0x19c>
 8022b3a:	4613      	mov	r3, r2
 8022b3c:	e782      	b.n	8022a44 <RMP_Malloc+0x19c>
 8022b3e:	f108 030a 	add.w	r3, r8, #10
 8022b42:	e77f      	b.n	8022a44 <RMP_Malloc+0x19c>
 8022b44:	f108 0309 	add.w	r3, r8, #9
 8022b48:	e77c      	b.n	8022a44 <RMP_Malloc+0x19c>
 8022b4a:	bf00      	nop
 8022b4c:	08023050 	.word	0x08023050

08022b50 <RMP_Free>:
    volatile struct RMP_Mem_Head* Left_Head;
    volatile struct RMP_Mem_Head* Right_Head;
    rmp_cnt_t Merge_Left;

    /* Check if pointer is null */
    if((Pool==0)||(Mem_Ptr==0))
 8022b50:	2800      	cmp	r0, #0
 8022b52:	d04a      	beq.n	8022bea <RMP_Free+0x9a>
              void* Mem_Ptr - The pointer returned by "RMP_Malloc".
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Free(volatile void* Pool, void* Mem_Ptr)
{
 8022b54:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
    volatile struct RMP_Mem_Head* Left_Head;
    volatile struct RMP_Mem_Head* Right_Head;
    rmp_cnt_t Merge_Left;

    /* Check if pointer is null */
    if((Pool==0)||(Mem_Ptr==0))
 8022b58:	fab1 f381 	clz	r3, r1
 8022b5c:	095b      	lsrs	r3, r3, #5
 8022b5e:	2b00      	cmp	r3, #0
 8022b60:	d141      	bne.n	8022be6 <RMP_Free+0x96>
    else
        RMP_COVERAGE_MARKER();
    
    /* See if the address is within the allocatable address range. If not, abort directly. */
    Mem=(volatile struct RMP_Mem*)Pool;
    if((((rmp_ptr_t)Mem_Ptr)<=((rmp_ptr_t)Mem))||(((rmp_ptr_t)Mem_Ptr)>=(((rmp_ptr_t)Mem)+Mem->Size)))
 8022b62:	4281      	cmp	r1, r0
 8022b64:	d93f      	bls.n	8022be6 <RMP_Free+0x96>
 8022b66:	6902      	ldr	r2, [r0, #16]
 8022b68:	4402      	add	r2, r0
 8022b6a:	4291      	cmp	r1, r2
 8022b6c:	d23b      	bcs.n	8022be6 <RMP_Free+0x96>
        return;
    }
    else
        RMP_COVERAGE_MARKER();

    Mem_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)Mem_Ptr)-sizeof(struct RMP_Mem_Head));
 8022b6e:	f1a1 0610 	sub.w	r6, r1, #16
    /* See if the block can really be freed */
    if(Mem_Head->State==RMP_MEM_FREE)
 8022b72:	68b2      	ldr	r2, [r6, #8]
 8022b74:	2a00      	cmp	r2, #0
 8022b76:	d036      	beq.n	8022be6 <RMP_Free+0x96>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Now we are sure that it can be freed. Delete it from the allocated list now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022b78:	f851 2c10 	ldr.w	r2, [r1, #-16]
 8022b7c:	460d      	mov	r5, r1
 8022b7e:	6871      	ldr	r1, [r6, #4]
 8022b80:	4604      	mov	r4, r0
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022b82:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 8022b84:	6051      	str	r1, [r2, #4]
    else
        RMP_COVERAGE_MARKER();

    /* Now we are sure that it can be freed. Delete it from the allocated list now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
    Mem_Head->State=RMP_MEM_FREE;
 8022b86:	60b3      	str	r3, [r6, #8]
    
    /* Now check if we can merge it with the higher blocks */
    Right_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
 8022b88:	f8d6 900c 	ldr.w	r9, [r6, #12]
    if(((rmp_ptr_t)Right_Head)!=(((rmp_ptr_t)Mem)+Mem->Size))
 8022b8c:	6903      	ldr	r3, [r0, #16]
    /* Now we are sure that it can be freed. Delete it from the allocated list now */
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
    Mem_Head->State=RMP_MEM_FREE;
    
    /* Now check if we can merge it with the higher blocks */
    Right_Head=(struct RMP_Mem_Head*)(((rmp_ptr_t)(Mem_Head->Tail))+sizeof(struct RMP_Mem_Tail));
 8022b8e:	f109 0704 	add.w	r7, r9, #4
    if(((rmp_ptr_t)Right_Head)!=(((rmp_ptr_t)Mem)+Mem->Size))
 8022b92:	4403      	add	r3, r0
 8022b94:	429f      	cmp	r7, r3
 8022b96:	d001      	beq.n	8022b9c <RMP_Free+0x4c>
    {
        RMP_COVERAGE_MARKER();
        /* If this one is unoccupied */
        if((Right_Head->State)==RMP_MEM_FREE)
 8022b98:	68bb      	ldr	r3, [r7, #8]
 8022b9a:	b33b      	cbz	r3, 8022bec <RMP_Free+0x9c>
    else
        RMP_COVERAGE_MARKER();

    /* Now check if we can merge it with the lower blocks */
    Merge_Left=0;
    if((rmp_ptr_t)Mem_Head!=Mem->Start)
 8022b9c:	68e3      	ldr	r3, [r4, #12]
 8022b9e:	429e      	cmp	r6, r3
 8022ba0:	d004      	beq.n	8022bac <RMP_Free+0x5c>
    {
        RMP_COVERAGE_MARKER();
        Left_Head=((struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Tail)))->Head;
 8022ba2:	f855 7c14 	ldr.w	r7, [r5, #-20]

        /* If this one is unoccupied */
        if(Left_Head->State==RMP_MEM_FREE)
 8022ba6:	68bb      	ldr	r3, [r7, #8]
 8022ba8:	2b00      	cmp	r3, #0
 8022baa:	d041      	beq.n	8022c30 <RMP_Free+0xe0>
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022bac:	68f7      	ldr	r7, [r6, #12]
 8022bae:	3f10      	subs	r7, #16
 8022bb0:	1bbf      	subs	r7, r7, r6

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022bb2:	4638      	mov	r0, r7
 8022bb4:	f7fe faa0 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022bb8:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022bba:	3806      	subs	r0, #6
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022bbc:	40df      	lsrs	r7, r3
 8022bbe:	f007 0707 	and.w	r7, r7, #7
 8022bc2:	eb07 00c0 	add.w	r0, r7, r0, lsl #3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
 8022bc6:	1d42      	adds	r2, r0, #5
 8022bc8:	eb04 01c0 	add.w	r1, r4, r0, lsl #3
 8022bcc:	eb04 02c2 	add.w	r2, r4, r2, lsl #3

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
 8022bd0:	6acb      	ldr	r3, [r1, #44]	; 0x2c
 8022bd2:	429a      	cmp	r2, r3
 8022bd4:	d071      	beq.n	8022cba <RMP_Free+0x16a>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
 8022bd6:	6acb      	ldr	r3, [r1, #44]	; 0x2c
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022bd8:	601e      	str	r6, [r3, #0]
    New->Next=(struct RMP_List*)Next;
 8022bda:	6073      	str	r3, [r6, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022bdc:	f845 2c10 	str.w	r2, [r5, #-16]
    Prev->Next=(struct RMP_List*)New;
 8022be0:	62ce      	str	r6, [r1, #44]	; 0x2c
 8022be2:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8022be6:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8022bea:	4770      	bx	lr
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022bec:	68fb      	ldr	r3, [r7, #12]
 8022bee:	f1a3 0810 	sub.w	r8, r3, #16
 8022bf2:	ebc7 0808 	rsb	r8, r7, r8
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022bf6:	4640      	mov	r0, r8
 8022bf8:	f7fe fa7e 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022bfc:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022bfe:	3806      	subs	r0, #6
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022c00:	f8d9 2004 	ldr.w	r2, [r9, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c04:	fa28 f803 	lsr.w	r8, r8, r3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022c08:	6879      	ldr	r1, [r7, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c0a:	f008 0807 	and.w	r8, r8, #7
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022c0e:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 8022c10:	6051      	str	r1, [r2, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c12:	eb08 03c0 	add.w	r3, r8, r0, lsl #3
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last block */
    if(Slot==Slot->Next)
 8022c16:	1d5a      	adds	r2, r3, #5
 8022c18:	eb04 02c2 	add.w	r2, r4, r2, lsl #3
 8022c1c:	6851      	ldr	r1, [r2, #4]
 8022c1e:	4291      	cmp	r1, r2
 8022c20:	d064      	beq.n	8022cec <RMP_Free+0x19c>
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool,Right_Head);
            _RMP_Mem_Block(Mem_Head,
                           ((rmp_ptr_t)(Right_Head->Tail))+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Mem_Head);
 8022c22:	68fb      	ldr	r3, [r7, #12]
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 8022c24:	2200      	movs	r2, #0
 8022c26:	60b2      	str	r2, [r6, #8]
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022c28:	60f3      	str	r3, [r6, #12]
    Mem_Head->Tail->Head=Mem_Head;
 8022c2a:	68f3      	ldr	r3, [r6, #12]
 8022c2c:	601e      	str	r6, [r3, #0]
 8022c2e:	e7b5      	b.n	8022b9c <RMP_Free+0x4c>
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;    
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022c30:	68fd      	ldr	r5, [r7, #12]
 8022c32:	3d10      	subs	r5, #16
 8022c34:	1bed      	subs	r5, r5, r7
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022c36:	4628      	mov	r0, r5
 8022c38:	f7fe fa5e 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c3c:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022c3e:	3806      	subs	r0, #6
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022c40:	683a      	ldr	r2, [r7, #0]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c42:	40dd      	lsrs	r5, r3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);
 8022c44:	6879      	ldr	r1, [r7, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c46:	f005 0507 	and.w	r5, r5, #7
Output      : None.
Return      : None.
******************************************************************************/
void RMP_List_Del(volatile struct RMP_List* Prev,volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)Prev;
 8022c4a:	600a      	str	r2, [r1, #0]
    Prev->Next=(struct RMP_List*)Next;
 8022c4c:	6051      	str	r1, [r2, #4]
    
    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c4e:	eb05 03c0 	add.w	r3, r5, r0, lsl #3
   
    RMP_List_Del(Mem_Head->Head.Prev,Mem_Head->Head.Next);

    /* See if there are any blocks in the level, equal means no. So
     * what we deleted is the last block */
    if(Slot==Slot->Next)
 8022c52:	1d5a      	adds	r2, r3, #5
 8022c54:	eb04 02c2 	add.w	r2, r4, r2, lsl #3
 8022c58:	6851      	ldr	r1, [r2, #4]
 8022c5a:	4291      	cmp	r1, r2
 8022c5c:	d039      	beq.n	8022cd2 <RMP_Free+0x182>
        {
            RMP_COVERAGE_MARKER();
            /* Delete, merge */
            _RMP_Mem_Del(Pool, Left_Head);
            _RMP_Mem_Block(Left_Head,
                           (rmp_ptr_t)((rmp_ptr_t)(Mem_Head->Tail)+sizeof(struct RMP_Mem_Tail)-(rmp_ptr_t)Left_Head));
 8022c5e:	68f3      	ldr	r3, [r6, #12]
    
    /* Get the big memory block's size and position */
    Mem_Head=(struct RMP_Mem_Head*)Addr;
    
    /* Initialize the big memory block */
    Mem_Head->State=RMP_MEM_FREE;
 8022c60:	2200      	movs	r2, #0
 8022c62:	60ba      	str	r2, [r7, #8]
    Mem_Head->Tail=(struct RMP_Mem_Tail*)(((rmp_ptr_t)Mem_Head)+Size-sizeof(struct RMP_Mem_Tail));
 8022c64:	60fb      	str	r3, [r7, #12]
    Mem_Head->Tail->Head=Mem_Head;
 8022c66:	68fb      	ldr	r3, [r7, #12]
 8022c68:	601f      	str	r7, [r3, #0]
    volatile struct RMP_Mem* Mem;
    volatile struct RMP_List* Slot;
    
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);
 8022c6a:	68fd      	ldr	r5, [r7, #12]
 8022c6c:	3d10      	subs	r5, #16
 8022c6e:	1bed      	subs	r5, r5, r7

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022c70:	4628      	mov	r0, r5
 8022c72:	f7fe fa41 	bl	80210f8 <RMP_MSB_Get>
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c76:	1ec3      	subs	r3, r0, #3
    /* Get the memory pool and block size */
    Mem=(volatile struct RMP_Mem*)Pool;
    Size=(rmp_ptr_t)(Mem_Head->Tail)-((rmp_ptr_t)Mem_Head)-sizeof(struct RMP_Mem_Head);

    /* Guarantee the Mem_Size is bigger than 64 or a failure will surely occur here */
    FLI_Level=RMP_MSB_Get(Size)-6;
 8022c78:	3806      	subs	r0, #6
    /* Decide the SLI level directly from the FLI level */
    SLI_Level=(Size>>(FLI_Level+3))&0x07;
    Level=(FLI_Level<<3)+SLI_Level;
 8022c7a:	40dd      	lsrs	r5, r3
 8022c7c:	f005 0507 	and.w	r5, r5, #7
 8022c80:	eb05 03c0 	add.w	r3, r5, r0, lsl #3
    /* Get the slot */
    Slot=&(Mem->Table[RMP_MEM_POS(FLI_Level,SLI_Level)]);
 8022c84:	1d5a      	adds	r2, r3, #5
 8022c86:	eb04 01c3 	add.w	r1, r4, r3, lsl #3
 8022c8a:	eb04 02c2 	add.w	r2, r4, r2, lsl #3

    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
 8022c8e:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
 8022c90:	4282      	cmp	r2, r0
 8022c92:	d006      	beq.n	8022ca2 <RMP_Free+0x152>
    }
    else
        RMP_COVERAGE_MARKER();

    /* Insert the node now */
    RMP_List_Ins(&(Mem_Head->Head), Slot, Slot->Next);
 8022c94:	6acb      	ldr	r3, [r1, #44]	; 0x2c
******************************************************************************/
void RMP_List_Ins(volatile struct RMP_List* New,
                  volatile struct RMP_List* Prev,
                  volatile struct RMP_List* Next)
{
    Next->Prev=(struct RMP_List*)New;
 8022c96:	601f      	str	r7, [r3, #0]
    New->Next=(struct RMP_List*)Next;
 8022c98:	607b      	str	r3, [r7, #4]
    New->Prev=(struct RMP_List*)Prev;
 8022c9a:	603a      	str	r2, [r7, #0]
    Prev->Next=(struct RMP_List*)New;
 8022c9c:	62cf      	str	r7, [r1, #44]	; 0x2c
 8022c9e:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8022ca2:	0958      	lsrs	r0, r3, #5
    /* See if there are any blocks in the level, equal means no. So what we inserted is the first block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Set the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]|=1<<(Level&RMP_WORD_MASK);
 8022ca4:	2501      	movs	r5, #1
 8022ca6:	f003 031f 	and.w	r3, r3, #31
 8022caa:	eb04 0080 	add.w	r0, r4, r0, lsl #2
 8022cae:	fa05 f303 	lsl.w	r3, r5, r3
 8022cb2:	6944      	ldr	r4, [r0, #20]
 8022cb4:	4323      	orrs	r3, r4
 8022cb6:	6143      	str	r3, [r0, #20]
 8022cb8:	e7ec      	b.n	8022c94 <RMP_Free+0x144>
 8022cba:	0947      	lsrs	r7, r0, #5
 8022cbc:	f000 031f 	and.w	r3, r0, #31
 8022cc0:	2001      	movs	r0, #1
 8022cc2:	eb04 0487 	add.w	r4, r4, r7, lsl #2
 8022cc6:	fa00 f303 	lsl.w	r3, r0, r3
 8022cca:	6967      	ldr	r7, [r4, #20]
 8022ccc:	433b      	orrs	r3, r7
 8022cce:	6163      	str	r3, [r4, #20]
 8022cd0:	e781      	b.n	8022bd6 <RMP_Free+0x86>
 8022cd2:	095a      	lsrs	r2, r3, #5
     * what we deleted is the last block */
    if(Slot==Slot->Next)
    {
        RMP_COVERAGE_MARKER();
        /* Clear the corresponding bit in the TLSF bitmap */
        Mem->Bitmap[Level>>RMP_WORD_ORDER]&=~(1<<(Level&RMP_WORD_MASK));
 8022cd4:	2101      	movs	r1, #1
 8022cd6:	f003 031f 	and.w	r3, r3, #31
 8022cda:	eb04 0282 	add.w	r2, r4, r2, lsl #2
 8022cde:	fa01 f303 	lsl.w	r3, r1, r3
 8022ce2:	6951      	ldr	r1, [r2, #20]
 8022ce4:	ea21 0303 	bic.w	r3, r1, r3
 8022ce8:	6153      	str	r3, [r2, #20]
 8022cea:	e7b8      	b.n	8022c5e <RMP_Free+0x10e>
 8022cec:	095a      	lsrs	r2, r3, #5
 8022cee:	2101      	movs	r1, #1
 8022cf0:	f003 031f 	and.w	r3, r3, #31
 8022cf4:	eb04 0282 	add.w	r2, r4, r2, lsl #2
 8022cf8:	fa01 f303 	lsl.w	r3, r1, r3
 8022cfc:	6951      	ldr	r1, [r2, #20]
 8022cfe:	ea21 0303 	bic.w	r3, r1, r3
 8022d02:	6153      	str	r3, [r2, #20]
 8022d04:	e78d      	b.n	8022c22 <RMP_Free+0xd2>
 8022d06:	bf00      	nop

08022d08 <__libc_init_array>:
 8022d08:	b570      	push	{r4, r5, r6, lr}
 8022d0a:	4e0f      	ldr	r6, [pc, #60]	; (8022d48 <__libc_init_array+0x40>)
 8022d0c:	4d0f      	ldr	r5, [pc, #60]	; (8022d4c <__libc_init_array+0x44>)
 8022d0e:	1b76      	subs	r6, r6, r5
 8022d10:	10b6      	asrs	r6, r6, #2
 8022d12:	bf18      	it	ne
 8022d14:	2400      	movne	r4, #0
 8022d16:	d005      	beq.n	8022d24 <__libc_init_array+0x1c>
 8022d18:	3401      	adds	r4, #1
 8022d1a:	f855 3b04 	ldr.w	r3, [r5], #4
 8022d1e:	4798      	blx	r3
 8022d20:	42a6      	cmp	r6, r4
 8022d22:	d1f9      	bne.n	8022d18 <__libc_init_array+0x10>
 8022d24:	4e0a      	ldr	r6, [pc, #40]	; (8022d50 <__libc_init_array+0x48>)
 8022d26:	4d0b      	ldr	r5, [pc, #44]	; (8022d54 <__libc_init_array+0x4c>)
 8022d28:	1b76      	subs	r6, r6, r5
 8022d2a:	f000 fa1d 	bl	8023168 <_init>
 8022d2e:	10b6      	asrs	r6, r6, #2
 8022d30:	bf18      	it	ne
 8022d32:	2400      	movne	r4, #0
 8022d34:	d006      	beq.n	8022d44 <__libc_init_array+0x3c>
 8022d36:	3401      	adds	r4, #1
 8022d38:	f855 3b04 	ldr.w	r3, [r5], #4
 8022d3c:	4798      	blx	r3
 8022d3e:	42a6      	cmp	r6, r4
 8022d40:	d1f9      	bne.n	8022d36 <__libc_init_array+0x2e>
 8022d42:	bd70      	pop	{r4, r5, r6, pc}
 8022d44:	bd70      	pop	{r4, r5, r6, pc}
 8022d46:	bf00      	nop
 8022d48:	08023180 	.word	0x08023180
 8022d4c:	08023180 	.word	0x08023180
 8022d50:	08023188 	.word	0x08023188
 8022d54:	08023180 	.word	0x08023180

08022d58 <register_fini>:
 8022d58:	4b02      	ldr	r3, [pc, #8]	; (8022d64 <register_fini+0xc>)
 8022d5a:	b113      	cbz	r3, 8022d62 <register_fini+0xa>
 8022d5c:	4802      	ldr	r0, [pc, #8]	; (8022d68 <register_fini+0x10>)
 8022d5e:	f000 b805 	b.w	8022d6c <atexit>
 8022d62:	4770      	bx	lr
 8022d64:	00000000 	.word	0x00000000
 8022d68:	08022d79 	.word	0x08022d79

08022d6c <atexit>:
 8022d6c:	2300      	movs	r3, #0
 8022d6e:	4601      	mov	r1, r0
 8022d70:	461a      	mov	r2, r3
 8022d72:	4618      	mov	r0, r3
 8022d74:	f000 b814 	b.w	8022da0 <__register_exitproc>

08022d78 <__libc_fini_array>:
 8022d78:	b538      	push	{r3, r4, r5, lr}
 8022d7a:	4d07      	ldr	r5, [pc, #28]	; (8022d98 <__libc_fini_array+0x20>)
 8022d7c:	4c07      	ldr	r4, [pc, #28]	; (8022d9c <__libc_fini_array+0x24>)
 8022d7e:	1b2c      	subs	r4, r5, r4
 8022d80:	10a4      	asrs	r4, r4, #2
 8022d82:	d005      	beq.n	8022d90 <__libc_fini_array+0x18>
 8022d84:	3c01      	subs	r4, #1
 8022d86:	f855 3d04 	ldr.w	r3, [r5, #-4]!
 8022d8a:	4798      	blx	r3
 8022d8c:	2c00      	cmp	r4, #0
 8022d8e:	d1f9      	bne.n	8022d84 <__libc_fini_array+0xc>
 8022d90:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8022d94:	f000 b9ee 	b.w	8023174 <_fini>
 8022d98:	0802318c 	.word	0x0802318c
 8022d9c:	08023188 	.word	0x08023188

08022da0 <__register_exitproc>:
 8022da0:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8022da4:	4c25      	ldr	r4, [pc, #148]	; (8022e3c <__register_exitproc+0x9c>)
 8022da6:	6825      	ldr	r5, [r4, #0]
 8022da8:	f8d5 4148 	ldr.w	r4, [r5, #328]	; 0x148
 8022dac:	4606      	mov	r6, r0
 8022dae:	4688      	mov	r8, r1
 8022db0:	4692      	mov	sl, r2
 8022db2:	4699      	mov	r9, r3
 8022db4:	b3c4      	cbz	r4, 8022e28 <__register_exitproc+0x88>
 8022db6:	6860      	ldr	r0, [r4, #4]
 8022db8:	281f      	cmp	r0, #31
 8022dba:	dc17      	bgt.n	8022dec <__register_exitproc+0x4c>
 8022dbc:	1c43      	adds	r3, r0, #1
 8022dbe:	b176      	cbz	r6, 8022dde <__register_exitproc+0x3e>
 8022dc0:	eb04 0580 	add.w	r5, r4, r0, lsl #2
 8022dc4:	2201      	movs	r2, #1
 8022dc6:	f8c5 a088 	str.w	sl, [r5, #136]	; 0x88
 8022dca:	f8d4 1188 	ldr.w	r1, [r4, #392]	; 0x188
 8022dce:	4082      	lsls	r2, r0
 8022dd0:	4311      	orrs	r1, r2
 8022dd2:	2e02      	cmp	r6, #2
 8022dd4:	f8c4 1188 	str.w	r1, [r4, #392]	; 0x188
 8022dd8:	f8c5 9108 	str.w	r9, [r5, #264]	; 0x108
 8022ddc:	d01e      	beq.n	8022e1c <__register_exitproc+0x7c>
 8022dde:	3002      	adds	r0, #2
 8022de0:	6063      	str	r3, [r4, #4]
 8022de2:	f844 8020 	str.w	r8, [r4, r0, lsl #2]
 8022de6:	2000      	movs	r0, #0
 8022de8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8022dec:	4b14      	ldr	r3, [pc, #80]	; (8022e40 <__register_exitproc+0xa0>)
 8022dee:	b303      	cbz	r3, 8022e32 <__register_exitproc+0x92>
 8022df0:	f44f 70c8 	mov.w	r0, #400	; 0x190
 8022df4:	f3af 8000 	nop.w
 8022df8:	4604      	mov	r4, r0
 8022dfa:	b1d0      	cbz	r0, 8022e32 <__register_exitproc+0x92>
 8022dfc:	f8d5 3148 	ldr.w	r3, [r5, #328]	; 0x148
 8022e00:	2700      	movs	r7, #0
 8022e02:	e880 0088 	stmia.w	r0, {r3, r7}
 8022e06:	f8c5 4148 	str.w	r4, [r5, #328]	; 0x148
 8022e0a:	4638      	mov	r0, r7
 8022e0c:	2301      	movs	r3, #1
 8022e0e:	f8c4 7188 	str.w	r7, [r4, #392]	; 0x188
 8022e12:	f8c4 718c 	str.w	r7, [r4, #396]	; 0x18c
 8022e16:	2e00      	cmp	r6, #0
 8022e18:	d0e1      	beq.n	8022dde <__register_exitproc+0x3e>
 8022e1a:	e7d1      	b.n	8022dc0 <__register_exitproc+0x20>
 8022e1c:	f8d4 118c 	ldr.w	r1, [r4, #396]	; 0x18c
 8022e20:	430a      	orrs	r2, r1
 8022e22:	f8c4 218c 	str.w	r2, [r4, #396]	; 0x18c
 8022e26:	e7da      	b.n	8022dde <__register_exitproc+0x3e>
 8022e28:	f505 74a6 	add.w	r4, r5, #332	; 0x14c
 8022e2c:	f8c5 4148 	str.w	r4, [r5, #328]	; 0x148
 8022e30:	e7c1      	b.n	8022db6 <__register_exitproc+0x16>
 8022e32:	f04f 30ff 	mov.w	r0, #4294967295
 8022e36:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8022e3a:	bf00      	nop
 8022e3c:	08023150 	.word	0x08023150
 8022e40:	00000000 	.word	0x00000000
 8022e44:	20525349 	.word	0x20525349
 8022e48:	6c69614d 	.word	0x6c69614d
 8022e4c:	20786f62 	.word	0x20786f62
 8022e50:	646e6573 	.word	0x646e6573
 8022e54:	69616620 	.word	0x69616620
 8022e58:	6572756c 	.word	0x6572756c
 8022e5c:	0000203a 	.word	0x0000203a
 8022e60:	6e657320 	.word	0x6e657320
 8022e64:	0d2e7364 	.word	0x0d2e7364
 8022e68:	0000000a 	.word	0x0000000a
 8022e6c:	20525349 	.word	0x20525349
 8022e70:	616d6573 	.word	0x616d6573
 8022e74:	726f6870 	.word	0x726f6870
 8022e78:	6f702065 	.word	0x6f702065
 8022e7c:	66207473 	.word	0x66207473
 8022e80:	756c6961 	.word	0x756c6961
 8022e84:	203a6572 	.word	0x203a6572
 8022e88:	00000000 	.word	0x00000000
 8022e8c:	736f7020 	.word	0x736f7020
 8022e90:	0d2e7374 	.word	0x0d2e7374
 8022e94:	0000000a 	.word	0x0000000a
 8022e98:	2a2a0a0d 	.word	0x2a2a0a0d
 8022e9c:	4b0a0d2a 	.word	0x4b0a0d2a
 8022ea0:	656e7265 	.word	0x656e7265
 8022ea4:	6170206c 	.word	0x6170206c
 8022ea8:	2063696e 	.word	0x2063696e
 8022eac:	6f6e202d 	.word	0x6f6e202d
 8022eb0:	79732074 	.word	0x79732074
 8022eb4:	6e69636e 	.word	0x6e69636e
 8022eb8:	0a0d3a67 	.word	0x0a0d3a67
 8022ebc:	00000000 	.word	0x00000000
 8022ec0:	64656d2f 	.word	0x64656d2f
 8022ec4:	702f6169 	.word	0x702f6169
 8022ec8:	432f7972 	.word	0x432f7972
 8022ecc:	2f65646f 	.word	0x2f65646f
 8022ed0:	65646f43 	.word	0x65646f43
 8022ed4:	62694c5f 	.word	0x62694c5f
 8022ed8:	79726172 	.word	0x79726172
 8022edc:	55434d2f 	.word	0x55434d2f
 8022ee0:	74754d2f 	.word	0x74754d2f
 8022ee4:	73757461 	.word	0x73757461
 8022ee8:	50354d2f 	.word	0x50354d2f
 8022eec:	754d5f31 	.word	0x754d5f31
 8022ef0:	6b6f7250 	.word	0x6b6f7250
 8022ef4:	6e6f7261 	.word	0x6e6f7261
 8022ef8:	72504d2f 	.word	0x72504d2f
 8022efc:	72616b6f 	.word	0x72616b6f
 8022f00:	422f6e6f 	.word	0x422f6e6f
 8022f04:	68636e65 	.word	0x68636e65
 8022f08:	6b72616d 	.word	0x6b72616d
 8022f0c:	616c502f 	.word	0x616c502f
 8022f10:	726f6674 	.word	0x726f6674
 8022f14:	6d722f6d 	.word	0x6d722f6d
 8022f18:	65745f70 	.word	0x65745f70
 8022f1c:	535f7473 	.word	0x535f7473
 8022f20:	32334d54 	.word	0x32334d54
 8022f24:	37363746 	.word	0x37363746
 8022f28:	725f4749 	.word	0x725f4749
 8022f2c:	682e6d76 	.word	0x682e6d76
 8022f30:	00000000 	.word	0x00000000
 8022f34:	4c202c20 	.word	0x4c202c20
 8022f38:	20656e69 	.word	0x20656e69
 8022f3c:	00000000 	.word	0x00000000
 8022f40:	20706553 	.word	0x20706553
 8022f44:	32203232 	.word	0x32203232
 8022f48:	00383130 	.word	0x00383130
 8022f4c:	00202c20 	.word	0x00202c20
 8022f50:	323a3731 	.word	0x323a3731
 8022f54:	32353a37 	.word	0x32353a37
 8022f58:	00000000 	.word	0x00000000
 8022f5c:	6f6d654d 	.word	0x6f6d654d
 8022f60:	74207972 	.word	0x74207972
 8022f64:	20747365 	.word	0x20747365
 8022f68:	6c696166 	.word	0x6c696166
 8022f6c:	3a657275 	.word	0x3a657275
 8022f70:	00000020 	.word	0x00000020
 8022f74:	6e757220 	.word	0x6e757220
 8022f78:	0a0d2e73 	.word	0x0a0d2e73
 8022f7c:	00000000 	.word	0x00000000
 8022f80:	6f6d654d 	.word	0x6f6d654d
 8022f84:	203a7972 	.word	0x203a7972
 8022f88:	00000000 	.word	0x00000000
 8022f8c:	63796320 	.word	0x63796320
 8022f90:	2e73656c 	.word	0x2e73656c
 8022f94:	00000a0d 	.word	0x00000a0d
 8022f98:	6c656959 	.word	0x6c656959
 8022f9c:	00203a64 	.word	0x00203a64
 8022fa0:	6c69614d 	.word	0x6c69614d
 8022fa4:	3a786f62 	.word	0x3a786f62
 8022fa8:	00000020 	.word	0x00000020
 8022fac:	616d6553 	.word	0x616d6553
 8022fb0:	726f6870 	.word	0x726f6870
 8022fb4:	00203a65 	.word	0x00203a65
 8022fb8:	6c69614d 	.word	0x6c69614d
 8022fbc:	2d786f62 	.word	0x2d786f62
 8022fc0:	3a525349 	.word	0x3a525349
 8022fc4:	00000020 	.word	0x00000020
 8022fc8:	616d6553 	.word	0x616d6553
 8022fcc:	726f6870 	.word	0x726f6870
 8022fd0:	53492d65 	.word	0x53492d65
 8022fd4:	00203a52 	.word	0x00203a52

08022fd8 <RVM_Img>:
 8022fd8:	56495254 316d6f44 00000000 00000000     TRIVDom1........
 8022fe8:	00000000 08020005 20029214 00000100     ........... ....
 8022ff8:	08021115 20029314 00000400 200290f4     ....... ....... 
 8023008:	20029088 00000001 0000000a 00000080     ... ............
 8023018:	20029188 00000020 200290f0 00000001     ...  ...... ....
 8023028:	08023038 00000001 0802304c 00000000     80......L0......

08023038 <RVM_Pgtbl>:
	...
 8023040:	001d0003 3f3f3f3f 3f3f3f3f              ....????????

0802304c <RVM_Kcap>:
 802304c:	0000001e                                ....

08023050 <RMP_RBIT_Table>:
 8023050:	c0408000 e060a020 d0509010 f070b030     ..@. .`...P.0.p.
 8023060:	c8488808 e868a828 d8589818 f878b838     ..H.(.h...X.8.x.
 8023070:	c4448404 e464a424 d4549414 f474b434     ..D.$.d...T.4.t.
 8023080:	cc4c8c0c ec6cac2c dc5c9c1c fc7cbc3c     ..L.,.l...\.<.|.
 8023090:	c2428202 e262a222 d2529212 f272b232     ..B.".b...R.2.r.
 80230a0:	ca4a8a0a ea6aaa2a da5a9a1a fa7aba3a     ..J.*.j...Z.:.z.
 80230b0:	c6468606 e666a626 d6569616 f676b636     ..F.&.f...V.6.v.
 80230c0:	ce4e8e0e ee6eae2e de5e9e1e fe7ebe3e     ..N...n...^.>.~.
 80230d0:	c1418101 e161a121 d1519111 f171b131     ..A.!.a...Q.1.q.
 80230e0:	c9498909 e969a929 d9599919 f979b939     ..I.).i...Y.9.y.
 80230f0:	c5458505 e565a525 d5559515 f575b535     ..E.%.e...U.5.u.
 8023100:	cd4d8d0d ed6dad2d dd5d9d1d fd7dbd3d     ..M.-.m...].=.}.
 8023110:	c3438303 e363a323 d3539313 f373b333     ..C.#.c...S.3.s.
 8023120:	cb4b8b0b eb6bab2b db5b9b1b fb7bbb3b     ..K.+.k...[.;.{.
 8023130:	c7478707 e767a727 d7579717 f777b737     ..G.'.g...W.7.w.
 8023140:	cf4f8f0f ef6faf2f df5f9f1f ff7fbf3f     ..O./.o..._.?...

08023150 <_global_impure_ptr>:
 8023150:	20020008                                ... 

Disassembly of section .inits:

08023154 <__data_regions_array_start>:
 8023154:	08024000 	.word	0x08024000
 8023158:	20020000 	.word	0x20020000
 802315c:	20020430 	.word	0x20020430

08023160 <__bss_regions_array_start>:
 8023160:	20020430 	.word	0x20020430
 8023164:	20029214 	.word	0x20029214

08023168 <_init>:
 8023168:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 802316a:	bf00      	nop
 802316c:	bcf8      	pop	{r3, r4, r5, r6, r7}
 802316e:	bc08      	pop	{r3}
 8023170:	469e      	mov	lr, r3
 8023172:	4770      	bx	lr

08023174 <_fini>:
 8023174:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8023176:	bf00      	nop
 8023178:	bcf8      	pop	{r3, r4, r5, r6, r7}
 802317a:	bc08      	pop	{r3}
 802317c:	469e      	mov	lr, r3
 802317e:	4770      	bx	lr

08023180 <__init_array_start>:
 8023180:	08022d59 	.word	0x08022d59

08023184 <__frame_dummy_init_array_entry>:
 8023184:	08020115                                ....

08023188 <__do_global_dtors_aux_fini_array_entry>:
 8023188:	080200f1                                ....
