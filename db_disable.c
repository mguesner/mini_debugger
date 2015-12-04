#include "mini_debugger.h"

void	db_disable(t_env *e, char **args)
{
	if (!args[1])
	{
		t_node_break *curr = e->lst_break.begin;
		while(curr)
		{
			curr->is_enable = 0;
			curr = curr->next;
		}
		return ;
	}
	unsigned int num = atoi(args[1]);
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == num)
		{
			curr->is_enable = 0;
			return ;
		}
		curr = curr->next;
	}
	printf("No breakpoint number %d.\n", num);
}