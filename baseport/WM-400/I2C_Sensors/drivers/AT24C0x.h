/*
 * AT24C0x.h
 *
 * Created: 16/02/2014 18:14
 *  Author: Nando
 *  Device: AT24C04/AT24C08
 */ 


#ifndef AT24C0X_H_
#define AT24C0X_H_

#include <compiler.h>
#include <twi_master.h>

status_code_t		at24c0x_read_byte(uint8_t device, uint8_t* data);
status_code_t		at24c0x_read_page(uint8_t device, uint16_t address, uint8_t* data);
status_code_t		at24c0x_write_byte(uint8_t device, uint16_t address, uint8_t data);
status_code_t		at24c0x_write_page(uint8_t device, uint16_t address, uint8_t* data);

#endif /* AT24C0X_H_ */