#include "mini_debugger.h"

void	db_delete(t_env *e, char **args)
{
	if (!args[1])
	{
		t_node_break *curr = e->lst_break.begin;
		t_node_break *prev;
		while(curr)
		{
			prev = curr;
			curr = curr->next;
			delete_node(&e->lst_break, prev);
		}
		return ;
	}
	unsigned int num = atoi(args[1]);
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		if (curr->num + 1 == num)
		{
			delete_node(&e->lst_break, curr);
			return ;
		}
		curr = curr->next;
	}
	printf("No breakpoint number %d.\n", num);
}