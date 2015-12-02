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
	int		i = 0;
	long	addr = 0;
	if (!args[1])
	{
		if (!(addr = e->regs.rip))
		{
			printf("No default breakpoint address now.\n");
			return ;
		}
	}
	else if (args[1][0] == '*')
		addr = strtol(args[1], NULL, 16);
	else
	{
		while (i < e->size_sym_tab)
		{
			if (e->sym_tab[i].name && !strcmp(e->sym_tab[i].name, args[1]))
			{
				addr = e->sym_tab[i].addr + 4;
				break;
			}
			i++;
		}
		if (i == e->size_sym_tab)
		{
			printf("Function \"%s\" not defined.\n", args[1]);
			return;
		}
	}
	t_node_break	*node;
	node = malloc(sizeof(t_node_break));
	node->num = e->lst_break.num_bp;
	node->addr = addr;
	node->is_enable = 1;
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
	printf("breakpoint %d at 0x%llx\n", node->num + 1, node->addr);
}