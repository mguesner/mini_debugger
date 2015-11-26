#ifndef LINE_EDITION2_H
# define LINE_EDITION2_H

#include "../line_edition.h"
#include <term.h>
#include <stdlib.h>
#include <unistd.h>

typedef	struct 					s_line
{
	char						*line;
	int							err;
	int							size;
	int							allocated_size;
	struct termios 				old;
	struct termios				termios_p;
	int							size_x;
}								t_line;

int putonterm(int c);

#endif