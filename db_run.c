#include "mini_debugger.h"

void	db_run(t_env *e)
{
	printf("run\n");
	e->is_running = 1;
}