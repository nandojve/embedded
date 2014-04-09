/*
 * twim_driver.c
 *
 * Created: 14/11/2011 01:24:00
 *  Author: Nando
 */
#ifdef TWI_MASTER_DRIVER

#include "twim_driver.h"

TWI_Master_t							twi;

void twim_init(uint8_t bitrate)
{
	TWBR								= bitrate;										// Set bit rate register (Baudrate). Defined in header file.
	//TWSR								= TWI_TWPS;										// Not used. Driver presumes prescaler to be 00.
	TWDR								= 0xFF;											// Default content = SDA released.
	TWCR								= (1 << TWEN) |									// Enable TWI-interface and release TWI pins.
										  (0 << TWIE) | (0 << TWINT) |					// Disable Interrupt.
										  (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |	// No Signal requests.
										  (0 << TWWC);

	twi.state							= TWIM_STANDBY;
	twi.result							= TWIM_RESULT_UNKNOWN;
	twi.bytesRead						= 0;
	twi.bytesWritten					= 0;
}
void twim_wait(void)
{
	while(twi.state != TWIM_STANDBY)
	{
		twim_task();
	}
}
void twim_task(void)
{
	if(twi.state == TWIM_BUSY)
	{
		return;
	}
	else if(twi.state == TWIM_START)
	{
		twi.state						= TWIM_BUSY;
		TWCR							= (1 << TWEN) |									// TWI Interface enabled.
										  (1 << TWIE) | (1 << TWINT) |					// Enable TWI Interrupt and clear the flag.
										  (0 << TWEA) | (1 << TWSTA) | (0 << TWSTO) |	// Initiate a START condition.
										  (0 << TWWC);
	}
	else if(twi.state != TWIM_STANDBY)
	{
		// SLA+W has been transmitted and NACK received
		// SLA+R has been transmitted and NACK received
		// Data byte has been transmitted and NACK received
		// Bus error due to an illegal START or STOP condition
		if(twi.state == TWI_MTX_ADR_NACK || twi.state == TWI_MRX_ADR_NACK || twi.state == TWI_MTX_DATA_NACK || twi.state == TWI_BUS_ERROR)
		{
			TWCR					= (1<<TWEN) |										// TWI Interface enabled
									  (1<<TWIE) | (1<<TWINT) |							// Enable TWI Interrupt and clear the flag
									  (0<<TWEA) | (1<<TWSTA) | (0<<TWSTO) |				// Initiate a (RE)START condition.
									  (0<<TWWC);

			twi.state				= TWIM_START;
//			twi.state				= TWIM_STANDBY;
		}
	}
}
/*! \brief Returns true if transaction is ready.
 *
 *  This function returns a boolean whether the TWI Master is ready
 *  for a new transaction.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool twim_ready(void)
{
	return(twi.state == TWIM_STANDBY);
}
/*! \brief TWI write transaction.
 *
 *  This function is TWI Master wrapper for a write-only transaction.
 *
 *  \param address      Slave address.
 *  \param writeData    Pointer to data to write.
 *  \param bytesToWrite Number of data bytes to write.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool twim_write(uint8_t address, uint8_t* writeData, uint8_t bytesToWrite)
{
	bool	twi_status			= twim_writeread(address, writeData, bytesToWrite, 0);

	return(twi_status);
}
/*! \brief TWI read transaction.
 *
 *  This function is a TWI Master wrapper for read-only transaction.
 *
 *  \param address        The slave address.
 *  \param bytesToRead    The number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool twim_read(uint8_t address, uint8_t bytesToRead)
{
	bool	twi_status			= twim_writeread(address, NULL, 0, bytesToRead);

	return(twi_status);
}
/*! \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param address			The slave DEVICE address.
 *  \param writeData		Pointer to data to write.
 *  \param bytesToWrite		Number of bytes to write.
 *  \param bytesToRead		Number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool twim_writeread(uint8_t address, uint8_t* writeData, uint8_t bytesToWrite, uint8_t bytesToRead)
{
	// Initiate transaction if bus is ready (not busy).
	if(!(TWCR & (1 << TWIE)))
	{
		// Parameter sanity check.
		if(bytesToWrite > TWIM_WRITE_BUFFER_SIZE - 1)
		{
			return(false);
		}
		if(bytesToRead > TWIM_READ_BUFFER_SIZE)
		{
			return(false);
		}

		twi.address						= address << 1;
		twi.bytesToWrite				= bytesToWrite + 1;
		twi.bytesToRead					= bytesToRead;
		twi.bytesWritten				= 0;
		twi.bytesRead					= 0;

		// If read command, send the START condition + Address + 'R/_W = 1'
		if (twi.bytesToRead > 0)
		{
			twi.writeData[0]			= twi.address | 0x01;
		}
		else
		{
			// If write command, send the START condition + Address + 'R/_W = 0'
			twi.writeData[0]			= twi.address & ~0x01;
		}

		/* Fill write data buffer. */
		for(uint8_t bufferIndex = 0; bufferIndex < bytesToWrite; bufferIndex++)
		{
			twi.writeData[bufferIndex + 1]	= writeData[bufferIndex];
		}

		twi.result						= TWIM_RESULT_UNKNOWN;
		twi.state						= TWIM_START;

		return(true);
	}
	else
	{
		return(false);
	}
}
/*! \brief TWI get data from Read Transaction
 *
 *  This function get data read from a read transaction. The function
 *  can be used after read operation from a TWI Slave in one transaction.
 *
 *  \param readData			Pointer to data to read.
 *  \param size				Number of bytes to read.
 *
 *  \retval Number of bytes read
 */
