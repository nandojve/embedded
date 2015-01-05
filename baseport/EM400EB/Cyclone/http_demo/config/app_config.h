/*
 * app_config.h
 *
 * Created: 13/10/2014 22:08:53
 *  Author: nando
 */ 


#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_

//Application configuration
#define APP_MAC_ADDR					"00-AB-CD-EF-04-16"

#define APP_USE_DHCP					ENABLED
#define APP_IPV4_HOST_ADDR				"192.168.10.50"
#define APP_IPV4_SUBNET_MASK			"255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY		"192.168.10.1"
#define APP_IPV4_PRIMARY_DNS			"8.8.8.8"
#define APP_IPV4_SECONDARY_DNS			"8.8.4.4"

#define APP_USE_SLAAC					ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR		"fe80::416"
#define APP_IPV6_PREFIX					"2001:db8::"
#define APP_IPV6_PREFIX_LENGTH			64
#define APP_IPV6_GLOBAL_ADDR			"2001:db8::416"
#define APP_IPV6_ROUTER					"fe80::1"
#define APP_IPV6_PRIMARY_DNS			"2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS			"2001:4860:4860::8844"

#endif /* APP_CONFIG_H_ */