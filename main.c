#include "mini_debugger.h"

const t_command commands[] = {
		{"r", db_run},
		{"run", db_run},
		{"q", db_quit},
		{"quit", db_quit}
	};

int main(int argc, char **argv)
{
	(void) argc;
	mini_debugger(argv + 1);
	return (0);
}