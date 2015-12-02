#include "mini_debugger.h"

t_sym_info	get_current_sym(t_env e, long addr)
{
	t_sym_info	ret;
	int			i = 0;
	while(i < e.size_sym_tab)
	{
		if (e.sym_tab[i].addr > addr)
			break;
		i++;
	}
	ret.addr_off = addr - e.sym_tab[i - 1].addr;
	ret.name = e.sym_tab[i - 1].name;
	return ret;
}