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

/*Fonction principale de ft_nm qui détermine la représentation du symbole analysé en fonction de diverses infos.*/
char parse_symbol_letter_64(ELF_datas_64 *elf_datas, Elf64_Sym *symbol_to_parse)
{
    if (symbol_to_parse == NULL || get_section_by_idx_64(elf_datas, symbol_to_parse->st_shndx) == NULL)
        return UNKNOWN;// '?' => ne devrait jamais arriver dans ces conditons avec nm sans options.
    else if (get_section_by_idx_64(elf_datas, symbol_to_parse->st_shndx)->sh_type == SHN_ABS)
        return ABSOLUTE;//'A'
    if (strcmp(get_section_name_by_idx_64(elf_datas, symbol_to_parse->st_shndx), '.bss'))
    {
        if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return BSS_GLOB;// 'B'
        return BSS_LOC;// 'b'
    }
    if (strcmp(get_section_name_by_idx_64(elf_datas, symbol_to_parse->st_shndx), '.data'))
    {
        if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return DATA_GLOB;// 'D'
        else
            return DATA_LOC;// 'd'
    }
    if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL \
        && ELF64_ST_TYPE(symbol_to_parse->st_info) == STT_OBJECT \
        && get_section_by_idx_64(elf_datas, symbol_to_parse->st_shndx)->sh_type == SHN_COMMON)
        return COMMON_GLOB;// 'C', 'c' ne peut pas exister de nos jours car les sections.scommon sont automatiquement ajoutées à .sbss
    else if (get_section_by_idx_64(elf_datas, symbol_to_parse->st_shndx)->sh_type == SHT_NULL)
    {
        if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_WEAK)
        {
            if (ELF64_ST_TYPE(symbol_to_parse->st_info) == STT_NOTYPE)
                return WEAK_NOTYPE_SHTNULL;// 'w'
            else
                return WEAK_OBJ_SHTNULL;// 'v'
        }
        else
        {
            return UNDEFINED;// 'U' ex : __libc_start_main qui a un lien STB_GLOBAL
        }
    }
    else if (ELF64_ST_TYPE(symbol_to_parse->st_info) == STT_GNU_IFUNC)
    {
        return INDIRECT_FUN;// 'i'
    }
    else if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_WEAK)
    {
        if (ELF64_ST_TYPE(symbol_to_parse->st_info) == STT_OBJECT)
            return WEAK_OBJ;// 'V'
        else
            return WEAK_NOTYPE;// 'W' => ne peut que être ça car 'v' et 'w' ont une précédence dans le traitement (on a le cas particulier de U dedans)
    }
    else if (ELF64_ST_BIND(symbol_to_parse->st_info) == STB_GNU_UNIQUE)
    {
        return UNIQUE;// 'u'
    }
    return UNKNOWN;// '?'
}

int parse_symbols_64(ELF_datas_64 *elf_datas)
{
    int i = 0;// pas -1 car on sait que la première section est à passer. cf proch. boucle while
    int res;

    elf_datas->symbol_list = malloc(sizeof(Sym_list));
    if( elf_datas->symbol_list == NULL)
        return ERROR;
    elf_datas->symbol_list->last_sym = NULL;
    elf_datas->symbol_list->first_sym = NULL;
    /*nécessaire pour ret. une erreur descriptive plutôt que de faire un simple check dans la boucle while.*/
    if (elf_datas->symtab_hdr_64[i + 1] == NULL)
        return print_err(0, NO_SYMBOL);
    while (++i < (int)elf_datas->nb_of_symbols_64)
    {// les types section et file ne sont pas dans nm basique (sans le -a)
        if (ELF64_ST_TYPE(elf_datas->symtab_hdr_64[i].st_info) == STT_SECTION \
            || ELF64_ST_TYPE(elf_datas->symtab_hdr_64[i].st_info) == STT_FILE)
            continue;
        res = new_symbol_pushback(elf_datas->symbol_list);//  ajout d'un noeud dans la liste
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