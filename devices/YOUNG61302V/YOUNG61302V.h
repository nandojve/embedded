/*
 * YOUNG61302V.h
 *
 * Created: 30/05/2012 22:32
 *  Author: Nando
 */ 
#ifndef YOUNG61302V_H_
#define YOUNG61302V_H_

typedef struct Young302V_data_tag
{
	double	pression;
} Young302V_data_t;

void		young61302v_init(void);
void		young61302v_task(void);
void		young61302v_read(Young302V_data_t* data);

#endif /* YOUNG61302V_H_ */