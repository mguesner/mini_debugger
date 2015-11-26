#include "mini_debugger.h"

void	db_quit(t_env *e, char **args)
{
	(void)args;
	(void)e;
	exit(0);
}