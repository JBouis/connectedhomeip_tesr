/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "ethernetif.h"
#include <string.h>

/* USER CODE BEGIN 0 */
#include "AppConfig.h"
/* USER CODE END 0 */
/* Private function prototypes -----------------------------------------------*/
static void ethernet_link_status_updated(struct netif *netif);
/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
// ip4_addr_t ipaddr;
// ip4_addr_t netmask;
// ip4_addr_t gw;

ip_addr_t ipaddr;
ip_addr_t netmask;
ip_addr_t gw;

uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];
/* USER CODE BEGIN OS_THREAD_ATTR_CMSIS_RTOS_V2 */
#define INTERFACE_THREAD_STACK_SIZE ( 1024 )
//osThreadAttr_t attributes;
/* USER CODE END OS_THREAD_ATTR_CMSIS_RTOS_V2 */

/* USER CODE BEGIN 2 */
/* ETH_CODE: workaround to call LOCK_TCPIP_CORE after tcpip_init in MX_LWIP_Init
 * This is to keep the code after MX code re-generation */
static inline void tcpip_init_wrap(tcpip_init_done_fn tcpip_init_done, void *arg){
	tcpip_init(tcpip_init_done, arg);
	LOCK_TCPIP_CORE();
}
#define tcpip_init tcpip_init_wrap
/* USER CODE END 2 */

/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  // IP_ADDRESS[0] = 192;
  // IP_ADDRESS[1] = 168;
  // IP_ADDRESS[2] = 1;
  // IP_ADDRESS[3] = 10;
  // NETMASK_ADDRESS[0] = 255;
  // NETMASK_ADDRESS[1] = 255;
  // NETMASK_ADDRESS[2] = 255;
  // NETMASK_ADDRESS[3] = 0;
  // GATEWAY_ADDRESS[0] = 0;
  // GATEWAY_ADDRESS[1] = 0;
  // GATEWAY_ADDRESS[2] = 0;
  // GATEWAY_ADDRESS[3] = 0;

/* USER CODE BEGIN IP_ADDRESSES */

/* USER CODE END IP_ADDRESSES */

  /* Initilialize the LwIP stack with RTOS */
  tcpip_init( NULL, NULL );

  //IPv6 only
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);

  /* add the network interface */
  netif_add(&gnetif,  (ip_2_ip4(&ipaddr)), (ip_2_ip4(&netmask)), (ip_2_ip4(&gw)), NULL,     &ethernetif_init, &tcpip_input);

  gnetif.name[0] = 'S';
  gnetif.name[1] = 'T';

  /* Create IPv6 local address */
  gnetif.ip6_autoconfig_enabled = 1;
  netif_create_ip6_linklocal_address(&gnetif, 1);
  //netif_ip6_addr_set_state(&gnetif, 0, IP6_ADDR_VALID);
  STM32_LOG("ip6 linklocal address: %s\n", ip6addr_ntoa(netif_ip6_addr(&gnetif, 0)));

  gnetif.flags |= NETIF_FLAG_MLD6;

  /*  Registers the default network interface. */
  netif_set_default(&gnetif);

  ethernet_link_status_updated(&gnetif);

#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);

  xTaskCreate( ethernet_link_thread,
	"EthLink",
	configMINIMAL_STACK_SIZE *2,
	&gnetif,
	3, /* Normal task priority */
	NULL );
#endif
}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

/**
  * @brief  Notify the User about the network interface config status
  * @param  netif: the network interface
  * @retval None
  */
static void ethernet_link_status_updated(struct netif *netif)
{
  if (netif_is_up(netif))
  {
/* USER CODE BEGIN 5 */
	  HAL_GPIO_WritePin(USER_LED1_GPIO_Port, USER_LED1_Pin, GPIO_PIN_SET);
/* USER CODE END 5 */
  }
  else /* netif is down */
  {
/* USER CODE BEGIN 6 */
	  HAL_GPIO_WritePin(USER_LED1_GPIO_Port, USER_LED1_Pin, GPIO_PIN_RESET);
/* USER CODE END 6 */
  }
}

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
