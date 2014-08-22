/*
 * YOUNG61302V.c
 *
 * Created: 30/05/2012 22:32
 *  Author: Nando
 */ 

#include "YOUNG61302V.h"
#include "RS485_Driver.h"

static volatile uint16_t timer_count;
static Young302V_data_t sensor_data;

ISR(TIMER2_COMPA_vect)
{
	timer_count++;			/* Performance counter for this module */
}

void young61302v_init(void)
{
	timer_count = 0;
	OCR2A = F_CPU / 1024 / (100 - 1);	// OC2: 100Hz interval timer
	TCCR2A |= _BV(WGM21);
	TCCR2B |= 0x03;
	TIMSK2 |= _BV(OCIE2A);
}
void young61302v_task(void)
{
	// 1 second pass?
	if(timer_count < 200)
		return;
	
	timer_count = 0;	
	// polling execute
	uint8_t	buffer[16];
	uint8_t	txSize;
	
	txSize = sprintf(buffer, "M0!");
	rs485_write(buffer, txSize);

	// Wait 300ms to receive data otherwise exit
	while(timer_count < 100)
	{
		if(rs485_rx_length() >= 9)
			break;
	}
	
	if(timer_count >= 100)
		return;

	rs485_read(buffer, 9);
	if(buffer[7] == '\r' && buffer[8] == '\n')
		sensor_data.pression = atof(buffer);
	else
		rs485_purge();
}
void young61302v_read(Young302V_data_t* data)
{
	if(!data)
		return;

	*data = sensor_data;
}
