#include "mini_debugger.h"

void	db_quit(t_env *e, char **args)
{
	(void)args;
	if (e->is_running)
	{
		printf("A debugging session is active.\n\
\n\
	Inferior 1 [process %d] will be killed.\n\
\n\
\n", e->child);
		while(!line_edition_getsubline(g_line, "Quit anyway? (y or n) "))
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
				printf("Please answer y or n.\n\
A debugging session is active.\n\
\n\
	Inferior 1 [process %d] will be killed.\n\
\n\
\n", e->child);
		}
	}
	exit(0);
}