#ifndef LINE_EDITION2_H
# define LINE_EDITION2_H

#include "../line_edition.h"
#include <term.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef	struct 					s_line
{
	char						*line;
	int							err;
	int							size;
	int							allocated_size;
	struct termios 				old;
	struct termios 				pause;
	struct termios				termios_p;
	int							size_x;
	char						*prompt;
	int							prompt_size;
	int							pos;
}								t_line;

typedef struct
{
	int						func_id;
	void						(*func)(t_line *);
}								t_func;

int putonterm(int c);

/*
**escaped char
*/
extern const t_func		g_funcs[];
#define FUNC_SIZE 2
// void					f_delete(t_line *line);
void					f_left(t_line *line);
void					f_right(t_line *line);

#endif