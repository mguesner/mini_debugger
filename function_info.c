#include "mini_debugger.h"

void	function_info(t_env *e, char **args)
{
	(void)args;
	printf("All defined functions:\n");
	int i = 0;
	while (i < e->size_sym_tab)
	{
		printf("0x%.16lx %s\n", e->sym_tab[i].addr, e->sym_tab[i].name);
		i++;
	}
}