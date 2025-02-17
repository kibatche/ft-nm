#include "../includes/ft_nm.h"

extern struct stat buf;
extern void *ptr;

int check_current_machine_endianess()
{
    int i = 1;
    char *p = (char *)&i;

    if (p[0] == 1)
        return L_ENDIAN;
    else
        return B_ENDIAN;
}

void convert_and_fill_to_right_endianess_64(ELF_datas *elf_datas, int file_endianess)
{
    elf_datas->hdr_64 = (Elf64_Ehdr *)ptr;
    elf_datas->proc_bits = 64;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        if (file_endianess == ELFDATA2MSB)
        {
            elf_datas->offset_section_table_64 = be64toh(elf_datas->hdr_64->e_shoff);
            elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->hdr_64->e_shnum);
            elf_datas->size_of_entry_section_table_64 = be16toh(elf_datas->hdr_64->e_shentsize);
            elf_datas->type = be16toh(elf_datas->hdr_64->e_type);
            elf_datas->header_size = be16toh(elf_datas->hdr_64->e_ehsize);
            elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
            if (elf_datas->nb_of_entries_section_table_64 == 0)
            {
                if (be16toh(elf_datas->shdr_64->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->shdr_64->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
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
                    print_err(0, BAD_INDEX_FOR_STRING_TABLE);
                }
            }
            elf_datas->shstrtab_64 = &elf_datas->shdr_64[elf_datas->shstrndx];
            if (elf_datas->shstrtab_64 == NULL)
                print_err(0, NO_SYMBOL);
            elf_datas->string_table = (ptr + be16toh(elf_datas->shstrtab_64->sh_offset));
            uint16_t i = 0;
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
                print_err(0, NO_SYMBOL);
            elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + be64toh(elf_datas->shsymtab_64->sh_offset));
        }
        else if (file_endianess == ELFDATA2LSB)
        {
            elf_datas->offset_section_table_64 = le64toh(elf_datas->hdr_64->e_shoff);
            elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->hdr_64->e_shnum);
            elf_datas->size_of_entry_section_table_64 = le16toh(elf_datas->hdr_64->e_shentsize);
            elf_datas->type = le16toh(elf_datas->hdr_64->e_type);
            elf_datas->header_size = le16toh(elf_datas->hdr_64->e_ehsize);
            elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
            if (elf_datas->nb_of_entries_section_table_64 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
            {
                if (le16toh(elf_datas->shdr_64->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->shdr_64->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
                }
            }
            elf_datas->shstrndx = le16toh(elf_datas->hdr_64->e_shstrndx);
            if (elf_datas->shstrndx >= SHN_LORESERVE)
            {
                if (elf_datas->shstrndx == 0xffff && le16toh(elf_datas->shdr_64->sh_link) >= SHN_LORESERVE)
                {
                    elf_datas->shstrndx = le16toh(elf_datas->shdr_64->sh_link);
                }
                else
                {
                    print_err(0, BAD_INDEX_FOR_STRING_TABLE);
                }
            }
            elf_datas->shstrtab_64 = &elf_datas->shdr_64[elf_datas->shstrndx];
            if (elf_datas->shstrtab_64 == NULL)
                print_err(0, NO_SYMBOL);
            elf_datas->string_table = (ptr + le16toh(elf_datas->shstrtab_64->sh_offset));
            uint16_t i = 0;
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
                print_err(0, NO_SYMBOL);
            elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + le64toh(elf_datas->shsymtab_64->sh_offset));
        }
    }
    else
    {
        elf_datas->offset_section_table_64 = elf_datas->hdr_64->e_shoff;
        elf_datas->nb_of_entries_section_table_64 = elf_datas->hdr_64->e_shnum;
        elf_datas->size_of_entry_section_table_64 = elf_datas->hdr_64->e_shentsize;
        elf_datas->type = elf_datas->hdr_64->e_type;
        elf_datas->header_size = elf_datas->hdr_64->e_ehsize;
        elf_datas->shdr_64 = (Elf64_Shdr *)(ptr + elf_datas->offset_section_table_64);
        if (elf_datas->nb_of_entries_section_table_64 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
        {
            if (elf_datas->shdr_64->sh_size != 0)
            {
                elf_datas->nb_of_entries_section_table_64 = elf_datas->shdr_64->sh_size;
            }
            else
            {
                print_err(0, WRONG_NB_OF_ENTRIES);
            }
        }
        elf_datas->shstrndx = elf_datas->hdr_64->e_shstrndx;
        if (elf_datas->shstrndx >= SHN_LORESERVE)
        {
            if (elf_datas->shstrndx == 0xffff && elf_datas->shdr_64->sh_link >= SHN_LORESERVE)
            {
                elf_datas->shstrndx = elf_datas->shdr_64->sh_link;
            }
            else
            {
                print_err(0, BAD_INDEX_FOR_STRING_TABLE);
            }
        }
        elf_datas->shstrtab_64 = &elf_datas->shdr_64[elf_datas->shstrndx];
        if (elf_datas->shstrtab_64 == NULL)
            print_err(0, NO_SHSTRTAB);
        elf_datas->string_table = (ptr + elf_datas->shstrtab_64->sh_offset);
        uint16_t i = 0;
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
            print_err(0, NO_SYMBOL);
        elf_datas->symtab_hdr_64 = (Elf64_Sym *)(ptr + elf_datas->shsymtab_64->sh_offset);
        if (elf_datas->shsymtab_64->sh_entsize)
            elf_datas->nb_of_symbols_64 = elf_datas->shsymtab_64->sh_size / elf_datas->shsymtab_64->sh_entsize;
        uint64_t idx = 0;
        while (idx < elf_datas->nb_of_symbols_64)
        {
            // Elf64_Sym *tmp = elf_datas->symtab_hdr_64;
        //     if (tmp && tmp->st_name != 0)
        //     {
        //         elf_datas->symtab_entry_hdr = malloc(sizeof(Symtab_entry));
        //         elf_datas->symtab_entry_hdr->next = NULL;
        //         elf_datas->symtab_entry_hdr->sym_rdx = 0;
        //         elf_datas->symtab_entry_hdr->sym_type = 0;
        //     }
        }
    }
}

