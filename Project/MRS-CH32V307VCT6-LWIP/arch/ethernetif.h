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

#include "ch32v30x_eth.h"

#define PHY_ADDRESS                    (0x01)

/* Definition of the Ethernet driver buffers size and count */
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */
#define ETH_RXBUFNB                    ((uint32_t)4)       /* 4 Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB                    ((uint32_t)4)       /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */

/* Netif thread info */
#define NETIF_IN_TASK_PRIO              (2U)
#define NETIF_IN_TASK_STACK_SIZE        (4096U)

extern struct netif LWIP_Netif;
extern ETH_DMADESCTypeDef *DMATxDescToSet;
extern ETH_DMADESCTypeDef *DMARxDescToGet;

void ethernetif_input(void* netif);
err_t ethernetif_init(struct netif* netif);

#endif
/* End Define ****************************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
