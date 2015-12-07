#include "mini_debugger.h"

static void	add_node(t_lst_break *lst, t_node_break *node)
{
	if (!lst->size)
	{
		lst->begin = node;
		lst->end = node;
		lst->size = 1;
	}
	else
	{
		lst->end->next = node;
		lst->end = node;
		lst->size++;
	}
	lst->num_bp++;
}

void	db_break(t_env *e, char **args)
{
	int			i = 0;
	long		addr = 0;
	int			is_pending = 0;
	t_b_type	type = 0;

	if (!args[1])
	{
		if (!(addr = e->regs.rip))
		{
			printf("No default breakpoint address now.\n");
			return ;
		}
	}
	else if (args[1][0] == '*')
	{
		addr = strtol(args[1] + 1, NULL, 16);
		type = ADDRESS;
		is_pending = 0;
	}
	else
	{
		type = SYMBOL;
		while (i < e->size_sym_tab)
		{
			if (e->sym_tab[i].name && !strcmp(e->sym_tab[i].name, args[1]))
			{
				addr = e->sym_tab[i].addr + 4;
				is_pending = 0;
				break;
			}
			i++;
		}
		if (i == e->size_sym_tab)
		{
			printf("Function \"%s\" not defined.\n", args[1]);
			while(!line_edition_getsubline(g_line, "Make breakpoint pending on future shared library load? (y or [n]) "))
			{
				if (!strcmp("n", g_line->line))
					return ;
				else if (!strcmp("y", g_line->line))
				{
					is_pending = 1;
					break ;
				}
				else
					printf("Please answer y or n.\n");
			}
		}
	}
	t_node_break	*node;
	node = malloc(sizeof(t_node_break));
	node->num = e->lst_break.num_bp;
	node->addr = addr;
	node->type = type;
	node->is_enable = 1;
	if (type == SYMBOL)
		node->sym = strdup(args[1]);
	else
		node->sym = NULL;
	node->is_pending = is_pending;
	node->nb_hit = 0;
	node->disp = args[0][0] == 't' ? DELETE : KEEP;
	node->next = NULL;
	if (e->is_running)
	{
		long tmp = ptrace(PTRACE_PEEKDATA, e->child, addr, NULL);
		printf("opcode -> 0x%lx\n", tmp);
		node->opcode = tmp & 0b11111111;
		tmp = tmp - (tmp & 0b11111111) + 0xcc;
		printf("opcode -> 0x%lx\n", tmp);
		ptrace(PTRACE_POKEDATA, e->child, addr, tmp);
	}
	add_node(&e->lst_break, node);
	if (is_pending)
		printf("breakpoint %d (%s) pending.\n", node->num + 1, node->sym);
	else
		printf("breakpoint %d at 0x%llx\n", node->num + 1, node->addr);
}