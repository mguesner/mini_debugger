#include "mini_debugger.h"

void	enable_once(t_env *e, char **args)
{
	if (!args[2])
	{
		printf("Argument required (one or more breakpoint numbers).\n");
		return ;
	}
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == (unsigned int)atoi(args[2]))
		{
			curr->is_enable = 1;
			curr->disp = DISPOSE;
			curr->nb_disable = 1;
			return ;
		}
		curr = curr->next;
	}
	printf("No breakpoint number %s.\n", args[2]);
}