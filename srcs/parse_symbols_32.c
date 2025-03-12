#include "ft_nm.h"

char *parse_symbol_address_32(Elf32_Sym *symbol_to_parse)
{
    if (symbol_to_parse->st_value == 0 && symbol_to_parse->st_size == 0)
        return ft_strdup("        ");
    char *tmp = ft_itoa_base(symbol_to_parse->st_value, 16);
    int len_addr = ft_strlen(tmp);
    int start_idx;
    int i = 0;

    if (len_addr > 8) // l"adresse fait plus de 16 caractères => erreur
    {
        REEF(tmp);
        return NULL;
    }
    char *addr = malloc(sizeof(char) * 9);
    if (addr == NULL)
    {
        REEF(tmp);
        return NULL;
    }
    start_idx = 8 - len_addr;
    ft_memset(addr, '0', 8);
    addr[8] = 0;
    while ((start_idx + i) < 8)
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
char parse_symbol_letter_32(ELF_datas_32 *elf_datas, Elf32_Sym *symbol_to_parse)
{
    const char *section_name = (const char *)get_section_name_by_idx_32(elf_datas, symbol_to_parse->st_shndx);
    Elf32_Shdr *curr_section = get_section_by_idx_32(elf_datas, symbol_to_parse->st_shndx);
    if (symbol_to_parse == NULL || elf_datas->shdr_32 == NULL || section_name == NULL)
        return UNKNOWN;// "?" => ne devrait jamais arriver dans ces conditons avec nm sans options.
    else if (curr_section && curr_section->sh_type == SHN_ABS)
        return ABSOLUTE;//"A"
    if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL \
    && ELF32_ST_TYPE(symbol_to_parse->st_info) == STT_OBJECT \
    && (symbol_to_parse->st_shndx == SHN_COMMON || curr_section->sh_type == SHN_COMMON))
        return COMMON_GLOB;// "C", "c" ne peut pas exister de nos jours car les sections.scommon sont automatiquement ajoutées à .sbss
    if (curr_section && curr_section->sh_type == SHT_NULL)
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_WEAK)
        {
            if (ELF32_ST_TYPE(symbol_to_parse->st_info) != STT_OBJECT)
                return WEAK_NOTYPE_SHTNULL;// "w"
            else
                return WEAK_OBJ_SHTNULL;// "v"
        }
        else
        {
            return UNDEFINED;// "U" ex : __libc_start_main qui a un lien STB_GLOBAL
        }
    }
    if (ELF32_ST_TYPE(symbol_to_parse->st_info) == STT_GNU_IFUNC)
        return INDIRECT_FUN;// "i"
    if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_WEAK)
    {
        if (ELF32_ST_TYPE(symbol_to_parse->st_info) == STT_OBJECT)
            return WEAK_OBJ;// "V"
        else
            return WEAK_NOTYPE;// "W" => ne peut que être ça car "v" et "w" ont une précédence dans le traitement (on a le cas particulier de U dedans)
    }
    if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GNU_UNIQUE)
        return UNIQUE;// "u"
    if (!strcmp(section_name, ".data") \
        || !strcmp(section_name, ".fini_array") \
        || !strcmp(section_name, ".init_array") \
        || !strncmp(section_name, ".got", 4) \
        || !strcmp(section_name, ".dynamic") \
        || (curr_section->sh_type == SHT_PROGBITS \
        && (curr_section->sh_flags & SHF_ALLOC)
        && (curr_section->sh_flags &  SHF_WRITE)))//man elf.h
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return DATA_GLOB;// "D"
        else
            return DATA_LOC;// "d"
    }
    if (!strcmp(section_name, ".bss") || \
        (curr_section->sh_type == SHT_NOBITS \
        && (curr_section->sh_flags & SHF_ALLOC) \
        && (curr_section->sh_flags & SHF_WRITE)))//man elf.h
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return BSS_GLOB;// "B"
        else
            return BSS_LOC;// "b"
    }
    if (!strcmp(section_name, ".sbss"))
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return SMALL_BSS_GLOB;// "S"
        else
            return SMALL_BSS_LOC;// "s"
    }
    if (!strcmp(section_name, ".sdata"))
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return SDATA_GLOB;// "G"
        else
            return SDATA_LOC;// "g"
    }
    if (!strcmp(section_name, ".text") \
        || !strcmp(section_name, ".fini") \
        || !strcmp(section_name, ".init") \
        || (curr_section && (curr_section->sh_type == SHT_PROGBITS) \
        && (curr_section->sh_flags & SHF_ALLOC) \
        &&  (curr_section->sh_flags & SHF_EXECINSTR)))//man elf.h
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return TEXT_GLOB;// "T"
        else
            return TEXT_LOC;// "t"
    }
    if (!strcmp(section_name, ".rodata") \
        || !strcmp(section_name, ".rodata1") \
        || curr_section->sh_type == SHT_NOTE \
        || !strncmp(section_name, ".eh_frame", 9) \
        || (curr_section && (curr_section->sh_type == SHT_PROGBITS) \
        && (curr_section->sh_flags & SHF_ALLOC) \
        &&  !(curr_section->sh_flags & SHF_WRITE)))//man elf.h
    {
        if (ELF32_ST_BIND(symbol_to_parse->st_info) == STB_GLOBAL)
            return READ_ONLY_GLOB;// "R"
        else
            return READ_ONLY_LOC;// "r"
    }
    return UNKNOWN;// "?"
}

