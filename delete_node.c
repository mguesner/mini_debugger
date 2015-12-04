#include "mini_debugger.h"

void	delete_node(t_lst_break *lst, t_node_break *node)
{
	if (node == lst->begin)
	{
		lst->begin = node->next;
		free(node);
		lst->size--;
		return ;
	}
	t_node_break *curr = lst->begin->next;
	t_node_break *prev = lst->begin;
	while(curr)
	{
		if (curr == node)
		{
			if (curr == lst->end)
				lst->end = prev;
			prev->next = curr->next;
			lst->size--;
			free(curr);
			curr = prev->next;
		}
	}
}