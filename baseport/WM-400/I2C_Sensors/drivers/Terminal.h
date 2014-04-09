/*
 * Terminal.h
 *
 * Created: 17/02/2014 20:47:11
 *  Author: nando
 */ 


#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <stdint.h>

#define	SHELLCMD	"SHELL>"
#define CEOS		'\0'
#define CENTER		'\r'
#define CNEWLINE	'\n'
#define CBS			'\b'
#define CBLANK		' '

#define CMAXCL		50

typedef enum eTerminalState
{
	TS_BOOT			= 0,
	TS_INIT,
	TS_RUN,
	TS_RETURN,
	TS_REDRAW,
	TS_RESET,
	TS_USER,
} TerminalState;

typedef enum eTerminalError
{
	TE_NOERROR		= 0,
	TE_UNKNOWN		= 1,	
} TerminalError;

typedef void (*terminal_user_process_command)(TerminalState* state, char* command);

char* GetCLParmStr(char cl[], uint8_t n);

TerminalState* terminal_init(terminal_user_process_command userfunction);
void terminal_task(void);
void terminal_process_command(TerminalState* state, char* command);
void terminal_console_string(TerminalState* state, char* s, uint8_t mxc);


#endif /* TERMINAL_H_ */