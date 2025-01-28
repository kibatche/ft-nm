#ifndef FT_NM_H
# define FT_NM_H
# include <elf.h>


typedef struct ELF_datas
{
    Elf64_Ehdr *hdr_64;/* header pour les elf 64 bits */
    Elf32_Ehdr *hdr_32;/* header pour les elf 32 bits */
    int         proc_bits;
} ELF_datas;
#endif