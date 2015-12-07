#include "mini_debugger.h"
#include <errno.h>
#include <signal.h>

static void	insert_breakpoint(t_env *e)
{
	t_node_break *curr = e->lst_break.begin;
	while(curr)
	{
		unsigned long tmp = ptrace(PTRACE_PEEKDATA, e->child, curr->addr, NULL);
		// printf("opcode -> 0x%lx\n", tmp);
		curr->opcode = tmp & 0b11111111;
		// printf("opcode -> 0x%x at addr -> 0x%llx\n", curr->opcode, curr->addr);
		tmp = tmp - (tmp & 0b11111111) + 0xcc;
		// printf("opcode -> 0x%lx\n", tmp);
		ptrace(PTRACE_POKEDATA, e->child, curr->addr, tmp);
		curr = curr->next;
	}
}

static int	wait_event_startup(t_env *e, int *status)
{
	ptrace(PTRACE_CONT, e->child, 0, 0);
	waitpid(e->child, status, 0);
	if (WIFEXITED(*status) || WIFSIGNALED(*status))
	{
		printf("During startup program exited with code %d.\n", WEXITSTATUS(*status));
        return -1;
	}
	return 0;
}

static void do_father(t_env *e)
{
	// long tmp;
	waitpid(e->child, &e->status, WUNTRACED);
	if (!WIFSTOPPED(e->status))
		exit(-1);
	ptrace(PTRACE_SEIZE, e->child, 0, 0);
	ptrace(PTRACE_SETOPTIONS, e->child, 0, PTRACE_O_TRACEEXEC);


	if (wait_event_startup(e, &e->status) < 0)
		return ;
	e->is_running = 1;
	insert_breakpoint(e);
	wait_event(e);
}

static void	do_child(t_env *e, char **args)
{
	if (setpgid(0, 0) < 0)
		perror("pgid");
	raise(SIGSTOP);
	free(args[0]);
	args[0] = e->exec_name;
	printf("Starting program: %s\n\n", e->exec_name);
	execvp(e->exec_name, args);
	perror(e->exec_name);
    _exit(126);
}

void	db_run(t_env *e, char **args)
{
	if (!e->file_name)
	{
		printf("No executable file specified.\n\
Use the \"file\" command.\n");
		return ;
	}
	if (e->is_running)
	{
		while(!line_edition_getsubline(g_line, "The program being debugged has been started already.\n\
Start it from the beginning? (y or n) "))
		{
			if (!strcmp("n", g_line->line))
				return ;
			else if (!strcmp("y", g_line->line))
			{
				kill(e->child, SIGKILL);
				waitpid(e->child, &(e->status), 0);
				break ;
			}
			else
				printf("Please answer y or n.\n");
		}
	}
	e->child = fork();
	if (e->child < 0)
    {
        perror("fork");
		return ;
    }
    else if (!e->child)
        do_child(e, args);
	else
        do_father(e);
}