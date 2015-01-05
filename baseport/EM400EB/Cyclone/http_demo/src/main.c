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
#include <stdlib.h>
#include "board.h"
#include "led.h"
#include "app_config.h"
#include "os_port.h"
#include "core/tcp_ip_stack.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "ftp/ftp_client.h"
#include "std_services/echo.h"
#include "std_services/discard.h"
#include "std_services/chargen.h"
#include "smtp/smtp_client.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "yarrow.h"
#include "str.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "drivers/sam4e_eth.h"
#include "drivers/ksz8081.h"
#include "inc/ext_int_driver.h"
#include "error.h"
#include "debug.h"

static const char _unknown[] = "unknown";
static const char *const chip_id_eproc_names[] = {
	_unknown,                               /* 0 */
	"arm946es",                             /* 1 */
	"arm7tdmi",                             /* 2 */
	"cortex-m3",                            /* 3 */
	"arm920t",                              /* 4 */
	"arm926ejs",                            /* 5 */
	"cortex-a5",                            /* 6 */
	"cortex-m4"                             /* 7 */
};

static const char *const chip_id_nvpsize[] = {
	"none",                                 /*  0 */
	"8K bytes",                             /*  1 */
	"16K bytes",                            /*  2 */
	"32K bytes",                            /*  3 */
	_unknown,                               /*  4 */
	"64K bytes",                            /*  5 */
	_unknown,                               /*  6 */
	"128K bytes",                           /*  7 */
	_unknown,                               /*  8 */
	"256K bytes",                           /*  9 */
	"512K bytes",                           /* 10 */
	_unknown,                               /* 11 */
	"1024K bytes",                          /* 12 */
	_unknown,                               /* 13 */
	"2048K bytes",                          /* 14 */
	_unknown,                               /* 15 */
};

static const char *const chip_id_sramsize[] = {
	"48K bytes",                            /*  0 */
	"1K bytes",                             /*  1 */
	"2K bytes",                             /*  2 */
	"6K bytes",                             /*  3 */
	"112K bytes",                           /*  4 */
	"4K bytes",                             /*  5 */
	"80K bytes",                            /*  6 */
	"160K bytes",                           /*  7 */
	"8K bytes",                             /*  8 */
	"16K bytes",                            /*  9 */
	"32K bytes",                            /* 10 */
	"64K bytes",                            /* 11 */
	"128K bytes",                           /* 12 */
	"256K bytes",                           /* 13 */
	"96K bytes",                            /* 14 */
	"512K bytes",                           /* 15 */
};

static const struct archnames { unsigned value; const char *name; }
chip_id_archnames[] = {
	{ 0x19, "AT91SAM9xx Series" },
	{ 0x29, "AT91SAM9XExx Series" },
	{ 0x34, "AT91x34 Series" },
	{ 0x37, "CAP7 Series" },
	{ 0x39, "CAP9 Series" },
	{ 0x3B, "CAP11 Series" },
	{ 0x3C, "SAM4E Series" },
	{ 0x40, "AT91x40 Series" },
	{ 0x42, "AT91x42 Series" },
	{ 0x55, "AT91x55 Series" },
	{ 0x60, "AT91SAM7Axx Series" },
	{ 0x61, "AT91SAM7AQxx Series" },
	{ 0x63, "AT91x63 Series" },
	{ 0x70, "AT91SAM7Sxx Series" },
	{ 0x71, "AT91SAM7XCxx Series" },
	{ 0x72, "AT91SAM7SExx Series" },
	{ 0x73, "AT91SAM7Lxx Series" },
	{ 0x75, "AT91SAM7Xxx Series" },
	{ 0x76, "AT91SAM7SLxx Series" },
	{ 0x80, "ATSAM3UxC Series (100-pin version)" },
	{ 0x81, "ATSAM3UxE Series (144-pin version)" },
	{ 0x83, "ATSAM3AxC Series (100-pin version)" },
	{ 0x84, "ATSAM3XxC Series (100-pin version)" },
	{ 0x85, "ATSAM3XxE Series (144-pin version)" },
	{ 0x86, "ATSAM3X8H Series (217-pin version)" },
	{ 0x88, "ATSAM3SxA Series (48-pin version)" },
	{ 0x89, "ATSAM3SxB Series (64-pin version)" },
	{ 0x8A, "SAM4S Series" },
	{ 0x92, "AT91x92 Series" },
	{ 0x95, "ATSAM3NxC Series (100-pin version)" },
	{ 0xF0, "AT75Cxx Series" },
	{ 0, NULL },
};

