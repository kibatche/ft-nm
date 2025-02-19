#include "ft_nm.h"

char *parse_symbol_address_64(Elf64_Sym *symbol_to_parse)
{
    if (symbol_to_parse->st_value == 0 && symbol_to_parse->st_size == 0)
        return ft_strdup("");
    char *tmp = ft_itoa_base(symbol_to_parse->st_value, 16);
    int len_addr = ft_strlen(tmp);
    int start_idx;
    int i = 0;

    if (len_addr > 16) // l'adresse fait plus de 16 caractères => erreur
    {
        REEF(tmp);
        return NULL;
    }
    char *addr = malloc(sizeof(char) * 17);
    if (addr == NULL)
    {
        REEF(tmp);
        return NULL;
    }
    start_idx = 16 - len_addr;
    ft_memset(addr, '0', 16);
    addr[16] = 0;
    while ((start_idx + i) < 16)
    {
        if (tmp[i] >= 'A' && tmp[i] <= 'Z')// on met en minuscule car nm met les adresses en minuscule
            addr[start_idx + i] = tmp[i] + 32;
        else
            addr[start_idx + i] = tmp[i];
        i++;
    }
    REEF(tmp);
    return addr;
}

char parse_symbol_letter_64(Elf64_Sym *symbol_to_parse)
{
    (void)symbol_to_parse;
    return 0;
}

int parse_symbols_64(ELF_datas_64 *elf_datas)
{
    int i = 0;
    int res;

    elf_datas->symbol_list = malloc(sizeof(Sym_list));
    if( elf_datas->symbol_list == NULL)
        return ERROR;
    elf_datas->symbol_list->last_sym = NULL;
    elf_datas->symbol_list->first_sym = NULL;
    while (++i < (int)elf_datas->nb_of_symbols_64)
    {// les types section et file ne sont pas dans nm basique (sans le -a)
        if (ELF64_ST_TYPE(elf_datas->symtab_hdr_64[i].st_info) == STT_SECTION \
            || ELF64_ST_TYPE(elf_datas->symtab_hdr_64[i].st_info) == STT_FILE)
            continue;
        res = new_symbol_pushback(elf_datas->symbol_list);
        if (res == ERROR)
            return ERROR;
        elf_datas->symbol_list->last_sym->addr = parse_symbol_address_64(&elf_datas->symtab_hdr_64[i]);
        if (elf_datas->symbol_list->last_sym->addr == NULL)
            return ERROR;
        Elf64_Sym t = elf_datas->symtab_hdr_64[i];
        (void) t;
        elf_datas->symbol_list->last_sym->name = ft_strdup((char *)(&elf_datas->string_table[elf_datas->symtab_hdr_64[i].st_name]));
        if (elf_datas->symbol_list->last_sym->name == NULL)
            return ERROR;
        elf_datas->symbol_list->last_sym->letter = parse_symbol_letter_64(&elf_datas->symtab_hdr_64[i]);
        if (elf_datas->symbol_list->last_sym->letter == 0)
            return ERROR;
    }
    return SUCCESS;
}

// int parse_symbols_32(ELF_datas_32 *elf_datas)
// {
//     unsigned int i = 0;

//     while (i < elf)
// }