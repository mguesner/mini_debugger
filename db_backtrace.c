#include "mini_debugger.h"

void	db_backtrace(t_env *e, char **args)
{
	if (!e->is_running)
	{
		printf("No stack.\n");
		return ;
	}
	long long	rip = e->regs.rip;
	long long	rsp = e->regs.rbp;
	if (rip > 0x700000000000)
		 rsp = e->regs.rsp;
	int			i = 0;
	(void) args;

	while (rsp)
	{
		t_sym_info info = get_current_sym(*e, rip);
		if (!info.name)
		{
			printf("lost backtrace.\n");
			break ;
		}
		printf("#%d  0x%.16llx in %s()\n", i, rip, info.name);
		if (!strcmp("main", info.name))
			break;
		if (rip > 0x700000000000)
		{
			rip = ptrace(PTRACE_PEEKDATA, e->child, rsp, NULL);
			rsp = e->regs.rbp;
		}
		else
		{
			rip = ptrace(PTRACE_PEEKDATA, e->child, rsp + 8, NULL);
			rsp = ptrace(PTRACE_PEEKDATA, e->child, rsp, NULL);
		}
		i++;
	}
}