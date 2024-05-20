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
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
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
#include "swiic.h"
#include "pcf8574.h"
#include "string.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/snmp.h"
#include "arch/ethernetif.h"
#include "netif/etharp.h"

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
rmp_u8_t ETH_MAC_Addr[6]={0xB8,0xAE,0x1D,0x00,0x03,0x00};
ETH_HandleTypeDef ETH_Handler;
struct RMP_Sem ETH_Rx_Semaphore;

/* Helper functions */
uint32_t HAL_GetTick(void)
{
    return RMP_Timestamp;
}

void HAL_Delay(uint32_t Delay)
{
    RMP_Thd_Delay(Delay);
}

swiic_ret_t SDA_Read(swiic_ptr_t* Val)
{
    *Val=HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_5);
    if(*Val==0||*Val==1)
        return 0;
    else
        return -1;
}

swiic_ret_t SDA_Write(swiic_ptr_t Val)
{
    if(Val==1)
    {
        HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_SET);
        return 0;
    }
    else if(Val==0)
    {
        HAL_GPIO_WritePin(GPIOH,GPIO_PIN_5,GPIO_PIN_RESET);
        return 0;
    }
    else return -1;
}

swiic_ret_t SCL_Read(swiic_ptr_t* Val)
{
    *Val=HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_4);
    if(*Val==0||*Val==1)
        return 0;
    else
        return -1;
}

