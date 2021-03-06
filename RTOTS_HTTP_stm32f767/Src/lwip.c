/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */

/* USER CODE BEGIN 0 *//*

#define IP_ADDR0  169
#define IP_ADDR1  254
#define IP_ADDR2  41   //137
#define IP_ADDR3  205*/
/*
#define IP_ADDR0  192
#define IP_ADDR1  168
#define IP_ADDR2  1
#define IP_ADDR3  83
#define GW_ADDR0  192
#define GW_ADDR1  168
#define GW_ADDR2  1
#define GW_ADDR3  1*/


#define IP_ADDR0  192
#define IP_ADDR1  168
#define IP_ADDR2  8
#define IP_ADDR3  22

#define GW_ADDR0  192
#define GW_ADDR1  168
#define GW_ADDR2  8
#define GW_ADDR3  1
/*
#define IP_ADDR0  192
#define IP_ADDR1  168
#define IP_ADDR2  1
#define IP_ADDR3  83

#define GW_ADDR0  192
#define GW_ADDR1  168
#define GW_ADDR2  1
#define GW_ADDR3  1

#define GW_ADDR0  0
#define GW_ADDR1  0
#define GW_ADDR2  0
#define GW_ADDR3  0
*/

#define MASK_ADDR0  255
#define MASK_ADDR1  255
#define MASK_ADDR2  255
#define MASK_ADDR3  0
/*
#define DNS_ADDR0  192
#define DNS_ADDR1  168
#define DNS_ADDR2  43
#define DNS_ADDR3  1

#define DNS_ADDR0  192
#define DNS_ADDR1  168
#define DNS_ADDR2  8
#define DNS_ADDR3  1

#define DNS_ADDR0  192
#define DNS_ADDR1  168
#define DNS_ADDR2  43
#define DNS_ADDR3  1

*/

#define DNS_ADDR0  163
#define DNS_ADDR1  121
#define DNS_ADDR2  128
#define DNS_ADDR3  134
/*
#define DNS_ADDR0  163
#define DNS_ADDR1  121
#define DNS_ADDR2  128
#define DNS_ADDR3  134*/

/*
#define DNS_ADDR0  62
#define DNS_ADDR1  240
#define DNS_ADDR2  110
#define DNS_ADDR3  198*/
/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN 1 */
#define MAX_DHCP_TRIES  4
__IO uint8_t DHCP_state = DHCP_OFF;
/* USER CODE END 1 */

/* Variables Initialization */
uint8_t iptxt[100];

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];
ip4_addr_t dnsserver;
/* USER CODE BEGIN 2 */
void User_notification(struct netif *netif)
{
  if (netif_is_up(netif))
 {

    /* Update DHCP state machine */
 //   DHCP_state = DHCP_START;
    sprintf((char *)iptxt, "User_notification Static IP address: %s\n", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
   HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
    /* Turn On LED 1 to indicate ETH and LwIP init success*/
 }

  else
  {
    /* Update DHCP state machine */
   // DHCP_state = DHCP_LINK_DOWN;
    sprintf((char *)iptxt,"The network cable is not connected \n");
     HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
    /* Turn On LED 2 to indicate ETH and LwIP init error */
  }
}
/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  IP_ADDRESS[0] = 0;
  IP_ADDRESS[1] = 0;
  IP_ADDRESS[2] = 0;
  IP_ADDRESS[3] = 0;
  NETMASK_ADDRESS[0] = 0;
  NETMASK_ADDRESS[1] = 0;
  NETMASK_ADDRESS[2] = 0;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 0;
  GATEWAY_ADDRESS[1] = 0;
  GATEWAY_ADDRESS[2] = 0;
  GATEWAY_ADDRESS[3] = 0;
  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  /* IP addresses initialization without DHCP (IPv4) */

  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  IP4_ADDR(&netmask, MASK_ADDR0, MASK_ADDR1, MASK_ADDR2, MASK_ADDR3);
  IP4_ADDR(&dnsserver,DNS_ADDR0,DNS_ADDR1,DNS_ADDR2,DNS_ADDR3);
  dns_setserver (0, &dnsserver);

  printf("MyIP %d.%d.%d.%d\n\r",(ipaddr.addr & 0xff), ((ipaddr.addr >> 8) & 0xff), ((ipaddr.addr >> 16) & 0xff), (ipaddr.addr >> 24));
  printf("dnsserverIP %d.%d.%d.%d\n\r",(dnsserver.addr & 0xff), ((dnsserver.addr >> 8) & 0xff), ((dnsserver.addr >> 16) & 0xff), (dnsserver.addr >> 24));
  /*
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
  ip_addr_set_zero_ip4(&dnsserver);
  dns_setserver (0, &dnsserver);*/
  /* add the network interface (IPv4/IPv6) with RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

/* USER CODE BEGIN 3 */
  User_notification(&gnetif);
//  osThreadDef(DHCP, DHCP_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
 // osThreadCreate (osThread(DHCP), &gnetif);
/* USER CODE END 3 */
}

