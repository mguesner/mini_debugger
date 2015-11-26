#include "mini_debugger.h"

void	init_debugger(t_env *e)
{
	e->is_running = 0;
	e->file_name = NULL;
}