#include <line_edition2.h>

void					f_right(t_line *line)
{
	if (line->pos < line->size)
	{
		if (line->pos % line->size_x == 0)
		{
			tputs(tgetstr((char *)"do", NULL), 1, putonterm);
			tputs(tgoto(tgetstr((char *)"LE", NULL), 0, line->size_x), 1, putonterm);
		}
		else
			tputs(tgetstr((char *)"nd", NULL), 1, putonterm);
		line->pos++;
	}
}