/*
 * Terminal.c
 *
 * Created: 17/02/2014 20:47:06
 *  Author: nando
 */

#include <stdio.h>
#include "Terminal.h"

static	TerminalState	term_state		= TS_INIT;
static	char			term_cmd[CMAXCL];
static	terminal_user_process_command	user_process_funcion = NULL;

void terminal_console_string(TerminalState *state, char* s, uint8_t mxc)
{
	static uint8_t	len;
	static int		getcres;
	static uint8_t	c;

	if(*state == TS_INIT)
	{
		*state							= TS_RUN;
		s[0]							= CEOS;
		len								= 0;
	}

	getcres								= fgetc(stdin);

	if(getcres != 0x00)
	{
		c								= (uint8_t) (getcres & 0x00ff);
		switch(c)
		{
			case CBS: 
				if(len > 0)
				{
					s[--len]			= CEOS;

					putc(CBS, stdout);
					putc(CBLANK, stdout);
					putc(CBS, stdout);
				}
				break;
			case CENTER:
				break;
			case CNEWLINE:
				s[len]					= CEOS;
				*state					= TS_RETURN;
				break;
			default: 
				if(len < mxc - 1)
				{
					s[len++]			= c;
					putc(c, stdout);
				}
				break;
		}
	}
}

char* GetCLParmStr(char cl[], uint8_t n)
{
	uint8_t i=0, ac=0;

	// skip leading whitepaces if any
	while( (cl[i]!= CEOS) && (cl[i]==CBLANK)) i++;
	
	// go to n-th arg (0 is the command)
	for(ac=0; ac<n; ac++)
	{
		// find first character of argument
		while( (cl[i]!= CEOS) && (cl[i]!=CBLANK)) i++;
		// skip whitespaces separating arguments
		while( (cl[i]!= CEOS) && (cl[i]==CBLANK)) i++;
	}
	// return address of first character in n-th argument
	return &cl[i];
}

TerminalState* terminal_init(terminal_user_process_command userfunction)
{
	term_state							= TS_INIT;
	user_process_funcion				= userfunction;

	return(&term_state);
}

void terminal_task()
{
	switch(term_state)
	{
		case TS_RUN:
			terminal_console_string(&term_state, term_cmd, CMAXCL);
			break;
		case TS_INIT:
			printf("%s", SHELLCMD);
			terminal_console_string(&term_state, term_cmd, CMAXCL);
			break;
		case TS_RETURN:
			term_state					= TS_INIT;
			terminal_process_command(&term_state, term_cmd);
			break;
		case TS_REDRAW:
			term_state					= TS_RUN;
			printf("%s%s", SHELLCMD, term_cmd);
			break;
		case TS_RESET:
		case TS_BOOT:
		default:
			break;
	}
}

void terminal_process_command(TerminalState* state, char* command)
{
	putc('\r', stdout);
	putc('\n', stdout);

	if(user_process_funcion)
	{
		(user_process_funcion)(state, command);
	}
}
bool terminal_wait_any_key(void)
{
	return(fgetc(stdin) != EOF);
}
void terminal_reset(void)
{
	term_state							= TS_INIT;
	
	// flush buffer
	while(fgetc(stdin) != 0x00);

	putc('\r', stdout);
	putc('\n', stdout);
}
void terminal_close(void)
{
	putc('\r', stdout);
	putc('\n', stdout);
}
