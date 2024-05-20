/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRaNTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "rmp.h"
#include "core_riscv.h"
#include "string.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/snmp.h"
#include "arch/ethernetif.h"
#include "netif/etharp.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_rng.h"

/* Ethernet DMA memory info */
__attribute__((aligned(4))) ETH_DMADESCTypeDef ETH_DMAMEM_TX_DESC[ETH_TXBUFNB];
__attribute__((aligned(4))) ETH_DMADESCTypeDef ETH_DMAMEM_RX_DESC[ETH_RXBUFNB];
__attribute__((aligned(4))) rmp_u8_t Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
__attribute__((aligned(4))) rmp_u8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
rmp_u8_t ETH_MAC_Addr[6]={0x38,0x3B,0x26,0x62,0x52,0x15};
struct RMP_Sem ETH_Rx_Semaphore;

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif* netif)
{
    /* Define all variables */
    GPIO_InitTypeDef GPIO={0};
    static u32_t timeout;
    static ETH_InitTypeDef* ETH_InitStructure;
    u32_t Reg_Tmp;
    static uint16_t Reg_Value;

    /* Initialize variables */
    Reg_Tmp=0;
    Reg_Value=0;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO.GPIO_Pin=GPIO_Pin_8;
    GPIO.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO);

    RCC_PLL3Cmd(DISABLE);
    RCC_PREDIV2Config(RCC_PREDIV2_Div2);
    RCC_PLL3Config(RCC_PLL3Mul_15);
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
    RCC_PLL3Cmd(ENABLE);
    RMP_Thd_Delay(100);

    if(RCC_GetFlagStatus(RCC_FLAG_PLL3RDY)==RESET)
    {
        RMP_DBG_S("Wait for PLL3 ready.\n");
        RMP_Thd_Delay(100);
    }

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    /* Ethernet LED Configuration */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
    GPIO.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO);
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
    GPIO_SetBits(GPIOB,GPIO_Pin_9);

    /* Ethernet_Configuration */
    ETH_InitStructure=mem_malloc(sizeof(ETH_InitTypeDef));
    memset(ETH_InitStructure,0,sizeof(ETH_InitTypeDef));
    /* Enable Ethernet MAC clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC|RCC_AHBPeriph_ETH_MAC_Tx|RCC_AHBPeriph_ETH_MAC_Rx,ENABLE);

    EXTEN->EXTEN_CTR|=EXTEN_ETH_10M_EN;
    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();
    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    timeout=10;
    if(ETH->DMABMR&ETH_DMABMR_SR)
    {
        timeout--;
        /* Error:Eth soft-reset timeout! */
        /* Please check RGMII TX & RX clock line. */
        RMP_ASSERT(timeout);
        RMP_Thd_Delay(100);
    }

    /* Ethernet Configuration */
    ETH_StructInit(ETH_InitStructure);

    /* Fill ETH_InitStructure parameters */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure->ETH_Mode=ETH_Mode_FullDuplex;
    ETH_InitStructure->ETH_Speed=ETH_Speed_1000M;
    ETH_InitStructure->ETH_AutoNegotiation=ETH_AutoNegotiation_Enable  ;
    ETH_InitStructure->ETH_LoopbackMode=ETH_LoopbackMode_Disable;
    ETH_InitStructure->ETH_RetryTransmission=ETH_RetryTransmission_Disable;
    ETH_InitStructure->ETH_AutomaticPadCRCStrip=ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure->ETH_ReceiveAll=ETH_ReceiveAll_Enable;
    ETH_InitStructure->ETH_BroadcastFramesReception=ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure->ETH_PromiscuousMode=ETH_PromiscuousMode_Enable;
    ETH_InitStructure->ETH_MulticastFramesFilter=ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure->ETH_UnicastFramesFilter=ETH_UnicastFramesFilter_Perfect;
    ETH_InitStructure->ETH_ChecksumOffload=ETH_ChecksumOffload_Enable;

    /*------------------------   DMA   -----------------------------------*/
    ETH_InitStructure->ETH_DropTCPIPChecksumErrorFrame=ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure->ETH_ReceiveStoreForward=ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure->ETH_TransmitStoreForward=ETH_TransmitStoreForward_Enable;
    ETH_InitStructure->ETH_ForwardErrorFrames=ETH_ForwardErrorFrames_Enable;
    ETH_InitStructure->ETH_ForwardUndersizedGoodFrames=ETH_ForwardUndersizedGoodFrames_Enable;
    ETH_InitStructure->ETH_SecondFrameOperate=ETH_SecondFrameOperate_Disable;
    ETH_InitStructure->ETH_AddressAlignedBeats=ETH_AddressAlignedBeats_Enable;
    ETH_InitStructure->ETH_FixedBurst=ETH_FixedBurst_Enable;
    ETH_InitStructure->ETH_RxDMABurstLength=ETH_RxDMABurstLength_32Beat;
    ETH_InitStructure->ETH_TxDMABurstLength=ETH_TxDMABurstLength_32Beat;
    ETH_InitStructure->ETH_DMAArbitration=ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* PHY layer configuration */
    Reg_Tmp=ETH->MACMIIAR;
    Reg_Tmp&=MACMIIAR_CR_MASK;
    Reg_Tmp|=(uint32_t)ETH_MACMIIAR_CR_Div42;
    ETH->MACMIIAR=(uint32_t)Reg_Tmp;

    /* Reset PHY layer */
    ETH_WritePHYRegister(PHY_ADDRESS,PHY_BCR,PHY_Reset);
    RMP_Thd_Delay(100);

    timeout=10000;
    Reg_Value=ETH_ReadPHYRegister(PHY_ADDRESS,PHY_BCR);
    if((Reg_Value&(PHY_Reset)))
    {
        timeout--;
        /* Error: Wait PHY software timeout! Please check PHY/MID. */
        while(timeout<=0) ;
        RMP_Thd_Delay(100);
    }

    timeout=10000;
    Reg_Value=ETH_ReadPHYRegister(PHY_ADDRESS,PHY_BSR);
    if((Reg_Value&(PHY_Linked_Status))==0)
    {
        timeout--;
        /* Error: Wait PHY linking timeout! */
        while(timeout<=0) ;
        RMP_Thd_Delay(100);
    }

    timeout=10000;
    Reg_Value=ETH_ReadPHYRegister(PHY_ADDRESS,PHY_BSR);
    if((Reg_Value&PHY_AutoNego_Complete)==0)
    {
        timeout--;
        /* Error: Wait PHY auto-negotiation complete timeout! */
        while(timeout<=0) ;
        RMP_Thd_Delay(100);
    }

    /* print some info */
    Reg_Value=ETH_ReadPHYRegister(PHY_ADDRESS,0x10);

    ETH_InitStructure->ETH_Mode=Reg_Value&(1<<2)?ETH_Mode_FullDuplex:ETH_Mode_HalfDuplex;
    ETH_InitStructure->ETH_Speed=ETH_Speed_10M;
    RMP_Thd_Delay(100);
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    /* MAC Register Configuration */
    Reg_Tmp=ETH->MACCR;
    Reg_Tmp&=MACCR_CLEAR_MASK;
    Reg_Tmp|=(uint32_t)(ETH_InitStructure->ETH_Watchdog|
                        ETH_InitStructure->ETH_Jabber|
                        ETH_InitStructure->ETH_InterFrameGap|
                        ETH_InitStructure->ETH_CarrierSense|
                        ETH_InitStructure->ETH_Speed|
                        ETH_InitStructure->ETH_ReceiveOwn|
                        ETH_InitStructure->ETH_LoopbackMode|
                        ETH_InitStructure->ETH_Mode|
                        ETH_InitStructure->ETH_ChecksumOffload|
                        ETH_InitStructure->ETH_RetryTransmission|
                        ETH_InitStructure->ETH_AutomaticPadCRCStrip|
                        ETH_InitStructure->ETH_BackOffLimit|
                        ETH_InitStructure->ETH_DeferralCheck);
    ETH->MACCR=(uint32_t)Reg_Tmp;
    ETH->MACCR|=ETH_Internal_Pull_Up_Res_Enable;
    ETH->MACFFR=(uint32_t)(ETH_InitStructure->ETH_ReceiveAll|
                           ETH_InitStructure->ETH_SourceAddrFilter|
                           ETH_InitStructure->ETH_PassControlFrames|
                           ETH_InitStructure->ETH_BroadcastFramesReception|
                           ETH_InitStructure->ETH_DestinationAddrFilter|
                           ETH_InitStructure->ETH_PromiscuousMode|
                           ETH_InitStructure->ETH_MulticastFramesFilter|
                           ETH_InitStructure->ETH_UnicastFramesFilter);

    /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
    /* Write to ETHERNET MACHTHR */
    ETH->MACHTHR=(uint32_t)ETH_InitStructure->ETH_HashTableHigh;
    /* Write to ETHERNET MACHTLR */
    ETH->MACHTLR=(uint32_t)ETH_InitStructure->ETH_HashTableLow;

    /*----------------------- ETHERNET MACFCR Configuration --------------------*/
    /* Get the ETHERNET MACFCR value */
    Reg_Tmp=ETH->MACFCR;
    /* Clear xx bits */
    Reg_Tmp&=MACFCR_CLEAR_MASK;
    Reg_Tmp|=(uint32_t)((ETH_InitStructure->ETH_PauseTime<<16)|
                         ETH_InitStructure->ETH_ZeroQuantaPause|
                         ETH_InitStructure->ETH_PauseLowThreshold|
                         ETH_InitStructure->ETH_UnicastPauseFrameDetect|
                         ETH_InitStructure->ETH_ReceiveFlowControl|
                         ETH_InitStructure->ETH_TransmitFlowControl);
    ETH->MACFCR=(uint32_t)Reg_Tmp;
    ETH->MACVLANTR=(uint32_t)(ETH_InitStructure->ETH_VLANTagComparison|
                              ETH_InitStructure->ETH_VLANTagIdentifier);
    Reg_Tmp=ETH->DMAOMR;
    Reg_Tmp&=DMAOMR_CLEAR_MASK;
    Reg_Tmp|=(uint32_t)(ETH_InitStructure->ETH_DropTCPIPChecksumErrorFrame|
                        ETH_InitStructure->ETH_ReceiveStoreForward|
                        ETH_InitStructure->ETH_FlushReceivedFrame|
                        ETH_InitStructure->ETH_TransmitStoreForward|
                        ETH_InitStructure->ETH_TransmitThresholdControl|
                        ETH_InitStructure->ETH_ForwardErrorFrames|
                        ETH_InitStructure->ETH_ForwardUndersizedGoodFrames|
                        ETH_InitStructure->ETH_ReceiveThresholdControl|
                        ETH_InitStructure->ETH_SecondFrameOperate);
    ETH->DMAOMR=(uint32_t)Reg_Tmp;
    ETH->DMABMR=(uint32_t)(ETH_InitStructure->ETH_AddressAlignedBeats|
                           ETH_InitStructure->ETH_FixedBurst|
                           ETH_InitStructure->ETH_RxDMABurstLength|
                           ETH_InitStructure->ETH_TxDMABurstLength|
                          (ETH_InitStructure->ETH_DescriptorSkipLength<<2)|
                           ETH_InitStructure->ETH_DMAArbitration|
                           ETH_DMABMR_USP);
    mem_free(ETH_InitStructure);
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R|ETH_DMA_IT_T,ENABLE);
    NVIC_SetPriority(ETH_IRQn,0xff);
    NVIC_EnableIRQ(ETH_IRQn);
    ETH_DMATxDescChainInit(ETH_DMAMEM_TX_DESC,&Tx_Buff[0][0],ETH_TXBUFNB);
    ETH_DMARxDescChainInit(ETH_DMAMEM_RX_DESC,&Rx_Buff[0][0],ETH_RXBUFNB);
    ETH_Start();

    /* Initialize netif */
    netif->hwaddr_len=ETHARP_HWADDR_LEN;
    netif->hwaddr[0]=ETH_MAC_Addr[0]; 
    netif->hwaddr[1]=ETH_MAC_Addr[1]; 
    netif->hwaddr[2]=ETH_MAC_Addr[2];
    netif->hwaddr[3]=ETH_MAC_Addr[3];   
    netif->hwaddr[4]=ETH_MAC_Addr[4];
    netif->hwaddr[5]=ETH_MAC_Addr[5];
    netif->mtu=1500;
    netif->flags=NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP|NETIF_FLAG_UP;

    /* Create a semaphore and a task to handle receive */
    RMP_Sem_Crt(&ETH_Rx_Semaphore,0);
    sys_thread_new("eth_thread",
                   ethernetif_input,
                   netif,
                   NETIF_IN_TASK_STACK_SIZE,
                   NETIF_IN_TASK_PRIO);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t
