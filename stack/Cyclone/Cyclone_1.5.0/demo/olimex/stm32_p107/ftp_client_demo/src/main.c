/**
 * @file main.c
 * @brief Main routine
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

//Dependencies
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32_p107.h"
#include "os_port.h"
#include "core/tcp_ip_stack.h"
#include "drivers/stm32f107_eth.h"
#include "drivers/ksz8721.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "ftp/ftp_client.h"
#include "ext_int_driver.h"
#include "debug.h"

//Application configuration
#define APP_MAC_ADDR "00-AB-CD-EF-01-07"

#define APP_USE_DHCP ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::107"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::107"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

//Global variables
DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;


/**
 * @brief I/O initialization
 **/

void ioInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //Enable GPIO clocks
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

   //Configure LED1
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   //Configure LED2
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);

   //Configure WKUP button
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   //Configure TAMPER button
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
 * @brief FTP client test routine
 * @return Error code
 **/

error_t ftpClientTest(void)
{
   error_t error;
   size_t length;
   IpAddr ipAddr;
   FtpClientContext ftpContext;
   static char_t buffer[256];

   //Debug message
   TRACE_INFO("\r\n\r\nResolving server name...\r\n");
   //Resolve FTP server name
   error = getHostByName(NULL, "ftp.gnu.org", &ipAddr, 0);

   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_INFO("Failed to resolve server name!\r\n");
      //Exit immediately
      return error;
   }

   //Debug message
   TRACE_INFO("Connecting to FTP server %s\r\n", ipAddrToString(&ipAddr, NULL));
   //Connect to the FTP server
   error = ftpConnect(&ftpContext, NULL, &ipAddr, 21, FTP_NO_SECURITY | FTP_PASSIVE_MODE);

   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_INFO("Failed to connect to FTP server!\r\n");
      //Exit immediately
      return error;
   }

   //Debug message
   TRACE_INFO("Successful connection\r\n");

   //Start of exception handling block
   do
   {
      //Login to the FTP server using the provided username and password
      error = ftpLogin(&ftpContext, "anonymous", "password", "");
      //Any error to report?
      if(error) break;

      //Open the specified file for reading
      error = ftpOpenFile(&ftpContext, "welcome.msg", FTP_FOR_READING | FTP_BINARY_TYPE);
      //Any error to report?
      if(error) break;

      //Dump the contents of the file
      while(1)
      {
         //Read data
         error = ftpReadFile(&ftpContext, buffer, sizeof(buffer) - 1, &length, 0);
         //End of file?
         if(error) break;

         //Properly terminate the string with a NULL character
         buffer[length] = '\0';
         //Dump current data
         TRACE_INFO("%s", buffer);
      }

      //End the string with a line feed
      TRACE_INFO("\r\n");
      //Close the file
      error = ftpCloseFile(&ftpContext);

      //End of exception handling block
   } while(0);

   //Close the connection
   ftpClose(&ftpContext);
   //Debug message
   TRACE_INFO("Connection closed...\r\n");

   //Return status code
   return error;
}


/**
 * @brief User task
 **/

void userTask(void *param)
{
   //Endless loop
   while(1)
   {
      //WKUP button pressed?
      if(GPIO_ReadInputData(WKUP_GPIO) & WKUP_PIN)
      {
         //FTP client test routine
         ftpClientTest();

         //Wait for the WKUP button to be released
         while(GPIO_ReadInputData(WKUP_GPIO) & WKUP_PIN);
      }

      //Loop delay
      osDelayTask(100);
   }
}


/**
 * @brief LED blinking task
 **/

void blinkTask(void *parameters)
{
   //Endless loop
   while(1)
   {
      GPIO_SetBits(LED1_GPIO, LED1_PIN);
      osDelayTask(100);
      GPIO_ResetBits(LED1_GPIO, LED1_PIN);
      osDelayTask(900);
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   error_t error;
   NetInterface *interface;
   OsTask *task;
   MacAddr macAddr;
#if (APP_USE_DHCP == DISABLED)
   Ipv4Addr ipv4Addr;
#endif
#if (APP_USE_SLAAC == DISABLED)
   Ipv6Addr ipv6Addr;
#endif

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("*** CycloneTCP FTP Client Demo ***\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("Copyright: 2010-2014 Oryx Embedded\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: STM32F107\r\n");
   TRACE_INFO("\r\n");

   //Configure I/Os
   ioInit();

   //Clear LEDs
   GPIO_ResetBits(LED1_GPIO, LED1_PIN);
   GPIO_ResetBits(LED2_GPIO, LED2_PIN);

   //TCP/IP stack initialization
   error = tcpIpStackInit();
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
   }

   //Configure the first Ethernet interface
   interface = &netInterface[0];

   //Set interface name
   tcpIpStackSetInterfaceName(interface, "eth0");
   //Set host name
   tcpIpStackSetHostname(interface, "FTPClientDemo");
   //Select the relevant network adapter
   tcpIpStackSetDriver(interface, &stm32f107EthDriver);
   tcpIpStackSetPhyDriver(interface, &ksz8721PhyDriver);
   //Set external interrupt line driver
   tcpIpStackSetExtIntDriver(interface, &extIntDriver);
   //Set host MAC address
   macStringToAddr(APP_MAC_ADDR, &macAddr);
   tcpIpStackSetMacAddr(interface, &macAddr);

   //Initialize network interface
   error = tcpIpStackConfigInterface(interface);
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
   }

#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP == ENABLED)
   //Get default settings
   dhcpClientGetDefaultSettings(&dhcpClientSettings);
   //Set the network interface to be configured by DHCP
   dhcpClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpClientSettings.rapidCommit = FALSE;

   //DHCP client initialization
   error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
   //Failed to initialize DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCP client!\r\n");
   }

   //Start DHCP client
   error = dhcpClientStart(&dhcpClientContext);
   //Failed to start DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCP client!\r\n");
   }
#else
   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(interface, ipv4Addr);

   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(interface, ipv4Addr);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(interface, ipv4Addr);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
   //Get default settings
   slaacGetDefaultSettings(&slaacSettings);
   //Set the network interface to be configured
   slaacSettings.interface = interface;

   //SLAAC initialization
   error = slaacInit(&slaacContext, &slaacSettings);
   //Failed to initialize SLAAC?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize SLAAC!\r\n");
   }

   //Start IPv6 address autoconfiguration process
   error = slaacStart(&slaacContext);
   //Failed to start SLAAC process?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start SLAAC!\r\n");
   }
#else
   //Set link-local address
   ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   ipv6SetLinkLocalAddr(interface, &ipv6Addr, IPV6_ADDR_STATE_VALID);

   //Set IPv6 prefix
   ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
   ipv6SetPrefix(interface, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

   //Set global address
   ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
   ipv6SetGlobalAddr(interface, &ipv6Addr, IPV6_ADDR_STATE_VALID);

   //Set router
   ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
   ipv6SetRouter(interface, &ipv6Addr);

   //Set primary and secondary DNS servers
   ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 0, &ipv6Addr);
   ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 1, &ipv6Addr);
#endif
#endif

   //Create user task
   task = osCreateTask("User Task", userTask, NULL, 500, 1);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Create a task to blink the LED
   task = osCreateTask("Blink", blinkTask, NULL, 500, 1);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Start the execution of tasks
   osStartKernel();

   //This function should never return
   return 0;
}
