#ifndef LINE_EDITION2_H
# define LINE_EDITION2_H

#include <term.h>
#include <stdlib.h>
#include <unistd.h>

typedef	struct 					s_line
{
	char						*line;
	int							size;
	int							allocated_size;
}								t_line;

struct termios 	old;
struct termios	termios_p;
int				size_x;
t_line			g_line;

int putonterm(int c);

#endif