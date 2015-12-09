#include "mini_debugger.h"

void	handle_int(int sig)
{
	(void)sig;
	printf("SIGINT\n");
}

void	set_signal()
{
	struct sigaction	sa;

	sa.sa_handler = &handle_int;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
}