void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  struct dhcp *dhcp;
  uint8_t iptxt[20];
  for (;;)
  {
    switch (DHCP_state)
    {
    case DHCP_START:
      {
        ip_addr_set_zero_ip4(&netif->ip_addr);
        ip_addr_set_zero_ip4(&netif->netmask);
        ip_addr_set_zero_ip4(&netif->gw);
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
        sprintf((char *)iptxt,"  State: Looking for DHCP server ...\n");
         HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
      }
      break;

    case DHCP_WAIT_ADDRESS:
      {
        if (dhcp_supplied_address(netif))
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;

          sprintf((char *)iptxt,"IP address assigned by a DHCP server: %s\n", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
                  HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
                  sprintf((char *)iptxt,(char *)iptxt, "IP: %s\n\r", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
                  HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
        //  sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
         // LCD_UsrLog ("IP address assigned by a DHCP server: %s\n", iptxt);

         // BSP_LED_On(LED1);
//#endif
        }
        else
        {
          dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

          /* DHCP timeout */
          if (dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
            /* Stop DHCP */
            dhcp_stop(netif);
            /* Static address used */
            IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP_ADDR4(&netmask, MASK_ADDR0, MASK_ADDR1, MASK_ADDR2, MASK_ADDR3);
            IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            IP4_ADDR(&dnsserver,DNS_ADDR0,DNS_ADDR1,DNS_ADDR2,DNS_ADDR3);
            dns_setserver (0, &dnsserver);
            netif_set_addr(netif, ip_2_ip4(&ipaddr), ip_2_ip4(&netmask), ip_2_ip4(&gw));
            sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
            HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
            sprintf ((char *)iptxt,"DHCP Timeout !! \n");
            HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
         sprintf((char *)iptxt,"Static IP address: %s\n", iptxt);
         HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);
         sprintf((char *)iptxt, "%s", ip4addr_ntoa(&dnsserver));
         sprintf((char *)iptxt,"DNS IP address: %s\n", iptxt);
         HAL_UART_Transmit(&huart3,iptxt,strlen(iptxt),1000);

          }
        }
      }
      break;
  case DHCP_LINK_DOWN:
    {
      /* Stop DHCP */
      dhcp_stop(netif);
      DHCP_state = DHCP_OFF;
    }
    break;
    default: break;
    }

    /* wait 250 ms */
    osDelay(250);
  }
}
#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  sio_fd_t sd;

/* USER CODE BEGIN 7 */
  sd = 0; // dummy code
/* USER CODE END 7 */
	
  return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
/* USER CODE BEGIN 8 */
/* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 9 */
  recved_bytes = 0; // dummy code
/* USER CODE END 9 */	
  return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  u32_t recved_bytes;

/* USER CODE BEGIN 10 */
  recved_bytes = 0; // dummy code
/* USER CODE END 10 */	
  return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
