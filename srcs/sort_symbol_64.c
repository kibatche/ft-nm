#include "ft_nm.h"


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
        while (s1_cpy[i] && (s1_cpy[i] == '_' || s1_cpy[i] == '(' || s1_cpy[i] == ')' || s1_cpy[i] == '*'))
            i++;
        while (s2_cpy[j] && (s2_cpy[j] == '_' || s2_cpy[j] == '(' || s2_cpy[j] == ')' || s2_cpy[j] == '*'))
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

static int addr_cmp(const char *s1, const char *s2)
{
    unsigned long int addr1 = ft_atoi_base(s1, 16);
    unsigned long int addr2 = ft_atoi_base(s2, 16);
    return (addr1 - addr2);
}

static int sym_letter_cmp(const char c1, const char c2)
{
    return c1 - c2;
}

int is_pos(long long int i)
{
    return i > 0;
}

void sort_symbol_64(ELF_datas_64 *elf_datas)
{
    (void)elf_datas;
    int res;

    Symbol *curr = elf_datas->symbol_list->first_sym;
    while (curr)
    {
        Symbol *ptr = curr->next;
        Symbol *min = NULL;
        while (ptr)
        {
            res = alphabtical_cmp(curr->name, ptr->name);
            if (is_pos(res))
            {
                if (!min || is_pos(alphabtical_cmp(min->name, ptr->name)))
                   min = ptr;
            }
            ptr = ptr->next;
        }
        if (min != NULL)
        {
            char *tmp_addr = curr->addr;
            char tmp_letter = curr->letter;
            char *tmp_name = curr->name;
            curr->addr = min->addr;
            curr->letter = min->letter;
            curr->name = min->name;
            min->addr = tmp_addr;
            min->letter = tmp_letter;
            min->name = tmp_name;
        }
        curr = curr->next;
    }
    curr = elf_datas->symbol_list->first_sym;
    while (curr)
    {
        Symbol *ptr = curr->next;
        Symbol *min = NULL;
        while (ptr)
        {
            res = alphabtical_cmp(curr->name, ptr->name);
            if (res == 0)
            {
                res = addr_cmp(curr->addr, ptr->addr);
                if (is_pos(res))
                    min = ptr;
                else if (res == 0)
                {
                    res = sym_letter_cmp(curr->letter, ptr->letter);
                    if (is_pos(res))
                        min = ptr;
                }
            }
            ptr = ptr->next;
        }
        if (min != NULL)
        {
            char *tmp_addr = curr->addr;
            char tmp_letter = curr->letter;
            char *tmp_name = curr->name;
            curr->addr = min->addr;
            curr->letter = min->letter;
            curr->name = min->name;
            min->addr = tmp_addr;
            min->letter = tmp_letter;
            min->name = tmp_name;
        }
        curr = curr->next;
    }
}