swiic_ret_t SCL_Write(swiic_ptr_t Val)
{
    if(Val==1)
    {
        HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_SET);
        return 0;
    }
    else if(Val==0)
    {
        HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,GPIO_PIN_RESET);
        return 0;
    }
    else return -1;
}

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
    GPIO_InitTypeDef GPIO_Init_Struct;
    MPU_Region_InitTypeDef MPU_Region_Init_Struct;
    
    /* Clock initialization */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_ETH_CLK_ENABLE();
    
    /* SWIIC GPIO initialization */
    GPIO_Init_Struct.Pin=GPIO_PIN_4;
    GPIO_Init_Struct.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Struct.Pull=GPIO_PULLUP;
    GPIO_Init_Struct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOH,&GPIO_Init_Struct);
    
    GPIO_Init_Struct.Pin=GPIO_PIN_5;
    GPIO_Init_Struct.Mode=GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(GPIOH,&GPIO_Init_Struct);

    /* RMII GPIO initialization
     * ETH_MDIO -------------------------> PA2
     * ETH_MDC --------------------------> PC1
     * ETH_RMII_REF_CLK------------------> PA1
     * ETH_RMII_CRS_DV ------------------> PA7
     * ETH_RMII_RXD0 --------------------> PC4
     * ETH_RMII_RXD1 --------------------> PC5
     * ETH_RMII_TX_EN -------------------> PB11
     * ETH_RMII_TXD0 --------------------> PG13
     * ETH_RMII_TXD1 --------------------> PG14 */
    GPIO_Init_Struct.Mode=GPIO_MODE_AF_PP;
    GPIO_Init_Struct.Pull=GPIO_NOPULL;
    GPIO_Init_Struct.Speed=GPIO_SPEED_HIGH;
    GPIO_Init_Struct.Alternate=GPIO_AF11_ETH;
    /* ETH_CLK/ETH_MDIO/ETH_CRS */
    GPIO_Init_Struct.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA,&GPIO_Init_Struct);
    /* ETH_MDC/ETH_RXD0/ETH_RXD1 */
    GPIO_Init_Struct.Pin=GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC,&GPIO_Init_Struct);
    /* ETH_TX_EN */
    GPIO_Init_Struct.Pin=GPIO_PIN_11; 
    HAL_GPIO_Init(GPIOB,&GPIO_Init_Struct);
    /* ETH_TXD0/ETH_TXD1 */
    GPIO_Init_Struct.Pin=GPIO_PIN_13|GPIO_PIN_14; 
    HAL_GPIO_Init(GPIOG,&GPIO_Init_Struct);
    
    /* PHY reset */
    PCF8574_Pin_Set(7,1);
    RMP_Thd_Delay(100);
    PCF8574_Pin_Set(7,0);
    RMP_Thd_Delay(100);

    /* MAC initialization */
    ETH_Handler.Instance=ETH;
    ETH_Handler.Init.AutoNegotiation=ETH_AUTONEGOTIATION_ENABLE;
    ETH_Handler.Init.Speed=ETH_SPEED_100M;
    ETH_Handler.Init.DuplexMode=ETH_MODE_FULLDUPLEX;
    ETH_Handler.Init.MACAddr=ETH_MAC_Addr;
    ETH_Handler.Init.RxMode=ETH_RXINTERRUPT_MODE;
    ETH_Handler.Init.ChecksumMode=ETH_CHECKSUM_BY_HARDWARE;
    ETH_Handler.Init.MediaInterface=ETH_MEDIA_INTERFACE_RMII;
    RMP_ASSERT(HAL_ETH_Init(&ETH_Handler)==HAL_OK);
    HAL_NVIC_SetPriority(ETH_IRQn,0x07,0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
    
    /* DMA Buffer initialization */
    HAL_MPU_Disable();
    MPU_Region_Init_Struct.Enable=MPU_REGION_ENABLE;
    MPU_Region_Init_Struct.Number=MPU_REGION_NUMBER1;
    MPU_Region_Init_Struct.BaseAddress=0x20070000;
    MPU_Region_Init_Struct.Size=MPU_REGION_SIZE_64KB;
    MPU_Region_Init_Struct.SubRegionDisable=0x00;
    MPU_Region_Init_Struct.TypeExtField=MPU_TEX_LEVEL0;
    MPU_Region_Init_Struct.DisableExec=MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_Region_Init_Struct.AccessPermission=MPU_REGION_FULL_ACCESS;
    MPU_Region_Init_Struct.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
    MPU_Region_Init_Struct.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
    MPU_Region_Init_Struct.IsBufferable=MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_Region_Init_Struct);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
    HAL_ETH_DMATxDescListInit(&ETH_Handler,ETH_DMAMEM_TX_DESC,ETH_DMAMEM_TX_BUF,ETH_TXBUFNB);
    HAL_ETH_DMARxDescListInit(&ETH_Handler,ETH_DMAMEM_RX_DESC,ETH_DMAMEM_RX_BUF,ETH_RXBUFNB);
    
    /* Initialize netif */
    netif->hwaddr_len=ETHARP_HWADDR_LEN;
    netif->hwaddr[0]=ETH_MAC_Addr[0]; 
    netif->hwaddr[1]=ETH_MAC_Addr[1]; 
    netif->hwaddr[2]=ETH_MAC_Addr[2];
    netif->hwaddr[3]=ETH_MAC_Addr[3];   
    netif->hwaddr[4]=ETH_MAC_Addr[4];
    netif->hwaddr[5]=ETH_MAC_Addr[5];
    netif->mtu=1500;
    netif->flags=NETIF_FLAG_BROADCAST|NETIF_FLAG_ETHARP|NETIF_FLAG_LINK_UP;

    /* Create a semaphore and a task to handle receive */
    RMP_Sem_Crt(&ETH_Rx_Semaphore,0);
    sys_thread_new("eth_thread",
                   ethernetif_input,
                   netif,
                   NETIF_IN_TASK_STACK_SIZE,
                   NETIF_IN_TASK_PRIO);
    
    /* Start ETH */
    HAL_ETH_Start(&ETH_Handler);
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
    err_t Ret;
    struct pbuf* Tmp_Buf;
    
    rmp_u8_t* Buffer;
    rmp_u8_t* Payload;
    __IO ETH_DMADescTypeDef* DMA_TX_Desc;
    rmp_u32_t Frame_Len;
    rmp_u32_t Buffer_Ptr;
    rmp_u32_t Bytes_Left;
    rmp_u32_t Payload_Ptr;

    Frame_Len=0;
    Buffer_Ptr=0;
    Bytes_Left=0;
    Buffer_Ptr=0;
    Payload_Ptr=0;
    DMA_TX_Desc=ETH_Handler.TxDesc;
    Buffer=(rmp_u8_t*)(ETH_Handler.TxDesc->Buffer1Addr);

#if ETH_PAD_SIZE
  pbuf_remove_header(p,ETH_PAD_SIZE); /* drop the padding word */
