#include "../includes/ft_nm.h"

extern void *ptr;

void fill_string_and_sym_struct(ELF_datas *elf_datas)
{
    unsigned char *tmp = (elf_datas->proc_bits == 64 ? ((unsigned char *)ptr + elf_datas->offset_section_table_64) : ((unsigned char *)ptr + elf_datas->offset_section_table_32));
    elf_datas.
}
