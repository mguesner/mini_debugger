#include <line_edition2.h>

void	line_editor_end()
{
	tcsetattr(0, TCSAFLUSH, &(old));
	tputs(tgetstr((char *)"ei", NULL), 1, putonterm);
}