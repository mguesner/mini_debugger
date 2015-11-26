#ifndef MINI_DEBUGGER_H
#define MINI_DEBUGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <line_edition.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>

#define ARRAY_SIZE 4

typedef struct s_env t_env;

typedef struct
{
	char						*command_name;
	void						(*command)(t_env *e);
}								t_command;

typedef struct					s_sym
{
	long						addr;
	char						*name;
}								t_sym;

struct							s_env
{
	char						*file_name;
	int							is_running;
	int							child;
	struct user_regs_struct		regs;
	t_sym						*sym_tab;
	// t_lst_break					lst_break;
	// t_node_break				*current_break;
	int							size_sym_tab;
};

t_edit_line *g_line;

//commands
void	db_run(t_env *e);
void	db_quit(t_env *e);

extern const t_command commands[];

void	mini_debugger(char **args);
void	init_debugger(t_env *e);
void	load_symbol(t_env *e);

#endif