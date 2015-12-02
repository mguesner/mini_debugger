#include "mini_debugger.h"

void	enable_breakpoint(t_env *e, char **args)
{
	if (!args[2])
	{
		t_node_break *curr = e->lst_break.begin;
		while(curr)
		{
			curr->is_enable = 1;
			curr = curr->next;
		}
		return ;
	}
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == (unsigned int)atoi(args[2]))
			break;
		curr = curr->next;
	}
	if (curr)
	{
		curr->is_enable = 1;
		return ;
	}
}