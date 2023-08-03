/**
  ******************************************************************************
  * @file           : main.cpp
  * @author 		: MCD Application Team
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  */

/* USER CODE END Header */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "string.h"   /* Console output */
#include <ctype.h>
#include <inttypes.h>
#include <string.h>

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/opt.h"
#include "lwipopts.h"
#include "ethernetif.h"
#include "app_ethernet.h"

/* USER CODE END Includes */
#ifdef __cplusplus
}
#endif

/* ------------------------ Includes C++ MATTER ------------------------ */

#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "AppTask.h"

#include "STM32FreeRtosHooks.h"
#include "CHIPDevicePlatformConfig.h"
#include "AppConfig.h"
// #include "Device.h"
#include "DeviceCallbacks.h"
#include <app-common/zap-generated/af-structs.h>
#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/ConcreteAttributePath.h>
#include <app/clusters/identify-server/identify-server.h>
#include <app/reporting/reporting.h>
#include <app/util/attribute-storage.h>
#include <common/Stm32AppServer.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <lib/core/CHIPError.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/ErrorStr.h>
#include <lib/support/ZclString.h>

#include <app/InteractionModelEngine.h>
#include <app/server/Server.h>
#include <platform/STM32/NetworkCommissioningDriver.h>

#include <app-common/zap-generated/callback.h>

#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>
#include <app/server/OnboardingCodesUtil.h>

/* ------------------------ Includes C++ MATTER ------------------------ */

#ifdef __cplusplus
extern "C" {
#endif
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* The SPI driver polls at a high priority. The logging task's priority must also
 * be high to be not be starved of CPU time. */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int errno;                                  /* Required by LwIP */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* MATTER BRIDGE APPLICATION -------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/* USER CODE BEGIN PFP */

#if CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER
#include <platform/STM32/STM32FactoryDataProvider.h>
//#else
//#include <platform/TestOnlyCommissionableDataProvider.h>
#endif // CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_STM32_DEVICE_INFO_PROVIDER
#include <platform/STM32/STM32DeviceInfoProvider.h>
#else
#include <DeviceInfoProviderImpl.h>
#endif // CONFIG_ENABLE_STM32_DEVICE_INFO_PROVIDER

namespace {
#if CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER
chip::DeviceLayer::STM32FactoryDataProvider sFactoryDataProvider;
//#else
//chip::DeviceLayer::TestOnlyCommissionableDataProvider gTestOnlyCommissionableDataProvider;
#endif // CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_STM32_DEVICE_INFO_PROVIDER
chip::DeviceLayer::STM32DeviceInfoProvider gExampleDeviceInfoProvider;
#else
chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
#endif // CONFIG_ENABLE_STM32_DEVICE_INFO_PROVIDER
} // namespace

struct netif gnetif; /* network interface structure */

using namespace ::chip;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceManager;
using namespace ::chip::DeviceLayer;

static const char * TAG = "light-app";

static AppDeviceCallbacks EchoCallbacks;

#ifdef __cplusplus
extern "C" {
#endif

static void InitServer(intptr_t context)
{
	tcpip_init(NULL, NULL);

	/* Initialize the LwIP stack */
	Netif_Config();

	// Print QR Code URL
	PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kOnNetwork));

	Stm32AppServer::Init(); // Init ZCL Data Model and CHIP App Server AND Initialize device attestation config

	chip::DeviceLayer::ConfigurationMgr().LogDeviceConfig();
}

/**
 * @brief  The application entry point.
 * @retval int
 */
int app_main(void)
{
	/* USER CODE BEGIN 1 */
	STM32_LOG("==================================================");
    STM32_LOG("Matter STM32 light example starting");
    STM32_LOG("==================================================");
	/* USER CODE END 1 */

	/* USER CODE BEGIN 2 */
	/* USER CODE END 2 */

	/* Create the thread(s) */
	/* USER CODE BEGIN RTOS_THREADS */

	freertos_mbedtls_init();

	/* USER CODE END RTOS_THREADS */

	DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    CHIPDeviceManager & deviceMgr = CHIPDeviceManager::GetInstance();
    CHIP_ERROR error              = deviceMgr.Init(&EchoCallbacks);
    if (error != CHIP_NO_ERROR)
    {
        STM32_LOG("device.Init() failed: %s", ErrorStr(error));
    }

#if CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER
	SetCommissionableDataProvider(&sFactoryDataProvider);
	SetDeviceAttestationCredentialsProvider(&sFactoryDataProvider);
#if CONFIG_ENABLE_STM32_DEVICE_INSTANCE_INFO_PROVIDER
	SetDeviceInstanceInfoProvider(&sFactoryDataProvider);
#endif
#else
	//chip::DeviceLayer::SetCommissionableDataProvider(&gTestOnlyCommissionableDataProvider);
	SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif // CONFIG_ENABLE_STM32_FACTORY_DATA_PROVIDER

	chip::DeviceLayer::PlatformMgr().ScheduleWork(InitServer, reinterpret_cast<intptr_t>(nullptr));

	error = GetAppTask().StartAppTask();
    if (error != CHIP_NO_ERROR)
    {
        STM32_LOG("GetAppTask().StartAppTask() failed : %s", ErrorStr(error));
    }

    vTaskStartScheduler();
}

/**
 * @brief  Initializes the lwIP stack
 * @param  None
 * @retval None
 */
void Netif_Config(void)
{
	//IPv6 only
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);

	/* add the network interface */
	netif_add(&gnetif,  (ip_2_ip4(&ipaddr)), (ip_2_ip4(&netmask)), (ip_2_ip4(&gw)), NULL, &ethernetif_init, &tcpip_input);

	gnetif.name[0] = 'A';
	gnetif.name[1] = 'R';

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

#if ( ( configUSE_TRACE_FACILITY == 1 ) && (INCLUDE_uxTaskGetStackHighWaterMark == 1) )
void print_stacks_highwatermarks( void )
{
	UBaseType_t task_nr =	uxTaskGetNumberOfTasks();
	TaskStatus_t *p_task_status = (TaskStatus_t *) pvPortMalloc(task_nr * sizeof(TaskStatus_t));
	if (p_task_status != NULL)
	{
		if (task_nr == uxTaskGetSystemState(p_task_status, task_nr, NULL))
		{
			//	  for (int i = 0; i < task_nr; i++)
			//	  {
			//		vLoggingPrintf("StackHighWaterMark: %s=%lu\n", p_task_status[i].pcTaskName, uxTaskGetStackHighWaterMark(p_task_status[i].xHandle));
			//	  }
		}
		vPortFree(p_task_status);
	}
}
#endif

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
