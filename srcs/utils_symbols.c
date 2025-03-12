# include "ft_nm.h"


int alphabtical_cmp(const char *s1, const char *s2)
{
    size_t          i;
    size_t          j;
	unsigned char	*s1_cpy;
	unsigned char	*s2_cpy;

    i = 0;
    j = 0;
    s1_cpy = (unsigned char *)s1;
    s2_cpy = (unsigned char *)s2;
    while (s1_cpy[i] && s2_cpy[j])
    {
        while (s1_cpy[i] && (!ft_isalnum(s1_cpy[i]) && s1_cpy[i] != '$'))// oui oui, les dollars comptent...
            i++;
        while (s2_cpy[j] && (!ft_isalnum(s2_cpy[j]) && s2_cpy[j] != '$'))
            j++;
        if (s1_cpy[i] && s2_cpy[j] && ft_tolower(s1_cpy[i]) == ft_tolower(s2_cpy[j]))
        {
            i++;
            j++;
        }
        else
        {
            break;
        }
    }
    int res = ft_tolower(s1_cpy[i]) - ft_tolower(s2_cpy[j]);
	return res;
}

int coll_cmp(const char *s1, const char *s2)
{
    size_t          i;
    size_t          j;
	unsigned char	*s1_cpy;
	unsigned char	*s2_cpy;
    static int tab[127];
    static int is_tab_is_filled = 0;
    char *char_array = "0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";
    i = 0;
    if (is_tab_is_filled == 0)
    {
        ft_memset(tab, 256, 127);
        size_t len = (size_t)ft_strlen(char_array);
        while (++i < len)
            tab[(int)char_array[i]] = i;
        is_tab_is_filled = 1;
    }
    i = 0;
    j = 0;
    s1_cpy = (unsigned char *)s1;
    s2_cpy = (unsigned char *)s2;
    while (s1_cpy[i] && s2_cpy[j])
    {
        while (s1_cpy[i] && (s1_cpy[i] == '_'))
            i++;
        while (s2_cpy[j] && (s2_cpy[j] == '_'))
            j++;
        if (s1_cpy[i] && s2_cpy[j] && tab[(int)s1_cpy[i]] == tab[(int)s2_cpy[j]])
        {
            i++;
            j++;
        }
        else
        {
            break;
        }
    }
    int res = tab[(int)s1_cpy[i]] - tab[(int)s2_cpy[j]];
	return res;
}

int addr_cmp(const char *s1, const char *s2)
{
    unsigned long int addr1 = ft_atoi_base(s1, 16);
    unsigned long int addr2 = ft_atoi_base(s2, 16);
    return (addr1 - addr2);
}

int sym_letter_cmp(const char c1, const char c2)
{
    static int tab[127];
    static int is_tab_is_filled = 0;
    char *char_array = "aAcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTbBuUvVwWxXyYzZ";
    size_t i = 0;

    if (is_tab_is_filled == 0)
    {
        ft_memset(tab, 256, 127);
        size_t len = ft_strlen(char_array);
        while (++i < len)
            tab[(int)char_array[i]] = i;
        is_tab_is_filled = 1;
    }
    return tab[(int)c1] - tab[(int)c2];
}

int is_pos(long long int i)
{
    return i > 0;
}

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
	if (elf_datas->shdr_64 && idx > elf_datas->nb_of_entries_section_table_64)
		return NULL;
	return &elf_datas->shdr_64[idx];
}

char *get_section_name_by_idx_64(ELF_datas_64 *elf_datas, unsigned int idx)
{
	Elf64_Shdr *to_return = NULL;

	if (elf_datas->shdr_64 && idx < elf_datas->nb_of_entries_section_table_64)
	{
		to_return = &elf_datas->shdr_64[idx];
	}
	else if (idx == SHN_COMMON)// idx == 65522 => veut dire que la symbole est attente d'une section par le linker
		return "";
	else
		return NULL;
	return &elf_datas->section_strtab_64[to_return->sh_name];
}

Elf32_Shdr *get_section_by_name_32(ELF_datas_32 *elf_datas, const char *name)
{
	unsigned int i = 0;

	while (name && i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *to_return = &elf_datas->shdr_32[i];
		if (!ft_strcmp(name, (const char *)&elf_datas->section_strtab_32[to_return->sh_name]))
			return to_return;
        i++;
    }
	return NULL;
}

Elf32_Shdr *get_section_by_idx_32(ELF_datas_32 *elf_datas, unsigned int idx)
{
	if (elf_datas->shdr_32 && idx > elf_datas->nb_of_entries_section_table_32)
		return NULL;
	return &elf_datas->shdr_32[idx];
}

char *get_section_name_by_idx_32(ELF_datas_32 *elf_datas, unsigned int idx)
{
	Elf32_Shdr *to_return = NULL;

	if (elf_datas->shdr_32 && idx < elf_datas->nb_of_entries_section_table_32)
	{
		to_return = &elf_datas->shdr_32[idx];
	}
	else if (idx == SHN_COMMON)// idx == 65522 => veut dire que la symbole est attente d'une section par le linker
		return "";
	else
		return NULL;
	return &elf_datas->section_strtab_32[to_return->sh_name];
}