//Forward declaration of functions
HttpAccessStatus httpServerAuthCallback(HttpConnection *connection, const char_t *user, const char_t *uri);
error_t httpServerCgiCallback(HttpConnection *connection, const char_t *param);
error_t httpServerUriNotFoundCallback(HttpConnection *connection, const char_t *uri);

DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;
HttpServerSettings httpServerSettings;
HttpServerContext httpServerContext;
YarrowContext yarrowContext;
uint8_t seed[32];

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
		//FTP client test routine
		ftpClientTest();

		//Loop delay
		osDelayTask(10000);
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
		LED_Off(LED1);
		osDelayTask(100);

		LED_On(LED1);
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

   //Update system core clock
	sysclk_init();
	board_init();

	// Initialize interrupt vector table support.
	irq_initialize_vectors();

	// Enable interrupts
	cpu_irq_enable();

	/* Call a local utility routine to initialize C-Library Standard I/O over
	 * a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
	 * supplied to configure the USB device correctly.
	 */
	stdio_usb_init();

   //Initialize kernel
   osInitKernel();
   //Configure debug UART

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("*** CycloneTCP Web Server Demo ***\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("Copyright: 2010-2014 Oryx Embedded\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: SAM4E\r\n");
   TRACE_INFO("\r\n");

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
   tcpIpStackSetHostname(interface, "WebServerDemo");
   //Select the relevant network adapter
   tcpIpStackSetDriver(interface, &sam4eEthDriver);
   tcpIpStackSetPhyDriver(interface, &ksz8081PhyDriver);
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

   //Get default settings
   httpServerGetDefaultSettings(&httpServerSettings);
   //Bind HTTP server to the desired interface
   httpServerSettings.interface = &netInterface[0];
   //Listen to port 80
   httpServerSettings.port = HTTP_PORT;
   //Specify the server's root directory
   strcpy(httpServerSettings.rootDirectory, "/www/");
   //Set default home page
   strcpy(httpServerSettings.defaultDocument, "index.htm");
   //Callback functions
   httpServerSettings.authCallback = httpServerAuthCallback;
   httpServerSettings.cgiCallback = httpServerCgiCallback;
   httpServerSettings.uriNotFoundCallback = httpServerUriNotFoundCallback;

   //HTTP server initialization
   error = httpServerInit(&httpServerContext, &httpServerSettings);
   //Failed to initialize HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize HTTP server!\r\n");
   }

   //Start HTTP server
   error = httpServerStart(&httpServerContext);
   //Failed to start HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start HTTP server!\r\n");
   }

   //Start TCP echo service
   error = tcpEchoStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP echo service!\r\n");
   }

   //Start UDP echo service
   error = udpEchoStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP echo service!\r\n");
   }

   //Start TCP discard service
   error = tcpDiscardStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP discard service!\r\n");
   }

   //Start UDP discard service
   error = udpDiscardStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP discard service!\r\n");
   }

   //Start TCP chargen service
   error = tcpChargenStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP chargen service!\r\n");
   }

   //Start UDP chargen service
   error = udpChargenStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP chargen service!\r\n");
   }

   ////Create user task
   //task = osCreateTask("User Task", userTask, NULL, 500, 1);
   ////Failed to create the task?
   //if(task == OS_INVALID_HANDLE)
   //{
      ////Debug message
      //TRACE_ERROR("Failed to create task!\r\n");
   //}

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


/**
 * @brief HTTP authentication callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] user NULL-terminated string specifying the user name
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Access status (HTTP_ACCESS_ALLOWED, HTTP_ACCESS_DENIED,
 *   HTTP_BASIC_AUTH_REQUIRED or HTTP_DIGEST_AUTH_REQUIRED)
 **/
HttpAccessStatus httpServerAuthCallback(HttpConnection *connection, const char_t *user, const char_t *uri)
{
   HttpAccessStatus status;

   //Manage access rights
   if(pathMatch(uri, "/passwords.txt"))
   {
      //This file is not visible
      status = HTTP_ACCESS_DENIED;
   }
   else if(pathMatch(uri, "/config/*"))
   {
      //This directory is not visible
      status = HTTP_ACCESS_DENIED;
   }
   else if(pathMatch(uri, "/admin/*"))
   {
      //Only the administrator can access this directory
      if(!strcmp(user, "administrator"))
      {
         //Check the administrator password
         if(httpCheckPassword(connection, "password", HTTP_AUTH_MODE_BASIC))
            status = HTTP_ACCESS_ALLOWED;
         else
            status = HTTP_ACCESS_BASIC_AUTH_REQUIRED;
      }
      else
      {
         //Users other than administrator cannot access this directory
         status = HTTP_ACCESS_BASIC_AUTH_REQUIRED;
      }
   }
   else
   {
      //No restriction for other directories
      status = HTTP_ACCESS_ALLOWED;
   }

   //Return access status
   return status;
}


