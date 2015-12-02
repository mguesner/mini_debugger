#include "mini_debugger.h"
#include <errno.h>

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
	kill(getpid(), SIGSTOP);
	char *pwd = getcwd(NULL, 0);
	char *name = malloc(strlen(pwd) + strlen(e->file_name) + 2);
	strcpy(name, pwd);
	strcat(name, "/");
	strcat(name, e->file_name);
	free(args[0]);
	free(pwd);
	args[0] = name;
	printf("Starting program: %s\n\n", name);
	execvp(name, args);
	perror(name);
    _exit(126);
}

void	db_run(t_env *e, char **args)
{
	(void)args;
	e->child = fork();
	if (e->child < 0)
    {
        // int error = errno;
        perror("fork");
		return ;
    }
    else if (!e->child)
        do_child(e, args);
	else
        do_father(e);
}