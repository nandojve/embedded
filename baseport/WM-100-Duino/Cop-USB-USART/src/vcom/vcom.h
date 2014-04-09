/*
 * vcom.h
 *
 * Created: 09/03/2013 19:27:44
 *  Author: nando
 */ 


#ifndef VCOM_H_
#define VCOM_H_

typedef enum AVRISP_MKII_VCOMSEL_STATE_t
{
	CMD_STATE_NOT_PRESS,
	CMD_STATE_PRESS,
	CMD_STATE_CHANGE_STATE,
	CMD_STATE_WAIT_UNPRESS,
} AVRISP_MKII_VCOMSEL_STATE_t;

void vcom_init(void);
void vcom_task(void);

#endif /* VCOM_H_ */