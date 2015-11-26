#include <line_edition2.h>

t_edit_line		*line_editor_init()
{
	t_line	*line = malloc(sizeof(t_line));
	tgetent(NULL, getenv("TERM"));
	tputs(tgetstr((char *)"im", NULL), 1, putonterm);
	tcgetattr(0, &(line->old));
	tcgetattr(0, &(line->termios_p));
	line->size_x = tgetnum((char *)"co");
	line->termios_p.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSADRAIN, &(line->termios_p));
	line->line = malloc(50);
	line->allocated_size = 50;
	return ((t_edit_line *)line);
}