int parse_symbols_32(ELF_datas_32 *elf_datas)
{
    int i = 0;// pas -1 car on sait que la première section est à passer. cf proch. boucle while
    int res;
    int nbsym = 0;

    elf_datas->symbol_list = malloc(sizeof(Sym_list));
    if( elf_datas->symbol_list == NULL)
        return ERROR;
    elf_datas->symbol_list->last_sym = NULL;
    elf_datas->symbol_list->first_sym = NULL;
    /*nécessaire pour ret. une erreur descriptive plutôt que de faire un simple check dans la boucle while.*/
    if (elf_datas->shdr_32 == NULL || &elf_datas->symtab_hdr_32[i + 1] == NULL)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    while (++i < (int)elf_datas->nb_of_symbols_32)
    {
        /*les types STT_SECTION et STT_FILE ne sont pas dans nm sans option (sans le -a)*/
        if (ELF32_ST_TYPE(elf_datas->symtab_hdr_32[i].st_info) == STT_SECTION \
        || ELF32_ST_TYPE(elf_datas->symtab_hdr_32[i].st_info) == STT_FILE)
            continue;
        /*nm sans options n"affiche pas les symboles de debug, mais ils sont valides pour autant :
        on les vire dès maintenant afin de ne pas les traiter dans la suite des fonctions*/
        char *section_name = get_section_name_by_idx_32(elf_datas, elf_datas->symtab_hdr_32[i].st_shndx);
        if (section_name && !ft_strncmp(section_name, ".debug", 6))
            continue;
        // if (strcmp(&elf_datas->string_table[elf_datas->symtab_hdr_32[i].st_name], "_cgo_9c8efe9babca_C2func_getaddrinfo"))
        //     continue;
        res = new_symbol_pushback(elf_datas->symbol_list);
        if (res == ERROR)
            return ERROR;
        if (&elf_datas->string_table[elf_datas->symtab_hdr_32[i].st_name])
            elf_datas->symbol_list->last_sym->name = ft_strdup((char *)(&elf_datas->string_table[elf_datas->symtab_hdr_32[i].st_name]));
        else
            elf_datas->symbol_list->last_sym->name = ft_strdup("");
        if (elf_datas->symbol_list->last_sym->name == NULL)
            return ERROR;
        elf_datas->symbol_list->last_sym->letter = parse_symbol_letter_32(elf_datas, &elf_datas->symtab_hdr_32[i]);
        if (elf_datas->symbol_list->last_sym->letter == 0)
            return ERROR;
        if (elf_datas->symbol_list->last_sym->letter == 'U')//règle à la noix...
            elf_datas->symbol_list->last_sym->addr = ft_strdup("        ");
        else
            elf_datas->symbol_list->last_sym->addr = parse_symbol_address_32(&elf_datas->symtab_hdr_32[i]);
        if (elf_datas->symbol_list->last_sym->addr == NULL)
            return ERROR;
        elf_datas->symbol_list->last_sym->size = elf_datas->symtab_hdr_32[i].st_size;
        nbsym++;
    }
    return SUCCESS;
}