#endif
    
    /* Copy the data to be sent from pbuf. */
    for(Tmp_Buf=p;Tmp_Buf!=RMP_NULL;Tmp_Buf=Tmp_Buf->next)
    {
        Payload=(rmp_u8_t*)Tmp_Buf->payload;
        /* Check if this transmit descriptor is valid */
        /* Namely check if this transmit descriptor is owned by the Ethernet DMA */
        if((DMA_TX_Desc->Status&ETH_DMATXDESC_OWN)!=(rmp_u32_t)RESET)
        {
            /* Invalid descriptor */
            Ret=ERR_USE;
            goto error;
        }
        
        /*  Length of data to be sent */
        Bytes_Left=Tmp_Buf->len;
        Payload_Ptr=0;
        
        /* Write the data in pbuf to the Ethernet send descriptor */
        while((Bytes_Left+Buffer_Ptr)>ETH_TX_BUF_SIZE)
        {
            /* Copy data into the Tx Buffer of the Ethernet transmit descriptor */
            memcpy(&Buffer[Buffer_Ptr],&Payload[Payload_Ptr],(ETH_TX_BUF_SIZE-Buffer_Ptr));
            /* DmaTxDsc points to the next transmit descriptor */
            DMA_TX_Desc=(ETH_DMADescTypeDef*)(DMA_TX_Desc->Buffer2NextDescAddr);
            /* Check if the new send descriptor is valid */
            if((DMA_TX_Desc->Status&ETH_DMATXDESC_OWN)!=(rmp_u32_t)RESET)
            {
                /* Invalid descriptor */
                Ret=ERR_USE;
                goto error;
            }
            
            /* Update the Buffer address */
            Buffer=(rmp_u8_t*)(DMA_TX_Desc->Buffer1Addr);
            Bytes_Left=Bytes_Left-(ETH_TX_BUF_SIZE-Buffer_Ptr);
            Payload_Ptr=Payload_Ptr+(ETH_TX_BUF_SIZE-Buffer_Ptr);
            Frame_Len=Frame_Len+(ETH_TX_BUF_SIZE-Buffer_Ptr);
            Buffer_Ptr=0;
        }
        /* Copy the remaining data */
        memcpy(&Buffer[Buffer_Ptr],&Payload[Payload_Ptr],Bytes_Left);
        Buffer_Ptr=Buffer_Ptr+Bytes_Left;
        Frame_Len=Frame_Len+Bytes_Left;
    }
    
    HAL_ETH_TransmitFrame(&ETH_Handler,Frame_Len);
    Ret=ERR_OK;
error:            
    /* Send Buffer underflow */
    if((ETH_Handler.Instance->DMASR&ETH_DMASR_TUS)!=(rmp_u32_t)RESET)
    {
        /* Clear underflow flag */
        ETH_Handler.Instance->DMASR=ETH_DMASR_TUS;
        /* Wake-up TxDMA */
        ETH_Handler.Instance->DMATPDR=0;
    }
    
#if ETH_PAD_SIZE
  pbuf_add_header(p,ETH_PAD_SIZE); /* reclaim the padding word */
#endif
    
    return Ret;
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
    struct pbuf* Ret_Buf;
    struct pbuf* Tmp_Buf;
    rmp_u16_t Length;
    rmp_u8_t* Buffer;
    rmp_u8_t* Payload;
    __IO ETH_DMADescTypeDef* DMA_RX_Desc;
    rmp_u32_t Buffer_Ptr;
    rmp_u32_t Payload_Ptr;
    rmp_u32_t Bytes_Left;
    rmp_u32_t Count;
    
    Buffer_Ptr=0;
    Payload_Ptr=0;
    Bytes_Left=0;
    Count=0;
    
    /* Check if data is received */
    if(HAL_ETH_GetReceivedFrame(&ETH_Handler)!=HAL_OK)
        return RMP_NULL;
    
    /*  Get received Ethernet frame length */
    Length=ETH_Handler.RxFrameInfos.length;
    
