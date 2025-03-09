#include "ft_nm.h"

void print_symlist(ELF_datas_64 *elf_datas)
{
    Symbol *head = elf_datas->symbol_list->first_sym;

    while (head)
    {
        printf("%s %c %s\n",head->addr, head->letter, head->name);
        head = head->next;
    }
}