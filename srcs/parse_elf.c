#include "ft_nm.h"

extern void *ptr;
extern struct stat buf;

int check_current_machine_endianess()
{
    int i = 1;
    char *p = (char *)&i;

    if (p[0] == 1)
        return L_ENDIAN;
    else
        return B_ENDIAN;
}

int is_valid_elf_file(ELF_datas_64 *elf_datas_64, ELF_datas_32 *elf_datas_32)
{
    unsigned char *tmp = (unsigned char *)ptr;// on cast en unsigned tant qu'on ne sait si 32 ou 64 bits

    if (strncmp(ELFMAG, (const char *)tmp, SELFMAG) != 0)// est-ce que les 4 premiers bytes == \177ELF ?
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] != ELFCLASS64 && tmp[EI_CLASS] != ELFCLASS32)
        return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);// ni 64 ni 32 bits
    switch (tmp[EI_CLASS])
    {
        case ELFCLASS64:
            if (convert_and_fill_to_right_endianess_64(elf_datas_64, tmp[EI_DATA]) == ERROR)
            return ERROR;
            if (elf_datas_64->header_size != 0x40)
                return print_err(0, BAD_HEADER_SIZE);
            else if (elf_datas_64->type == ET_CORE)// nb : on ne gère pas le type core
                return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            // else if ((elf_datas_64->offset_section_table_64 + (elf_datas_64->size_of_entry_section_table_64 * elf_datas_64->nb_of_entries_section_table_64)) != (unsigned long)buf.st_size)
            //     if ((elf_datas_64->offset_program_table_64 + (elf_datas_64->size_of_entry_program_table_64 * elf_datas_64->nb_of_entries_program_table_64)) != (unsigned long)buf.st_size)
            //         return print_err(0, SIZE_OF_THE_FILE_MISMATCH);// offset de la section hdr sect tab + taille de la section hdr tab n'est pas égal à la taille du fichier. 
            return 64;
        case ELFCLASS32:
            if (convert_and_fill_to_right_endianess_32(elf_datas_32, tmp[EI_DATA]) == ERROR)
                    return ERROR;
            if (elf_datas_32->header_size != 0x34)
                return print_err(0, BAD_HEADER_SIZE);
            else if (elf_datas_32->type == ET_CORE)
                return print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            // else if ((elf_datas_32->offset_section_table_32 + (elf_datas_32->size_of_entry_section_table_32 * elf_datas_32->nb_of_entries_section_table_32)) != buf.st_size)
            //     return print_err(0, SIZE_OF_THE_FILE_MISMATCH);
            return 32;
        default:
            return print_err(0, NO_SYMBOL);
    }
}