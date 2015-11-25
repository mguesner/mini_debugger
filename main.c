#include "mini_debugger.h"

int main(int argc, char **argv)
{
	line_editor_init();
	(void) argc;
	mini_debugger(argv + 1);
	line_editor_end();
	return (0);
}