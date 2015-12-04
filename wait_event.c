#include "mini_debugger.h"

char *signaux[] =
{
	[0] = "NONE",
	[1] = "SIGHUP",
	[2] = "SIGINT",
	[3] = "SIGQUIT",
	[4] = "SIGILL",
	[5] = "SIGTRAP",
	[6] = "SIGABRT",
	[7] = "SIGBUS",
	[8] = "SIGFPE",
	[9] = "SIGKILL",
	[10] = "SIGUSR1",
	[11] = "SIGSEGV",
	[12] = "SIGUSR2",
	[13] = "SIGPIPE",
	[14] = "SIGALRM",
	[15] = "SIGTERM",
	[16] = "SIGSTKFLT",
	[17] = "SIGCHLD",
	[18] = "SIGCONT",
	[19] = "SIGSTOP",
	[20] = "SIGTSTP",
	[21] = "SIGTTIN",
	[22] = "SIGTTOU",
	[23] = "SIGURG",
	[24] = "SIGXCPU",
	[25] = "SIGXFSZ",
	[26] = "SIGVTALRM",
	[27] = "SIGPROF",
	[28] = "SIGWINCH",
	[29] = "SIGIO",
	[30] = "SIGPWR",
	[31] = "SIGSYS"
};

void		wait_event(t_env *e)
{

	line_edition_pause(g_line);
	ptrace(PTRACE_CONT, e->child, 0, WSTOPSIG(e->status) == 5 ? 0 : WSTOPSIG(e->status));
	waitpid(e->child, &(e->status), 0);
	line_edition_cont(g_line);
	if (WIFSTOPPED(e->status) && WSTOPSIG(e->status) == SIGTRAP)
	{
		ptrace(PTRACE_GETREGS, e->child, NULL, &e->regs);
		t_node_break *curr = e->lst_break.begin;
		while(curr)
		{
			if (curr->addr + 1 == e->regs.rip)
				break;
			curr = curr->next;
		}
		e->current_break = curr;
		t_sym_info info = get_current_sym(*e, e->regs.rip);
		if (curr)
		{
			if (curr->is_enable)
			{
				unsigned long tmp = ptrace(PTRACE_PEEKDATA, e->child, e->current_break->addr, NULL);
        // printf("opcode -> 0x%lx\n", tmp);
        // printf("opcode -> 0x%x at addr -> 0x%llx\n", e->current_break->opcode, e->current_break->addr);
				tmp = tmp - (tmp & 0b11111111) + e->current_break->opcode;
        // printf("opcode -> 0x%lx\n", tmp);
				ptrace(PTRACE_POKEDATA, e->child, e->current_break->addr, tmp);
				e->regs.rip--;
				ptrace(PTRACE_SETREGS, e->child, NULL, &e->regs);
				curr->nb_hit++;
				if (curr->disp == KEEP)
					printf("Breakpoint %d, 0x%.16llx in %s()\n", curr->num + 1, e->regs.rip - 1, info.name); 
				if(curr->disp == DISPOSE)
					printf("Breakpoint %d, 0x%.16llx in %s()\n", curr->num + 1, e->regs.rip - 1, info.name); 
				else if (curr->disp == DELETE)
					printf("Temporary breakpoint %d, 0x%.16llx in %s()\n", curr->num + 1, e->regs.rip - 1, info.name); 
			}
			else
				wait_event(e);
		}
		else
		{
			printf("Program received signal %s\n", signaux[WSTOPSIG(e->status)]);
			printf("0x%.16llx in %s()\n", e->regs.rip, info.name);
		}

	}
	else if (WIFEXITED(e->status))
	{
		e->is_running = 0;
		printf("[process %d exited normally]\n", e->child);
	}
	else if (WIFSTOPPED(e->status))
	{
		if (ptrace(PTRACE_GETREGS, e->child, NULL, &e->regs) < 0)
			perror("ptrace");
		t_sym_info info = get_current_sym(*e, e->regs.rip);
		printf("Program received signal %s\n", signaux[WSTOPSIG(e->status)]);
		printf("0x%.16llx in %s()\n", e->regs.rip, info.name);
	}
	else if (WIFSIGNALED(e->status))
	{
		e->is_running = 0;
		printf("Program received signal %s\n", signaux[WTERMSIG(e->status)]);
		printf("The program no longer exists.\n");
	}
}