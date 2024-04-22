/******************************************************************************
Filename    : ethernetif.h
Author      : hrs 
Date        : 04/09/2024
Licence     : The Unlicense; see LICENSE for details.
Description : Define netif priority and stacksize and declare ethernetif_init
              and ethernetif_input
******************************************************************************/

/* Define ********************************************************************/
#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* Ethernet DMA memory info */
#define ETH_DMAMEM_BASE                 (0x20070000U)

#define ETH_DMAMEM_TX_DESC_BASE         (ETH_DMAMEM_BASE)
#define ETH_DMAMEM_TX_DESC_SIZE         (sizeof(ETH_DMADescTypeDef)*ETH_TXBUFNB)
#define ETH_DMAMEM_TX_DESC              ((ETH_DMADescTypeDef*)ETH_DMAMEM_TX_DESC_BASE)

#define ETH_DMAMEM_RX_DESC_BASE         (ETH_DMAMEM_TX_DESC_BASE+ETH_DMAMEM_TX_DESC_SIZE)
#define ETH_DMAMEM_RX_DESC_SIZE         (sizeof(ETH_DMADescTypeDef)*ETH_RXBUFNB)
#define ETH_DMAMEM_RX_DESC              ((ETH_DMADescTypeDef*)ETH_DMAMEM_RX_DESC_BASE)

#define ETH_DMAMEM_TX_BUF_BASE          (ETH_DMAMEM_RX_DESC_BASE+ETH_DMAMEM_RX_DESC_SIZE)
#define ETH_DMAMEM_TX_BUF_SIZE          (sizeof(rmp_u8_t)*ETH_TX_BUF_SIZE*ETH_TXBUFNB)
#define ETH_DMAMEM_TX_BUF               ((rmp_u8_t*)ETH_DMAMEM_TX_BUF_BASE)

#define ETH_DMAMEM_RX_BUF_BASE          (ETH_DMAMEM_TX_BUF_BASE+ETH_DMAMEM_TX_BUF_SIZE)
#define ETH_DMAMEM_RX_BUF_SIZE          (sizeof(rmp_u8_t)*ETH_RX_BUF_SIZE*ETH_RXBUFNB)
#define ETH_DMAMEM_RX_BUF               ((rmp_u8_t*)ETH_DMAMEM_RX_BUF_BASE)

#define ETH_DMAMEM_SIZE                 (ETH_DMAMEM_RX_BUF_BASE+ETH_DMAMEM_RX_BUF_SIZE-ETH_DMAMEM_BASE)

/* Netif thread info */
#define NETIF_IN_TASK_PRIO              (2U)
#define NETIF_IN_TASK_STACK_SIZE        (4096U)


void ethernetif_input(void* netif);
err_t ethernetif_init(struct netif* netif);

#endif
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