uint8_t twim_getdata(uint8_t* readData, uint8_t size)
{
	if(twi.state == TWIM_STANDBY && twi.result == TWIM_RESULT_OK)
	{
		uint8_t	toRead					= size < twi.bytesRead ? size : twi.bytesRead;
		
		for(uint8_t pos = 0; pos < toRead; pos++)
		{
			readData[pos]				= twi.readData[pos];
		}

		return(toRead);
	}
	
	return(0);
}
/*! \brief TWI get result of Transaction
 *
 *  This function get the result of read/write transaction. The function
 *  can be used after read/write operation from a TWI Slave in one transaction.
 *
 *  \retval Result status
 */
TWIM_RESULT_t twim_getresult(void)
{
	return(twi.result);
}
// ********** Interrupt Handlers ********** //
/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
****************************************************************************/
ISR(TWI_vect)
{
	switch(TWSR & 0xF8)
	{
		case TWI_START:																	// START has been transmitted  
		case TWI_REP_START:																// Repeated START has been transmitted
			// Set buffer pointer to the TWI Address location
			twi.bytesRead		= 0;
			twi.bytesWritten	= 0;
		case TWI_MTX_ADR_ACK:															// SLA+W has been transmitted and ACK received
		case TWI_MTX_DATA_ACK:															// Data byte has been transmitted and ACK received
		{
			if(twi.bytesWritten < twi.bytesToWrite)
			{
				TWDR			= twi.writeData[twi.bytesWritten++];					// Set data to send
				TWCR			= (1<<TWEN) |											// TWI Interface enabled
								  (1<<TWIE) | (1<<TWINT) |								// Enable TWI Interrupt and clear the flag to send byte
								  (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |
								  (0<<TWWC);
			}
			else																		// Send STOP after last byte
			{
				TWCR			= (1<<TWEN) |											// TWI Interface enabled
								  (0<<TWIE) | (1<<TWINT) |								// Disable TWI Interrupt and clear the flag
								  (0<<TWEA) | (0<<TWSTA) | (1<<TWSTO) |					// Initiate a STOP condition.
								  (0<<TWWC);

				twi.result		= TWIM_RESULT_OK;										// Set status bits to completed successfully. 
				twi.state			= TWIM_STANDBY;											// Store TWSR and automatically.
			}
			break;
		}			
		case TWI_MRX_DATA_ACK:															// Data byte has been received and ACK transmitted
			if(twi.bytesRead < twi.bytesToRead)
			{
				twi.readData[twi.bytesRead++]	= TWDR;
			}				
		case TWI_MRX_ADR_ACK:															// SLA+R has been transmitted and ACK received
			if(twi.bytesRead < twi.bytesToRead)
			{
				TWCR			= (1<<TWEN) |											// TWI Interface enabled
								  (1<<TWIE) | (1<<TWINT) |								// Enable TWI Interrupt and clear the flag to read next byte
								  (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |					// Send ACK after reception
								  (0<<TWWC);
			}
			else																		// Send NACK after next reception
			{
				TWCR			= (1<<TWEN) |											// TWI Interface enabled
								  (1<<TWIE) | (1<<TWINT) |								// Enable TWI Interrupt and clear the flag to read next byte
								  (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |					// Send NACK after reception
								  (0<<TWWC);
			}    
			break; 
		case TWI_MRX_DATA_NACK:															// Data byte has been received and NACK transmitted
		{
			uint8_t data		= TWDR;

			TWCR				= (1<<TWEN) |											// TWI Interface enabled
								  (0<<TWIE) | (1<<TWINT) |								// Disable TWI Interrupt and clear the flag
								  (0<<TWEA) | (0<<TWSTA) | (1<<TWSTO) |					// Initiate a STOP condition.
								  (0<<TWWC);

			twi.result			= TWIM_RESULT_OK;										// Set status bits to completed successfully. 
			twi.state			= TWIM_STANDBY;											// Store TWSR and automatically.
			break;
		}			
		case TWI_ARB_LOST:																// Arbitration lost
			twi.state			= (TWSR & 0xF8);										// Store TWSR and automatically.
			TWCR				= (1<<TWEN) |											// TWI Interface enabled
								  (1<<TWIE) | (1<<TWINT) |								// Enable TWI Interrupt and clear the flag
								  (0<<TWEA) | (1<<TWSTA) | (0<<TWSTO) |					// Initiate a (RE)START condition.
								  (0<<TWWC);
								  
			twi.result			= TWIM_RESULT_ARBITRATION_LOST;							// Set status bits to arbitration lost. 
			break;
		case TWI_MTX_ADR_NACK:															// SLA+W has been transmitted and NACK received
		case TWI_MRX_ADR_NACK:															// SLA+R has been transmitted and NACK received    
		case TWI_MTX_DATA_NACK:															// Data byte has been transmitted and NACK received
			twi.state			= (TWSR & 0xF8);										// Store TWSR and automatically.
			twi.result			= TWIM_RESULT_NACK_RECEIVED;							// Set status bits to arbitration lost

			// Reset TWI Interface
			TWCR				= (1<<TWEN) |											// Enable TWI-interface and release TWI pins
								  (0<<TWIE) | (0<<TWINT) |								// Disable Interrupt
								  (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |					// No Signal requests
								  (0<<TWWC);
			break;
		//case TWI_NO_STATE																// No relevant state information available; TWINT = “0”
		case TWI_BUS_ERROR:																// Bus error due to an illegal START or STOP condition
			twi.state			= (TWSR & 0xF8);										// Store TWSR and automatically.
			twi.result			= TWIM_RESULT_BUS_ERROR;								// Set status bits to arbitration lost

			// Reset TWI Interface
			TWCR				= (1<<TWEN) |											// Enable TWI-interface and release TWI pins
								  (0<<TWIE) | (0<<TWINT) |								// Disable Interrupt
								  (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |					// No Signal requests
								  (0<<TWWC);
			break;
		default:
			twi.state			= TWIM_STANDBY;											// Store TWSR and automatically.
//			twi.result			= TWIM_RESULT_OK;										// Set status bits to completed successfully. 

			// Reset TWI Interface
			TWCR				= (1<<TWEN) |											// Enable TWI-interface and release TWI pins
								  (0<<TWIE) | (0<<TWINT) |								// Disable Interrupt
								  (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) |					// No Signal requests
								  (0<<TWWC);
	}
}

#endif /* TWI_MASTER_DRIVER */