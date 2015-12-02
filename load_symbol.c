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

void	load_symbol(t_env *e)
{
	int fd;
	struct stat buf;
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
	}
	void *file_start = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
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
	int sym_size;
	char *sym_str_tbl;
	unsigned long size;
	for(int i = 0; i < hdr->e_shnum; i++)
	{
		Elf64_Shdr *section = section_start + i;
		if (section->sh_type == SHT_STRTAB)
		{
			sym_str_tbl = (char *)(file_start + section->sh_offset);
		}
		else if (section->sh_type == SHT_SYMTAB)
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
	e->sym_tab = malloc(sym_size / sizeof(Elf64_Sym) * sizeof(t_sym));
	int i = 0;
	while((void *)curr_sym < ((void *)sym) + sym_size)
	{
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