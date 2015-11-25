#include <line_edition2.h>
#include <stdio.h>
#include "line_edition.h"

static inline void	insert_in_line(char c, int pos)
{
	if (g_line.size + 1 >= g_line.allocated_size)
	{
		g_line.line = realloc(g_line.line, g_line.allocated_size * 2);
		g_line.allocated_size *= 2;
	}
	int i = g_line.size - 1;
	while (i >= pos)
	{
		g_line.line[i + 1] = g_line.line[i];
		i--;
	}
	g_line.line[pos] = c;
	g_line.size++;
	g_line.line[g_line.size] = 0;
}

int		line_edition_get_line(char **line)
{
	int		pos = 0;
	int		ret;
	int 	ch = 0;
	g_line.line[0] = 0;
	g_line.size = 0;

	while ((ret = read(0, &ch, 4)) > 0)
	{
		if (ch == 10)
			break;
		else if (ch == 4479771) //LEFT
		{
			if (pos > 0)
			{
				if (pos % size_x == 0)
				{
					tputs(tgetstr((char *)"up", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"RI", NULL), 0, size_x), 1, putonterm);
				}
				else
					tputs(tgetstr((char *)"le", NULL), 1, putonterm);
				pos--;
			}
		}
		else if (ch == 4414235) //RIGHT
		{
			if (pos < g_line.size)
			{
				if (pos % size_x == 0)
				{
					tputs(tgetstr((char *)"do", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"LE", NULL), 0, size_x), 1, putonterm);
				}
				else
					tputs(tgetstr((char *)"nd", NULL), 1, putonterm);
				pos++;
			}
		}
		else if (ch < 128)
		{
			insert_in_line(ch, pos++);
			write(1, &ch, 3);
			tputs(tgetstr((char *)"cd", NULL), 1, putonterm);
			tputs(g_line.line + pos, 1, putonterm);
			if (g_line.size % size_x == 0)
			{
				tputs(tgetstr((char *)"do", NULL), 1, putonterm);
				tputs(tgetstr((char *)"cr", NULL), 1, putonterm);
			}
			int index = g_line.size - pos;
			if ((g_line.size - index) / size_x != g_line.size / size_x && index != 0)
			{
				tputs(tgetstr((char *)"up", NULL), 1, putonterm);
				tputs(tgoto(tgetstr((char *)"RI", NULL), 0, size_x - index), 1, putonterm);
			}
			else if (index != 0)
				tputs(tgoto(tgetstr((char *)"LE", NULL), 0, index), 1, putonterm);
		}
		ch = 0;
	}
	printf("ret --> %d\n", ret);
	*line = g_line.line;
	return ret;
}