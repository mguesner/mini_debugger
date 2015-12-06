#include <line_edition2.h>
#include <stdio.h>
#include "line_edition.h"

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

const t_func g_funcs[] = {
	{4479771, f_left},
	{4414235, f_right}
};

int		line_edition_get_line(t_edit_line *line)
{
	t_line	*true_line = (t_line *)line;
	true_line->pos = 0;
	int		ret;
	int		i;
	unsigned char 	ch[4];
	true_line->line[0] = 0;
	true_line->size = 0;
	write(1, true_line->prompt, true_line->prompt_size);
	while ((ret = read(0, ch, 4)) > 0)
	{
		if (ch[0] == 27)
		{
			i = 0;
			while (i < FUNC_SIZE)
			{
				if (g_funcs[i].func_id == *(int*)ch)
				{
					g_funcs[i].func(true_line);
					break ;
				}
				i++;
			}
			*(int*)ch = 0;
			continue ;
		}
		// printf("%d %d %d %d\n", ch[3], ch[2], ch[1], ch[0]);
		i = 0;
		while (i < 4 && ch[i])
		{
			if (ch[i] == 10)
			{
				tputs(tgetstr((char *)"do", NULL), 1, putonterm);
				tputs(tgetstr((char *)"cr", NULL), 1, putonterm);
				return ret < 0 ? ret : 0;
			}
			else if (ch[i] == 127)
			{
				if (true_line->pos > 0)
				{
					if (true_line->size % true_line->size_x == 0)
					{
						tputs(tgetstr((char *)"up", NULL), 1, putonterm);
						tputs(tgoto(tgetstr((char *)"RI", NULL), 0, true_line->size_x), 1, putonterm);
					}
					else
						tputs(tgetstr((char *)"le", NULL), 1, putonterm);
					tputs(tgetstr((char *)"cd", NULL), 1, putonterm);
					true_line->pos--;
					delete_in_line(true_line->pos, true_line);
					tputs(true_line->line + true_line->pos, 1, putonterm);
					int index = true_line->size - true_line->pos;
					while (index > 0)
					{
						tputs(tgetstr((char *)"le", NULL), 1, putonterm);
						index--;
					}
				}
			}
			else if (ch[i] < 128)
			{
				insert_in_line(ch[i], true_line->pos++, true_line);
				write(1, ch + i, 1);
				tputs(tgetstr((char *)"cd", NULL), 1, putonterm);
				tputs(true_line->line + true_line->pos, 1, putonterm);
				if (true_line->size % true_line->size_x == 0)
				{
					tputs(tgetstr((char *)"do", NULL), 1, putonterm);
					tputs(tgetstr((char *)"cr", NULL), 1, putonterm);
				}
				int index = true_line->size - true_line->pos;
				if ((true_line->size - index) / true_line->size_x != true_line->size / true_line->size_x && index != 0)
				{
					tputs(tgetstr((char *)"up", NULL), 1, putonterm);
					tputs(tgoto(tgetstr((char *)"RI", NULL), 0, true_line->size_x - index), 1, putonterm);
				}
				else if (index != 0)
					tputs(tgoto(tgetstr((char *)"LE", NULL), 0, index), 1, putonterm);
			}
			i++;
		}
		*(int*)ch = 0;
		// printf("\n true_line->pos:%d\n", true_line->pos);
	}
	return ret < 0 ? ret : 0;
}