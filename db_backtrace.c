#include "mini_debugger.h"

void	db_backtrace(t_env *e, char **args)
{
	if (!e->is_running)
	{
		printf("No stack.\n");
		return ;
	}
	long long	rip = e->regs.rip;
	long long	rbp = e->regs.rbp;
	int			i = 0;
	(void) args;

	while (rip < 0x700000000000)
	{
		t_sym_info info = get_current_sym(*e, rip);
		printf("#%d  0x%.16llx in %s()\n", i, rip, info.name);
		rip = ptrace(PTRACE_PEEKDATA, e->child, rbp + 8, NULL);
		rbp = ptrace(PTRACE_PEEKDATA, e->child, rbp, NULL);
		i++;
	}
}