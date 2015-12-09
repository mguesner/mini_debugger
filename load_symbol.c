#include <elf.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "mini_debugger.h"

static void	swap(t_sym *tab1, t_sym *tab2)
{
	long tmp1;
	char	*tmp2;
	tmp1 = tab1->addr;
	tmp2 = tab1->name;
	tab1->addr = tab2->addr;
	tab1->name = tab2->name;
	tab2->addr = tmp1;
	tab2->name = tmp2;
}

static void	sort_array(t_sym *array, int begin, int end)
{
	if (end <= begin)
		return;
	long pivot = (array + begin)->addr;
	
	int l = begin;
	int r = end;
	while (1) 
	{
		if (l < end && (array + l)->addr < pivot)
			l++;
		else if (r >= begin && (array + r)->addr > pivot)
			r--;
		else if (l < r)
		{
			swap(array + l, array + r);
			l++;
			r--;
		}
		else
			break;
	}
	sort_array(array, begin, r);
	sort_array(array, r + 1, end);
}

static int	elf_check(Elf64_Ehdr *hdr)
{
	if(!hdr || hdr->e_ident[EI_MAG0] != ELFMAG0 || hdr->e_ident[EI_MAG1] != ELFMAG1
		|| hdr->e_ident[EI_MAG2] != ELFMAG2 || hdr->e_ident[EI_MAG3] != ELFMAG3)
		return 0;
	return 1;
}

static void	*get_file_fd(t_env *e)
{
	int		i = 0;
	char	*path = NULL;
	int 	fd;
	struct stat	buf;
	if ((fd = open(e->file_name, O_RDONLY)) >= 0 && !fstat(fd, &buf) && S_ISREG(buf.st_mode))
	{
		if ((unsigned long)buf.st_size < sizeof(Elf64_Ehdr))
		{
			printf("%s: not in executable format: File truncated\n", e->file_name);
			return NULL;
		}
		char *pwd = getcwd(NULL, 0);
		e->exec_name = malloc(strlen(e->file_name) + strlen(pwd) + 2);
		strcpy(e->exec_name, pwd);
		strcat(e->exec_name, "/");
		strcat(e->exec_name, e->file_name);
		free(pwd);
		void *tmp = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
		printf("%p\n", tmp);
		close(fd);
		printf("%p\n", tmp);
		return tmp;
	}
	while (e->env[i])
	{
		if (!strncmp(e->env[i], "PATH=", 5))
		{
			path = strdup(e->env[i] + 5);
			break ;
		}
		i++;
	}
	char *part = strtok(path, ":");
	while (part)
	{
		printf("%s\n", part);
		part = strtok(NULL, ":");
		e->exec_name = malloc(strlen(e->file_name) + strlen(part) + 2);
		printf("%s\n", e->exec_name);
		strcpy(e->exec_name, part);
		strcat(e->exec_name, "/");
		strcat(e->exec_name, e->file_name);
		if ((fd = open(e->exec_name, O_RDONLY)) >= 0 && !fstat(fd, &buf) && S_ISREG(buf.st_mode))
		{
			void *tmp = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
			free(path);
			close(fd);
			return tmp;
		}
		free(e->exec_name);
		e->exec_name = NULL;
		close(fd);
	}
	free(path);
	printf("%s: Aucun fichier ou dossier de ce type.\n", e->file_name);
	return NULL;
}

