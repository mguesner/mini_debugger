#include "mini_debugger.h"

void	address_info(t_env *e, char **args)
{
	if (!args[2])
		printf("Argument required.\n");
	else
	{
		int i = 0;
		while (i < e->size_sym_tab)
		{
			if (e->sym_tab[i].name && !strcmp(e->sym_tab[i].name, args[2]))
			{
				printf("Symbol \"%s\" is at 0x%lx in a file compiled without debugging.\n"
					, e->sym_tab[i].name, e->sym_tab[i].addr);
				return ;
			}
			i++;
		}
		printf("No symbol \"%s\" in current context.\n", args[2]);
	}
}