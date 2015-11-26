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
	if (end > begin) {
      long pivot = (array + begin)->addr;
      int l = begin + 1;
      int r = end;
      while(l < r) {
         if ((array + l)->addr <= pivot)
            l += 1;
         else if ((array + r)->addr > pivot)
            r -= 1;
         else if ( l < r )
            swap(array+l, array+r);
      }
      l -= 1;
      swap(array+begin, array+l);
      sort_array(array, begin, l);
      sort_array(array, r, end);
   }
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
	Elf64_Sym *sym ;
	int sym_size;
	char *sym_str_tbl;
	for(int i = 0; i < hdr->e_shnum; i++)
	{
		Elf64_Shdr *section = (Elf64_Shdr*)(file_start + off);
		if (section->sh_type == SHT_STRTAB)
		{
			sym_str_tbl = (char *)(file_start + section->sh_offset);
		}
		if (section->sh_type == SHT_SYMTAB)
		{
			sym = (Elf64_Sym *)(file_start + section->sh_offset);
			sym_size = section->sh_size;
		}
		if (section->sh_type == SHT_RELA)
		{
			for(unsigned long idx = 0; idx < section->sh_size / section->sh_entsize; idx++)
			{
				Elf64_Rela *reltab = (Elf64_Rela *)(file_start + section->sh_offset) + idx;
				Elf64_Shdr *target = section_start + section->sh_info;
				char *addr = (file_start + target->sh_offset);
				int *ref = (int *)(addr + reltab->r_offset);

				Elf64_Shdr *target2 = section_start + section->sh_link;

				printf("%p, %p\n", addr, ref);
				printf("off -> %lx, section -> %d, section -> %d\n", reltab->r_offset, section->sh_info, section->sh_link);
			}
		}
		off += sizeof(Elf64_Shdr);
	}
	Elf64_Sym *curr_sym = sym;
	e->sym_tab = malloc(sym_size / sizeof(Elf64_Sym) * sizeof(t_sym));
	int i = 0;
	while((void *)curr_sym < ((void *)sym) + sym_size)
	{
		if (ELF64_ST_TYPE(curr_sym->st_info) == STT_FUNC)
		{
			e->sym_tab[i].addr = curr_sym->st_value;
			e->sym_tab[i].name = sym_str_tbl + curr_sym->st_name;
			printf("Name: %s, Addr: 0x%lx, Bind:%d\n", e->sym_tab[i].name , e->sym_tab[i].addr
				, curr_sym->st_shndx);
			i++;
		}
		curr_sym = curr_sym + 1;
	}
	e->size_sym_tab = i;
	sort_array(e->sym_tab, 0, e->size_sym_tab);
	// i = 0;
	// while (i < e->size_sym_tab)
	// {
	// 	printf("Name: %s, Addr: 0x%lx\n", e->sym_tab[i].name , e->sym_tab[i].addr );
	// 	i++;
	// }
}