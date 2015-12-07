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
		printf("%-8dbreakpoint     %-5s%-4c"
			, curr->num + 1, curr->disp == KEEP ? "keep" : curr->disp == DISPOSE ? "dis" : "del"
			, curr->is_enable ? 'y' : 'n');
		if (!curr->is_pending)
		{
			printf("0x%.16llx", curr->addr);
			if (info.name)
			printf(" <%s+%d>"
				, info.name, info.addr_off);
		}
		else
			printf("%-18s %s", "<PENDING>", curr->sym);
		printf("\n");
		curr = curr->next;
	}
}