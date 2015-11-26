#include "mini_debugger.h"

static void		do_command(t_env *e, char *line)
{
	unsigned long	i = 0;

	// printf("do_command\n");
	while(i < ARRAY_SIZE)
	{
		// printf("line:%s <?> command:%s\n", line, commands[i].command_name);
		if (!strcmp(line, commands[i].command_name))
		{
			commands[i].command(e);
			break;
		}
		i++;
	}
}

static void		db_exit()
{
	printf("EXIT BAM\n");
	line_editor_end(g_line);
}

void			mini_debugger(char **args)
{
	t_env 	e;
	g_line = line_editor_init();
	atexit(db_exit);

	init_debugger(&e);
	if (*args)
	{
		e.file_name = *args;
		load_symbol(&e);
	}
	while(!line_edition_get_line(g_line))
		do_command(&e, g_line->line);
	
}