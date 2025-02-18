#include "ft_nm.h"

void parse_symbol_address(ELF_datas_64 *elf_datas, Symbol_entry *symbol)
{

}

int parse_symbols_64(ELF_datas_64 *elf_datas, Symbol_list *symbol_list)
{
    unsigned int i = 0;
    Symbol_entry *first = new_symbol();
    if (first == NULL)
        return ERROR;
    symbol_list->first = first;
    while (i < elf_datas->nb_of_symbols_64)
    {

    }
}

int parse_symbols_32(ELF_datas_32 *elf_datas)
{
    unsigned int i = 0;

    while (i < elf)
}