void convert_and_fill_to_right_endianess_32(ELF_datas *elf_datas, int file_endianess)
{
    elf_datas->hdr_32 = (Elf32_Ehdr *)ptr;
    elf_datas->proc_bits = 32;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        if (file_endianess == ELFDATA2MSB)
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
                    print_err(0, WRONG_NB_OF_ENTRIES);
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
                    print_err(0, BAD_INDEX_FOR_STRING_TABLE);
                }
            }
            elf_datas->shstrtab_32 = (Elf32_Shdr *)&elf_datas->shdr_32[elf_datas->shstrndx];
            if (elf_datas->shstrtab_32 == NULL)
                print_err(0, NO_SYMBOL);
            elf_datas->string_table = (ptr + be16toh(elf_datas->shstrtab_32->sh_offset));
            uint16_t i = 0;
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
                print_err(0, NO_SYMBOL);
            elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + be16toh(elf_datas->shsymtab_32->sh_offset));
        }
        else if (file_endianess == ELFDATA2LSB)
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
                    print_err(0, WRONG_NB_OF_ENTRIES);
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
                    print_err(0, BAD_INDEX_FOR_STRING_TABLE);
                }
            }
            elf_datas->shstrtab_32 = &elf_datas->shdr_32[elf_datas->shstrndx];
            if (elf_datas->shstrtab_32 == NULL)
                print_err(0, NO_SYMBOL);
            elf_datas->string_table = (ptr + le16toh(elf_datas->shstrtab_32->sh_offset));
            uint16_t i = 0;
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
                print_err(0, NO_SYMBOL);
            elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + le16toh(elf_datas->shsymtab_32->sh_offset));
        }
    }
    else
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
                print_err(0, WRONG_NB_OF_ENTRIES);
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
                print_err(0, BAD_INDEX_FOR_STRING_TABLE);
            }
        }
        elf_datas->shstrtab_32 = &elf_datas->shdr_32[elf_datas->shstrndx];
        if (elf_datas->shstrtab_32 == NULL)
            print_err(0, NO_SYMBOL);
        elf_datas->string_table = (ptr + elf_datas->shstrtab_32->sh_offset);
        uint16_t i = 0;
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
            print_err(0, NO_SYMBOL);
        elf_datas->symtab_hdr_32 = (Elf32_Sym *)(ptr + elf_datas->shsymtab_32->sh_offset);
    }
}

void is_valid_elf_file(ELF_datas *elf_datas)
{
    unsigned char *tmp = (unsigned char *)ptr;// on cast en unsigned tant qu'on ne sait si 32 ou 64 bits

    if (strncmp(ELFMAG, (const char *)tmp, SELFMAG) != 0)// est-ce que les 4 premiers bytes == \177ELF ?
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] != ELFCLASS64 && tmp[EI_CLASS] != ELFCLASS32)
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);// ni 64 ni 32 bits
    else if (tmp[EI_DATA] != ELFDATA2LSB \
        && tmp[EI_DATA] != ELFDATA2MSB)// ni LSB ni MSB
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] == ELFCLASS64)// cast dans la bonne struct
        convert_and_fill_to_right_endianess_64(elf_datas, tmp[EI_DATA]);
    else
        convert_and_fill_to_right_endianess_32(elf_datas, tmp[EI_DATA]);
    switch (elf_datas->proc_bits)
    {
        case 64:
            if (elf_datas->header_size != 0x40)
                print_err(0, BAD_HEADER_SIZE);
            else if (elf_datas->type == ET_CORE)// nb : on ne gère pas le type core
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            else if ((elf_datas->offset_section_table_64 + (elf_datas->size_of_entry_section_table_64 * elf_datas->nb_of_entries_section_table_64)) != (unsigned long)buf.st_size)
                print_err(0, SIZE_OF_THE_FILE_MISMATCH);// offset de la section hdr sect tab + taille de la section hdr tab n'est pas égal à la taille du fichier. 
            break;
        case 32 :
            if (elf_datas->header_size != 0x34)
                print_err(0, BAD_HEADER_SIZE);
            if (elf_datas->type == ET_CORE)
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            else if ((elf_datas->offset_section_table_32 + (elf_datas->size_of_entry_section_table_32 * elf_datas->nb_of_entries_section_table_32)) != buf.st_size)
                print_err(0, SIZE_OF_THE_FILE_MISMATCH);
            break;
        default:
            print_err(0, UNKNOWN_ERR);
    }
}