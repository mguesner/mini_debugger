#include "mini_debugger.h"

static void		do_command_info(t_env *e, char **args)
{
	unsigned long	i = 0;
	// printf("do_command\n");
	while(i < ARRAY_SIZE_INFO)
	{
		// printf("line:%s <?> command:%s\n", args[0], commands[i].command_name);
		if (!strcmp(args[1], commands_info[i].command_name))
		{
			commands_info[i].command(e, args);
			break;
		}
		i++;
	}
}

void	db_info(t_env *e, char **args)
{
	(void)e;
	(void)args;
	if (args[1])
	{
		do_command_info(e, args);
	}
	else
	{
		printf("List of info subcommands:\n\n\
info address -- Describe where symbol SYM is stored\n\
info breakpoints -- Status of specified breakpoints (all user-settable breakpoints if no argument)\n\
info functions -- All function names\n\
info symbol -- Describe what symbol is at location ADDR\n");
	}
}