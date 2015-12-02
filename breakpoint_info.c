#include "mini_debugger.h"

void	breakpoint_info(t_env *e, char **args)
{
	(void)args;
	printf("Num     Type           Disp Enb Address            What\n");
	t_node_break *curr = e->lst_break.begin;
	if (!curr)
	{
		printf("No breakpoints or watchpoints.\n");
		return ;
	}
	while (curr)
	{
		t_sym_info info = get_current_sym(*e, curr->addr);
		printf("%-8dbreakpoint     %-5s%-4c0x%.16llx <%s+%d>\n"
			, curr->num + 1, curr->disp == KEEP ? "keep" : curr->disp == DISPOSE ? "dis" : "del"
			, curr->is_enable ? 'y' : 'n', curr->addr, info.name, info.addr_off);
		curr = curr->next;
	}
}