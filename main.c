#include "mini_debugger.h"

const t_command commands[] = {
		{"r", db_run},
		{"run", db_run},
		{"q", db_quit},
		{"quit", db_quit},
		{"i", db_info},
		{"info", db_info},
		{"c", db_cont},
		{"continue", db_cont},
		{"bt", db_backtrace},
		{"backtrace", db_backtrace},
		{"b", db_break},
		{"break", db_break},
		{"tb", db_break},
		{"tbreak", db_break},
		{"enable", db_enable}
	};

const t_command commands_info[] = {
		{"functions", function_info},
		{"breakpoint", breakpoint_info}
	};

const t_command commands_enable[] = {
		{"count", enable_count},
		{"breakpoint", enable_breakpoint}
	};

int main(int argc, char **argv)
{
	(void) argc;
	mini_debugger(argv + 1);
	return (0);
}