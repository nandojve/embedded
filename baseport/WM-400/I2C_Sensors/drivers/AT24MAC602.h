/*
 * AT24MAC602.h
 *
 * Created: 01/03/2014 09:58
 *  Author: Nando
 *  Device: AT24MAC602
 */ 


#ifndef _AT24MAC602_H_
#define _AT24MAC602_H_

#include "AT24C0x.h"

#define			AT24MAC602_MEM_DEVICE						0x57
#define			AT24MAC602_EUI_DEVICE						0x5F

#define			at24mac602_read_byte(data)					at24c0x_read_byte(AT24MAC602_MEM_DEVICE, data)
#define			at24mac602_read_page(address, data)			at24c0x_read_page(AT24MAC602_MEM_DEVICE, address, data)
#define			at24mac602_write_byte(address, data)		at24c0x_write_byte(AT24MAC602_MEM_DEVICE, address, data)
#define			at24mac602_write_page(address, data)		at24c0x_write_page(AT24MAC602_MEM_DEVICE, address, data)
status_code_t	at24mac602_read_sn(uint8_t* data);
status_code_t	at24mac602_read_eui(uint8_t* data);


#endif /* _AT24MAC602_H_ */