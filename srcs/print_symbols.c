#include "ft_nm.h"

void print_symlist(Sym_list *symbol_list)
{
    Symbol *head = symbol_list->first_sym;

    while (head)
    {
        write(1, head->addr, ft_strlen(head->addr));
        write(1, " ", 1);
        write(1, &head->letter, 1);
        write(1, " ", 1);
        write(1, head->name, ft_strlen(head->name));
        write(1, "\n", 1);
        head = head->next;
    }
}
