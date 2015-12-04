#include <line_edition2.h>

static inline void	insert_in_line(char c, int pos, t_line *true_line)
{
	if (true_line->size + 1 >= true_line->allocated_size)
	{
		true_line->line = realloc(true_line->line, true_line->allocated_size * 2);
		true_line->allocated_size *= 2;
	}
	int i = true_line->size - 1;
	while (i >= pos)
	{
		true_line->line[i + 1] = true_line->line[i];
		i--;
	}
	true_line->line[pos] = c;
	true_line->size++;
	true_line->line[true_line->size] = 0;
}

static inline void	delete_in_line(int pos, t_line *true_line)
{
	int i = pos;
	while (i < true_line->size)
	{
		true_line->line[i] = true_line->line[i + 1];
		i++;
	}
	true_line->size--;
}

int		line_edition_getsubline(t_edit_line *line, char *comment)
{
	t_line	*true_line = (t_line *)line;
	int		pos = 0;
	int		ret;
	int 	ch = 0;
	true_line->line[0] = 0;
	true_line->size = 0;
	tputs(comment, 1, putonterm);
	while ((ret = read(0, &ch, 4)) > 0)
	{
		if (ch == 10)
		{
			tputs(tgetstr((char *)"do", NULL), 1, putonterm);
			tputs(tgetstr((char *)"cr", NULL), 1, putonterm);
			break;
		}
		else if (ch == 127)
		{
			if (pos > 0)
			{
				if (true_line->size % true_line->size_x == 0)
				{
					tputs(tgetstr((char *)"up", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"RI", NULL), 0, true_line->size_x), 1, putonterm);
				}
				else
					tputs(tgetstr((char *)"le", NULL), 1, putonterm);
				tputs(tgetstr((char *)"cd", NULL), 1, putonterm);
				pos--;
				delete_in_line(pos, true_line);
				tputs(true_line->line + pos, 1, putonterm);
				int index = true_line->size - pos;
				while (index > 0)
				{
					tputs(tgetstr((char *)"le", NULL), 1, putonterm);
					index--;
				}
			}
		}
		else if (ch == 4479771) //LEFT
		{
			if (pos > 0)
			{
				if (pos % true_line->size_x == 0)
				{
					tputs(tgetstr((char *)"up", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"RI", NULL), 0, true_line->size_x), 1, putonterm);
				}
				else
					tputs(tgetstr((char *)"le", NULL), 1, putonterm);
				pos--;
			}
		}
		else if (ch == 4414235) //RIGHT
		{
			if (pos < true_line->size)
			{
				if (pos % true_line->size_x == 0)
				{
					tputs(tgetstr((char *)"do", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"LE", NULL), 0, true_line->size_x), 1, putonterm);
				}
				else
					tputs(tgetstr((char *)"nd", NULL), 1, putonterm);
				pos++;
			}
		}
		else if (ch < 128)
		{
			insert_in_line(ch, pos++, true_line);
			write(1, &ch, 3);
			tputs(tgetstr((char *)"cd", NULL), 1, putonterm);
			tputs(true_line->line + pos, 1, putonterm);
			if (true_line->size % true_line->size_x == 0)
			{
				tputs(tgetstr((char *)"do", NULL), 1, putonterm);
				tputs(tgetstr((char *)"cr", NULL), 1, putonterm);
			}
			int index = true_line->size - pos;
			if ((true_line->size - index) / true_line->size_x != true_line->size / true_line->size_x && index != 0)
			{
				tputs(tgetstr((char *)"up", NULL), 1, putonterm);
				tputs(tgoto(tgetstr((char *)"RI", NULL), 0, true_line->size_x - index), 1, putonterm);
			}
			else if (index != 0)
				tputs(tgoto(tgetstr((char *)"LE", NULL), 0, index), 1, putonterm);
		}
		ch = 0;
		// printf("\n pos:%d\n", pos);
	}
	return ret < 0 ? ret : 0;
}