#define CHIPID_NVTYP   (((CHIPID->CHIPID_CIDR) & CHIPID_CIDR_NVPTYP_Msk) >>  CHIPID_CIDR_NVPTYP_Pos)
#define CHIPID_NVPSIZ  (((CHIPID->CHIPID_CIDR) & CHIPID_CIDR_NVPSIZ_Msk) >>  CHIPID_CIDR_NVPSIZ_Pos)
#define CHIPID_SRAMSIZ (((CHIPID->CHIPID_CIDR) & CHIPID_CIDR_SRAMSIZ_Msk) >>  CHIPID_CIDR_SRAMSIZ_Pos)
#define CHIPID_ARCH    (((CHIPID->CHIPID_CIDR) & CHIPID_CIDR_ARCH_Msk) >>  CHIPID_CIDR_ARCH_Pos)
#define CHIPID_EPRCOC  (((CHIPID->CHIPID_CIDR) & CHIPID_CIDR_EPROC_Msk) >>  CHIPID_CIDR_EPROC_Pos)

/**
 * \brief Return the chip ID name.
 *
 * \param idx Index in the chip_id_eproc_names table.
 *
 * \return The chip id name.
 */
static const char *chipid_eproc_name(int idx)
{
	if (idx <= 7) {
		return chip_id_eproc_names[idx];
	}

	return _unknown;
}

/**
 * \brief Return the non volatile programmable memory size.
 *
 * \param idx Index in the chip_id_nvpsize table.
 *
 * \return The chip non volatile programmable memory size.
 */
static const char *chipid_nvpsize(int idx)
{
	if (idx <= 15) {
		return chip_id_nvpsize[idx];
	}

	return _unknown;
}

/**
 * \brief Return the chip SRAM size.
 *
 * \param idx Index in the chip_id_sramsize table.
 *
 * \return The chip SRAM size.
 */
static const char *chipid_sramsize(int idx)
{
	if (idx <= 15) {
		return chip_id_sramsize[idx];
	}

	return _unknown;
}

/**
 * \brief Return the chip architecture name.
 *
 * \param idx Index in the chip_id_archnames table.
 *
 * \return The chip architecture name.
 */
static const char *chipid_archnames(unsigned value)
{
	for (int i = 0; chip_id_archnames[i].name; i++) {
		if (chip_id_archnames[i].value == value) {
			return chip_id_archnames[i].name;
		}
	}

	return _unknown;
}
/**
 * @brief CGI callback function
 * @param[in] connection Handle referencing a client connection
 * @param[in] param NULL-terminated string that contains the CGI parameter
 * @return Error code
 **/
error_t httpServerCgiCallback(HttpConnection *connection, const char_t *param)
{
   static uint_t pageCounter = 0;
   uint_t length;

   //Underlying network interface
   NetInterface *interface = connection->socket->interface;

   //Check parameter name
   if(!strcasecmp(param, "status"))
   {

   }
   else if(!strcasecmp(param, "/get_chipinfo"))
   {
	   	sprintf(connection->buffer, "{\"core_name\":\"%s\",\"arch_name\":\"%s\",\"sram_size\":\"%s\",\"flash_size\":\"%s\"}",
	   		chipid_eproc_name(CHIPID_EPRCOC),
	   		chipid_archnames(CHIPID_ARCH),
	   		chipid_sramsize(CHIPID_SRAMSIZ),
	   		chipid_nvpsize(CHIPID_NVPSIZ));
   }
   else
   {
      return ERROR_INVALID_TAG;
   }

   //Get the length of the resulting string
   length = strlen(connection->buffer);

   //Send the contents of the specified environment variable
   return httpWriteStream(connection, connection->buffer, length);
}


/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/
error_t httpServerUriNotFoundCallback(HttpConnection *connection, const char_t *uri)
{
	return ERROR_NOT_FOUND;
}
