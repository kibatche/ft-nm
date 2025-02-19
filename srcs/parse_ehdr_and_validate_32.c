#include "../includes/ft_nm.h"

extern void *ptr;
extern struct stat buf;

int convert_and_fill_to_MSB_32(ELF_datas_32 *elf_datas)
{
    elf_datas->offset_section_table_32 = le32toh(elf_datas->hdr_32->e_shoff);
    elf_datas->nb_of_entries_section_table_32 = le16toh(elf_datas->hdr_32->e_shnum);
    elf_datas->size_of_entry_section_table_32 = le16toh(elf_datas->hdr_32->e_shentsize);
    elf_datas->type = le16toh(elf_datas->hdr_32->e_type);
    elf_datas->header_size = le16toh(elf_datas->hdr_32->e_ehsize);
    elf_datas->shdr_32 = (Elf32_Shdr *)(ptr + elf_datas->offset_section_table_32);
    if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
    {
        if (le16toh(elf_datas->shdr_32->sh_size) != 0)
        {
            elf_datas->nb_of_entries_section_table_32 = le16toh(elf_datas->shdr_32->sh_size);
        }
        else
        {
            return print_err(0, WRONG_NB_OF_ENTRIES);
        }
    }
    elf_datas->shstrndx = le16toh(elf_datas->hdr_32->e_shstrndx);
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        if (elf_datas->shstrndx == 0xffff && le16toh(elf_datas->shdr_32->sh_link) >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = le16toh(elf_datas->shdr_32->sh_link);
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    elf_datas->section_strtab_32 = &elf_datas->shdr_32[elf_datas->shstrndx];
    if (elf_datas->section_strtab_32 == NULL)
        return print_err(0, NO_SYMBOL);
    uint16_t i = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *strtab_tmp = &elf_datas->shdr_32[i];
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_32[le16toh(strtab_tmp->sh_name)]))
            {
                elf_datas->string_table = (ptr + le16toh(elf_datas->shdr_32[i].sh_offset));
                break;
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *symtmp = &elf_datas->shdr_32[i];
        if (symtmp && symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_32 = &elf_datas->shdr_32[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + le16toh(elf_datas->shsymtab_32->sh_offset));
    if (elf_datas->shsymtab_32->sh_entsize)
        elf_datas->nb_of_symbols_32 = le32toh(elf_datas->shsymtab_32->sh_size) / le32toh(elf_datas->shsymtab_32->sh_entsize);
    else
        return print_err(0, NO_SYMBOL);
    return SUCCESS;
}

int convert_and_fill_to_LSB_32(ELF_datas_32 *elf_datas)
{
    elf_datas->offset_section_table_32 = be32toh(elf_datas->hdr_32->e_shoff);
    elf_datas->nb_of_entries_section_table_32 = be16toh(elf_datas->hdr_32->e_shnum);
    elf_datas->size_of_entry_section_table_32 = be16toh(elf_datas->hdr_32->e_shentsize);
    elf_datas->type = be16toh(elf_datas->hdr_32->e_type);
    elf_datas->header_size = be16toh(elf_datas->hdr_32->e_ehsize);
    elf_datas->shdr_32 = (Elf32_Shdr *)(ptr + elf_datas->offset_section_table_32);
    if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
    {
        if (be16toh(elf_datas->shdr_32->sh_size) != 0)
        {
            elf_datas->nb_of_entries_section_table_32 = be16toh(elf_datas->shdr_32->sh_size);
        }
        else
        {
            return print_err(0, WRONG_NB_OF_ENTRIES);
        }
    }
    elf_datas->shstrndx = be16toh(elf_datas->hdr_32->e_shstrndx);
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        if (elf_datas->shstrndx == 0xffff && be16toh(elf_datas->shdr_32->sh_link) >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = be16toh(elf_datas->shdr_32->sh_link);
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    elf_datas->section_strtab_32 = (Elf32_Shdr *)&elf_datas->shdr_32[elf_datas->shstrndx];
    if (elf_datas->section_strtab_32 == NULL)
        return print_err(0, NO_SYMBOL);
    uint16_t i = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *strtab_tmp = &elf_datas->shdr_32[i];
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_32[be16toh(strtab_tmp->sh_name)]))
            {
                elf_datas->string_table = (ptr + be16toh(elf_datas->shdr_32[i].sh_offset));
                break;
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *symtmp = &elf_datas->shdr_32[i];
        if (symtmp && symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_32 = &elf_datas->shdr_32[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + be16toh(elf_datas->shsymtab_32->sh_offset));
    if (elf_datas->shsymtab_32->sh_entsize)
        elf_datas->nb_of_symbols_32 = be32toh(elf_datas->shsymtab_32->sh_size) / be32toh(elf_datas->shsymtab_32->sh_entsize);
    else
        return print_err(0, NO_SYMBOL);
    return SUCCESS;
}

int fill_32(ELF_datas_32 *elf_datas)
{
    elf_datas->offset_section_table_32 = elf_datas->hdr_32->e_shoff;
    elf_datas->nb_of_entries_section_table_32 = elf_datas->hdr_32->e_shnum;
    elf_datas->size_of_entry_section_table_32 = elf_datas->hdr_32->e_shentsize;
    elf_datas->type = elf_datas->type;
    elf_datas->header_size = elf_datas->hdr_32->e_ehsize;
    elf_datas->shdr_32 = (Elf32_Shdr *)(ptr + elf_datas->offset_section_table_32);
    if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
    {
        if (elf_datas->shdr_32->sh_size != 0)
        {
            elf_datas->nb_of_entries_section_table_32 = elf_datas->shdr_32->sh_size;
        }
        else
        {
            return print_err(0, WRONG_NB_OF_ENTRIES);
        }
    }
    elf_datas->shstrndx = elf_datas->hdr_32->e_shstrndx;
    if (elf_datas->shstrndx >= SHN_LORESERVE)
    {
        if (elf_datas->shstrndx == 0xffff && elf_datas->shdr_32->sh_link >= SHN_LORESERVE)
        {
            elf_datas->shstrndx = elf_datas->shdr_32->sh_link;
        }
        else
        {
            return print_err(0, BAD_INDEX_FOR_STRING_TABLE);
        }
    }
    elf_datas->section_strtab_32 = &elf_datas->shdr_32[elf_datas->shstrndx];
    if (elf_datas->section_strtab_32 == NULL)
        return print_err(0, NO_SYMBOL);
    uint16_t i = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *strtab_tmp = &elf_datas->shdr_32[i];
        if (strtab_tmp->sh_type == SHT_STRTAB)
        {
            if (!ft_strcmp(".strtab", (const char *)&elf_datas->section_strtab_32[strtab_tmp->sh_name]))
            {
                elf_datas->string_table = (ptr + elf_datas->shdr_32[i].sh_offset);
                break;
            }
        }
        i++;
    }
    i = 0;
    unsigned int is_there_shsymtab = 0;
    while (i < elf_datas->nb_of_entries_section_table_32)
    {
        Elf32_Shdr *symtmp = &elf_datas->shdr_32[i];
        if (symtmp && symtmp->sh_type == SHT_SYMTAB)
        {
            is_there_shsymtab = 1;
            elf_datas->shsymtab_32 = &elf_datas->shdr_32[i];
            break;
        }
        i++;
    }
    if (is_there_shsymtab == 0)
        return print_err(0, NO_SYMBOL);
    elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + elf_datas->shsymtab_32->sh_offset);
    if (elf_datas->shsymtab_32->sh_entsize)
        elf_datas->nb_of_symbols_32 = elf_datas->shsymtab_32->sh_size / elf_datas->shsymtab_32->sh_entsize;
    else
        return print_err(0, NO_SYMBOL);
    return SUCCESS;
}

int convert_and_fill_to_right_endianess_32(ELF_datas_32 *elf_datas, int file_endianess)
{
    elf_datas->hdr_32 = (Elf32_Ehdr *)ptr;
    elf_datas->proc_bits = 32;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        switch (file_endianess)
        {
            case ELFDATA2MSB:
                return convert_and_fill_to_LSB_32(elf_datas);
            case ELFDATA2LSB:
                return convert_and_fill_to_MSB_32(elf_datas);
            default:
                return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
        }
    }
    else
        return fill_32(elf_datas);
}