low_level_output(struct netif* netif,struct pbuf* p)
{
    /* Define and initialize variables */
    struct pbuf* Iter_Buffer;
    u16_t Length;
    u8_t* Buffer_Ptr;

    Length=p->tot_len;
    Buffer_Ptr=(u8_t*)ETH_GetCurrentTxBufferAddress();

#if ETH_PAD_SIZE
  pbuf_remove_header(p,ETH_PAD_SIZE); /* drop the padding word */
#endif

    for(Iter_Buffer=p;Iter_Buffer!=NULL;Iter_Buffer=Iter_Buffer->next)
    {
        /* Send the data from pbuf to interface, one pbuf at a time. */
        /* The size of data in each pbuf is kept in the ->len varibale. */
        memcpy(Buffer_Ptr,Iter_Buffer->payload,Iter_Buffer->len);
        Buffer_Ptr+=Iter_Buffer->len;
    }

    /* Check if the packet should be sent() */
    if((DMATxDescToSet->Status&ETH_DMATxDesc_OWN)==(u32)RESET)
    {
        /* Setting the Frame Length: bits[12:0] */
        DMATxDescToSet->ControlBufferSize=(Length&ETH_DMATxDesc_TBS1);
        DMATxDescToSet->Status|=ETH_DMATxDesc_LS|ETH_DMATxDesc_FS|ETH_DMATxDesc_CIC_TCPUDPICMP_Full|ETH_DMATxDesc_OWN;
        /* When Tx Buffer unavailable flag is set: clear it and resume transmission */
        if((ETH->DMASR&ETH_DMASR_TBUS)!=(u32)RESET)
        {
            /* Clear TBUS ETHERNET DMA flag */
            ETH->DMASR=ETH_DMASR_TBUS;
            /* Resume DMA transmission*/
            ETH->DMATPDR=0;
        }
        /* Selects the next DMA Tx descriptor list for next buffer to send */
        DMATxDescToSet=(ETH_DMADESCTypeDef*)(DMATxDescToSet->Buffer2NextDescAddr);
    }

    MIB2_STATS_NETIF_ADD(netif,ifoutoctets,Length);
    /* Check if broadcast or multicast packet or not. */
    if(((rmp_u8_t*)p->payload)[0]&1)
        MIB2_STATS_NETIF_INC(netif,ifoutnucastpkts);
    else
        MIB2_STATS_NETIF_INC(netif,ifoutucastpkts);

#if ETH_PAD_SIZE
  pbuf_add_header(p,ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         RMP_NULL on memory error
 */
static struct pbuf*
low_level_input(struct netif* netif)
{
    struct pbuf* Ret_Buffer;
    struct pbuf* Iter_Buffer;
    u8_t* Buffer_Ptr;

    /* Relevant struct associated with data frame. */
    void* Frame_Next;
    u32_t Frame_Buffer;
    u32_t Frame_Length;
    ETH_DMADESCTypeDef* Frame_Desc;

    Frame_Next=RMP_NULL;

    if((DMARxDescToGet->Status&ETH_DMARxDesc_OWN)!=(u32)RESET)
    {
        Frame_Length=ETH_ERROR;
        if((ETH->DMASR&ETH_DMASR_RBUS)!=(u32)RESET)
        {
            /* Clear RBUS ETHERNET DMA flag */
            ETH->DMASR=ETH_DMASR_RBUS;
            /* Resume DMA reception */
            ETH->DMARPDR=0;
        }
        RMP_DBG_S("ethernetif.c line 474: Error: ETH_DMARxDesc_OWN.\r\n");
    }
    else
    {
        if(((DMARxDescToGet->Status&ETH_DMARxDesc_ES)==(u32)RESET)&&
           ((DMARxDescToGet->Status&ETH_DMARxDesc_LS)!=(u32)RESET)&&
           ((DMARxDescToGet->Status&ETH_DMARxDesc_FS)!=(u32)RESET))
        {
            /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
            Frame_Length=((DMARxDescToGet->Status&ETH_DMARxDesc_FL)>>16)-4;
            Frame_Buffer=DMARxDescToGet->Buffer1Addr;
        }
        else
        {
            Frame_Length=ETH_ERROR;
            RMP_DBG_S("ethernetif.c line 489: Error: recv error frame, status :" );
            RMP_DBG_I(DMARxDescToGet->Status);
            RMP_DBG_S("\r\n" );
        }
        Frame_Desc=DMARxDescToGet;
        DMARxDescToGet=(ETH_DMADESCTypeDef*)(DMARxDescToGet->Buffer2NextDescAddr);
    }

    Buffer_Ptr=(u8_t*)Frame_Buffer;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
    if(Frame_Length==ETH_ERROR)
        return RMP_NULL;
    Ret_Buffer=pbuf_alloc(PBUF_RAW,Frame_Length,PBUF_POOL);
    if(Ret_Buffer!=RMP_NULL)
    {
        for(Iter_Buffer=Ret_Buffer;Iter_Buffer!=NULL;Iter_Buffer=Iter_Buffer->next)
        {
            memcpy(Iter_Buffer->payload,Buffer_Ptr,Iter_Buffer->len);
            Buffer_Ptr+=Iter_Buffer->len;
        }
        MIB2_STATS_NETIF_ADD(netif,ifoutoctets,Length);
        /* Check if broadcast or multicast packet or not. */
        if(((u8_t*)Ret_Buffer->payload)[0]&1)
            MIB2_STATS_NETIF_INC(netif,ifoutnucastpkts);
        else
            MIB2_STATS_NETIF_INC(netif,ifoutucastpkts);
        LINK_STATS_INC(link.recv);
    }
    else
    {
        /* Put drop packet handler function here. */
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif,ifindiscards);
    }

    Frame_Desc->Status|=ETH_DMARxDesc_OWN;

    return Ret_Buffer;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void _ETH_IRQHandler(void)
{
    if(ETH->DMASR&ETH_DMA_IT_R)
    {
        ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
        RMP_Sem_Post_ISR(&ETH_Rx_Semaphore,1);
    }

    if(ETH->DMASR&ETH_DMA_IT_T)
        ETH_DMAClearITPendingBit(ETH_DMA_IT_T);

    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}
void
ethernetif_input(void* netif)
{
    struct netif* Netif;
    struct pbuf* Tmp_Buffer;
    
    Netif=(struct netif*)netif;
    LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));

    while(1)
    {
        if(RMP_Sem_Pend(&ETH_Rx_Semaphore,RMP_SLICE_MAX)!=RMP_ERR_OPER)
        {
            /* Pull packet from the driver */
            Tmp_Buffer=low_level_input(Netif);
            /* If no packet could be read, just ignore it. */
            /* Pass all packets to ethernet_input, which decides what packets it supports. */
            if(Tmp_Buffer==NULL) continue;

            if(Netif->input(Tmp_Buffer,Netif)!=ERR_OK)
                pbuf_free(Tmp_Buffer);
        }
    }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif* netif)
{
    struct ethernetif* ETH_If;

    LWIP_ASSERT("netif!=RMP_NULL",(netif!=RMP_NULL));

    ETH_If=mem_malloc(sizeof(struct ethernetif));
    
    if(ETH_If==RMP_NULL)
    {
        LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_init: out of memory\n"));
        return ERR_MEM;
    }

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname="lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    netif->state=ETH_If;
    netif->name[0]='c';
    netif->name[1]='h';
    /* We directly use etharp_output() here to save a function call.
    * You can instead declare your own function an call etharp_output()
    * from it if you have to do some checks before sending (e.g. if link
    * is available...) */
#if LWIP_IPV4
    netif->output=etharp_output;
#endif /* LWIP_IPV4 */
#if LWIP_IPV6
    netif->output_ip6=ethip6_output;
#endif /* LWIP_IPV6 */
    netif->linkoutput=low_level_output;

    ETH_If->ethaddr=(struct eth_addr*)&(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}
