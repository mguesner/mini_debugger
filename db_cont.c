#include "mini_debugger.h"

void	db_cont(t_env *e, char **args)
{
	(void)args;
	if (!e->is_running)
	{
		printf("The program is not being run.\n");
		return ;
	}
	if (e->current_break)
	{
		if (e->current_break->disp == KEEP)
		{
			ptrace(PTRACE_SINGLESTEP, e->child, 0, 0);
			waitpid(e->child, &e->status, 0);
			unsigned long tmp = ptrace(PTRACE_PEEKDATA, e->child, e->current_break->addr, NULL);
			// printf("opcode -> 0x%x at addr -> 0x%llx\n", e->current_break->opcode, e->current_break->addr);
			tmp = tmp - (tmp & 0b11111111) + 0xcc;
			// printf("opcode -> 0x%lx\n", tmp);
			ptrace(PTRACE_POKEDATA, e->child, e->current_break->addr, tmp);
		}
		else if (e->current_break->disp == DELETE)
			delete_node(&e->lst_break, e->current_break);
		else if (e->current_break->disp == DISPOSE)
		e->current_break = NULL;
	}
	printf("Continuing.\n");
	wait_event(e);
}