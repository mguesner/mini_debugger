#ifndef MINI_DEBUGGER_H
#define MINI_DEBUGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <line_edition.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/wait.h>

#define ARRAY_SIZE 18
#define ARRAY_SIZE_INFO 3
#define ARRAY_SIZE_ENABLE 4

typedef struct s_env t_env;

typedef struct
{
	char						*command_name;
	void						(*command)(t_env *e, char **args);
}								t_command;

typedef struct					s_sym
{
	long						addr;
	char						*name;
}								t_sym;

typedef enum
{
	KEEP,
	DISPOSE,
	DELETE,
}								t_bp_disp;

typedef enum
{
	SYMBOL,
	ADDRESS,
}								t_b_type;

typedef struct					s_node_break
{
	unsigned int				num;
	unsigned char				opcode;							
	unsigned long long			addr;
	t_b_type					type;
	char						*sym;
	int							is_pending;
	int							is_enable;
	t_bp_disp					disp;
	int							nb_hit;
	int							nb_disable;
	struct s_node_break			*next;
}								t_node_break;

typedef struct					s_lst_break
{
	t_node_break				*begin;
	t_node_break				*end;
	int							size;
	int							num_bp;
}								t_lst_break;

struct							s_env
{
	char						*file_name;
	char						**args;
	int							is_running;
	int							child;
	struct user_regs_struct		regs;
	t_sym						*sym_tab;
	int							status;
	t_lst_break					lst_break;
	t_node_break				*current_break;
	int							size_sym_tab;
};

typedef struct
{
	int							addr_off;
	char						*name;
}								t_sym_info;

t_edit_line *g_line;

//commands
extern const t_command commands[];
void		db_run(t_env *e, char **args);
void		db_quit(t_env *e, char **args);
void		db_info(t_env *e, char **args);
void		db_cont(t_env *e, char **args);
void		db_backtrace(t_env *e, char **args);
void		db_break(t_env *e, char **args);
void		db_enable(t_env *e, char **args);
void		db_delete(t_env *e, char **args);
void		db_disable(t_env *e, char **args);
void		db_file(t_env *e, char **args);

//commands info
extern const t_command commands_info[];
void		function_info(t_env *e, char **args);
void		breakpoint_info(t_env *e, char **args);
void		address_info(t_env *e, char **args);

//commands enable
extern const t_command commands_enable[];
void		enable_count(t_env *e, char **args);
void		enable_breakpoint(t_env *e, char **args);
void		enable_once(t_env *e, char **args);
void		enable_delete(t_env *e, char **args);

void		mini_debugger(char **args);
void		init_debugger(t_env *e);
void		load_symbol(t_env *e);
char		**explode(char *str);
void		wait_event(t_env *e);
t_sym_info	get_current_sym(t_env e, long addr);
void		delete_node(t_lst_break *lst, t_node_break *node);

#endif