#include "mini_debugger.h"

void	db_file(t_env *e, char **args)
{
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
	}
	if (args[1])
	{
		e->file_name = strdup(args[1]);
		load_symbol(e);
	}
}
