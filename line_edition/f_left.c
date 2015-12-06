#include <line_edition2.h>

void					f_left(t_line *line)
{
	if (line->pos > 0)
	{
		if (line->pos % line->size_x == 0)
		{
			tputs(tgetstr((char *)"up", NULL), 1, putonterm);
			tputs(tgoto(tgetstr((char *)"RI", NULL), 0, line->size_x), 1, putonterm);
		}
		else
			tputs(tgetstr((char *)"le", NULL), 1, putonterm);
		line->pos--;
	}
}