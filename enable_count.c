#include "mini_debugger.h"

void	enable_count(t_env *e, char **args)
{
	(void)e;
	(void)args;
	if (!args[2])
	{
		printf("Argument required (hit count).\n");
		return ;
	}
	if (!args[3])
	{
		printf("Argument required (one or more breakpoint numbers).\n");
		return ;
	}
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == (unsigned int)atoi(args[3]))
			break;
		curr = curr->next;
	}
	if (curr)
	{
		curr->is_enable = 1;
		curr->disp = DISPOSE;
		curr->nb_disable = atoi(args[2]);
		return ;
	}
	printf("No breakpoint number %s.\n", args[2]);
}