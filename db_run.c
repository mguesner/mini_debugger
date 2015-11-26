#include "mini_debugger.h"

void	db_run(t_env *e, char **args)
{
	(void)args;
	printf("run\n");
	e->is_running = 1;
}