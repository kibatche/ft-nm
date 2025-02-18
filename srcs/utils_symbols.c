# include "ft_nm.h"

Symbol_entry *new_symbol()
{
    Symbol_entry *sym = malloc(sizeof(Symbol_entry));
    if (sym == NULL)
        return NULL;
    memset(sym->sym_name, 48, 17);
    sym->sym_rdx = 0;
    sym->sym_type = 0;
    sym->next = NULL;
    return sym;
}