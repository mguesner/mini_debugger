#ifndef MINI_DEBUGGER_H
#define MINI_DEBUGGER_H

#include <line_edition.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>

typedef struct 					s_env
{
	int							is_running;
	int							child;
	struct user_regs_struct		regs;
	// t_sym						*sym_tab;
	// t_lst_break					lst_break;
	// t_node_break				*current_break;
	int							size_sym_tab;
}								t_env;

void	mini_debugger(char **args);

#endif