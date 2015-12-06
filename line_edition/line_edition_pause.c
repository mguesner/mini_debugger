#include <line_edition2.h>

void	line_edition_pause(t_edit_line *line)
{
	(void)line;
	t_line	*true_line = (t_line *)line;
	tcsetattr(0, TCSAFLUSH, &(true_line->pause));
	tputs(tgetstr((char *)"ei", NULL), 1, putonterm);
}