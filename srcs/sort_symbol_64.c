#include "ft_nm.h"

void sort_symbol_64(ELF_datas_64 *elf_datas)
{
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
            int size = curr->size;
            curr->addr = min->addr;
            curr->letter = min->letter;
            curr->name = min->name;
            curr->size = min->size;
            min->addr = tmp_addr;
            min->letter = tmp_letter;
            min->name = tmp_name;
            min->size = size;
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
                res = coll_cmp(curr->name, ptr->name);
                if (is_pos(res) && (!min || coll_cmp(min->name, ptr->name) > 0))
                    min = ptr;
                else if (res == 0)
                {
                    res = sym_letter_cmp(curr->letter, ptr->letter);
                    if (is_pos(res) && (!min || sym_letter_cmp(min->letter, ptr->letter) > 0))
                        min = ptr;
                    else if (res == 0)
                    {
                        res = addr_cmp(curr->addr, ptr->addr);
                        if (is_pos(res) && (!min || addr_cmp(min->addr, ptr->addr) > 0))
                            min = ptr;
                        else if (res == 0)
                        {
                            res = curr->size - ptr->size;
                            if (is_pos(res) && (!min || is_pos(min->size - ptr->size)))
                                min = ptr;
                        }
                    }
                }
            }
            ptr = ptr->next;
        }
        if (min != NULL)
        {
            char *tmp_addr = curr->addr;
            char tmp_letter = curr->letter;
            char *tmp_name = curr->name;
            int size = curr->size;
            curr->addr = min->addr;
            curr->letter = min->letter;
            curr->name = min->name;
            curr->size = min->size;
            min->addr = tmp_addr;
            min->letter = tmp_letter;
            min->name = tmp_name;
            min->size = size;
        }
        curr = curr->next;
    }
}
