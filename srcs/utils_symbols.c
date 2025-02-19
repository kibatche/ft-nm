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