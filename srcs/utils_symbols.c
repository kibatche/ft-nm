# include "ft_nm.h"

int new_symbol_pushback(Sym_list *symbol_lst)
{
    Symbol *sym;

	sym = malloc(sizeof(Symbol));
	if (sym == NULL)
		return ERROR;
    sym->addr = NULL;
    sym->name = NULL;// nom du symbole (s'il existe)
    sym->letter = 0;// lettre du symbole
    sym->prev = symbol_lst->last_sym;
    sym->next = NULL;
	if (symbol_lst->last_sym)
		symbol_lst->last_sym->next = sym;
	else
		symbol_lst->first_sym = sym;
	symbol_lst->last_sym = sym;
    return SUCCESS;
}

void	ft_clear_lst(Sym_list *symbol_lst)
{
	Symbol	*temp;
	Symbol	*curr;

	if (symbol_lst)
		curr = symbol_lst->first_sym;
	else
		return;
	while (curr)
	{
		temp = curr;
		curr = curr->next;
        REEF(temp->addr);
        REEF(temp->name);
		REEF(temp);
	}
	symbol_lst->first_sym = NULL;
	symbol_lst->last_sym = NULL;
    REEF(symbol_lst);
}

Elf64_Shdr *get_section_by_name_64(ELF_datas_64 *elf_datas, const char *name)
{
	unsigned int i = 0;

	while (name && i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *to_return = &elf_datas->shdr_64[i];
		if (!ft_strcmp(name, (const char *)&elf_datas->section_strtab_64[to_return->sh_name]))
			return to_return;
        i++;
    }
	return NULL;
}

Elf64_Shdr *get_section_by_idx_64(ELF_datas_64 *elf_datas, unsigned int idx)
{
	if (idx > elf_datas->nb_of_entries_section_table_64)
		return NULL;
	return &elf_datas->shdr_64[idx];
}

char *get_section_name_by_idx_64(ELF_datas_64 *elf_datas, unsigned int idx)
{
	if (idx < elf_datas->nb_of_entries_section_table_64)
		Elf64_Shdr *to_return = &elf_datas->shdr_64[idx];
	else
		return NULL;
	return &elf_datas->section_strtab_64[to_return->sh_name];
}

Elf32_Shdr *get_section_by_name_32(ELF_datas_32 *elf_datas, const char *name)
{
	unsigned int i = 0;

	while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *to_return = &elf_datas->shdr_32[i];
		if (!ft_strcmp(name, (const char *)&elf_datas->section_strtab_32[to_return->sh_name]))
			return to_return;
        i++;
    }
}

char *get_name_by_idx_32(ELF_datas_32 *elf_datas, unsigned int idx)
{
	Elf32_Shdr *to_return = &elf_datas->shdr_32[idx];
	return &elf_datas->section_strtab_32[to_return->sh_name];
}