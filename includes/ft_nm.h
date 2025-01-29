#ifndef FT_NM_H
# define FT_NM_H
# include <elf.h>


typedef struct ELF_datas
{
    Elf64_Ehdr *hdr_64;/* elf header pour les elf 64 bits */
    Elf64_Shdr *shdr_64;/* section header pour les elf 64 bits */
    Elf32_Ehdr *hdr_32;/* elf header pour les elf 32 bits */
    Elf32_Shdr *shdr_32;/* elf header pour les elf 32 bits */
    int         proc_bits;
    int         current_machine_endianess;
    uint16_t    type;/*LSB ou MSB*/
    uint16_t    header_size;
    uint32_t    offset_section_table_32;
    uint16_t    nb_of_entries_section_table_32;
    uint16_t    size_of_entry_section_table_32;
    uint64_t    offset_section_table_64;
    uint16_t    nb_of_entries_section_table_64;
    uint32_t    size_of_entry_section_table_64;
} ELF_datas;
#endif