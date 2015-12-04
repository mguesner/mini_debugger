#include "mini_debugger.h"

void	enable_delete(t_env *e, char **args)
{
	if (!args[2])
	{
		printf("Argument required (hit count).\n");
		return ;
	}
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == (unsigned int)atoi(args[2]))
		{
			curr->is_enable = 1;
			curr->disp = DELETE;
			return ;
		}
		curr = curr->next;
	}
}