#if ETH_PAD_SIZE
  Length += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif
    /* Get the data Buffer of the received Ethernet frame */
    Buffer=(rmp_u8_t*)ETH_Handler.RxFrameInfos.buffer;
    
    /* Allocate space for pbuf */
    Ret_Buf=pbuf_alloc(PBUF_RAW,Length,PBUF_POOL);
    
    if(Ret_Buf!=RMP_NULL)
    {
        /* Get the first descriptor in the receive descriptor linked list */
        DMA_RX_Desc=ETH_Handler.RxFrameInfos.FSRxDesc;
        Buffer_Ptr=0;
        
        for(Tmp_Buf=Ret_Buf;Tmp_Buf!=RMP_NULL;Tmp_Buf=Tmp_Buf->next)
        { 
            Payload=(rmp_u8_t*)Tmp_Buf->payload;
            Bytes_Left=Tmp_Buf->len;
            Payload_Ptr=0;
            
            /* Copy the data from the Rx Buffer to pbuf */
            while((Bytes_Left+Buffer_Ptr)>ETH_RX_BUF_SIZE)
            {
                /* Copy data to pbuf */
                memcpy(&Payload[Payload_Ptr],&Buffer[Buffer_Ptr],(ETH_RX_BUF_SIZE-Buffer_Ptr));
                 /* DMA_RX_Desc points to the next receive descriptor */
                DMA_RX_Desc=(ETH_DMADescTypeDef*)(DMA_RX_Desc->Buffer2NextDescAddr);
                /* Update Buffer address */
                Buffer=(rmp_u8_t*)(DMA_RX_Desc->Buffer1Addr);
 
                Bytes_Left=Bytes_Left-(ETH_RX_BUF_SIZE-Buffer_Ptr);
                Payload_Ptr=Payload_Ptr+(ETH_RX_BUF_SIZE-Buffer_Ptr);
                Buffer_Ptr=0;
            }
            /* Copy the remaining data */
            memcpy(&Payload[Payload_Ptr],&Buffer[Buffer_Ptr],Bytes_Left);
            Buffer_Ptr+=Bytes_Left;
        }
    }
    else
    {
        pbuf_free(Ret_Buf);
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
        MIB2_STATS_NETIF_INC(netif,ifindiscards);
    }
    
    /* Release DMA Descriptor */
    DMA_RX_Desc=ETH_Handler.RxFrameInfos.FSRxDesc;
    
    for(Count=0;Count<ETH_Handler.RxFrameInfos.SegCount;Count++)
    {
        /* Marker descriptors are owned by the DMA */
        DMA_RX_Desc->Status|=ETH_DMARXDESC_OWN;
        DMA_RX_Desc=(ETH_DMADescTypeDef*)(DMA_RX_Desc->Buffer2NextDescAddr);
    }
    /* Clear Segment Counter */
    ETH_Handler.RxFrameInfos.SegCount=0;
    
    if((ETH_Handler.Instance->DMASR&ETH_DMASR_RBUS)!=(rmp_u32_t)RESET)
    {
        /* Clear receive Buffer unavailable flag */
        ETH_Handler.Instance->DMASR=ETH_DMASR_RBUS;
        /* Wake-up RxDMA */
        ETH_Handler.Instance->DMARPDR=0;
    }
    
    return Ret_Buf;
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
void ETH_IRQHandler(void)
{
    /* If we receive a complete non-zero length frame, inform the stack */
    if(((ETH_Handler.RxDesc->Status&ETH_DMARXDESC_OWN)==(rmp_u32_t)RESET)&&
       ((ETH_Handler.RxDesc->Status&ETH_DMARXDESC_ES)==(rmp_u32_t)RESET)&&
       ((ETH_Handler.RxDesc->Status&ETH_DMARXDESC_LS)!=(rmp_u32_t)RESET))
    {
        if(((ETH_Handler.RxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT)!=0)
            RMP_Sem_Post_ISR(&ETH_Rx_Semaphore,1);
    }

    /* Clear possible interrupts */
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS);
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R);
}
void
ethernetif_input(void* netif)
{
    struct netif* Netif;
    struct pbuf* Tmp_Buf;
    
    Netif=(struct netif*)netif;
    LWIP_DEBUGF(NETIF_DEBUG,("ethernetif_input: IP input error\n"));

    while(1)
    {
        if(RMP_Sem_Pend(&ETH_Rx_Semaphore,RMP_SLICE_MAX)!=RMP_ERR_OPER)
        {
            /* Pull packet from the driver */
            Tmp_Buf=low_level_input(Netif);
            if(Tmp_Buf==RMP_NULL)
                continue;

            /* Throw the packet at the lwIP stack */
            if(Netif->input(Tmp_Buf,Netif)!=ERR_OK)
                pbuf_free(Tmp_Buf);
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
    MIB2_INIT_NETIF(netif,snmp_ifType_ethernet_csmacd,LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state=ETH_If;
    netif->name[0]='e';
    netif->name[1]='n';
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