void	load_symbol(t_env *e)
{
	void *file_start = get_file_fd(e);
	if (!file_start)
		return ;
	/*struct stat	buf;
	if ((fd = open(e->file_name, O_RDONLY)) < 0)
	{
		printf("%s: Aucun fichier ou dossier de ce type.\n", e->file_name);
		return ;
	}
	fstat(fd, &buf);
	if (!S_ISREG(buf.st_mode))
	{
		printf("%s: Aucun fichier ou dossier de ce type.\n", e->file_name);
		return ;
	}*/
	// void *file_start = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	// if ((unsigned long)buf.st_size < sizeof(Elf64_Ehdr))
	// {
	// 	printf("%s: not in executable format: File truncated\n", e->file_name);
	// 	return ;
	// }
	Elf64_Ehdr *hdr = (Elf64_Ehdr*)file_start;
	if (!elf_check(hdr))
	{
		printf("%s: Format de fichier non reconnu.\n", e->file_name);
		return ;
	}
	int off = hdr->e_shoff;
	Elf64_Shdr *section_start = (Elf64_Shdr*)(file_start + off);
	char *strtab = file_start + (section_start + hdr->e_shstrndx)->sh_offset;
	Elf64_Sym *sym ;
	t_sym *reloc_array;
	int sym_size = 0;
	char *sym_str_tbl = NULL;
	unsigned long size;
	for(int i = 0; i < hdr->e_shnum; i++)
	{
		Elf64_Shdr *section = section_start + i;
		if (!strcmp(strtab + section->sh_name, ".dynstr"))
		{
			printf("new tab\n");
			write(1, file_start + section->sh_offset, section->sh_size);
			write(1, "\n", 1);
			sym_str_tbl = (char *)(file_start + section->sh_offset);
			unsigned int i = 0;
			while (i < section->sh_size)
			{
				int ret = printf("%s", sym_str_tbl + i);
				printf(" at %d -> %d\n", i, ret);
				i += (ret + 1);
			}
		}
		else if (!strcmp(strtab + section->sh_name, ".dynsym"))
		{
			// printf("symtab -> %d\n", i);
			sym = (Elf64_Sym *)(file_start + section->sh_offset);
			sym_size = section->sh_size;
		}
		else if (section->sh_type == SHT_RELA && !strcmp(strtab + section->sh_name, ".rela.plt"))
		{
			size = section->sh_size / section->sh_entsize;
			// printf("%s\n", strtab + section->sh_name);
			reloc_array = malloc(size * sizeof(t_sym));
			for(unsigned long idx = 0; idx < size; idx++)
			{

				Elf64_Rela *reltab = (Elf64_Rela *)(file_start + section->sh_offset) + idx;
				Elf64_Shdr *plt_section = section_start + section->sh_info;
				// char *addr = (file_start + plt_section->sh_offset);
				// int *ref = (int *)(addr + reltab->r_offset);

				Elf64_Shdr *dynsym_section = section_start + section->sh_link;
				// printf("%p, %p\n", addr, ref);
				// printf("off -> %lx, info -> %lx, section %d -> %s, section %d -> %s\n", reltab->r_offset, ELF64_R_SYM(reltab->r_info)
				// 	, section->sh_info ,strtab + plt_section->sh_name
					// 	, section->sh_link ,strtab + dynsym_section->sh_name);
				Elf64_Sym *dynsym = (Elf64_Sym *)(file_start + dynsym_section->sh_offset) + ELF64_R_SYM(reltab->r_info);
				// printf("Name : %s, Addr : %lx\n", sym_str_tbl + dynsym->st_name, dynsym->st_value);
				reloc_array[idx].addr = plt_section->sh_addr + (ELF64_R_SYM(reltab->r_info) * plt_section->sh_entsize);
				reloc_array[idx].name = sym_str_tbl + dynsym->st_name;
			}
			// for(unsigned long idx = 0; idx < size; idx++)
			// 	printf("Name: %s, Addr: 0x%lx\n",reloc_array[idx].name ,reloc_array[idx].addr );
		}
		// }
	}
	Elf64_Sym *curr_sym = sym;
	printf("size = %lu\n", sym_size / sizeof(Elf64_Sym));
	e->sym_tab = malloc(sym_size / sizeof(Elf64_Sym) * sizeof(t_sym));
	int i = 0;
	printf("%s\n", sym_str_tbl + 1);
	while((void *)curr_sym < ((void *)sym) + sym_size)
	{
		// printf("%s at %d\n", sym_str_tbl + curr_sym->st_name, curr_sym->st_name);
		if (ELF64_ST_TYPE(curr_sym->st_info) == STT_FUNC)
		{
			e->sym_tab[i].addr = curr_sym->st_value;
			e->sym_tab[i].name = strdup(sym_str_tbl + curr_sym->st_name);
			for(unsigned long idx = 0; idx < size; idx++)
			{
				if (!strncmp(sym_str_tbl + curr_sym->st_name, reloc_array[idx].name, strlen(reloc_array[idx].name)))
				{
					free(e->sym_tab[i].name);
					e->sym_tab[i].addr = reloc_array[idx].addr;
					e->sym_tab[i].name = malloc(strlen(reloc_array[idx].name) + 5);
					strcpy(e->sym_tab[i].name, reloc_array[idx].name);
					strcat(e->sym_tab[i].name, "@plt");
				}		
			}
			// printf("#%d\tOffset: %x\tName: %s, Addr: 0x%lx, Bind:%d\n", i, (int)curr_sym - (int)file_start
			// 	, e->sym_tab[i].name , e->sym_tab[i].addr
			// 	, curr_sym->st_shndx);
			i++;
		}
		curr_sym = curr_sym + 1;
	}
	e->size_sym_tab = i;
	free(reloc_array);
	sort_array(e->sym_tab, 0, e->size_sym_tab - 1);
	// i = 0;
	// while (i < e->size_sym_tab)
	// {
	// 	printf("Name: %s, Addr: 0x%lx\n", e->sym_tab[i].name , e->sym_tab[i].addr );
	// 	i++;
	// }
}