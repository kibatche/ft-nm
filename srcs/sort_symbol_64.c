#include "ft_nm.h"


int alphabtical_cmp(const char *s1, const char *s2, int *tab)
{
    size_t			i;
	unsigned char	*s1_cpy;
	unsigned char	*s2_cpy;

	i = 0;
	s1_cpy = (unsigned char *)s1;
	s2_cpy = (unsigned char *)s2;
	while (s1_cpy[i] && s2_cpy[i]
		&& tab[s1_cpy[i]] == tab[s2_cpy[i]])
    {
        i++;
        printf("tab[S1[i]] = %d tab[S2[i]] = %d\n", tab[s1_cpy[i]], tab[s2_cpy[i]]);
    }
    int res = tab[s1_cpy[i]] - tab[s2_cpy[i]];
	return res;
}

int is_pos(long long i)
{
    return i > 0;
}

void sort_symbol_64(ELF_datas_64 *elf_datas)
{
    (void)elf_datas;
    // static int tab[127];
    // static int is_tab_is_filled = 0;
    // unsigned int i = 0;
    // int res;
    // char *alphabet = "0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";

    // if (!is_tab_is_filled)
    // {
    //     ft_memset(tab, -1, 127);// -1 == char pas compté dans le tri
    //     while (alphabet[i] != 0)
    //     {
    //         tab[(int)alphabet[i]] = i;
    //         i++;
    //     }
    //     is_tab_is_filled = 1;
    // }
    // Symbol *curr = elf_datas->symbol_list->first_sym;
    // while (curr)
    // {
    //     Symbol *next = curr->next;
    //     Symbol *ptr = next;
    //     Symbol *min = NULL;
    //     while (ptr)
    //     {
    //         res = alphabtical_cmp(curr->name, ptr->name, tab);
    //         if (is_pos(res))
    //             min = ptr;
    //         ptr = ptr->next;
    //     }
    //     if (min != NULL)
    //     {
    //         char *tmp_addr = curr->addr;
    //         char tmp_letter = curr->letter;
    //         char *tmp_name = curr->name;
    //         curr->addr = min->addr;
    //         curr->letter = min->letter;
    //         curr->name = min->name;
    //         min->addr = tmp_addr;
    //         min->letter = tmp_letter;
    //         min->name = tmp_name;
    //     }
    //     curr = curr->next;
    // }
}