#include "../includes/ft_nm.h"

extern void *ptr;
extern struct stat buf;

int convert_and_fill_to_MSB_64(ELF_datas_64 *elf_datas)
{
    elf_datas->offset_section_table_64 = le64toh(elf_datas->hdr_64->e_shoff);
    elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->hdr_64->e_shnum);
    elf_datas->size_of_entry_section_table_64 = le16toh(elf_datas->hdr_64->e_shentsize);
    elf_datas->type = le16toh(elf_datas->hdr_64->e_type);
    elf_datas->header_size = le16toh(elf_datas->hdr_64->e_ehsize);
    if (elf_datas->offset_section_table_64 > (unsigned int)buf.st_size)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (le16toh(elf_datas->hdr_64->e_shstrndx) > le16toh(elf_datas->hdr_64->e_shnum))
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
    Elf64_Shdr *shstrtab = &elf_datas->shdr_64[le16toh(elf_datas->hdr_64->e_shstrndx)];
    if (elf_datas->nb_of_entries_section_table_64 == 0)
    {
        if (le16toh(elf_datas->shdr_64->sh_size) != 0)
        {
            elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->hdr_64->e_shnum);
        }
        else
        {
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        }
    }
    elf_datas->shstrndx = le16toh(elf_datas->hdr_64->e_shstrndx);
    if (elf_datas->shstrndx > elf_datas->hdr_64->e_shnum)
        return print_err(0, NO_SYMBOL);
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        if (elf_datas->shstrndx == 0xffff && le16toh(elf_datas->shdr_64->sh_link) >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = le16toh(elf_datas->shdr_64->sh_link);
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    if (elf_datas->shdr_64[elf_datas->shstrndx].sh_type != SHT_STRTAB)
        return print_err(0, NO_SYMBOL);
    elf_datas->section_strtab_64 = (ptr + le64toh(elf_datas->shdr_64[elf_datas->shstrndx].sh_offset));// table des noms pour les sections
    if (elf_datas->section_strtab_64 == NULL)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    uint16_t i = 0;
    int is_length_err_detected = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *strtab_tmp = &elf_datas->shdr_64[i];
        if (strtab_tmp && shstrtab && strtab_tmp->sh_name > shstrtab->sh_size)
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        if (!is_length_err_detected && (strtab_tmp->sh_size + strtab_tmp->sh_offset > (unsigned int)buf.st_size))
        {
            is_length_err_detected = 1;
            print_err(0, "a length problem has been detected");
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[strtab_tmp->sh_name]))
                return print_err(0, NO_SYMBOL);
        }
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[le16toh(strtab_tmp->sh_name)]))
            {
                elf_datas->string_table = (ptr + le16toh(elf_datas->shdr_64[i].sh_offset));
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *symtmp = &elf_datas->shdr_64[i];
        if (symtmp && symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_64 = &elf_datas->shdr_64[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + le64toh(elf_datas->shsymtab_64->sh_offset));
    if (elf_datas->shsymtab_64->sh_entsize)
        elf_datas->nb_of_symbols_64 = le64toh(elf_datas->shsymtab_64->sh_size) / le64toh(elf_datas->shsymtab_64->sh_entsize);
    else
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    return SUCCESS;
}

int convert_and_fill_to_LSB_64(ELF_datas_64 *elf_datas)
{
    elf_datas->offset_section_table_64 = be64toh(elf_datas->hdr_64->e_shoff);
    elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->hdr_64->e_shnum);
    elf_datas->size_of_entry_section_table_64 = be16toh(elf_datas->hdr_64->e_shentsize);
    elf_datas->type = be16toh(elf_datas->hdr_64->e_type);
    elf_datas->header_size = be16toh(elf_datas->hdr_64->e_ehsize);
    if (elf_datas->offset_section_table_64 > (unsigned int)buf.st_size)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (be16toh(elf_datas->hdr_64->e_shstrndx) > be16toh(elf_datas->hdr_64->e_shnum))
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
    Elf64_Shdr *shstrtab = &elf_datas->shdr_64[be16toh(elf_datas->hdr_64->e_shstrndx)];
    if (elf_datas->nb_of_entries_section_table_64 == 0)
    {
        if (be16toh(elf_datas->shdr_64->sh_size) != 0)
        {
            elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->hdr_64->e_shnum);
        }
        else
        {
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        }
    }
    elf_datas->shstrndx = be16toh(elf_datas->hdr_64->e_shstrndx);
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        if (elf_datas->shstrndx == 0xffff && be16toh(elf_datas->shdr_64->sh_link) >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = be16toh(elf_datas->shdr_64->sh_link);
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    if (elf_datas->shdr_64[elf_datas->shstrndx].sh_type != SHT_STRTAB)
        return print_err(0, NO_SYMBOL);
    elf_datas->section_strtab_64 = (ptr + be64toh(elf_datas->shdr_64[elf_datas->shstrndx].sh_offset));// table des noms pour les sections
    if (elf_datas->section_strtab_64 == NULL)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    uint16_t i = 0;
    int is_length_err_detected = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *strtab_tmp = &elf_datas->shdr_64[i];
        if (strtab_tmp && shstrtab && strtab_tmp->sh_name > shstrtab->sh_size)
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        if (!is_length_err_detected && (strtab_tmp->sh_size + strtab_tmp->sh_offset > (unsigned int)buf.st_size))
        {
            is_length_err_detected = 1;
            print_err(0, "a length problem has been detected");
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[strtab_tmp->sh_name]))
                return print_err(0, NO_SYMBOL);
        }
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[be16toh(strtab_tmp->sh_name)]))
            {
                elf_datas->string_table = (ptr + be16toh(elf_datas->shdr_64[i].sh_offset));
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *symtmp = &elf_datas->shdr_64[i];
        if (symtmp && symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_64 = &elf_datas->shdr_64[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + be64toh(elf_datas->shsymtab_64->sh_offset));
    if (elf_datas->shsymtab_64->sh_entsize)
        elf_datas->nb_of_symbols_64 = be64toh(elf_datas->shsymtab_64->sh_size) / be64toh(elf_datas->shsymtab_64->sh_entsize);
    else
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    return SUCCESS;
}

int fill_64(ELF_datas_64 *elf_datas)
{
    elf_datas->offset_section_table_64 = elf_datas->hdr_64->e_shoff;
    elf_datas->nb_of_entries_section_table_64 = elf_datas->hdr_64->e_shnum;
    elf_datas->size_of_entry_section_table_64 = elf_datas->hdr_64->e_shentsize;
    elf_datas->type = elf_datas->hdr_64->e_type;
    elf_datas->header_size = elf_datas->hdr_64->e_ehsize;
    if (elf_datas->offset_section_table_64 > (unsigned int)buf.st_size)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if ((elf_datas->hdr_64->e_shstrndx) > (elf_datas->hdr_64->e_shnum))
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
    Elf64_Shdr *shstrtab = &elf_datas->shdr_64[elf_datas->hdr_64->e_shstrndx];
    if (elf_datas->nb_of_entries_section_table_64 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
    {
        if (elf_datas->shdr_64->sh_size != 0)
        {
            elf_datas->nb_of_entries_section_table_64 = elf_datas->hdr_64->e_shnum;
        }
        else
        {
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        }
    }
    if (elf_datas->offset_section_table_64 + (elf_datas->hdr_64->e_shnum * elf_datas->hdr_64->e_shentsize) > (unsigned long)buf.st_size)
        return print_err(0, NO_SYMBOL);
    elf_datas->shstrndx = elf_datas->hdr_64->e_shstrndx;
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        /* localisation de la string table si pas de place dans la table des sections*/
        if (elf_datas->shstrndx == 0xffff && elf_datas->shdr_64->sh_link >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = elf_datas->shdr_64->sh_link;
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    if (elf_datas->shdr_64[elf_datas->shstrndx].sh_type != SHT_STRTAB)
        return print_err(0, NO_SYMBOL);
    elf_datas->section_strtab_64 = (ptr + elf_datas->shdr_64[elf_datas->shstrndx].sh_offset);// table des noms pour les sections
    if (elf_datas->section_strtab_64 == NULL)
        return print_err(0, NO_SHSTRTAB);
    uint16_t i = 0;
    int is_length_err_detected = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *strtab_tmp = &elf_datas->shdr_64[i];
        if (strtab_tmp && shstrtab && strtab_tmp->sh_name > shstrtab->sh_size)
            return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        if (!is_length_err_detected && (strtab_tmp->sh_size + strtab_tmp->sh_offset > (unsigned int)buf.st_size))
        {
            is_length_err_detected = 1;
            print_err(0, "a length problem has been detected");
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[strtab_tmp->sh_name]))
                return print_err(0, NO_SYMBOL);
        }
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {/*Détermine si la section de type SHT_STRTAB est bien celle des noms de la table des symboles*/
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_64[strtab_tmp->sh_name]))
            {
                elf_datas->string_table = (ptr + elf_datas->shdr_64[i].sh_offset);
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_64)
    {
        Elf64_Shdr *symtmp = &elf_datas->shdr_64[i];
        if (symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_64 = &elf_datas->shdr_64[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + elf_datas->shsymtab_64->sh_offset);
    if (elf_datas->shsymtab_64->sh_entsize)
        elf_datas->nb_of_symbols_64 = elf_datas->shsymtab_64->sh_size / elf_datas->shsymtab_64->sh_entsize;
    else
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    return SUCCESS;
}

int convert_and_fill_to_right_endianess_64(ELF_datas_64 *elf_datas, int file_endianess)
{
    elf_datas->hdr_64 = (Elf64_Ehdr *)ptr;
    elf_datas->proc_bits = 64;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        switch (file_endianess)
        {
            case ELFDATA2MSB:
                return convert_and_fill_to_LSB_64(elf_datas);
            case ELFDATA2LSB:
                return convert_and_fill_to_MSB_64(elf_datas);
            default:
                return print_err(0, NO_SYMBOL);
        }
    }
    else
        return fill_64(elf_datas);
}
