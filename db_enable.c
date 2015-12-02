#include "mini_debugger.h"

static void		do_command_enable(t_env *e, char **args)
{
	unsigned long	i = 0;
	// printf("do_command\n");
	while(i < ARRAY_SIZE_ENABLE)
	{
		// printf("line:%s <?> command:%s\n", args[0], commands[i].command_name);
		if (!strcmp(args[1], commands_enable[i].command_name))
		{
			commands_enable[i].command(e, args);
			break;
		}
		i++;
	}
}

void	db_enable(t_env *e, char **args)
{
	if (args[1])
		do_command_enable(e, args);
}