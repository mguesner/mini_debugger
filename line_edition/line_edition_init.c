#include <line_edition2.h>

void	line_editor_init()
{
	tgetent(NULL, getenv("TERM"));
	tputs(tgetstr((char *)"im", NULL), 1, putonterm);
	tcgetattr(0, &(old));
	tcgetattr(0, &(termios_p));
	size_x = tgetnum((char *)"co");
	termios_p.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSADRAIN, &(termios_p));
	g_line.line = malloc(50);
	g_line.allocated_size = 50;
}