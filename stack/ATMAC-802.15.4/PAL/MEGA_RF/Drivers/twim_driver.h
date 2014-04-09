/*
 * twim_driver.h
 *
 * Created: 14/11/2011 01:23:46
 *  Author: Nando
 */ 


#ifndef TWIM_DRIVER_H_
#define TWIM_DRIVER_H_

#include "avr_compiler.h"

typedef volatile uint8_t register8_t;

// Buffer size defines
#define	TWIM_WRITE_BUFFER_SIZE			8
#define	TWIM_READ_BUFFER_SIZE			8

typedef enum TWIM_STATE_enum
{
	TWIM_UNKNOWN						= 0,
	TWIM_START,
	TWIM_BUSY,
	TWIM_STANDBY,
	
// General TWI Master status codes                      
	TWI_START							= 0x08,  // START has been transmitted  
	TWI_REP_START						= 0x10,  // Repeated START has been transmitted
	TWI_ARB_LOST						= 0x38,  // Arbitration lost

// TWI Master Transmitter status codes                      
	TWI_MTX_ADR_ACK						= 0x18,  // SLA+W has been tramsmitted and ACK received
	TWI_MTX_ADR_NACK					= 0x20,  // SLA+W has been tramsmitted and NACK received 
	TWI_MTX_DATA_ACK					= 0x28,  // Data byte has been tramsmitted and ACK received
	TWI_MTX_DATA_NACK					= 0x30,  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver status codes  
	TWI_MRX_ADR_ACK						= 0x40,  // SLA+R has been tramsmitted and ACK received
	TWI_MRX_ADR_NACK					= 0x48,  // SLA+R has been tramsmitted and NACK received
	TWI_MRX_DATA_ACK					= 0x50,  // Data byte has been received and ACK tramsmitted
	TWI_MRX_DATA_NACK					= 0x58,  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter status codes
	TWI_STX_ADR_ACK						= 0xA8,  // Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST			= 0xB0,  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK					= 0xB8,  // Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK					= 0xC0,  // Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE			= 0xC8,  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

// TWI Slave Receiver status codes
	TWI_SRX_ADR_ACK						= 0x60,  // Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST			= 0x68,  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK						= 0x70,  // General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST			= 0x78,  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK				= 0x80,  // Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK				= 0x88,  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK				= 0x90,  // Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK				= 0x98,  // Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART				= 0xA0,  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
	TWI_NO_STATE						= 0xF8,  // No relevant state information available; TWINT = “0”
	TWI_BUS_ERROR						= 0x00,  // Bus error due to an illegal START or STOP condition
} TWIM_State_t;

typedef struct TWI_Master
{
	register8_t	state;
	register8_t address;							/*!< Slave address */
	register8_t writeData[TWIM_WRITE_BUFFER_SIZE];	/*!< Data to write */
	register8_t readData[TWIM_READ_BUFFER_SIZE];	/*!< Read data */
	register8_t bytesToWrite;						/*!< Number of bytes to write */
	register8_t bytesToRead;						/*!< Number of bytes to read */
	register8_t bytesWritten;						/*!< Number of bytes written */
	register8_t bytesRead;							/*!< Number of bytes read */
	register8_t result;								/*!< Result of transaction */
} TWI_Master_t;

// Transaction result enumeration.
typedef enum TWIM_RESULT_enum
{
	TWIM_RESULT_UNKNOWN					= (0x00 << 0),
	TWIM_RESULT_OK						= (0x01 << 0),
	TWIM_RESULT_BUFFER_OVERFLOW			= (0x02 << 0),
	TWIM_RESULT_ARBITRATION_LOST		= (0x03 << 0),
	TWIM_RESULT_BUS_ERROR				= (0x04 << 0),
	TWIM_RESULT_NACK_RECEIVED			= (0x05 << 0),
	TWIM_RESULT_FAIL					= (0x06 << 0),
} TWIM_RESULT_t;

void									twim_init(uint8_t bitrate);
void									twim_wait(void);
void									twim_task(void);
bool									twim_ready(void);
bool									twim_write(uint8_t address, uint8_t* writeData, uint8_t bytesToWrite);
bool									twim_read(uint8_t address, uint8_t bytesToRead);
bool									twim_writeread(uint8_t address, uint8_t* writeData, uint8_t bytesToWrite, uint8_t bytesToRead);
uint8_t									twim_getdata(uint8_t* readData, uint8_t size);
TWIM_RESULT_t							twim_getresult(void);

/*
00 0000 0	TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition
08 0000 1	TWI_START                  0x08  // START has been transmitted  
10 0001 0	TWI_REP_START              0x10  // Repeated START has been transmitted

// TWI Master Transmitter staus codes
18 0001 1	TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
20 0010 0	TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
28 0010 1	TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
30 0011 0	TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

38 0011 1	TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Receiver staus codes  
40 0100 0	TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
48 0100 1	TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
50 0101 0	TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
58 0101 1	TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Receiver staus codes
60 0110 0	TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
68 0110 1	TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
70 0111 0	TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
78 0111 1	TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
80 1000 0	TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
88 1000 1	TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
90 1001 0	TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
98 1001 1	TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
A0 1010 0	TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Slave Transmitter staus codes
A8 1010 1	TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
B0 1011 0	TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
B8 1011 1	TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
C0 1100 0	TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
C8 1100 1	TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

D0 1101 0
D8 1101 1
E0 1110 0
E8 1110 1
F0 1111 0
F8 1111 1	TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = “0”
*/

#endif /* TWIM_DRIVER_H_ */