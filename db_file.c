#include "mini_debugger.h"

void	db_file(t_env *e, char **args)
{
	int i = 0;
	if (e->file_name)
	{
		free(e->file_name);
		e->file_name = NULL;
		int i = 0;
		while (i < e->size_sym_tab)
		{
			free(e->sym_tab[i].name);
			i++;
		}
		e->size_sym_tab = 0;
		free(e->sym_tab);
		t_node_break *curr = e->lst_break.begin;
		while(curr)
		{
			if (curr->type == SYMBOL)
			{
				curr->is_pending = 1;
			}
			curr = curr->next;
		}
	}
	if (args[1])
	{
		e->file_name = strdup(args[1]);
		load_symbol(e);
		t_node_break *curr = e->lst_break.begin;
		while(curr)
		{
			if (curr->is_pending)
				while (i < e->size_sym_tab)
				{
					if (e->sym_tab[i].name && !strcmp(e->sym_tab[i].name, curr->sym))
					{
						curr->addr = e->sym_tab[i].addr + 4;
						curr->is_pending = 0;
						break;
					}
					i++;
				}
			curr = curr->next;
		}
	}
}