#include "mini_debugger.h"
#include <stdio.h>

void	mini_debugger(char **args)
{
	t_env 	e;
	char	*line;

	e.is_running = 0;
	if (*args)
	{
		while(!line_edition_get_line(&line))
		{
			printf("line --> %s\n", line);
		}
	}
}