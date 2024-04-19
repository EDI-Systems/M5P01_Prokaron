/******************************************************************************
Filename    : rmp_lwip_iperf.c
Author      : hrs
Date        : 04/09/2024
Licence     : The Unlicense; see LICENSE for details.
Description : lwIP iperf benchmark.
******************************************************************************/

/* Include *******************************************************************/
#include "rmp.h"
#include "lwip/tcpip.h"
#include "lwip/apps/lwiperf.h"
#include "arch/ethernetif.h"
/* End Include ***************************************************************/

/* Define ********************************************************************/
#define LWIP_IP_ADDR_0          (192U)
#define LWIP_IP_ADDR_1          (168U)
#define LWIP_IP_ADDR_2          (1U)
#define LWIP_IP_ADDR_3          (30U)

#define LWIP_NETMASK_0          (255U)
#define LWIP_NETMASK_1          (255U)
#define LWIP_NETMASK_2          (255U)
#define LWIP_NETMASK_3          (0U)

#define LWIP_GATEWAY_0          (192U)
#define LWIP_GATEWAY_1          (168U)
#define LWIP_GATEWAY_2          (1U)
#define LWIP_GATEWAY_3          (1U)

#define LWIP_THD_STACK_SIZE     (4096U)
#define LWIP_INIT_TASK_PRIO     (11U)
/* End Define ****************************************************************/

/* Variable ******************************************************************/
struct netif LWIP_Netif;
volatile struct RMP_Thd LWIP_Thd;
rmp_u8_t LWIP_Thd_Stack[LWIP_THD_STACK_SIZE];
/* End Variable **************************************************************/

/* Function:LWIP_Iperf_Report *************************************************
Description : Will be called by lwiperf to report result when it finishes.
Input       : void* arg - Additonal args.
              enum LWIP_Iperf_Report_type report_type - Type of report.
              const ip_addr_t* local_addr - Local address.
              u16_t local_port - Local port.
              const ip_addr_t* remote_addr - Remote address.
              u16_t remote_port - Remote port.
              u32_t bytes_transferred - Bytes transferred.
              u32_t ms_duration - Duration(ms).
              u32_t bandwidth_kbitpsec - Average bandwidth(kb/s).
Output      : None.
Return      : None.
******************************************************************************/
static void LWIP_Iperf_Report(void *arg,
                              enum lwiperf_report_type report_type,
                              const ip_addr_t* local_addr,
                              u16_t local_port,
                              const ip_addr_t* remote_addr,
                              u16_t remote_port,
                              u32_t bytes_transferred,
                              u32_t ms_duration,
                              u32_t bandwidth_kbitpsec)
{
    RMP_DBG_S("----------------------------------------------------\r\n");
  
    if((int)report_type<10)
    {
        if(report_type==0)
            RMP_DBG_S("TCP_DONE_SERVER");
        else if(report_type==5)
            RMP_DBG_S("TCP_ABORTED_REMOTE");
        else RMP_DBG_I(report_type);
        RMP_DBG_S("\r\nIPv4 Remote address                : ");
        RMP_DBG_I(((u8_t *)remote_addr)[0]);
        RMP_DBG_S(".");
        RMP_DBG_I(((u8_t *)remote_addr)[1]);
        RMP_DBG_S(".");
        RMP_DBG_I(((u8_t *)remote_addr)[2]);
        RMP_DBG_S(".");
        RMP_DBG_I(((u8_t *)remote_addr)[3]);
        RMP_DBG_S("\r\nIPv4 Remote port                   : ");
        RMP_DBG_I(remote_port);
        RMP_DBG_S("\r\nBytes transferred                  : ");
        RMP_DBG_I((int)bytes_transferred);
        RMP_DBG_S("\r\nDuration (ms)                      : ");
        RMP_DBG_I((int)ms_duration);
        RMP_DBG_S("\r\nBandwidth (kbps)                   : ");
        RMP_DBG_I((int)bandwidth_kbitpsec);
        RMP_DBG_S(" \r\n");
    }
    else
    {
        RMP_DBG_S(" IPERF report error\r\n");
    }
}
/* End Function:LWIP_Iperf_Report ********************************************/

/* Function:LWIP_Init_Task ****************************************************
Description : LwIP demo task.
Input       : void* Params - not used.
Output      : None.
Return      : None.
******************************************************************************/
void LWIP_Init_Task(void* Params)
{
    ip_addr_t IP_Addr; 
    ip_addr_t Netmask;
    ip_addr_t Gateway;
    
    /* Initialize LWIP stack */
    tcpip_init(NULL,NULL);
    
    /* Set address */
    IP4_ADDR(&IP_Addr,LWIP_IP_ADDR_0,LWIP_IP_ADDR_1,LWIP_IP_ADDR_2,LWIP_IP_ADDR_3);
    IP4_ADDR(&Netmask,LWIP_NETMASK_0,LWIP_NETMASK_1,LWIP_NETMASK_2,LWIP_NETMASK_3);
    IP4_ADDR(&Gateway,LWIP_GATEWAY_0,LWIP_GATEWAY_1,LWIP_GATEWAY_2,LWIP_GATEWAY_3);
    
    RMP_ASSERT(netif_add(&LWIP_Netif,&IP_Addr,&Netmask,&Gateway,NULL,&ethernetif_init,&tcpip_input)!=NULL);
    
    /* Bring interface up */
    netif_set_default(&LWIP_Netif);
    if (netif_is_link_up(&LWIP_Netif))
        netif_set_up(&LWIP_Netif);
    else
        netif_set_down(&LWIP_Netif);
    
    /* Start iperf server with given report function */
    RMP_ASSERT(lwiperf_start_tcp_server_default(LWIP_Iperf_Report,NULL)!=0);
    RMP_DBG_S("Server status                      : Ready\r\n");
   
    /* Dead loop */
    while(1)
    {
        RMP_Thd_Delay(5);
    }
}
/* End Function:LWIP_Init_Task ***********************************************/

/* Function:RMP_Init **********************************************************
Description : The init thread hook functions.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void RMP_Init_Hook(void)
{
    /* Print table header */
    RMP_DBG_S("\r\n    ___   __  ___ ___\r\n");
    RMP_DBG_S("   / _ \\ /  |/  // _ \\       Simple real-time kernel\r\n");
    RMP_DBG_S("  / , _// /|_/ // ___/       lwIP iperf benchmark test\r\n");
    RMP_DBG_S(" /_/|_|/_/  /_//_/\r\n");
    RMP_DBG_S("====================================================\r\n");
    RMP_DBG_S("IPv4 Local address                 : ");
    RMP_DBG_I(LWIP_IP_ADDR_0);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_IP_ADDR_1);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_IP_ADDR_2);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_IP_ADDR_3);
    RMP_DBG_S("\r\nIPv4 Subnet mask                   : ");
    RMP_DBG_I(LWIP_NETMASK_0);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_NETMASK_1);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_NETMASK_2);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_NETMASK_3);
    RMP_DBG_S("\r\nIPv4 Gateway                       : ");
    RMP_DBG_I(LWIP_GATEWAY_0);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_GATEWAY_1);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_GATEWAY_2);
    RMP_DBG_S(".");
    RMP_DBG_I(LWIP_GATEWAY_3);
    RMP_DBG_S("\r\n");
    
    /* Initialize main thread */
    RMP_Thd_Crt(&LWIP_Thd,
                LWIP_Init_Task,
                RMP_NULL,
                LWIP_Thd_Stack,
                sizeof(LWIP_Thd_Stack),
                LWIP_INIT_TASK_PRIO,
                1);
}

void RMP_Init_Idle(void)
{
    return;
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/
