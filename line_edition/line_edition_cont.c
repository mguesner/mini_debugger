#include <line_edition2.h>

void	line_edition_cont(t_edit_line *line)
{
	(void)line;
	t_line	*true_line = (t_line *)line;
	tcsetattr(0, TCSADRAIN, &(true_line->termios_p));
	tputs(tgetstr((char *)"im", NULL), 1, putonterm);
}